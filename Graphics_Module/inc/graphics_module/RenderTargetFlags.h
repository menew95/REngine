#pragma once

#include <common\common.h>
#include "common\math.h"

namespace graphics
{
	enum class RenderTargetType
	{
		RenderTarget = 1,
		Depth = 2,
		DepthStencil = 3,
		Stencil = 4,
	};

	struct AttachmentDesc
	{
		RenderTargetType _renderTargetType = RenderTargetType::RenderTarget;

		class Texture* _resource = nullptr;

		uint32 _mipLevel = 0;

		uint32 _arrayLayer = 0;

		uint32 _arraySize = 0;
	};

	struct RenderTargetDesc
	{
		math::Vector2 _extend;

		uint32 _sample = 1;

		std::vector<AttachmentDesc> _attachments;
	};
}