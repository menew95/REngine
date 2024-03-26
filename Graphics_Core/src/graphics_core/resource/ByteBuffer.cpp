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

		if (srcSize + startPoint >= m_size)
		{
			// 최대 사이즈가 부족할 경우 버퍼크기를 늘려줌
			SetBufferSize(m_size - startPoint + srcSize);
		}

		memcpy(m_buffer + startPoint, src, srcSize);
	}

	void ByteBuffer::SetBufferSize(size_t size)
	{
		delete[] m_buffer;

		m_buffer = new uint8[size];

		m_size = size;
	}
}