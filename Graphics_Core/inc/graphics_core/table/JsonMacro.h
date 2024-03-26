#pragma once

#include "JsonReader/JsonMacro.h"

/**** RenderTarget Enum ****/

JSON_SERIALIZE_ENUM(graphics::RenderTargetType,
	{
	{graphics::RenderTargetType::Depth, _T("Depth")},
	{graphics::RenderTargetType::DepthStencil, _T("DepthStencil")},
	{graphics::RenderTargetType::RenderTarget, _T("RenderTarget")},
	{graphics::RenderTargetType::Stencil, _T("Stencil")}
	});

/**** Texture Enum ****/

JSON_SERIALIZE_ENUM(graphics::TextureType,
	{
	{graphics::TextureType::Texture1D, _T("Texture1D")},
	{graphics::TextureType::Texture2D, _T("Texture2D")},
	{graphics::TextureType::Texture3D, _T("Texture3D")},
	{graphics::TextureType::TextureCube, _T("TextureCube")},
	{graphics::TextureType::Texture1DArray, _T("Texture1DArray")},
	{graphics::TextureType::Texture2DArray, _T("Texture2DArray")},
	{graphics::TextureType::TextureCubeArray, _T("TextureCubeArray")},
	{graphics::TextureType::Texture2DMS, _T("Texture2DMS")},
	{graphics::TextureType::Texture2DMSArray, _T("Texture2DMSArray")}
	});

JSON_SERIALIZE_ENUM(graphics::Format,
	{
	{graphics::Format::R8G8B8A8_UNORM, _T("R8G8B8A8_UNORM")},
	{graphics::Format::R32G32B32A32_FLOAT, _T("R32G32B32A32_FLOAT")},
	{graphics::Format::R24G8_TYPELESS, _T("R24G8_TYPELESS")},
	{graphics::Format::R32_TYPELESS, _T("R32_TYPELESS")},
	{graphics::Format::R32_UINT, _T("R32_UINT")}
	});


JSON_SERIALIZE_ENUM(graphics::MiscFlags::eMiscFlags,
	{
	{graphics::MiscFlags::DynamicUsage, _T("DynamicUsage")},
	{graphics::MiscFlags::FixedSamples, _T("FixedSamples")},
	{graphics::MiscFlags::GenerateMips, _T("GenerateMips")},
	{graphics::MiscFlags::NoInitialData, _T("NoInitialData")},
	{graphics::MiscFlags::Append, _T("Append")},
	{graphics::MiscFlags::Counter, _T("Counter")}
	});

JSON_SERIALIZE_ENUM(graphics::BindFlags::eBindFlags,
	{
	{graphics::BindFlags::VertexBuffer, _T("VertexBuffer")},
	{graphics::BindFlags::IndexBuffer, _T("IndexBuffer")},
	{graphics::BindFlags::ConstantBuffer, _T("ConstantBuffer")},
	{graphics::BindFlags::ShaderResource, _T("ShaderResource")},
	{graphics::BindFlags::StreamOutput, _T("StreamOutput")},
	{graphics::BindFlags::RenderTarget, _T("RenderTarget")},
	{graphics::BindFlags::DepthStencil, _T("DepthStencil")},
	{graphics::BindFlags::UnorderedAccess, _T("UnorderedAccess")},
	{graphics::BindFlags::Decoder, _T("Decoder")},
	{graphics::BindFlags::VideoEncoder, _T("VideoEncoder")}
	});

JSON_SERIALIZE_ENUM(graphics::StageFlags::eStageFlags,
	{
	{graphics::StageFlags::VS, _T("VS")},
	{graphics::StageFlags::HS, _T("HS")},
	{graphics::StageFlags::DS, _T("DS")},
	{graphics::StageFlags::GS, _T("GS")},
	{graphics::StageFlags::PS, _T("PS")},
	{graphics::StageFlags::CS, _T("CS")},
	{graphics::StageFlags::AllTessStages, _T("AllTessStages")},
	{graphics::StageFlags::AllGraphicsStages, _T("AllgraphicsStages")},
	{graphics::StageFlags::AllStages, _T("AllStages")}
	});

JSON_SERIALIZE_ENUM(graphics::ResourceType,
	{
	{graphics::ResourceType::Undefined, _T("Undefined")},
	{graphics::ResourceType::Buffer, _T("Buffer")},
	{graphics::ResourceType::Texture, _T("Texture")},
	{graphics::ResourceType::Sampler, _T("Sampler")}
	});

/**** Shader Enum ****/

