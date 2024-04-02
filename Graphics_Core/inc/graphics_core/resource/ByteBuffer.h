/**

    @file      ByteBuffer.h
    @brief     material property block, instance data 등과 같은 데이터를 다루기 위한 버퍼
    @details   ~
    @author    LWT
    @date      26.03.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\resource\ResourceBuffer.h>

namespace graphics
{
    class ByteBuffer : public ResourceBuffer
    {
    public:
        ByteBuffer();

        ~ByteBuffer();

        GRAPHICS_API void SetName(const char* name) override;

        GRAPHICS_API BufferType GetBufferType() override
        {
            return BufferType::BYTE;
        }

        GRAPHICS_API void UpdateBuffer(void* src, uint32 srcSize, uint32 startPoint /*= 0*/);

        GRAPHICS_API void SetBufferSize(size_t size);

        GRAPHICS_API void BufferResize(size_t size);

        GRAPHICS_API size_t GetSize() { return m_size; }

        GRAPHICS_API uint8* GetBuffer() { return m_buffer; }


    private:

        size_t m_size = 0;

        uint8* m_buffer = nullptr;
    };
}
