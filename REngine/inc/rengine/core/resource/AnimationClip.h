/**

    @file      AnimationClip.h
    @brief     animation clip class
    @details   ~
    @author    LWT
    @date      29.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\math.h>

#include <rengine\core\resource\Resource.h>

namespace rengine
{
    struct KeyFrame
    {
        KeyFrame() = default;
        KeyFrame(float rate, math::Vector3 data)
            : m_FrameRate{ rate }
            , m_Data{ data }
        {}
        KeyFrame(float rate, math::Quaternion data)
            : m_FrameRate{ rate }
            , m_Data{ data }
        {}
        float m_FrameRate;
        math::Vector4 m_Data;
    };

    struct AnimationSnap
    {
        enum eKeyType
        {
            Position,
            Rotate,
            Scale,
        };

        /**
         * \brief 애니메이션 초기화시 타겟될 게임 오브젝트를 찾기 위한 값 게임 오브젝트의 이름과 같은 값을 타겟 해준다.
         */
        tstring _target;

        /**
         * \brief 이 스냅이 담고있는 애니메이션의 최대 길이 값
         */
        float _maxFrameRate = 0;

        /**
         * \brief Transform Animation 를 담고 있는 컨테이너
         */
        std::vector<KeyFrame> _posKeyFrameList;
        std::vector<KeyFrame> _rotKeyFrameList;
        std::vector<KeyFrame> _scaleKeyFrameList;
    };

    class AnimationEvent
    {
    public:
        AnimationEvent() = default;

        ~AnimationEvent() = default;

    private:

        /**
         * \brief 애니메이션 초기화시 타겟될 게임 오브젝트를 찾기 위한 값 게임 오브젝트의 이름과 같은 값을 타겟 해준다.
         */
        tstring _target;

        /**
         * \brief 이 스냅이 담고있는 애니메이션의 최대 길이 값
         */
        float _maxFrameRate = 0;

        /**
         * \brief 파라미터 정보
         */
        float _floatParam;
        int _intParam;
        tstring _strParam;

        /**
         * \brief 함수 이벤트 파라미터 정보
         */
        tstring _funcName;
        tstring _compName;
    };

    class AnimationCurve
    {
    public:
        AnimationCurve() = default;

        ~AnimationCurve() = default;

    private:

        vector<KeyFrame> _keys;

        float _length;
    };

    class RENGINE_API AnimationClip : public Resource, public enable_shared_from_this<AnimationClip>
    {
    public:
        AnimationClip(uuid uuid);

        AnimationClip(const AnimationClip& clip) = default;

        AnimationClip(AnimationClip&& clip) = default;

        virtual ~AnimationClip();

        ResourceType GetResourceType() override
        {
            return ResourceType::ANIMATION_CLIP;
        };

        const auto& GetSnaps()            { return m_Snaps; }
        const auto& GetFrameRate()        { return m_frameRate; }
        const auto& GetTickPerFrame()     { return m_tickPerFrame; }
        const auto& GetTotalKeyFrame()    { return m_totalKeyFrame; }
        const auto& GetStartKeyFrame()    { return m_startKeyFrame; }
        const auto& GetEndKeyFrame()      { return m_endKeyFrame; }

        void SetSnaps(const auto& val) { m_Snaps  = val; }
        void SetFrameRate(const auto& val) { m_frameRate  = val; }
        void SetTickPerFrame(const auto& val) { m_tickPerFrame  = val; }
        void SetTotalKeyFrame(const auto& val) { m_totalKeyFrame  = val; }
        void SetStartKeyFrame(const auto& val) { m_startKeyFrame  = val; }
        void SetEndKeyFrame(const auto& val) { m_endKeyFrame  = val; }

    protected:
        vector<AnimationSnap> m_Snaps;

        float m_frameRate = 0.f; // 엔진에서 60프레임으로 돌린다고 했을때, 한 프레임을 얼만큼 보여줄건지에 대한 비율
        uint32 m_tickPerFrame = 0;
        uint32 m_totalKeyFrame = 0;
        uint32 m_startKeyFrame = 0;
        uint32 m_endKeyFrame = 0;

        RTTR_ENABLE(Resource);

        RTTR_REGISTRATION_FRIEND;
    };
}