
#ifndef GAMEOBJECT_INL
#define GAMEOBJECT_INL
#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Component.h>

namespace rengine
{
	template<typename T>
	inline std::shared_ptr<T> GameObject::AddComponent()
	{
		auto _this = shared_from_this();

		std::shared_ptr<T> _newComponent = std::make_shared<T>(_this);

		m_Components.emplace_back(_newComponent);

		return _newComponent;
	}

	template<typename T>
	inline std::shared_ptr<T> GameObject::GetComponent()
	{
		for (auto& component : m_Components)
		{
			if (typeid(*component).name() == typeid(T).name())
			{
				return std::static_pointer_cast<T>(component);
			}
		}

		return nullptr;
	}

	template<typename T>
	inline std::shared_ptr<T> GameObject::GetComponentInChildren()
	{

		return nullptr;
	}

	template<typename T>
	inline std::shared_ptr<T> GameObject::GetComponentInParent()
	{

		return nullptr;
	}

	template<typename T>
	inline std::vector<std::shared_ptr<T>>
		GameObject::GetComponentsInChildren()
	{

		return nullptr;
	}

	template<typename T>
	inline std::vector<std::shared_ptr<T>>
		GameObject::GetComponentsInParent()
	{

		return nullptr;
	}
}
#endif