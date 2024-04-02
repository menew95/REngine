#include <graphics_core\resource\ByteBuffer.h>

namespace graphics
{
	ByteBuffer::ByteBuffer()
	{
	
	}
	
	ByteBuffer::~ByteBuffer()
	{
		if (m_buffer != nullptr)
		{
			delete[] m_buffer;
		}
	}
	
	void ByteBuffer::SetName(const char* name)
	{
	
	}
	
	void ByteBuffer::UpdateBuffer(void* src, uint32 srcSize, uint32 startPoint)
	{
		if (src == nullptr) return;

		if (srcSize + startPoint > m_size)
		{
			// 최대 사이즈가 부족할 경우 버퍼크기를 늘려줌
			BufferResize(m_size - startPoint + srcSize);
		}

		memcpy(m_buffer + startPoint, src, srcSize);
	}

	void ByteBuffer::SetBufferSize(size_t size)
	{
		if(m_buffer != nullptr) delete[] m_buffer;

		m_buffer = new uint8[size];

		m_size = size;
	}

	void ByteBuffer::BufferResize(size_t size)
	{
		auto* _newBuf = new uint8[size];

		if (m_buffer != nullptr)
		{
			memcpy(_newBuf, m_buffer, sizeof(m_buffer));

			delete[] m_buffer;
		}

		m_buffer = _newBuf;

		m_size = size;
	}
}