#pragma once

#include "common\common.h"

namespace graphics
{
	void DXThrowIfFailed(const HRESULT hr, const char* info);
}