#include <rengine\core\component\Transform.h>
#include <rengine\core\object\GameObject.h>
#include <rengine\core\scene\scene.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Transform>("Transform")
	.constructor<std::shared_ptr<rengine::GameObject>&>()
	.constructor<std::shared_ptr<rengine::GameObject>&, uuid>()
	.property("m_world", &rengine::Transform::GetWorld, &rengine::Transform::SetWorld)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::MATRIX)
	)
	.property("m_local", &rengine::Transform::GetLocal, &rengine::Transform::SetLocal)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::MATRIX)
	);
}

namespace rengine
{
	Transform::Transform(std::shared_ptr<GameObject>& gameObj)
	: Component(gameObj)
	{
		SetName(TEXT("Transform"));
	}

	Transform::Transform(std::shared_ptr<GameObject>& gameObj, uuid uuid)
	: Component(gameObj, uuid)
	{
		SetName(TEXT("Transform"));
	}

	Transform::~Transform()
	{
		DetachChildren();
	}

	void Transform::SetParent(shared_ptr<Transform> parent)
	{
		if(parent == m_parent.lock())
			return;

		if (!m_parent.expired() && m_parent.lock() != nullptr)
		{
			m_parent.lock()->DetachChild(shared_from_this());
		}

		m_parent = parent;

		if (!m_parent.expired() && m_parent.lock() != nullptr)
		{
			m_local = m_world * m_parent.lock()->GetWorld().Invert();
		}
		else
		{
			m_local = m_world;

			GetGameObject()->GetScene().lock()->AddRootGameObject(GetGameObject());
		}
	}

	void Transform::DetachParent()
	{
		m_parent.reset();

		m_local = m_world;
	}

	void Transform::AddChild(shared_ptr<Transform> child)
	{
		auto _iter = find(m_childs.begin(), m_childs.end(), child);

		if (_iter != m_childs.end())
			return;

		m_childs.push_back(child);

		child->SetParent(shared_from_this());
	}

	void Transform::DetachChild(shared_ptr<Transform> child)
	{
		auto _iter = find(m_childs.begin(), m_childs.end(), child);

		if(_iter != m_childs.end())
			m_childs.erase(_iter);
	}

	void Transform::DetachChildren()
	{
		for (auto& _child : m_childs)
		{
			_child->SetParent(nullptr);
		}
	}

	void Transform::SetWorld(Matrix m)
	{
		m_world = m;

		if (!m_parent.expired() && m_parent.lock() != nullptr)
			m_local = m_world * m_parent.lock()->GetWorld().Invert();
		else
			m_local = m_world;
	}

	void Transform::SetLocal(Matrix m)
	{
		m_local = m;

		if(!m_parent.expired() && m_parent.lock() != nullptr)
			m_world = m_parent.lock()->GetWorld() * m_local;
		else
			m_world = m_local;
	}
}
