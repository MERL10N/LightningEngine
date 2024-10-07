/**
 CShaderManager
 @brief This class manages all the shaders used in this project.
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "ShaderManager.h"
#include <stdexcept>      // std::invalid_argument
#include <iostream>
#ifdef __APPLE__
#include "MetalShader.h"
#endif

/**
 @brief Constructor
 */
ShaderManager::ShaderManager(void) 
	: activeShader(nullptr)
{
}

/**
 @brief Destructor
 */
ShaderManager::~ShaderManager(void)
{
	// Clear the memory
	Destroy();
}

/**
 @brief Exit by deleting the shader
 */
void ShaderManager::Destroy(void)
{
#ifdef __APPLE__
    // Delete all scenes stored and empty the entire map
    std::map<std::string, MetalShader*>::iterator it, end;
#else
	// Delete all scenes stored and empty the entire map
	std::map<std::string, CShader*>::iterator it, end;
#endif
	end = shaderMap.end();
	for (it = shaderMap.begin(); it != end; ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
	shaderMap.clear();

	// Set pActiveShader to nullptr
	activeShader = nullptr;
}

#ifdef __APPLE__
void ShaderManager::Add(const std::string& _name, const char* shaderPath, MTK::View* view)
{
    // Check if there is already a similar shader name in the map
    if (Check(_name))
    {
        // Scene Exist, unable to proceed
        throw std::runtime_error("Duplicate shader name provided");
        //Change to avoid crashing
        return;
    }

    // Initialise a new Shader
    MetalShader* cNewShader = new MetalShader(shaderPath, view);

    if (cNewShader->IsLoaded())
    {
        std::cout << "Shader loaded" << std::endl;
        // Set a name to this Shader
        cNewShader->SetName(_name);

        // Nothing wrong, add the scene to our map
        shaderMap[_name] = cNewShader;
    }
    else
    {
            // Shader loading failed, clean up
            delete cNewShader;
    }
}

// Implementation
MTL::RenderPipelineDescriptor* ShaderManager::getRenderPipelineDescriptor(const std::string& shaderName) const
{
    auto it = shaderMap.find(shaderName);
    if (it != shaderMap.end() && it->second) {
        return it->second->GetRenderPipelineDescriptor();
    }
    return nullptr;
}

void ShaderManager::setRenderPipelineState(const std::string& shaderName, MTL::RenderPipelineState* metalRenderPSO) const
{
    auto it = shaderMap.find(shaderName);
    if (it != shaderMap.end() && it->second) {
        it->second->SetRenderPipelineState(metalRenderPSO);
    }
}

MTL::RenderPipelineState* ShaderManager::GetRenderPipelineState(const std::string& shaderName) const
{
    auto it = shaderMap.find(shaderName);
    if (it != shaderMap.end() && it->second) {
        return it->second->GetRenderPipelineState();
    }
    return nullptr;
}

MTL::DepthStencilState* ShaderManager::GetDepthStencilState(const std::string& shaderName) const
{
    auto it = shaderMap.find(shaderName);
    if (it != shaderMap.end() && it->second)
    {
        return it->second->GetDepthStencilState();
    }
    return nullptr;
}


void ShaderManager::BindResources(const std::string &shaderName, MTL::RenderCommandEncoder *encoder, MTL::Buffer* buffer)
{
    auto it = shaderMap.find(shaderName);
    if (it != shaderMap.end() && it->second) {
        it->second->BindResources(encoder, buffer);
    }
}

#else
/**
 @brief Add a Scene to this Shader Manager
 @param _name const std::string& variable which contains the name of the shader
 @param vertexPath const char* vertexPath variable which contains the path to the vertex shader file
 @param fragmentPath const char* fragmentPath variable which contains the path to the fragment shader file
 @param geometryPath const char* geometryPath variable which contains the path to the geometry shader file
 */
void CShaderManager::Add(	const std::string& _name, 
							const char* vertexPath, 
							const char* fragmentPath, 
							const char* geometryPath,
                            MTL::RenderPipielineState* metalPSO)
{
	// Check if there is already a similar shader name in the map
	if (Check(_name))
	{
		// Scene Exist, unable to proceed
		throw std::exception("Duplicate shader name provided");
		//Change to avoid crashing
		return;
	}

	// Initialise a new Shader
	CShader* cNewShader = new CShader(vertexPath, fragmentPath, geometryPath, metalPSO);

	if (cNewShader->IsLoaded())
	{
		// Set a name to this Shader
		cNewShader->setName(_name);

		// Nothing wrong, add the scene to our map
		shaderMap[_name] = cNewShader;
	}
}
#endif

/**
 @brief Remove a Shader from this Shader Manager
 @param  _name  std::string& variable which contains the name of the shader
 */
void ShaderManager::Remove(const std::string& _name)
{
	// Does nothing if it does not exist
	if (!Check(_name))
		return;

#ifdef __APPLE__
    MetalShader* target = shaderMap[_name];
#else
	CShader* target = shaderMap[_name];
#endif
    
    assert(target == activeShader);

	// Delete and remove from our map
	delete target;
	shaderMap.erase(_name);
}

/**
@brief Use a Shader
@param _name  const std::string& variable which contains the name of the shader
*/
void ShaderManager::Use(const std::string& _name, MTL::RenderCommandEncoder* commandEncoder)
{
    assert(!Check(_name));

	// Check if the new shader is different from the current shader...
	if ((activeShader == nullptr) || (activeShader->GetName().compare(_name) != 0))
	{
		// if Shader exist, set the pActiveShader pointer to that Shader
		activeShader = shaderMap[_name];
        activeShader->use(commandEncoder);
	}
}

/**
 @brief Check if a Scene exists in this Shader Manager
 @param _name const std::string& variable which contains the name of the shader
 @return true if the shader name exists in the map, else false
 */
bool ShaderManager::Check(const std::string& _name)
{
	return shaderMap.count(_name) != 0;
}


