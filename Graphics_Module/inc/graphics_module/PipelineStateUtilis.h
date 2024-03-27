#pragma once

#include <common\common.h>
#include <graphics_module\graphics_module_dllexport.h>

namespace graphics
{
	class Shader;

	struct GraphicsPipelineDesc;
	struct ComputePipelineDesc;

	
	GRAPHICS_DLL_DECLSPEC vector<Shader*> GetShadersAsArray(const GraphicsPipelineDesc& desc);

	GRAPHICS_DLL_DECLSPEC vector<Shader*> GetShadersAsArray(const ComputePipelineDesc& desc);

	template <typename T>
	inline vector<T*> CastShaderArray(const vector<Shader*>& shaders)
	{
		vector<T*> _ret;
		for (auto* sh : shaders)
			_ret.push_back(reinterpret_cast<T*>(sh));

		return _ret;
	}
}