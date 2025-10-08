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
    MetalTexture() = default;
    explicit MetalTexture(const char* p_Filepath);
    ~MetalTexture();
    
    void SetMetalDevice(MTL::Device* p_MetalDevice);
    
    inline MTL::Texture* GetTexture()
    {
        return m_Texture;
    }
    
    inline int GetWidth()
    {
        return m_Width;
    }
    
    inline int GetHeight()
    {
        return m_Height;
    }
    
private:
    MTL::Texture* m_Texture = nullptr;
    MTL::TextureDescriptor* m_TextureDescriptor = nullptr;
    MTL::Device* m_MetalDevice = nullptr;
    const char* m_Filepath = nullptr;
    int m_Width, m_Height, m_Channels;
};

#endif /* MetalTexture_hpp */
