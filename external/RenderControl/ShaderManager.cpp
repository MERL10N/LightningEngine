/**
 CShaderManager
 @brief This class manages all the shaders used in this project.
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "ShaderManager.h"
#include <stdexcept>      // std::invalid_argument
#ifdef __APPLE__
#include "MetalShader.h"
#endif

/**
 @brief Constructor
 */
CShaderManager::CShaderManager(void) 
	: pActiveShader(nullptr)
{
}

/**
 @brief Destructor
 */
CShaderManager::~CShaderManager(void)
{
	// Clear the memory
	Destroy();
}

/**
 @brief Exit by deleting the shader
 */
void CShaderManager::Destroy(void)
{
#ifdef __APPLE__
    // Delete all scenes stored and empty the entire map
    std::map<std::string, CMetalShader*>::iterator it, end;
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
	pActiveShader = nullptr;
}

#ifdef __APPLE__
void CShaderManager::Add(const std::string& _name, const char* shaderPath, const char* vertexFunction, const char* fragmentFunction,  MTL::Device* device)
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
    CMetalShader* cNewShader = new CMetalShader(shaderPath, vertexFunction, fragmentFunction, device);

    if (cNewShader->IsLoaded())
    {
        std::cout << "Shader loaded" << std::endl;
        // Set a name to this Shader
        cNewShader->setName(_name);

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
MTL::RenderPipelineDescriptor* CShaderManager::getRenderPipelineDescriptor(const std::string& shaderName) const
{
    auto it = shaderMap.find(shaderName);
    if (it != shaderMap.end() && it->second) {
        return it->second->getRenderPipelineDescriptor();
    }
    return nullptr;
}

void CShaderManager::setRenderPipelineState(const std::string& shaderName, MTL::RenderPipelineState* metalRenderPSO) const
{
    auto it = shaderMap.find(shaderName);
    if (it != shaderMap.end() && it->second) {
        it->second->setRenderPipelineState(metalRenderPSO);
    }
}

MTL::RenderPipelineState* CShaderManager::getRenderPipelineState(const std::string& shaderName) const
{
    auto it = shaderMap.find(shaderName);
    if (it != shaderMap.end() && it->second) {
        return it->second->getRenderPipelineState();
    }
    return nullptr;
}

void CShaderManager::BindResources(const std::string &shaderName, MTL::RenderCommandEncoder *encoder, MTL::Buffer* buffer)
{
    auto it = shaderMap.find(shaderName);
    if (it != shaderMap.end() && it->second) {
        it->second->bindResources(encoder, buffer);
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
void CShaderManager::Remove(const std::string& _name)
{
	// Does nothing if it does not exist
	if (!Check(_name))
		return;

#ifdef __APPLE__
    CMetalShader* target = shaderMap[_name];
#else
	CShader* target = shaderMap[_name];
#endif
	try {
		if (target == pActiveShader)
		{
//			throw std::exception("Unable to remove active Shader");
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	// Delete and remove from our map
	delete target;
	shaderMap.erase(_name);
}

/**
@brief Use a Shader
@param _name  const std::string& variable which contains the name of the shader
*/
void CShaderManager::Use(const std::string& _name, MTL::RenderCommandEncoder* commandEncoder)
{
	try {
		if (!Check(_name))
		{
			// Shader does not exist
//			throw std::exception("Shader does not exist");
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return;
	}

	// Check if the new shader is different from the current shader...
	if ((pActiveShader == nullptr) || (pActiveShader->getName().compare(_name) != 0))
	{
		// if Shader exist, set the pActiveShader pointer to that Shader
		pActiveShader = shaderMap[_name];
        pActiveShader->use(commandEncoder);
	}
}

/**
 @brief Check if a Scene exists in this Shader Manager
 @param _name const std::string& variable which contains the name of the shader
 @return true if the shader name exists in the map, else false
 */
bool CShaderManager::Check(const std::string& _name)
{
	return shaderMap.count(_name) != 0;
}
