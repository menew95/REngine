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
			: meshName(std::move(_meshName))
			, meshVertexList(std::move(_meshVertexList))
			, indices(std::move(_indices))
			, _boundingMinBox(boundingMin)
			, _boundingMaxBox(boundingMax)
			, _isSkinned(false)
		{}

		MeshBin(std::string _meshName, std::vector<VertexAttribute> _meshVertexList
			, std::vector<std::vector<uint32>> _indices, math::Vector3 boundingMin, math::Vector3 boundingMax, std::string bone)
			: meshName(std::move(_meshName))
			, meshVertexList(std::move(_meshVertexList))
			, indices(std::move(_indices))
			, _boundingMinBox(boundingMin)
			, _boundingMaxBox(boundingMax)
			, _isSkinned(true)
			, _boneName(std::move(bone))
		{}

		std::string								meshName = "";
		std::vector<VertexAttribute>	meshVertexList;
		std::vector<std::vector<uint32>>		indices;

		math::Vector3 _boundingMinBox;
		math::Vector3 _boundingMaxBox;

		bool _isSkinned;
		std::string _boneName;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& meshName;
			ar& meshVertexList;
			ar& indices;
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

		math::Matrix	_offsetMatrix;
		math::Matrix	_localMatrix;
		math::Matrix	_worldMatrix;

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