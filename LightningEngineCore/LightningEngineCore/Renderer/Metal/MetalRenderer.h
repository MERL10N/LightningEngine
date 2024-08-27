//
//  MetalRenderer.h
//  LightningEngine
//
//  Created by Kian Marvi on 6/26/24.
//

#ifndef Renderer_h
#define Renderer_h


#include <Camera/Camera.h>

namespace MTL
{
    class Device;
    class Library;
    class CommandQueue;
    class CommandBuffer;
    class RenderPipelineState;
    class Buffer;
    class Texture;
    class ArgumentEncoder;
    class RenderCommandEncoder;
    class RenderPassDescriptor;
    class RenderPassColorAttachmentDescriptor;
    class RenderPassDepthAttachmentDescriptor;
}

namespace NS
{
    class AutoreleasePool;
}
namespace MTK
{
    class View;
}

struct CGSize;

class MetalRenderer
{
public:
    MetalRenderer(MTL::Device* metalDevice);
    ~MetalRenderer();
    
    void Draw(MTK::View* view);

private:
    
    void CreateCube();
    void CreateDepthAndMSAATextures(MTK::View* view, CGSize &size);
    void ProcessInput();
    void UpdateMousePosition(float &x, float &y);
    
    MTL::Device* metalDevice;
    
    NS::AutoreleasePool* autoReleasePool;
    
    MTL::Library* metalDefaultLibrary;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPipelineState* metalRenderPSO;
    MTL::Buffer* cubeVertexBuffer;
    MTL::Buffer* transformationBuffer;
    MTL::RenderCommandEncoder* renderCommandEncoder;
    MTL::RenderPassDescriptor* renderPassDescriptor;
    MTL::RenderPassColorAttachmentDescriptor* colorAttachmentDescriptor;
    MTL::RenderPassDepthAttachmentDescriptor* depthAttachment;
    
    MTL::Texture* msaaRenderTargetTexture = nullptr;
    MTL::Texture* depthTexture;
    
    float width;
    float height;
    
    Camera camera;
    float mouseX;
    float mouseY;
    float lastX;
    float lastY;
    bool firstMouse = true;

    // Timing
    float deltaTime = 0.0f;    // time between current frame and last frame
    float lastFrame = 0.0f;
    float currentFrame;
    
    
    // Math
    float fov;
    float nearZ;
    float farZ;
    float angleInDegrees;
    float angleInRadians;
    float aspectRatio;
    
    // Transforms
    matrix_float4x4 translationMatrix;
    matrix_float4x4 perspectiveMatrix;
    matrix_float4x4 rotationMatrix;
    matrix_float4x4 modelMatrix;
    matrix_float4x4 viewMatrix;

};

#endif /* Renderer_hpp */
