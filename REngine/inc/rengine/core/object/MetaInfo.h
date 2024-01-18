/**

    @file      MetaInfo.h
    @brief     Meta data enum info
    @details   meta data에 관한 enum class info들
    @author    LWT
    @date      11.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

namespace rengine
{
    enum class MetaData
    {
        Serializable,
        ObjectType,
        Editor,
    };

    enum class MetaDataType
    {
        WSTRING,
        VECTOR2,
        VECTOR3,
        VECTOR4,
        MATRIX,
        UUID,
        BOOL,
        UINT32,
        INT32,
        FLOAT,
        DOUBLE,
        ENUM,
    };

    enum class SerializableType
    {
        SCENE       = 0,
        RESOURCE    = 1,
        PREFABS     = 2,
        UNKNOWN     = 3,
    };

    struct MetaInfo
    {
        // 메타파일 위치
        tstring _metaFilePath;

        // 메타파일이 가리키는 파일 위치
        tstring _filePath;

        // uuid
        tstring _guid;

        // serializer type
        SerializableType _type = SerializableType::UNKNOWN;
    };
}