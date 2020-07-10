#include "reflect.h"
namespace ExtObj
{
	void DeleteExtension(Extension *ext)
	{
		delete ext;
		ext = NULL;
	}

	void Destroy(ExtensionObject *obj)
	{
		delete obj;
		obj = NULL;
	}
}

ExtensionObject::~ExtensionObject()
{
	for (ExtensionList::iterator itr = m_Extensions.begin();
		itr != m_Extensions.end();
		++itr)
	{
		ExtObj::DeleteExtension(itr->second);
	}
	m_Extensions.clear();
}

Extension *ExtensionObject::GetExtension(std::string id)
{
	Extension *ret = NULL;
	ExtensionList::iterator itr = m_Extensions.find(id);
	if (itr != m_Extensions.end()) ret = itr->second;
	return ret;
}

Extension *ExtensionObject::AddExtension(std::string id, Extension *ext)
{
	ExtensionList::iterator itr = m_Extensions.find(id);
	if (itr != m_Extensions.end())
	{
		ExtObj::DeleteExtension(itr->second);
	}
	m_Extensions[id] = ext;

	return ext;
}

void ExtensionObject::RemoveExtension(std::string id)
{
	ExtensionList::iterator itr = m_Extensions.find(id);
	if (itr != m_Extensions.end())
	{
		ExtObj::DeleteExtension(itr->second);
		m_Extensions.erase(itr);
	}
}


