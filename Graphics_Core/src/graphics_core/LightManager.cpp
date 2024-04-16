#include <graphics_core\LightManager.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_core\resource\LightBuffer.h>

#include <graphics_module\Buffer.h>
#include <graphics_module\Texture.h>
#include <graphics_module\RenderTarget.h>
#include <graphics_module\CommandBuffer.h>

namespace graphics
{
	DEFINE_SINGLETON_CLASS(LightManager,
		{

		},
		{

		},
		{
			Instance.ReleaseAllResource();
 		});

	void graphics::LightManager::Initialize()
	{
		CreateResource();
	}

	LightBuffer* LightManager::CreateLightBuffer(uuid uuid)
	{
		auto _iter = m_lightBufferMap.find(uuid);

		// 이미 존재한 uuid가 들어왔음
		assert(_iter == m_lightBufferMap.end());

		return m_lightBufferMap.insert(make_pair(uuid, new LightBuffer())).first->second;
	}

	bool LightManager::ReleaseLightBuffer(uuid uuid)
	{
		auto _iter = m_lightBufferMap.find(uuid);

		if (_iter != m_lightBufferMap.end())
		{
			delete _iter->second;

			m_lightBufferMap.erase(_iter);

			return true;
		}

		return false;
	}

	void graphics::LightManager::Excute(CommandBuffer* command)
	{
		UpdateLightBuffer(command);
	}

	void graphics::LightManager::CreateResource()
	{
		BufferDesc _desc;

		_desc._bindFlags = BindFlags::ShaderResource | BindFlags::UnorderedAccess;
		_desc._format = Format::UNKNOWN;
		_desc._stride = sizeof(LightInfo);
		_desc._size = sizeof(LightInfo) * 100;
		_desc._cpuAccessFlags = CPUAccessFlags::Write;
		_desc._miscFlags = MiscFlags::DynamicUsage;

		m_lightBuffer = ResourceManager::GetInstance()->CreateBuffer(TEXT("Light Buffer View"), _desc);
	}

	void LightManager::ReleaseAllResource()
	{
		for (auto& [_uuid, _buffer] : m_lightBufferMap)
		{
			assert(_buffer != nullptr);

			delete _buffer;
		}

		m_lightBufferMap.clear();
	}

	void graphics::LightManager::UpdateLightBuffer(CommandBuffer* command)
	{

		uint32 _stride = sizeof(LightInfo);
		uint32 _dataSize = (uint32)m_lightBufferMap.size() * _stride;
		m_curLightCnt = 0;

		if(_dataSize == 0)
			return;

		char* _data = new char[_dataSize];

		size_t _offset = 0;

		for (auto& [_uuid, _buffer] : m_lightBufferMap)
		{
			if(!_buffer->GetEnable())
				continue;

			m_curLightCnt++;

			memcpy(_data + _offset, &_buffer->GetLightInfo(), _stride);

			_offset += _stride;
		}

		command->UpdateBuffer(*m_lightBuffer, 0, _data, _dataSize);

		//offsetof(PerFrame, _lightCnt);
		//
		//auto* buffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerFrame"));
		//
		//command->UpdateBuffer(*buffer, offsetof(PerFrame, _lightCnt), &m_curLightCnt, sizeof(m_curLightCnt));
	}
}