/**

    @file      Light.h
    @brief     Light Component
    @details   ~
    @author    LWT
    @date      3.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>
#include <common\math.h>

#include <rengine\core\component\Component.h>

namespace graphics
{
	class LightBuffer;
}

namespace rengine
{
    enum class LightType
    {
        Spot = 0,
        Directional = 1,
        Point = 2,
    };

	enum class ShadowResolution
    {
		Low = 0,
		Medium = 1,
		High = 2,
		VeryHigh = 3,
    };

    class Light : public Component, public enable_shared_from_this<Light>
    {
	public:
		Light(uuid uuid);

		Light(const Light& filter) = default;

		Light(Light&& filter) = default;

		~Light();

		void Awake() override;

		void Update() override;

		void OnEnable() override;

		void OnDisable() override;

		void OnDestroy() override;

		inline uint32 GetLightType() { return static_cast<uint32>(m_lightType); }
		void SetLightType(uint32 value);

		inline const math::Color& GetLightColor() { return m_lightColor; }
		void SetLightColor(const math::Color& value);
		
		inline float GetIntensity() { return m_intensity; }
		void SetIntensity(float value);
		
		inline float GetRange() { return m_range; }
		void SetRange(float value);
		
		inline float GetSpotAnlge() { return m_spotAngle; }
		void SetSpotAnlge(float value);
		
		inline float GetInnerSpotAngle() { return m_innerSpotAngle; }
		void SetInnerSpotAngle(float value);

		inline uint32 GetResolution() { return static_cast<uint32>(m_resolution); }
		void SetResolution(uint32 value);

	private:
		graphics::LightBuffer* m_pLightBuffer = nullptr;

		math::Vector3 m_prevPosition = math::Vector3::Zero;

		LightType m_lightType = LightType::Directional;
		math::Color m_lightColor = math::Color::White;
		float m_intensity = 2.5f;
		float m_range = 10.0f;
		float m_spotAngle = 30.0f;
		float m_innerSpotAngle = 21.80208f;	// _fallOffAngle

		// shadow
		ShadowResolution m_resolution = ShadowResolution::VeryHigh;

		RTTR_ENABLE(Component)

			RTTR_REGISTRATION_FRIEND
    };
}