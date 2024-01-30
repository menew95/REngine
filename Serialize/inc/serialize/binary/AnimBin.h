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

#include <rengine\core\resource\AnimationClip.h>

namespace utility
{
	struct KeyFrameBin
	{
		float _frameRate;
		math::Vector4 _data;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _frameRate;
			ar& _data;
		}

		rengine::KeyFrame Convert()
		{
			rengine::KeyFrame _snap;

			_snap.m_FrameRate = _frameRate;
			_snap.m_Data = _data;

			return std::move(_snap);
		}

		auto& operator=(const rengine::KeyFrame& frame)
		{
			_frameRate = frame.m_FrameRate;
			_data = frame.m_Data;

			return *this;
		}
	};

	struct AnimationSnapBin
	{
		AnimationSnapBin() = default;

		AnimationSnapBin(std::string name, std::string parent
		, std::vector<KeyFrameBin> posKeyFrameList
		, std::vector<KeyFrameBin> rotKeyFrameList
		, std::vector<KeyFrameBin> scaleKeyFrameList)
			: _nodeName(std::move(name))
			, _parentNodeName(std::move(parent))
			, _posKeyFrameList(std::move(posKeyFrameList))
			, _rotKeyFrameList(std::move(rotKeyFrameList))
			, _scaleKeyFrameList(std::move(scaleKeyFrameList))
		{}

		std::string _nodeName;
		std::string _parentNodeName;

		std::vector<KeyFrameBin> _posKeyFrameList;
		std::vector<KeyFrameBin> _rotKeyFrameList;
		std::vector<KeyFrameBin> _scaleKeyFrameList;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _nodeName;
			ar& _parentNodeName;
			ar& _posKeyFrameList;
			ar& _rotKeyFrameList;
			ar& _scaleKeyFrameList;
		}

		rengine::AnimationSnap Convert()
		{
			rengine::AnimationSnap _snap;

			_snap._target = StringHelper::StringToWString(_nodeName);

			_snap._posKeyFrameList.resize(_posKeyFrameList.size());
			_snap._rotKeyFrameList.resize(_rotKeyFrameList.size());
			_snap._scaleKeyFrameList.resize(_scaleKeyFrameList.size());

			for (size_t i = 0; i < _posKeyFrameList.size(); i++)
			{
				_snap._posKeyFrameList[i] = _posKeyFrameList[i].Convert();
			}
			for (size_t i = 0; i < _rotKeyFrameList.size(); i++)
			{
				_snap._rotKeyFrameList[i] = _rotKeyFrameList[i].Convert();
			}
			for (size_t i = 0; i < _scaleKeyFrameList.size(); i++)
			{
				_snap._scaleKeyFrameList[i] = _scaleKeyFrameList[i].Convert();
			}

			return _snap;
		}

		auto& operator=(const rengine::AnimationSnap& snap)
		{
			rengine::AnimationSnap _snap;

			_snap._target = StringHelper::StringToWString(_nodeName);

			_posKeyFrameList.resize(snap._posKeyFrameList.size());
			_rotKeyFrameList.resize(snap._rotKeyFrameList.size());
			_scaleKeyFrameList.resize(snap._scaleKeyFrameList.size());

			for (size_t i = 0; i < _posKeyFrameList.size(); i++)
			{
				_posKeyFrameList[i] = snap._posKeyFrameList[i];
			}
			for (size_t i = 0; i < _rotKeyFrameList.size(); i++)
			{
				_rotKeyFrameList[i] = snap._rotKeyFrameList[i];
			}
			for (size_t i = 0; i < _scaleKeyFrameList.size(); i++)
			{
				_scaleKeyFrameList[i] = snap._scaleKeyFrameList[i];
			}

			return *this;
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

		auto& operator=(const rengine::AnimationClip& clip)
		{
			assert(false);
			return *this;
		}
	};

}