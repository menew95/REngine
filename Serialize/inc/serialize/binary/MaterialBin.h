/**

	@file      MateriaBin.h
	@brief     material asset binary info
	@details   ~
	@author    LWT
	@date      25.01.2024
	@copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\math.h>

#include <boost\serialization\access.hpp>

namespace utility
{
	struct MaterialBin
	{
		friend boost::serialization::access;

		MaterialBin() = default;

		MaterialBin(std::string materialName
			, std::string albedoMap, std::string normalMap, std::string metalicRoughnessMap
			, std::string emissiveMap, std::string ambientMap, std::string specularMap
			, math::Vector4 ambientColor, math::Vector4 diffuseColor, math::Vector4 specularColor, math::Vector4 emissiveColor
			//, Float4 ambientColor, Float4 diffuseColor, Float4 specularColor, Float4 emissiveColor
			, float emissionFactor, float transparency, float reflectivity, float metallic, float roughness)
			: _materialName(std::move(materialName))

			, _albedoMapTexture(std::move(albedoMap))
			, _normalMapTexture(std::move(normalMap))
			, _metallicRoughnessMapTexture(std::move(metalicRoughnessMap))
			, _emissiveMapTexture(std::move(emissiveMap))
			, _ambientMapTexture(std::move(ambientMap))
			, _specularMapTexture(std::move(specularMap))

			, _diffuse(std::move(diffuseColor))
			, _ambient(std::move(ambientColor))
			, _specular(std::move(specularColor))
			, _emissive(std::move(emissiveColor))

			, _emissionFactor(std::move(emissionFactor))
			, _transparency(std::move(transparency))
			, _reflectivity(std::move(reflectivity))
			, _metallic(std::move(metallic))
			, _roughness(std::move(roughness))
		{}

		std::string _materialName = "";

		std::string _albedoMapTexture = "";
		std::string _normalMapTexture = "";
		std::string _metallicRoughnessMapTexture = "";
		std::string _emissiveMapTexture = "";
		std::string _ambientMapTexture = "";
		std::string _specularMapTexture = "";

		math::Color _ambient = { 0.f, 0.f, 0.f, 0.f };
		math::Color _diffuse = { 0.f, 0.f, 0.f, 0.f };
		math::Color _specular = { 0.f, 0.f, 0.f, 0.f };
		math::Color _emissive = { 0.f, 0.f, 0.f, 0.f };

		float _emissionFactor = 0.f;
		float _transparency = 0.f;
		float _reflectivity = 0.f;

		float _metallic = 0.f;
		float _roughness = 0.f;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _materialName;

			ar& _albedoMapTexture;
			ar& _normalMapTexture;
			ar& _metallicRoughnessMapTexture;
			ar& _ambientMapTexture;
			ar& _emissiveMapTexture;
			ar& _specularMapTexture;

			ar& _ambient;
			ar& _diffuse;
			ar& _specular;
			ar& _emissive;

			ar& _transparency;
			ar& _reflectivity;

			ar& _metallic;
			ar& _roughness;
		}
	};
}