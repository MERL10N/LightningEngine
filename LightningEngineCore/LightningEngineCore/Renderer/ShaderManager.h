/**
 CShaderManager
 @brief This class manages all the shaders used in this project.
 By: Toh Da Jun
 Date: Mar 2020
 
 Update:
 @brief This class has been updated to also manage metal shaders used in this project
 By: Kian Heydari Marvi
 Date: Jan 2024
*/
#pragma once

// Include SingletonTemplate
//#include "../DesignPatterns/SingletonTemplate.h"

#include <map>
#include <string>
#ifdef __APPLE__

class MetalShader;

namespace MTL
{
    class RenderPipelineDescriptor;
    class RenderPipelineState;
    class RenderCommandEncoder;
    class DepthStencilState;
    class Device;
    class Buffer;
    class Texture;
}

namespace MTK
{
    class View;
}

#else
#include "Shader.h"
#endif

class CShaderManager
{
	//friend SingletonTemplate<CShaderManager>;
public:
	// Destroy the instance
	void Destroy(void);

	// User Interface
	// Add a shader to this map
#ifdef __APPLE__
    void Add(const std::string& _name, const char* shaderPath, MTK::View* view);
    
    MTL::RenderPipelineDescriptor* getRenderPipelineDescriptor(const std::string& shaderName) const;
    void setRenderPipelineState(const std::string& shaderName, MTL::RenderPipelineState* metalRenderPSO) const;
    MTL::RenderPipelineState* GetRenderPipelineState(const std::string& shaderName) const;
    MTL::DepthStencilState* GetDepthStencilState(const std::string& shaderName) const;
    void BindResources(const std::string& shaderName, MTL::RenderCommandEncoder* encoder, MTL::Buffer* buffer);
#else
	void Add(const std::string& _name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
#endif
	// Remove a shader from this map
	void Remove(const std::string& _name);
	// Use a shader in this map
	void Use(const std::string& _name, MTL::RenderCommandEncoder* commandEncoder);
	// Check if a shader exists in this map
	bool Check(const std::string& _name);

#ifdef __APPLE__
    // The current active shader
    MetalShader* activeShader;
#else
	// The current active shader
	CShader* pActiveShader;
#endif

	// Constructor
	CShaderManager();
    // Destructor
    ~CShaderManager();

protected:

	
#ifdef __APPLE__
    // The map containing all the shaders loaded
    std::map<std::string, MetalShader*> shaderMap;
#else
	// The map containing all the shaders loaded
	std::map<std::string, CShader*> shaderMap;
#endif
};
