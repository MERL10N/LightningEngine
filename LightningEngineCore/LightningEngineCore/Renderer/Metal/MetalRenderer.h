//
//  MetalRenderer.h
//  LightningEngine
//
//  Created by Kian Marvi on 6/26/24.
//

#ifndef Renderer_h
#define Renderer_h


#include <Camera/Camera.h>
#include <Editor/Editor.h>
#include <CoreGraphics/CoreGraphics.h>

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
    class TextureDescriptor;
}

namespace MTK
{
    class View;
}


class MetalRenderer
{
public:
    MetalRenderer(MTK::View* view);
    ~MetalRenderer();
    
    void Init(MTK::View* view);
    void Update(float &deltaTime);
    void Draw(MTK::View* view);
    
    void Destroy();

private:
    
    void CreateCube();
    void CreateLight();
    void ProcessInput();
    void UpdateMousePosition(float &x, float &y);
    
    
    MTK::View* view;
    
    MTL::Library* metalDefaultLibrary;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPipelineState* metalRenderPSO;
    MTL::RenderPipelineState* lightRenderPSO;
    MTL::Buffer* cubeVertexBuffer;
    MTL::Buffer* lightVertexBuffer;
    MTL::Buffer* transformationBuffer;
    MTL::Buffer* lightTransformationBuffer;
    MTL::RenderCommandEncoder* renderCommandEncoder;
    MTL::RenderPassDescriptor* renderPassDescriptor;
    MTL::RenderPassDepthAttachmentDescriptor* depthAttachment;

    
    float width;
    float height;
    
    //Editor
    Editor editor;
    CGSize drawableSize;
    
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
