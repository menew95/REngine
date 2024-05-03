/**

    @file      GraphicsEngineFlags.h
    @brief     graphics engine setting flags
    @details   ~
    @author    LWT
    @date      5.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#pragma once

#include <common/common.h>

#include <common/math.h>

namespace graphics
{
	enum class API
	{
		DirectX11 = 1,
	};

	struct GraphicsEngineDesc
	{
		API _module = API::DirectX11;

		WindowInfo _info;
	};

    struct SceneInfo
    {
        float _deltaTime;
        float _timeStep;
    };

#pragma region ScreenSpaceReflection
    struct alignas(16) ScreenSpaceReflection
    {
        ScreenSpaceReflection() = default;
        ~ScreenSpaceReflection() = default;

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
    };
#pragma endregion

#pragma region ExponentialHeightFog
    struct alignas(16) ExponentialHeightFog
    {
        ExponentialHeightFog() = default;
        ~ExponentialHeightFog() = default;

        float _fogDensity;		  //밀도
        float _fogMaxOpacity;     //최대 안개 수치(0~1사이의 값)
        float _fogStartDistance;  //카메라로부터의 거리
        float _fogCutOffDistance; //카메라로부터 안개가 끝나는 지점의 거리

        float _fogHeightFalloff;  //높이 exponential의 수치.
        math::Vector3 _fogHeight;

        math::Vector3 _fogScatteringColor;
        float _lightScatterFlag; //directional light가 있을 넣어준다. 아니면 -1
    };
#pragma endregion

#pragma region RimLight
    struct alignas(16) RimLight
    {
        RimLight() = default;
        ~RimLight() = default;

        float _clipPoint;

        float _rimDecrease;

        Vector2 _rim_padding0 = Vector2::Zero;
    };
#pragma endregion

#pragma region bloom
    struct alignas(16) Bloom
    {
        Bloom() = default;
        ~Bloom() = default;

        float _intensity;

        float _threshhold;

        Vector2 _bloom_padding0 = Vector2::Zero;
    };
#pragma endregion

#pragma region SSAO

    constexpr int SSAO_MAX_KERNEL_COUNT = 64;
    constexpr int SSAO_ROTATION_NOISE = 16;

    struct alignas(16) SSAO
    {
        SSAO() = default;
        ~SSAO() = default;

        math::Vector4 _ssaoSampleKernel[SSAO_MAX_KERNEL_COUNT];

        math::Vector4 _ssaoRotationNoise[SSAO_ROTATION_NOISE];
    };
#pragma endregion

#pragma region Tone mapping
    struct alignas(16) ToneMapping
    {
        ToneMapping() = default;
        ~ToneMapping() = default;
        float _exposure = 1.0f;

        Vector3 _tone_padding0 = Vector3::Zero;
    };
#pragma endregion

    struct alignas(16) PostProcessingSetting
    {
        PostProcessingSetting() = default;
        ~PostProcessingSetting() = default;

        ScreenSpaceReflection _screenSpaceReflectionSetting;

        ExponentialHeightFog _exponentialHeightFogSettig;

        RimLight _rimLightSetting;

        Bloom _bloomSetting;

        SSAO _ssaoSetting;

        ToneMapping _toneMappingSetting;
    };

    struct alignas(16) GraphicsSetting
    {
        GraphicsSetting() = default;

        ~GraphicsSetting() = default;

        PostProcessingSetting _postProcessingSetting;
    };
}