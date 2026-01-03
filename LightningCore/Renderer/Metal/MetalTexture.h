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
    explicit MetalTexture(const char* p_FilePath, MTL::Device* p_MetalDevice);
    ~MetalTexture();

    inline MTL::Texture* GetTexture() const
    {
        return m_Texture;
    }
    
    inline int GetWidth() const
    {
        return m_Width;
    }
    
    inline int GetHeight() const
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
