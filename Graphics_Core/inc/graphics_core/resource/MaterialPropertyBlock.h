/**

    @file      MaterialPropertyBlock.h
    @brief     material property들을 관리 하는 클래스
    @details   ~
    @author    LWT
    @date      29.03.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <graphics_module\ShaderFlags.h>
#include <graphics_module\PipelineLayoutFlags.h>

namespace math
{
    struct Color;
    struct Vector4;
}

namespace graphics
{
    class MaterialPropertyBlock
    {
    public:
        MaterialPropertyBlock() = default;
        ~MaterialPropertyBlock() = default;

        void SetProperty(const PropertyDesc& desc);

        void BindProperty(class CommandBuffer* command);
        //void BindProperty(class PipelineLayout* command);

        /*
        * 일단 모든 프로퍼티를 “PerMaterial”라는 이름의 단일 CBUFFER로 선언합니다.
        * Shader cbuffer 이름이 "PerMaterial"이여야 작동이 됨
        */

        /**
            @brief material의 color값 설정
            @param name  - property name
            @param value - color value
        **/
        void SetColor(const tstring& name, const math::Color& value);

        /**
            @brief material의 vector값 설정
            @param name  - property name
            @param value - vector4 value
        **/
        void SetVector4(const tstring& name, const math::Vector4& value);

        /**
            @brief material의 float 값 설정
            @param name  - property name
            @param value - float value
        **/
        void SetFloat(const tstring& name, float value);

        /**
            @brief material의 새 텍스처 할당
            @param name    - property name
            @param texture - texture
        **/
        void SetTexture(const tstring& name, class TextureBuffer* textureBuffer);
        void SetTexture(const tstring& name, class Texture* texture);
        void SetResource(const tstring& name, class Resource* texture);

        /**
            @brief material의 int 값 설정
            @param name  - property name
            @param value - int value
        **/
        void SetInteger(const tstring& name, int value);

    private:
        bool m_bInit = false;

        PropertyDesc m_propertyDesc;

        // cbuffer 프로퍼티와 대응
        map<tstring, pair<BindingDescriptor, shared_ptr<class ByteBuffer>>> m_constantBlock;
        map<tstring, class Buffer*> m_constBuffers;

        // texture1D, texture2D, texture3D 등 텍스처 프로퍼티와 대응
        // 구조적 버퍼와 같은 내용도 추가가 필요해짐
        map<tstring, pair<BindingDescriptor, class Resource*>> m_resourceBlock;
        map<tstring, pair<BindingDescriptor, class Sampler*>> m_samplerBlock;
    };
}