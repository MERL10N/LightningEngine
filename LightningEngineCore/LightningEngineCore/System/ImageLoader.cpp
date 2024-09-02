/**
 CImageLoader
 @brief A class to load an image into system memory and also graphics card
 By: Toh Da Jun
 Date: Mar 2020
 
 Update:
 
 @brief CImageLoader has been updated to support Metal API
 Changes made by: Kian Heydari Marvi
 Date: January 2023
 
*/
#include "ImageLoader.h"

#include <iostream>
using namespace std;

// Include Metal
#ifdef __APPLE__
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#else

    #ifndef GLEW_STATIC
    #include <GL/glew.h>
    #define GLEW_STATIC
    #endif

   #include "Filesystem.h"
#endif

// STB Image
#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb/stb_image.h>


/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CImageLoader::CImageLoader(void)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CImageLoader::~CImageLoader(void)
{
}

/**
@brief Init Initialise this instance
*/
bool CImageLoader::Init(void)
{
    return false;
}

#ifdef __APPLE__
/**
 @brief Load an image into the graphics card and return its ID.
 @param filename A const char* storing the name of the image file
 */
void CImageLoader::LoadTexture(const char* filename, MTL::Device* device)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    assert(image != nullptr);
    
    MTL::TextureDescriptor* textureDescriptor = MTL::TextureDescriptor::alloc()->init();
    textureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    textureDescriptor->setWidth(width);
    textureDescriptor->setHeight(height);
    
    texture = device->newTexture(textureDescriptor);

    MTL::Region region = MTL::Region(0, 0, 0, width, height, 1);
    NS::UInteger bytesPerRow = 4 * width;

    texture->replaceRegion(region, 0, image, bytesPerRow);
        
    textureDescriptor->release();
    stbi_image_free(image);

    std::cout << "Texture loaded" << std::endl;
}

void CImageLoader::Destroy()
{
    texture->release();
    msaaRenderTargetTexture->release();
    depthTexture->release();
    resolvedTexture->release();
}

MTL::Texture* CImageLoader::GetTexture()
{
    return texture;
}

MTL::Texture* CImageLoader::GetTargetTexture()
{
    return msaaRenderTargetTexture;
}

MTL::Texture* CImageLoader::GetDepthTexture()
{
    return depthTexture;
}

MTL::Texture* CImageLoader::GetResolvedTexture()
{
    return resolvedTexture;
}

void CImageLoader::CreateDepthAndMSAATextures(float &width, float &height, CGSize &size, MTL::Device* device)
{
    width = size.width;
    height = size.height;
    
    // Deallocate the already existing target textures to avoid memory leak
    if (msaaRenderTargetTexture)
    {
        msaaRenderTargetTexture->release();
    }
    if (depthTexture)
    {
        depthTexture->release();
    }
    
    MTL::TextureDescriptor* msaaTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    msaaTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    msaaTextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    msaaTextureDescriptor->setWidth(width);
    msaaTextureDescriptor->setHeight(height);
    msaaTextureDescriptor->setSampleCount(4);
    msaaTextureDescriptor->setStorageMode(MTL::StorageModePrivate);
    msaaTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);
    
    msaaRenderTargetTexture = device->newTexture(msaaTextureDescriptor);

    MTL::TextureDescriptor* depthTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    depthTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    depthTextureDescriptor->setPixelFormat(MTL::PixelFormatDepth32Float);
    depthTextureDescriptor->setWidth(width);
    depthTextureDescriptor->setHeight(height);
    depthTextureDescriptor->setSampleCount(4);
    depthTextureDescriptor->setStorageMode(MTL::StorageModePrivate);
    depthTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);


    depthTexture = device->newTexture(depthTextureDescriptor);

    msaaTextureDescriptor->release();
    depthTextureDescriptor->release();
}

void CImageLoader::CreateResolveTexture(float &width, float &height, CGSize &size, MTL::Device* device)
{
    width = size.width;
    height = size.height;
    
    if (resolvedTexture)
    {
        resolvedTexture->release();
    }
    
    MTL::TextureDescriptor* resolvedTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    resolvedTextureDescriptor->setTextureType(MTL::TextureType2D);
    resolvedTextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    resolvedTextureDescriptor->setWidth(width);
    resolvedTextureDescriptor->setHeight(height);
    resolvedTextureDescriptor->setStorageMode(MTL::StorageModeManaged);
    resolvedTextureDescriptor->setUsage(MTL::TextureUsageShaderRead);

    resolvedTexture = device->newTexture(resolvedTextureDescriptor);
    resolvedTextureDescriptor->release();
}
#else

/**
     @brief Load an image and return as unsigned char*
     @param filename A const char* storing the name of the image file
     @param width A int variable passed in by reference. It stores the width of the image.
     @param height A int variable passed in by reference. It stores the width of the image.
     @param nrChannels A int variable passed in by reference. It stores the number of channels in the image.
     @param bInvert A const bool
 */
unsigned char * CImageLoader::Load(const char* filename, int& width, int& height, int& nrChannels, const bool bInvert)
{
    // tell stb_image.h to flip loaded texture's on the y-axis.
    stbi_set_flip_vertically_on_load(bInvert);

    // load image, create texture and generate mipmaps
    unsigned char *data = stbi_load(FileSystem::getPath(filename).c_str(),
                                    &width, &height, &nrChannels, 0);

    // Return the data but you must remember to free up the memory of the file data read in
    return data;
}

/**
 @brief Load an image into the graphics card and return its ID.
 @param filename A const char* storing the name of the image file
 @param bInvert A const bool
 */
unsigned int CImageLoader::LoadTextureGetID(const char* filename,  const bool bInvert)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    int nrChannels = 0;

    // tell stb_image.h to flip loaded texture's on the y-axis.
    stbi_set_flip_vertically_on_load(bInvert);

    // load image, create texture and generate mipmaps
    unsigned char* data = stbi_load(FileSystem::getPath(filename).c_str(),
        &image_width, &image_height, &nrChannels, 0);

    if (data == NULL)
    {
        cout << "CImageLoader::LoadTextureGetID(): Unable to load " << FileSystem::getPath(filename).c_str() << endl;
        return 0;
    }

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    if (nrChannels == 1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, image_width, image_height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
    else if (nrChannels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (nrChannels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    return image_texture;
}
#endif

