#include <graphics_module\PipelineStateUtilis.h>
#include <graphics_module\PipelineStateFlags.h>

namespace graphics
{

	vector<Shader*> GetShadersAsArray(const GraphicsPipelineDesc& desc)
	{
		vector<Shader*> _shaders(5);

		if (desc._shaderProgram._vertexShader != nullptr)
			_shaders.push_back(desc._shaderProgram._vertexShader);

		if (desc._shaderProgram._hullShader != nullptr)
			_shaders.push_back(desc._shaderProgram._hullShader);

		if (desc._shaderProgram._domainShader != nullptr)
			_shaders.push_back(desc._shaderProgram._domainShader);

		if (desc._shaderProgram._geometryShader != nullptr)
			_shaders.push_back(desc._shaderProgram._geometryShader);

		if (desc._shaderProgram._pixelShader != nullptr)
			_shaders.push_back(desc._shaderProgram._pixelShader);

		return _shaders;
	}

	vector<Shader*> GetShadersAsArray(const ComputePipelineDesc& desc)
	{
		vector<Shader*> _shaders;

		if (desc._shaderProgram._computeShader != nullptr)
			_shaders.push_back(desc._shaderProgram._computeShader);

		return _shaders; 
	}
}