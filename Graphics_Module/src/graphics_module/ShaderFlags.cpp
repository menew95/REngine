#include <graphics_module\ShaderFlags.h>

namespace graphics
{
	long GetStageFlags(const ShaderType type)
	{
		switch (type)
		{
			case ShaderType::Reserved0: break;
			case ShaderType::Vertex:    return StageFlags::VS;
			case ShaderType::Domain:    return StageFlags::DS;
			case ShaderType::Hull:		return StageFlags::HS;
			case ShaderType::Geometry:	return StageFlags::GS;
			case ShaderType::Pixel:		return StageFlags::PS;
			case ShaderType::Compute:	return StageFlags::CS;
		}
		return 0;
	}
}