JSON_SERIALIZE_ENUM(graphics::ShaderType,
	{
	{graphics::ShaderType::Vertex, _T("Vertex")},
	{graphics::ShaderType::Pixel, _T("Pixel")},
	{graphics::ShaderType::Geometry, _T("Geometry")},
	{graphics::ShaderType::Hull, _T("Hull")},
	{graphics::ShaderType::Domain, _T("Domain")},
	{graphics::ShaderType::Compute, _T("Compute")},
	{graphics::ShaderType::Reserved0, _T("Reserved0")}
	});

JSON_SERIALIZE_ENUM(graphics::ShaderSourceType,
	{
	{graphics::ShaderSourceType::CSO, _T("CSO")},
	{graphics::ShaderSourceType::HLSL, _T("HLSL")}
	});

/**** Pipeline-State enum ****/

JSON_SERIALIZE_ENUM(graphics::CompareOp,
	{
	{graphics::CompareOp::NeverPass,    _T("NeverPass")},
	{graphics::CompareOp::Less,         _T("Less")},
	{graphics::CompareOp::Equal,        _T("Equal")},
	{graphics::CompareOp::LessEqual,    _T("LessEqual")},
	{graphics::CompareOp::Greater,      _T("Greater")},
	{graphics::CompareOp::NotEqual,     _T("NotEqual")},
	{graphics::CompareOp::GreaterEqual, _T("GreaterEqual")},
	{graphics::CompareOp::AlwaysPass,   _T("AlwaysPass")}
	});

JSON_SERIALIZE_ENUM(graphics::StencilOp,
	{
	{graphics::StencilOp::Keep    , _T("Keep")},
	{graphics::StencilOp::Zero    , _T("Zero")},
	{graphics::StencilOp::Replace , _T("Replace")},
	{graphics::StencilOp::IncClamp, _T("IncClamp")},
	{graphics::StencilOp::DecClamp, _T("DecClamp")},
	{graphics::StencilOp::Invert  , _T("Invert")},
	{graphics::StencilOp::IncWrap , _T("IncWrap")},
	{graphics::StencilOp::DecWrap , _T("DecWrap")}
	});

JSON_SERIALIZE_ENUM(graphics::FillMode,
	{
	{graphics::FillMode::WireFrame, _T("WireFrame")},
	{graphics::FillMode::Solid, _T("Solid")}
	});

JSON_SERIALIZE_ENUM(graphics::CullMode,
	{
	{graphics::CullMode::None, _T("None")},
	{graphics::CullMode::Front, _T("Front")},
	{graphics::CullMode::Back, _T("Back")}
	});

JSON_SERIALIZE_ENUM(graphics::Blend,
	{
	{graphics::Blend::Zero,             _T("Zero")},
	{graphics::Blend::One,              _T("One")},
	{graphics::Blend::SrcColor,         _T("SrcColor")},
	{graphics::Blend::InvSrcColor,      _T("InvSrcColor")},
	{graphics::Blend::SrcAlpha,         _T("SrcAlpha")},
	{graphics::Blend::InvSrcAlpha,      _T("InvSrcAlpha")},
	{graphics::Blend::DstColor,         _T("DstColor")},
	{graphics::Blend::InvDstColor,      _T("InvDstColor")},
	{graphics::Blend::DstAlpha,         _T("DstAlpha")},
	{graphics::Blend::InvDstAlpha,      _T("InvDstAlpha")},
	{graphics::Blend::SrcAlphaSaturate, _T("SrcAlphaSaturate")},
	{graphics::Blend::BlendFactor,      _T("BlendFactor")},
	{graphics::Blend::InvBlendFactor,   _T("InvBlendFactor")},
	{graphics::Blend::Src1Color,        _T("Src1Color")},
	{graphics::Blend::InvSrc1Color,     _T("InvSrc1Color")},
	{graphics::Blend::Src1Alpha,        _T("Src1Alpha")},
	{graphics::Blend::InvSrc1Alpha,      _T("InvSrc1Alpha")}
	});

JSON_SERIALIZE_ENUM(graphics::BlendOp,
	{
	{graphics::BlendOp::Add, _T("Add")},
	{graphics::BlendOp::Subtract, _T("Subtract")},
	{graphics::BlendOp::RevSubtract, _T("RevSubtract")},
	{graphics::BlendOp::Min, _T("Min")},
	{graphics::BlendOp::Max, _T("Max")}
	});

