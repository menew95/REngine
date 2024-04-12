/**

	@file      MeshBin.h
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
#include <common\vertex_attribute.h>

namespace utility
{
	struct MeshBin
	{
		friend boost::serialization::access;

		MeshBin() = default;

		MeshBin(std::string _meshName, std::vector<VertexAttribute> _meshVertexList
			, std::vector<std::vector<uint32>> _indices, math::Vector3 boundingMin, math::Vector3 boundingMax)
			: _name(std::move(_meshName))
			, _vertices(std::move(_meshVertexList))
			, _indices(std::move(_indices))
			, _boundingMinBox(boundingMin)
			, _boundingMaxBox(boundingMax)
			, _isSkinned(false)
		{}

		MeshBin(std::string _meshName, std::vector<VertexAttribute> _meshVertexList
			, std::vector<std::vector<uint32>> _indices, math::Vector3 boundingMin, math::Vector3 boundingMax, std::string bone)
			: _name(std::move(_meshName))
			, _vertices(std::move(_meshVertexList))
			, _indices(std::move(_indices))
			, _boundingMinBox(boundingMin)
			, _boundingMaxBox(boundingMax)
			, _isSkinned(true)
			, _boneName(std::move(bone))
		{}

		std::string							_name = "";
		std::vector<VertexAttribute>		_vertices;
		std::vector<std::vector<uint32>>	_indices;

		math::Vector3 _boundingMinBox = { FLT_MAX, FLT_MAX, FLT_MAX };
		math::Vector3 _boundingMaxBox = { FLT_MIN, FLT_MIN, FLT_MIN };

		bool _isSkinned = false;
		std::string _boneName = "";

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _name;
			ar& _vertices;
			ar& _indices;
			ar& _boundingMinBox;
			ar& _boundingMaxBox;
			ar& _isSkinned;
			ar& _boneName;
		}
	};

	struct BoneBin
	{
		friend boost::serialization::access;

		BoneBin() = default;

		BoneBin(std::string boneName, std::string parentBoneName, int parentIndex
			, math::Matrix offsetMatrix, math::Matrix localMatrix, math::Matrix worldMatrix)
			: _boneName(std::move(boneName))
			, _parentBoneName(std::move(parentBoneName))
			, _parentIndex(std::move(parentIndex))
			, _offsetMatrix(std::move(offsetMatrix))
			, _localMatrix(std::move(localMatrix))
			, _worldMatrix(std::move(worldMatrix))
		{}

		std::string	_boneName = "";
		std::string	_parentBoneName = "";

		short 		_parentIndex = 0;

		math::Matrix	_offsetMatrix = math::Matrix::Identity;
		math::Matrix	_localMatrix = math::Matrix::Identity;
		math::Matrix	_worldMatrix = math::Matrix::Identity;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _boneName;
			ar& _parentBoneName;
			ar& _parentIndex;
			ar& _offsetMatrix;
			ar& _localMatrix;
			ar& _worldMatrix;
		}
	};

	struct SkinnedBin
	{
		std::vector<BoneBin> _boneBins;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _boneBins;
		}
	};
}