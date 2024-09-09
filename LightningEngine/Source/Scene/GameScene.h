//
//  GameScene.hpp
//  LightningEngine - macOS
//
//  Created by Kian Marvi on 9/6/24.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <LightningEngineCore/Camera.h>
#include <LightningEngineCore/ImageLoader.h>
#include <LightningEngineCore/ShaderManager.h>

class MetalRenderer;
struct TransformationData;
struct VertexData;

namespace MTK
{
    class View;
}
namespace MTL
{
  class Buffer;
}

class GameScene
{
public:
    void Init(MTK::View* view);
    void Render(MTK::View * view);
    void Release();
    
    void ProcessInput();
    void UpdateMousePosition(float &x, float &y);
private:
    
    bool firstMouse;
    float width;
    float height;
    float mouseX;
    float mouseY;
    float lastX;
    float lastY;
    float currentFrame;
    float lastFrame;
    float deltaTime;
    Camera camera;
    MetalRenderer* renderer;
    ImageLoader imageLoader;
    ShaderManager shaderManager;
    
    
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
    
    MTL::Buffer* meshVertexBuffer;
    MTL::Buffer* transformationBuffer;
};



#endif /* GameScene_hpp */