JSON_SERIALIZE_ENUM(graphics::PrimitiveTopology,
	{
	{graphics::PrimitiveTopology::PointList, _T("PointList")},
	{graphics::PrimitiveTopology::LineList, _T("LineList")},
	{graphics::PrimitiveTopology::LineStrip, _T("LineStrip")},
	{graphics::PrimitiveTopology::LineListAdjacency, _T("LineListAdjacency")},
	{graphics::PrimitiveTopology::LineStripAdjacency, _T("LineStripAdjacency")},
	{graphics::PrimitiveTopology::TriangleList, _T("TriangleList")},
	{graphics::PrimitiveTopology::TriangleStrip, _T("TriangleStrip")},
	{graphics::PrimitiveTopology::TriangleListAdjacency, _T("TriangleListAdjacency")},
	{graphics::PrimitiveTopology::TriangleStripAdjacency, _T("TriangleStripAdjacency")},
	{graphics::PrimitiveTopology::Patches1, _T("Patches1")},
	{graphics::PrimitiveTopology::Patches2, _T("Patches2")},
	{graphics::PrimitiveTopology::Patches3, _T("Patches3")},
	{graphics::PrimitiveTopology::Patches4, _T("Patches4")},
	{graphics::PrimitiveTopology::Patches5, _T("Patches5")},
	{graphics::PrimitiveTopology::Patches6, _T("Patches6")},
	{graphics::PrimitiveTopology::Patches7, _T("Patches7")},
	{graphics::PrimitiveTopology::Patches8, _T("Patches8")},
	{graphics::PrimitiveTopology::Patches9, _T("Patches9")},
	{graphics::PrimitiveTopology::Patches10, _T("Patches10")},
	{graphics::PrimitiveTopology::Patches11, _T("Patches11")},
	{graphics::PrimitiveTopology::Patches12, _T("Patches12")},
	{graphics::PrimitiveTopology::Patches13, _T("Patches13")},
	{graphics::PrimitiveTopology::Patches14, _T("Patches14")},
	{graphics::PrimitiveTopology::Patches15, _T("Patches15")},
	{graphics::PrimitiveTopology::Patches16, _T("Patches16")},
	{graphics::PrimitiveTopology::Patches17, _T("Patches17")},
	{graphics::PrimitiveTopology::Patches18, _T("Patches18")},
	{graphics::PrimitiveTopology::Patches19, _T("Patches19")},
	{graphics::PrimitiveTopology::Patches20, _T("Patches20")},
	{graphics::PrimitiveTopology::Patches21, _T("Patches21")},
	{graphics::PrimitiveTopology::Patches22, _T("Patches22")},
	{graphics::PrimitiveTopology::Patches23, _T("Patches23")},
	{graphics::PrimitiveTopology::Patches24, _T("Patches24")},
	{graphics::PrimitiveTopology::Patches25, _T("Patches25")},
	{graphics::PrimitiveTopology::Patches26, _T("Patches26")},
	{graphics::PrimitiveTopology::Patches27, _T("Patches27")},
	{graphics::PrimitiveTopology::Patches28, _T("Patches28")},
	{graphics::PrimitiveTopology::Patches29, _T("Patches29")},
	{graphics::PrimitiveTopology::Patches30, _T("Patches30")},
	{graphics::PrimitiveTopology::Patches31, _T("Patches31")},
	{graphics::PrimitiveTopology::Patches32, _T("Patches32")},
	});

/**** Sampler Enum ****/

