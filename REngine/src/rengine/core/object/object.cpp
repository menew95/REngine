#include <rengine/core/object/object_impl.h>

namespace rengine
{
	Object::Object()
	: m_pImpl(new Object::Impl())
	{

	}

	Object::Object(uuid uuid)
	: m_pImpl(new Object::Impl(uuid))
	{
		
	}

	Object::Object(uuid uuid, tstring name)
	: m_pImpl(new Object::Impl(uuid, name))
	{

	}

	Object::Object(uuid uuid, tstring name, tstring type)
	: m_pImpl(new Object::Impl(uuid, name, type))
	{

	}

	Object::~Object()
	{
		delete m_pImpl;
	}
	uuid Object::GetUUID()
	{
		return m_pImpl->GetUUID();
	}

	void Object::SetUUID(uuid uuid)
	{
		m_pImpl->SetUUID(uuid);
	}

	tstring Object::GetName()
	{
		return m_pImpl->GetName();
	}

	string Object::GetNameStr()
	{
		return m_pImpl->GetNameStr();
	}

	void Object::SetName(tstring val)
	{
		m_pImpl->SetName(val);
	}

	void Object::SetNameStr(string val)
	{
		m_pImpl->SetNameStr(val);
	}

	tstring Object::GetType()
	{
		return m_pImpl->GetType();
	}
}
