/**

    @file      UpdateResourceData.h
    @brief     리소스들을 업데이트 할 정보를 담은 객체
    @details   ~
    @author    LWT
    @date      30.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>
#include <common\math.h>

#include <graphics_module\Resource.h>

namespace graphics
{
	struct UpdateResourceData
	{
		inline UpdateResourceData()
		{}

		inline UpdateResourceData(uint32 slot, ResourceType type, void* src, uint32 size)
			: _bindSlot(slot)
			, _resourceType(type)
			, _dataSrc(src)
			, _datasize(size)
		{}

		inline UpdateResourceData(uint32 slot, void* src)
			: _bindSlot(slot)
			, _resourceType(ResourceType::Texture)
			, _dataSrc(src)
		{}

		inline UpdateResourceData(uint32 slot, void* src, uint32 size)
			: _bindSlot(slot)
			, _resourceType(ResourceType::Buffer)
			, _dataSrc(src)
			, _datasize(size)
		{}

		uint32 _bindSlot;

		ResourceType _resourceType = ResourceType::Undefined;

		void* _dataSrc = nullptr;

		uint32 _datasize = 0;

		bool operator=(UpdateResourceData& r)
		{
			return (_bindSlot == r._bindSlot &&
				_resourceType == r._resourceType &&
				_dataSrc == r._dataSrc &&
				_datasize == r._datasize);
		}
	};
}