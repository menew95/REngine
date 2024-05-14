﻿#include <rengine\core\component\Transform.h>
#include <rengine\core\object\GameObject.h>
#include <rengine\core\scene\Scene.h>

#include <rttr\registration.h>

std::weak_ptr<rengine::Transform> converter_func_weak(const weak_ptr<rengine::Object>& value, bool& ok)
{
	ok = true;

	std::weak_ptr<rengine::Transform> _ret = std::static_pointer_cast<rengine::Transform>(value.lock());

	return _ret;
}

std::vector<weak_ptr<rengine::Transform>> converter_func_weak_container(const vector<weak_ptr<rengine::Object>>& value, bool& ok)
{
	ok = true;

	std::vector<weak_ptr<rengine::Transform>> _ret;

	for (auto& _ptr : value)
	{
		_ret.push_back(std::static_pointer_cast<rengine::Transform>(_ptr.lock()));
	}

	return _ret;
}

std::shared_ptr<rengine::Transform> converter_func_shared(const shared_ptr<rengine::Object>& value, bool& ok)
{
	ok = true;

	std::shared_ptr<rengine::Transform> _ret = std::static_pointer_cast<rengine::Transform>(value);

	return _ret;
}

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Transform>("Transform")
	.constructor<const uuid&>()
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
	.property("m_parent", &rengine::Transform::GetParent, &rengine::Transform::SetParent)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
	)
	.property("m_childs", &rengine::Transform::GetChilds, &rengine::Transform::SetChilds)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
	);


	rttr::type::register_converter_func(converter_func_weak);
	rttr::type::register_converter_func(converter_func_weak_container);
	rttr::type::register_converter_func(converter_func_shared);
}

namespace rengine
{
	Transform::Transform(const uuid& uuid)
	: Component(uuid, TEXT("Transform"))
	{

	}

	Transform::~Transform()
	{
		//for (auto& child : m_childs)
		//{
		//	auto _childTrans = child.lock();
		//	
		//	// 자식 포인터가 연결이 끊겼다면 이상한거임
		//	assert(_childTrans != nullptr);

		//	auto _gameObject = _childTrans->GetGameObject().lock();

		//	// 게임 오브젝트 연결이 끊겼다면 이상한거임
		//	assert(_gameObject != nullptr);

		//	// 자식 게임오브젝트 또한 삭제함
		//	Destroy(_gameObject);
		//}
	}

	void Transform::OnDestroy()
	{
		vector<Transform*> _childs;

		for (auto& child : m_childs)
		{
			auto _childTrans = child.lock();
			
			// 자식 포인터가 연결이 끊겼다면 이상한거임
			assert(_childTrans != nullptr);
			
			_childs.emplace_back(_childTrans.get());
		}

		for (auto& child : _childs)
		{
			auto _gameObject = child->GetGameObject().lock();

			// 게임 오브젝트 연결이 끊겼다면 이상한거임
			assert(_gameObject != nullptr);

			Object* _object = _gameObject.get();

			_gameObject.reset();

			// 자식 게임오브젝트 또한 삭제함
			DestroyImmediate(_object);
		}

		// 모두 삭제 등록을 하고 자식들의 부모를 해제
		DetachChildren();

		// 트랜스폼은 사용자가 삭제가 불가능해야하기 때문에 게임 오브젝트가 삭제가 되면 트랜스폼도 삭제됨
		// 게임 오브젝트 삭제할 당시에 씬에 등록되어 있을 경우 삭제하도록 되어 있기에 이곳에서는 불필요함
		/*if (m_parent.lock() == nullptr)
		{
			auto _go = GetGameObject().lock();

			if(_go == nullptr)
				return;

			_go->GetScene()->RemoveRootGameObject(_go);
		}*/

		// 부모가 있을 경우 부모의 자식 리스트에서 삭제함
		if(m_parent.lock() != nullptr)
		{
			m_parent.lock()->DetachChild(shared_from_this());
		}
	}

	void Transform::SetParent(shared_ptr<Transform> parent)
	{
		auto _parent = m_parent.lock();

		if(parent == _parent)
			return;

		if (_parent != nullptr)
		{
			_parent->DetachChild(shared_from_this());
		}
		else if (GetGameObject().lock()->GetScene() != nullptr)
		{
			GetGameObject().lock()->GetScene()->RemoveRootGameObject(GetGameObject().lock());
		}

		m_parent = parent;

		if (parent != nullptr)
		{
			parent->AddChild(shared_from_this());

			m_local = m_world * parent->GetWorld().Invert();
		}
		else 
		{
			m_local = m_world;

			if (GetGameObject().lock()->GetScene() != nullptr)
				GetGameObject().lock()->GetScene()->AddRootGameObject(GetGameObject().lock());
		}
	}

	void Transform::DetachParent()
	{
		m_parent.reset();

		m_local = m_world;
	}

	void Transform::AddChild(shared_ptr<Transform> child)
	{
		//auto _iter = find(m_childs.begin(), m_childs.end(), child);
		auto _iter = find_if(m_childs.begin(), m_childs.end(), [child](auto& ptr)
			{
				return ptr.lock() == child;
			});

		if (_iter != m_childs.end())
			return;

		m_childs.push_back(child);

		//child->SetParent(shared_from_this());
	}

