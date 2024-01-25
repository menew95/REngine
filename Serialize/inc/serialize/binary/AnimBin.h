/**

	@file      AnimBin.h
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
	struct KeyFrameBin
	{
		float _time;

		math::Vector3 _pos;
		math::Quaternion _rot;
		math::Vector3 _scale;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _time;
			ar& _pos;
			ar& _rot;
			ar& _scale;
		}
	};

	struct AnimationSnapBin
	{
		AnimationSnapBin() = default;

		AnimationSnapBin(std::string name, std::string parent, std::vector<KeyFrameBin> keyFrameList)
			: _nodeName(std::move(name))
			, _parentNodeName(std::move(parent))
			, _keyFrameList(std::move(keyFrameList))
		{}

		std::string _nodeName;
		std::string _parentNodeName;

		std::vector<KeyFrameBin> _keyFrameList;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _nodeName;
			ar& _parentNodeName;
			ar& _keyFrameList;
		}
	};

	struct AnimationClipBin
	{
		AnimationClipBin() = default;

		AnimationClipBin(std::string clipName, std::vector<AnimationSnapBin> snapList
			, float frameRate, uint32 tickPerTime, uint32 totalKeyFrame, uint32 startKeyFrame, uint32 endKeyFrame)
			: _clipName(clipName)
			, _snapList(snapList)
			, _frameRate(frameRate)
			, _tickPerFrame(tickPerTime)
			, _totalKeyFrame(totalKeyFrame)
			, _startKeyFrame(startKeyFrame)
			, _endKeyFrame(endKeyFrame)
		{}

		std::string _clipName;
		std::vector<AnimationSnapBin> _snapList;


		float _frameRate; // 엔진에서 60프레임으로 돌린다고 했을때, 한 프레임을 얼만큼 보여줄건지에 대한 비율
		uint32 _tickPerFrame;
		uint32 _totalKeyFrame;
		uint32 _startKeyFrame;
		uint32 _endKeyFrame;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _clipName;
			ar& _snapList;

			ar& _frameRate;
			ar& _tickPerFrame;
			ar& _totalKeyFrame;
			ar& _startKeyFrame;
			ar& _endKeyFrame;
		}
	};

}