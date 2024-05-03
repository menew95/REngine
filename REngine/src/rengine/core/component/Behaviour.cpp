#include <rengine\core\component\Behaviour.h>

#include <rengine\core\object\GameObject.h>

#include <rttr\registration.h>

//RTTR_REGISTRATION
//{
//	rttr::registration::class_<rengine::Behaviour>("Behaviour")
//	.constructor<const uuid&>()
//	.property("m_bEnable", &rengine::Behaviour::GetEnable, &rengine::Behaviour::SetEnable)
//	(
//		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::BOOL),
//		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::BOOL)
//	);
//}

//namespace rengine
//{
//	Behaviour::Behaviour(const uuid& uuid, const tstring& type)
//		: Component(uuid, type)
//	{
//	}
//
//	Behaviour::~Behaviour()
//	{
//	}
//
//	void Behaviour::SetEnable(bool value)
//	{
//		if (m_pGameObject.lock() && m_pGameObject.lock()->GetActiveInHierarchy() && m_bEnable != value)
//		{
//			m_bEnable = value;
//
//			if (m_bEnable)
//			{
//				OnEnable();
//			}
//			else
//			{
//				OnDisable();
//			}
//		}
//	}
//}