	void Transform::DetachChild(shared_ptr<Transform> child)
	{
		//auto _iter = find(m_childs.begin(), m_childs.end(), child);
		auto _iter = find_if(m_childs.begin(), m_childs.end(), [child](auto& ptr)
			{
				return ptr.lock() == child;
			});

		if(_iter != m_childs.end())
			m_childs.erase(_iter);
	}

	void Transform::DetachChildren()
	{
		for (auto& _child : m_childs)
		{
			if(_child.lock() == nullptr)
				continue;

			_child.lock()->SetParent(nullptr);
		}
	}

	void Transform::SetWorld(Matrix m)
	{
		m_bIsDirty = true;

		m_world = m;

		if (m_parent.lock() != nullptr)
			m_local = m_world * m_parent.lock()->GetWorld().Invert();
		else
			m_local = m_world;
	}

	void Transform::SetLocal(Matrix m)
	{
		m_bIsDirty = true;

		m_local = m;

		if(m_parent.lock() != nullptr)
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

	math::Vector3 Transform::GetWorldPosition()
	{
		Vector3 _s, _t;
		Quaternion _r;

		if (!m_world.Decompose(_s, _r, _t))
			assert(false);

		return _t;
	}

	void Transform::SetWorldPosition(math::Vector3 val)
	{
		m_bIsDirty = true;

		Vector3 _s, _t;
		Quaternion _r;

		if (!m_world.Decompose(_s, _r, _t))
			assert(false);

		m_world = Matrix::CreateScale(_s) * Matrix::CreateFromQuaternion(_r) * Matrix::CreateTranslation(val);

		if (m_parent.lock() != nullptr)
		{
			m_local = m_world * m_parent.lock()->GetWorld().Invert();
		}
		else
		{
			m_local = m_world;
		}
	}

	math::Quaternion Transform::GetWorldRotation()
	{
		Vector3 _s, _t;
		Quaternion _r;

		if (!m_world.Decompose(_s, _r, _t))
			assert(false);

		return _r;
	}

	void Transform::SetWorldRotation(math::Quaternion val)
	{
		m_bIsDirty = true;

		Vector3 _s, _t;
		Quaternion _r;

		if (!m_world.Decompose(_s, _r, _t))
			assert(false);

		m_world = Matrix::CreateScale(_s) * Matrix::CreateFromQuaternion(val) * Matrix::CreateTranslation(_t);

		if (m_parent.lock() != nullptr)
		{
			m_local = m_world * m_parent.lock()->GetWorld().Invert();
		}
		else
		{
			m_local = m_world;
		}
	}

	math::Vector3 Transform::GetWorldEulerAngle()
	{
		Vector3 _s, _t;
		Quaternion _r;

		if (!m_world.Decompose(_s, _r, _t))
			assert(false);

		return _r.ToEuler() * math::Rad2Deg;
	}

	void Transform::SetWorldEulerAngle(math::Vector3 val)
	{
		m_bIsDirty = true;

		Vector3 _s, _t;
		Quaternion _r;

		if (!m_world.Decompose(_s, _r, _t))
			assert(false);

		m_world = Matrix::CreateScale(_s) * Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(val * math::Deg2Rad)) * Matrix::CreateTranslation(_t);

		if (m_parent.lock() != nullptr)
		{
			m_local = m_world * m_parent.lock()->GetWorld().Invert();
		}
		else
		{
			m_local = m_world;
		}
	}

	math::Vector3 Transform::GetWorldScale()
	{
		Vector3 _s, _t;
		Quaternion _r;

		if (!m_world.Decompose(_s, _r, _t))
			assert(false);

		return _s;
	}

	void Transform::SetWorldScale(math::Vector3 val)
	{
		m_bIsDirty = true;

		Vector3 _s, _t;
		Quaternion _r;

		if (!m_world.Decompose(_s, _r, _t))
			assert(false);

		m_world = Matrix::CreateScale(val) * Matrix::CreateFromQuaternion(_r) * Matrix::CreateTranslation(_t);

		if (m_parent.lock() != nullptr)
		{
			m_local = m_world * m_parent.lock()->GetWorld().Invert();
		}
		else
		{
			m_local = m_world;
		}
	}

	void Transform::PreDestroy()
	{
		__super::PreDestroy();

		for (auto& child : m_childs)
		{
			auto _childTrans = child.lock().get();

			// 자식 포인터가 연결이 끊겼다면 이상한거임
			assert(_childTrans != nullptr);

			auto _gameObject = _childTrans->GetGameObject().lock().get();

			// 게임 오브젝트 연결이 끊겼다면 이상한거임
			assert(_gameObject != nullptr);

			// 자식 게임오브젝트 또한 삭제함
			Destroy(_gameObject);
		}
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

		for (auto& _child : m_childs)
		{
			if(_child.lock() == nullptr)
				assert(false);

			_child.lock()->UpdateWorld();
		}
	}
}
