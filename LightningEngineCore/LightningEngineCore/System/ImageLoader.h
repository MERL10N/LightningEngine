//  ImageLoader.h
//  Metal-Tutorial
//  Created by Kian Marvi on 23/1/24.

#ifndef ImageLoader_h
#define ImageLoader_h

// Include SingletonTemplate

// Include Metal
#ifdef __APPLE__


namespace MTL
{
    class Texture;
    class Device;
}

namespace MTK
{
    class View;
}

struct CGSize;

#else
    #ifndef GLEW_STATIC
    #include <GL/glew.h>
    #define GLEW_STATIC
    #endif
#endif

class ImageLoader
{
public:
    // Constructor
    ImageLoader(void);

    // Destructor
    virtual ~ImageLoader(void);
    
    // Init
    bool Init(void);
#ifdef __APPLE__
    
    // Load an image and return as a Texture ID
    void LoadTexture(const char* filename, MTL::Device* device);
    MTL::Texture*  GetTexture();
    void Destroy();
#else
    // Load an image and return as unsigned char*
    unsigned char * Load(   const char* filename,
                            int& width, int& height,
                         int& nrChannels, const bool bInvert = false);

    // Load an image and return as a Texture ID
    unsigned int LoadTextureGetID(const char* filename, const bool bInvert);
#endif
 
private:
#ifdef __APPLE__
    MTL::Device* device;
    MTL::Texture* texture;
#endif

};



#endif /* ImageLoader_hpp */
