//
//  MetalTexture.hpp
//  LightningCore
//
//  Created by Kian Marvi on 6/15/25.
//

#ifndef MetalTexture_hpp
#define MetalTexture_hpp

namespace MTL
{
    class Device;
    class Texture;
    class TextureDescriptor;
}

class MetalTexture
{
public:
    MetalTexture(const char* p_Filepath, MTL::Device* p_MetalDevice);
    ~MetalTexture();
    
    MTL::Texture* GetTexture();
private:
    MTL::Texture* m_Texture;
    MTL::TextureDescriptor* m_TextureDescriptor;
    MTL::Device* m_MetalDevice;
    const char* m_Filepath;
    int width, height, channels;
};

#endif /* MetalTexture_hpp */
