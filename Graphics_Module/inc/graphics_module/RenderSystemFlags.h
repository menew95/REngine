#pragma once

#include "common\common.h"

namespace graphics
{
	struct RendererID
	{
		static constexpr int Undefined	= 0x00000000;
		static constexpr int Direct3D11 = 0x00000001;
		static constexpr int Direct3D12 = 0x00000002;
	};

    enum class CPUAccess
    {
        ReadOnly,
        WriteOnly,
        WriteDiscard,
        ReadWrite,
    };

	inline bool HasReadAccess(const CPUAccess access)
	{
		return (access == CPUAccess::ReadOnly || access == CPUAccess::ReadWrite);
	}

	inline bool HasWriteAccess(const CPUAccess access)
	{
		return (access >= CPUAccess::WriteOnly && access <= CPUAccess::ReadWrite);
	}

	struct RenderSystemDesc
	{
		RenderSystemDesc() = default;
		~RenderSystemDesc() = default;

		std::string _moduleName;

		long _flags = 0;
	};

}