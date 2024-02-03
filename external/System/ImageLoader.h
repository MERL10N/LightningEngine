//  ImageLoader.h
//  Metal-Tutorial
//
//  Created by Kian Marvi on 23/1/24.

#ifndef ImageLoader_h
#define ImageLoader_h

// Include SingletonTemplate
#include "../DesignPatterns/SingletonTemplate.h"

// Include Metal
#ifdef __APPLE__
#include <Metal/Metal.hpp>
#else

    #ifndef GLEW_STATIC
    #include <GL/glew.h>
    #define GLEW_STATIC
    #endif
#endif

class CImageLoader : public CSingletonTemplate<CImageLoader>
{
    friend CSingletonTemplate<CImageLoader>;
public:
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
protected:
    // Constructor
    CImageLoader(void);

    // Destructor
    virtual ~CImageLoader(void);
    
private:
#ifdef __APPLE__
    MTL::Texture* texture;
    MTL::Device* device;
#endif

};



#endif /* ImageLoader_hpp */
