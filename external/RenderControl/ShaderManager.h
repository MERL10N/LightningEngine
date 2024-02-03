/**
 CShaderManager
 @brief This class manages all the shaders used in this project.
 By: Toh Da Jun
 Date: Mar 2020
 
 Update:
 @brief This class has been updated to also manage metal shaders used in this project
 By: Kian Heydari Marvi
 Date: Jan 2023
*/
#pragma once

// Include SingletonTemplate
#include "../DesignPatterns/SingletonTemplate.h"

#include <map>
#include <string>
#ifdef __APPLE__
#include "MetalShader.h"
#else
#include "Shader.h"
#endif

class CShaderManager : public CSingletonTemplate<CShaderManager>
{
	friend CSingletonTemplate<CShaderManager>;
public:
	// Destroy the instance
	void Destroy(void);

	// User Interface
	// Add a shader to this map
#ifdef __APPLE__
    void Add(const std::string& _name, const char* shaderPath, const char* vertexFunction, const char* fragmentFunction,  MTL::Device* device);
    
    MTL::RenderPipelineDescriptor* getRenderPipelineDescriptor(const std::string& shaderName) const;
    void setRenderPipelineState(const std::string& shaderName, MTL::RenderPipelineState* metalRenderPSO) const;
    MTL::RenderPipelineState* getRenderPipelineState(const std::string& shaderName) const;
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
    CMetalShader* pActiveShader;
#else
	// The current active shader
	CShader* pActiveShader;
#endif

protected:
	// Constructor
	CShaderManager(void);

	// Destructor
	virtual ~CShaderManager(void);

#ifdef __APPLE__
    // The map containing all the shaders loaded
    std::map<std::string, CMetalShader*> shaderMap;
#else
	// The map containing all the shaders loaded
	std::map<std::string, CShader*> shaderMap;
#endif
};
