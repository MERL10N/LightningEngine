//
//  MetalTexture.hpp
//  LightningCore
//
//  Created by Kian Marvi on 6/15/25.
//

#ifndef MetalTexture_hpp
#define MetalTexture_hpp

#include <vector>

struct TextureInfo
{
    int m_Width;
    int m_Height;
};

namespace MTL
{
    class Device;
    class Texture;
    class TextureDescriptor;
    class ResidencySet;
    class Buffer;
}

class MetalTexture
{
public:
    MetalTexture() = default;
    explicit MetalTexture(const char* p_FilePath, MTL::Device* p_MetalDevice);
    
    explicit MetalTexture(const std::vector<const char*> &filePaths, MTL::Device* metalDevice);
    ~MetalTexture();

    inline const MTL::Texture* GetTexture() const
    {
        return m_Texture;
    }
    
    inline const MTL::Buffer* GetArgumentBuffer() const
    {
        return m_ArgumentBuffer;
    }
    
private:
    MTL::Device*            m_MetalDevice       = nullptr;
    MTL::TextureDescriptor* m_TextureDescriptor = nullptr;
    MTL::Texture*           m_Texture           = nullptr;
    MTL::Buffer*            m_ArgumentBuffer    = nullptr;
    
    const char*             m_Filepath          = nullptr;
    
    std::vector<int> m_Widths, m_Heights;
    std::vector<TextureInfo> m_TextureInfos;
};

#endif /* MetalTexture_hpp */