JSON_SERIALIZE_ENUM(graphics::Filter,
	{
	{graphics::Filter::MIN_MAG_MIP_POINT, _T("MIN_MAG_MIP_POINT")},
	{graphics::Filter::MIN_MAG_POINT_MIP_LINEAR, _T("MIN_MAG_POINT_MIP_LINEAR")},
	{graphics::Filter::MIN_POINT_MAG_LINEAR_MIP_POINT, _T("MIN_POINT_MAG_LINEAR_MIP_POINT")},
	{graphics::Filter::MIN_POINT_MAG_MIP_LINEAR, _T("MIN_POINT_MAG_MIP_LINEAR")},
	{graphics::Filter::MIN_LINEAR_MAG_MIP_POINT, _T("MIN_LINEAR_MAG_MIP_POINT")},
	{graphics::Filter::MIN_LINEAR_MAG_POINT_MIP_LINEAR, _T("MIN_LINEAR_MAG_POINT_MIP_LINEAR")},
	{graphics::Filter::MIN_MAG_LINEAR_MIP_POINT, _T("MIN_MAG_LINEAR_MIP_POINT")},
	{graphics::Filter::MIN_MAG_MIP_LINEAR, _T("MIN_MAG_MIP_LINEAR")},
	{graphics::Filter::ANISOTROPIC, _T("ANISOTROPIC")},
	{graphics::Filter::COMPARISON_MIN_MAG_MIP_POINT, _T("COMPARISON_MIN_MAG_MIP_POINT")},
	{graphics::Filter::COMPARISON_MIN_MAG_POINT_MIP_LINEAR, _T("COMPARISON_MIN_MAG_POINT_MIP_LINEAR")},
	{graphics::Filter::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT, _T("COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT")},
	{graphics::Filter::COMPARISON_MIN_POINT_MAG_MIP_LINEAR, _T("COMPARISON_MIN_POINT_MAG_MIP_LINEAR")},
	{graphics::Filter::COMPARISON_MIN_LINEAR_MAG_MIP_POINT, _T("COMPARISON_MIN_LINEAR_MAG_MIP_POINT")},
	{graphics::Filter::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR, _T("COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR")},
	{graphics::Filter::COMPARISON_MIN_MAG_LINEAR_MIP_POINT, _T("COMPARISON_MIN_MAG_LINEAR_MIP_POINT")},
	{graphics::Filter::COMPARISON_MIN_MAG_MIP_LINEAR, _T("COMPARISON_MIN_MAG_MIP_LINEAR")},
	{graphics::Filter::COMPARISON_ANISOTROPIC, _T("COMPARISON_ANISOTROPIC")},
	{graphics::Filter::MINIMUM_MIN_MAG_MIP_POINT, _T("MINIMUM_MIN_MAG_MIP_POINT")},
	{graphics::Filter::MINIMUM_MIN_MAG_POINT_MIP_LINEAR, _T("MINIMUM_MIN_MAG_POINT_MIP_LINEAR")},
	{graphics::Filter::MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT, _T("MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT")},
	{graphics::Filter::MINIMUM_MIN_POINT_MAG_MIP_LINEAR, _T("MINIMUM_MIN_POINT_MAG_MIP_LINEAR")},
	{graphics::Filter::MINIMUM_MIN_LINEAR_MAG_MIP_POINT, _T("MINIMUM_MIN_LINEAR_MAG_MIP_POINT")},
	{graphics::Filter::MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR, _T("MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR")},
	{graphics::Filter::MINIMUM_MIN_MAG_LINEAR_MIP_POINT, _T("MINIMUM_MIN_MAG_LINEAR_MIP_POINT")},
	{graphics::Filter::MINIMUM_MIN_MAG_MIP_LINEAR, _T("MINIMUM_MIN_MAG_MIP_LINEAR")},
	{graphics::Filter::MINIMUM_ANISOTROPIC, _T("MINIMUM_ANISOTROPIC")},
	{graphics::Filter::MAXIMUM_MIN_MAG_MIP_POINT, _T("MAXIMUM_MIN_MAG_MIP_POINT")},
	{graphics::Filter::MAXIMUM_MIN_MAG_POINT_MIP_LINEAR, _T("MAXIMUM_MIN_MAG_POINT_MIP_LINEAR")},
	{graphics::Filter::MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT, _T("MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POIN,")},
	{graphics::Filter::MAXIMUM_MIN_POINT_MAG_MIP_LINEAR, _T("MAXIMUM_MIN_POINT_MAG_MIP_LINEAR")},
	{graphics::Filter::MAXIMUM_MIN_LINEAR_MAG_MIP_POINT, _T("MAXIMUM_MIN_LINEAR_MAG_MIP_POINT")},
	{graphics::Filter::MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR, _T("MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR")},
	{graphics::Filter::MAXIMUM_MIN_MAG_LINEAR_MIP_POINT, _T("MAXIMUM_MIN_MAG_LINEAR_MIP_POINT")},
	{graphics::Filter::MAXIMUM_MIN_MAG_MIP_LINEAR, _T("MAXIMUM_MIN_MAG_MIP_LINEAR")},
	{graphics::Filter::MAXIMUM_ANISOTROPIC, _T("MAXIMUM_ANISOTROPIC")}
	});

JSON_SERIALIZE_ENUM(graphics::SamplerAddressMode,
	{
	{graphics::SamplerAddressMode::Wrap, _T("Wrap")},
	{graphics::SamplerAddressMode::Mirror, _T("Mirror")},
	{graphics::SamplerAddressMode::Clamp, _T("Clamp")},
	{graphics::SamplerAddressMode::Border, _T("Border")},
	{graphics::SamplerAddressMode::MirrorOnce, _T("MirrorOnce")},
	});