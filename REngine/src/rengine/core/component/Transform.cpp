﻿#include <rengine\core\component\Transform.h>
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
	/*(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::MATRIX)
	)*/
	.property("Position", &rengine::Transform::GetLocalPosition, &rengine::Transform::SetLocalPosition)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR3)
	)
	.property("Rotate", &rengine::Transform::GetLocalEulerAngle, &rengine::Transform::SetLocalEulerAngle)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR3)
	)
	.property("Scale", &rengine::Transform::GetLocalScale, &rengine::Transform::SetLocalScale)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR3)
	)
	.property("parent", &rengine::Transform::GetParent, &rengine::Transform::SetParent)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
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
		if(!m_parent.expired() && parent == m_parent.lock())
			return;

		if (!m_parent.expired() && m_parent.lock() != nullptr)
		{
			m_parent.lock()->DetachChild(shared_from_this());
		}

		m_parent = parent;

		if (!m_parent.expired() && m_parent.lock() != nullptr)
		{
			m_parent.lock()->AddChild(shared_from_this());

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

		//child->SetParent(shared_from_this());
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

	RENGINE_API math::Vector3 Transform::GetLocalPosition()
	{
		Vector3 _s, _t;
		Quaternion _r;

		if (!m_local.Decompose(_s, _r, _t))
			assert(false);

		return _t;
	}

	RENGINE_API void Transform::SetLocalPosition(math::Vector3 val)
	{
		m_bIsDirty = true;

		Vector3 _s, _t;
		Quaternion _r;

		if (!m_local.Decompose(_s, _r, _t))
			assert(false);

		// Todo : 이게 되는지 모르겠네 일단 이론상으론 기존의 forward 벡터와 up벡터를 그래고 쓰고 포지션만 바뀌는거니깐 정상적으로 작동이 되야한다. 
		m_local = Matrix::CreateWorld(val, m_local.Forward(), m_local.Up());
	}

	RENGINE_API math::Quaternion Transform::GetLocalRotation()
	{
		Vector3 _s, _t;
		Quaternion _r;

		if (!m_local.Decompose(_s, _r, _t))
			assert(false);

		return _r;
	}

	RENGINE_API void Transform::SetLocalRotation(math::Quaternion val)
	{
		m_bIsDirty = true;

		Vector3 _s, _t;
		Quaternion _r;

		if (!m_local.Decompose(_s, _r, _t))
			assert(false);

		m_local = Matrix::CreateScale(_s) * Matrix::CreateFromQuaternion(val) * Matrix::CreateTranslation(_t);
	}

	RENGINE_API math::Vector3 Transform::GetLocalEulerAngle()
	{
		Vector3 _s, _t;
		Quaternion _r;

		if (!m_local.Decompose(_s, _r, _t))
			assert(false);

		return _r.ToEuler() * math::Rad2Deg;
	}

	RENGINE_API void Transform::SetLocalEulerAngle(math::Vector3 val)
	{
		m_bIsDirty = true;

		Vector3 _s, _t;
		Quaternion _r;

		if (!m_local.Decompose(_s, _r, _t))
			assert(false);

		m_local = Matrix::CreateScale(_s) * Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(val * math::Deg2Rad)) * Matrix::CreateTranslation(_t);
	}

	RENGINE_API math::Vector3 Transform::GetLocalScale()
	{
		Vector3 _s, _t;
		Quaternion _r;

		if (!m_local.Decompose(_s, _r, _t))
			assert(false);

		return _s;
	}
	RENGINE_API void Transform::SetLocalScale(math::Vector3 val)
	{
		m_bIsDirty = true;

		Vector3 _s, _t;
		Quaternion _r;

		if (!m_local.Decompose(_s, _r, _t))
			assert(false);

		m_local = Matrix::CreateScale(val) * Matrix::CreateFromQuaternion(_r) * Matrix::CreateTranslation(_t);
	}

	void Transform::UpdateWorld()
	{
		m_bIsDirty = false;

		if (auto parent = m_parent.lock())
		{
			m_world = m_local * parent->GetWorld();
		}
		else
		{
			m_world = m_local;
		}
	}
}
