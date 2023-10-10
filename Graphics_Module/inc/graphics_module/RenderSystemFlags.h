#pragma once

#include "common\common.h"

namespace graphics
{
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
}