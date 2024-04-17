/**

    @file      GraphicsSetting.h
    @brief     graphics setting
    @details   scene에 적용 될 post processing과 같은 정보들
    @author    LWT
    @date      17.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\math.h>

#include <rengine\core\object\MetaInfo.h>

#include <rengine\rengine_dllexport.h>

#include <rttr\registration_friend.h>

#include <rttr\rttr_enable.h>

namespace rengine
{

#pragma region ScreenSpaceReflection
    struct RENGINE_API ScreenSpaceReflection
    {
        ScreenSpaceReflection() = default;
        ~ScreenSpaceReflection() = default;

        float   GetStride()                 { return _stride; }
        void    SetStride(float val)        { _stride = val; }

        float   GetMaxSteps()               { return _maxSteps; }
        void    SetMaxSteps(float val)      { _maxSteps = val; }

        float   GetMaxDistance()            { return _maxDistance; }
        void    SetMaxDistance(float val)   { _maxDistance = val; }

        float   GetStrideZCutoff()          { return _strideZCutoff; }
        void    SetStrideZCutoff(float val) { _strideZCutoff = val; }

        float   GetNumMips()                { return _numMips; }
        void    SetNumMips(float val)       { _numMips = val; }

        float   GetFadeStart()              { return _fadeStart; }
        void    SetFadeStart(float val)     { _fadeStart = val; }

        float   GetFadeEnd()                { return _fadeEnd; }
        void    SetFadeEnd(float val)       { _fadeEnd = val; }

        float   GetZThickness()             { return _zThickness; }
        void    SetZThickness(float val)    { _zThickness = val; }

        float _stride;
        // Step in horizontal or vertical pixels between samples. This is a float
        // because integer math is slow on GPUs, but should be set to an integer >= 1.
        float _maxSteps;    // Maximum number of iterations. Higher gives better images but may be slow.
        float _maxDistance; // Maximum camera-space distance to trace before returning a miss.
        float _strideZCutoff;
        // More distant pixels are smaller in screen space. This value tells at what point to
        // start relaxing the stride to give higher quality reflections for objects far from
        // the camera.

        float _numMips;       // the number of mip levelses in the convolved color buffer
        float _fadeStart;     // determines where to start screen edge fading of effect
        float _fadeEnd;       // determines where to end screen edge fading of effect
        float _sslr_padding0; // padding for alignment

        float _zThickness; // thickness to ascribe to each pixel in the depth buffer
        Vector3 _sslr_padding1 = Vector3::Zero;

        RTTR_REGISTRATION_FRIEND

        RTTR_ENABLE()
    };
#pragma endregion

#pragma region ExponentialHeightFog
    struct RENGINE_API ExponentialHeightFog
    {
        ExponentialHeightFog() = default;
        ~ExponentialHeightFog() = default;

        float           GetFogEnsity()                              { return _fogDensity; }
        void            SetFogEnsity(float val)                     { _fogDensity = val; }

        float           GetFogMaxOpacity()                          { return _fogMaxOpacity; }
        void            SetFogMaxOpacity(float val)                 { _fogMaxOpacity = val; }

        float           GetFogStartDistance()                       { return _fogStartDistance; }
        void            SetFogStartDistance(float val)              { _fogStartDistance = val; }

        float           GetCutOffDistance()                         { return _fogCutOffDistance; }
        void            SetCutOffDistance(float val)                { _fogCutOffDistance = val; }

        float           GetFogHeightFalloff()                       { return _fogHeightFalloff; }
        void            SetFogHeightFalloff(float val)              { _fogHeightFalloff = val; }

        const Vector3&  GetFogHeight()                              { return _fogHeight; }
        void            SetFogHeight(const Vector3& val)            { _fogHeight = val; }

        const Vector3&  GetFogScatteringColor()                     { return _fogScatteringColor; }
        void            SetFogScatteringColor(const Vector3& val)   { _fogScatteringColor = val; }

        float           GetLightScatterFlag()                       { return _lightScatterFlag; }
        void            SetLightScatterFlag(float val)              { _lightScatterFlag = val; }

        float _fogDensity;		  //밀도
        float _fogMaxOpacity;     //최대 안개 수치(0~1사이의 값)
        float _fogStartDistance;  //카메라로부터의 거리
        float _fogCutOffDistance; //카메라로부터 안개가 끝나는 지점의 거리

        float _fogHeightFalloff;  //높이 exponential의 수치.
        math::Vector3 _fogHeight;

        math::Vector3 _fogScatteringColor;
        float _lightScatterFlag; //directional light가 있을 넣어준다. 아니면 -1

        RTTR_REGISTRATION_FRIEND

        RTTR_ENABLE()
    };
#pragma endregion

#pragma region RimLight
    struct RENGINE_API RimLight
    {
        RimLight() = default;
        ~RimLight() = default;

        float   GetClipPoint()              { return _clipPoint; }
        void    SetClipPoint(float val)     { _clipPoint = val; }

        float   GetRimDecrease()            { return _rimDecrease; }
        void    SetRimDecrease(float val)   { _rimDecrease = val; }

        float _clipPoint;

        float _rimDecrease;

        Vector2 _rim_padding0 = Vector2::Zero;

        RTTR_REGISTRATION_FRIEND

        RTTR_ENABLE()
    };
#pragma endregion

#pragma region bloom
    struct RENGINE_API Bloom
    {
        Bloom() = default;
        ~Bloom() = default;

        float   GetIntensity()              { return _intensity; }
        void    SetIntensity(float val)     { _intensity = val; }

        float   GetThreshhold()             { return _threshhold; }
        void    SetThreshhold(float val)    { _threshhold = val; }

        float _intensity;

        float _threshhold;

        Vector2 _bloom_padding0 = Vector2::Zero;
        
        RTTR_REGISTRATION_FRIEND

        RTTR_ENABLE()
    };
#pragma endregion

#pragma region SSAO

    constexpr int SSAO_MAX_KERNEL_COUNT = 64;
    constexpr int SSAO_ROTATION_NOISE = 16;

    struct RENGINE_API SSAO
    {
        SSAO() = default;
        ~SSAO() = default;

        //const Vector4 GetSSAOSampleKernel() { return _ssaoSampleKernel; }
        //void    SetSSAOSampleKernel(const Vector4& val) { _ssaoSampleKernel = val; }

        //float   GetSSAORotationNoise() { return _ssaoRotationNoise; }
        //void    SetSSAORotationNoise(const Vector& val) { _ssaoRotationNoise = val; }

        math::Vector4 _ssaoSampleKernel[SSAO_MAX_KERNEL_COUNT];

        math::Vector4 _ssaoRotationNoise[SSAO_ROTATION_NOISE]; 
        
        RTTR_REGISTRATION_FRIEND

        RTTR_ENABLE()
    };
#pragma endregion

#pragma region Tone mapping
    struct RENGINE_API ToneMapping
    {
        ToneMapping() = default;
        ~ToneMapping() = default;

        float   GetExposure()           { return _exposure; }
        void    SetExposure(float val)  { _exposure = val; }

        float _exposure = 1.0f;

        Vector3 _tone_padding0 =  Vector3::Zero;

        RTTR_REGISTRATION_FRIEND

        RTTR_ENABLE()
    };
#pragma endregion

    struct RENGINE_API PostProcessingSetting
    {
        PostProcessingSetting() = default;
        ~PostProcessingSetting() = default;

        const ScreenSpaceReflection& GetScreenSpaceReflection() { return _screenSpaceReflectionSetting; }
        void SetScreenSpaceReflection(const ScreenSpaceReflection& val) { _screenSpaceReflectionSetting = val; }

        const ExponentialHeightFog& GetExponentialHeightFog() { return _exponentialHeightFogSettig; }
        void SetExponentialHeightFog(const ExponentialHeightFog& val) { _exponentialHeightFogSettig = val; }

        const RimLight& GetRimLight() { return _rimLightSetting; }
        void SetRimLight(const RimLight& val) { _rimLightSetting = val; }

        const Bloom& GetBloom() { return _bloomSetting; }
        void SetBloom(const Bloom& val) { _bloomSetting = val; }

        const SSAO& GetSSAO() { return _ssaoSetting; }
        void SetSSAO(const SSAO& val) { _ssaoSetting = val; }

        const ToneMapping& GetToneMapping() { return _toneMappingSetting; }
        void SetToneMapping(const ToneMapping& val) { _toneMappingSetting = val; }

        ScreenSpaceReflection _screenSpaceReflectionSetting;

        ExponentialHeightFog _exponentialHeightFogSettig;

        RimLight _rimLightSetting;

        Bloom _bloomSetting;
        
        SSAO _ssaoSetting;

        ToneMapping _toneMappingSetting;

        RTTR_REGISTRATION_FRIEND

        RTTR_ENABLE()
    };

    struct RENGINE_API GraphicsSetting
    {
        GraphicsSetting() = default;

        ~GraphicsSetting() = default;

        const PostProcessingSetting& GetPostProcessingSetting() { return _postProcessingSetting; }

        PostProcessingSetting _postProcessingSetting;

        RTTR_REGISTRATION_FRIEND

        RTTR_ENABLE()
    };
}