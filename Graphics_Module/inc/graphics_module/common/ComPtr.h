#pragma once

#include <wrl.h>

namespace graphics
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
}