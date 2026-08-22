//
//  MetalTexture.hpp
//  LightningCore
//
//  Created by Kian Marvi on 6/15/25.
//

#ifndef MetalTexture_hpp
#define MetalTexture_hpp

#include <vector>
#include <array>

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
namespace MTL4
{
    class RenderCommandEncoder;
    class ComputeCommandEncoder;
    class CommandBuffer;
    class CommandAllocator;
};

class MetalTexture
{
public:
    MetalTexture() = default;
    explicit MetalTexture(const char* p_FilePath, MTL::Device* p_MetalDevice);
    
    explicit MetalTexture(const std::vector<const char*> &filePaths, MTL::Device* metalDevice);
    
    explicit MetalTexture(const std::array<const char*, 6> &faces, MTL::Device* metalDevice);
    
    ~MetalTexture();
    
    /*
    MetalTexture(MetalTexture&& other)
    : m_Texture(other.m_Texture),
      m_ArgumentBuffer(other.m_ArgumentBuffer)
    {
        other.m_Texture = nullptr;
        other.m_ArgumentBuffer = nullptr;
    }
     */
    
    MetalTexture& operator=(MetalTexture&& other);
    
    MetalTexture(const MetalTexture&) = delete;
    MetalTexture& operator=(const MetalTexture&) = delete;
    
    inline const MTL::Texture* GetTexture() const
    {
        return m_Texture;
    }

    inline const std::vector<MTL::Texture*>& GetTextures() const
    {
        return m_Textures;
    }
    
    inline const MTL::Texture* GetCubeMap() const
    {
        return m_CubeMap;
    }
    
    inline const MTL::Buffer* GetArgumentBuffer() const
    {
        return m_ArgumentBuffer;
    }
    
private:
    void LoadCubeMap(const std::array<const char*, 6> &faces);
   
    
private:
    MTL::Device*                  m_MetalDevice               = nullptr;
    MTL::Texture*                 m_Texture                   = nullptr;
    MTL::Texture*                 m_CubeMap                   = nullptr;
    std::vector<MTL::Texture*>    m_Textures;
    MTL::Buffer*                  m_ArgumentBuffer            = nullptr;
    MTL4::ComputeCommandEncoder*  m_ComputeCommandEncoder     = nullptr;
    MTL4::CommandAllocator*       m_CommandAllocator          = nullptr;
    
    const char*                   m_Filepath                  = nullptr;
    
    std::vector<int> m_Widths, m_Heights;
    std::vector<TextureInfo> m_TextureInfos;
    
    void GenerateMipmaps(MTL::Texture* texture);
};

#endif /* MetalTexture_hpp */
