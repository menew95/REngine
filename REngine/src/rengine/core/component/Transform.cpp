#include <rengine\core\component\Transform.h>

namespace rengine
{
	Transform::Transform(std::shared_ptr<GameObject>& gameObj)
	: Component(gameObj)
	{
	}

	Transform::Transform(std::shared_ptr<GameObject>& gameObj, uuid uuid)
	: Component(gameObj, uuid)
	{
	}

	Transform::Transform(std::shared_ptr<GameObject>& gameObj, uuid uuid, tstring name)
	: Component(gameObj, uuid, name)
	{
	}

	Transform::~Transform()
	{
		DetachChildren();
	}

	void Transform::SetParent(shared_ptr<Transform> parent)
	{
		if(parent == m_parent)
			return;

		if (m_parent != nullptr)
		{
			m_parent->DetachChild(shared_from_this());
		}

		m_parent = parent;

		m_local = m_world * m_parent->GetWorld().Invert();
	}

	void Transform::DetachParent()
	{
		m_parent = nullptr;

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

	void Transform::SetWolrd(Matrix& m)
	{
		m_world = m;

		if (m_parent != nullptr)
			m_local = m_world * m_parent->GetWorld().Invert();
		else
			m_local = m_world;
	}

	void Transform::SetLocal(Matrix& m)
	{
		m_local = m;

		if(m_parent != nullptr)
			m_world = m_parent->GetWorld() * m_local;
		else
			m_world = m_local;
	}
}
