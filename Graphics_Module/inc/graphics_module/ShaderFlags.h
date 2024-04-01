#pragma once

#include <common/common.h>

namespace graphics
{
	struct StageFlags
	{
		enum eStageFlags
		{
			VS = (1 << 0),
			HS = (1 << 1),
			DS = (1 << 2),
			GS = (1 << 3),
			PS = (1 << 4),
			CS = (1 << 5),

			AllTessStages = (HS | DS),

			AllGraphicsStages = (VS | AllTessStages | GS | PS),

			AllStages = (AllGraphicsStages | CS),
		};
	};

	enum class ShaderType
	{
		Pixel		= 0,
		Vertex		= 1,
		Geometry	= 2,
		Hull		= 3,
		Domain		= 4,
		Compute		= 5,
		Reserved0	= 0xFFF0,
	};

	enum class ShaderSourceType
	{
		HLSL	= 0,
		CSO		= 1,
	};

	struct ShaderMacro
	{
		ShaderMacro() = default;
		ShaderMacro(const ShaderMacro&) = default;
		ShaderMacro& operator = (const ShaderMacro&) = default;

		inline ShaderMacro(const char* name, const char* definition = nullptr) :
			name{ name },
			definition{ definition }
		{
		}

		const char* name = nullptr;

		const char* definition = nullptr;
	};

	struct ShaderDesc
	{
		ShaderType _shaderType;
		ShaderSourceType _sourceType;

		tstring _filePath;
		uint32 _sourceSize = 0;

		const char* _entryPoint = nullptr;
		const char* _profile = nullptr;

		const ShaderMacro* _defines = nullptr;

		uint32 _flags = 0;
	};

	enum class FieldType
	{
		UNKNOWN,
		BOOL,
		BOOL2,
		BOOL3,
		BOOL4,
		INT,
		INT2,
		INT3,
		INT4,
		UINT,
		UINT2,
		UINT3,
		UINT4,
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		DOUBLE,
		DOUBLE2,
		DOUBLE3,
		DOUBLE4,
	};

	struct BufferField
	{
		tstring		_name;		// field name
		uint32		_size;		// size(byte)
		uint32		_offset;	// offset(byte) const buffer
		FieldType	_type;
		void*		_defaultValue; // default value(not set then nullptr)
	};

	// Constant Buffer 정보
	struct ConstantBufferDesc
	{
		tstring _name;
		uint32 _resourceType;
		uint32 _boundSlot;
		uint32 _boundCount;
		uint32 _size;
		long _stageFlags;
		vector<BufferField> _fields;
	};

	// 텍스처, 샘플러 바인드 정보
	struct BindResourceDesc
	{
		tstring _name;
		uint32 _resourceType;
		uint32 _boundSlot;
		uint32 _boundCount;
		long _stageFlags;
	};

	// 샘플러 바인드 정보
	struct BindSamplerDesc
	{
		tstring _name;
		uint32 _resourceType;
		uint32 _boundSlot;
		uint32 _boundCount;
		long _stageFlags;
	};

	// 파이프 라인에 바인드 되어야하는 정보
	struct PropertyDesc
	{
		vector<ConstantBufferDesc>	_bindBuffers;
		vector<BindResourceDesc>	_bindResources;
		/*
		* 아직 굳이 나눠야하는지 잘 모르겠다. sampler의 경우 최대 16, 텍스처는 128개 바인드가 가능하다.
		* 일단 안쓰도록 보류
		*/
		vector<BindSamplerDesc>		_bindSamplers;
	};

	struct ShaderProgramDesc
	{
		StageFlags _flags;
	};

	long GetStageFlags(const ShaderType type);
}