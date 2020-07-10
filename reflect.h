#ifndef REFLECT_H
#define REFLECT_H

#include <map>
#include <string>
#include <typeinfo>
#include <stdio.h>
#include <vector>
#include "log.h"

class Extension
{
public:
	virtual ~Extension() {}
};

typedef std::map<std::string, Extension*> ExtensionList;

//���������༯��
class ExtensionObject
{
public:
	ExtensionList m_Extensions;
	Extension *GetExtension(std::string id);
	Extension *AddExtension(std::string id, Extension *);
	void RemoveExtension(std::string id);

	virtual ~ExtensionObject();
};

//���跴��,�κ���̳�Reflect����
class Reflect : public Extension	
{
public:
	ExtensionObject *m_Context;
	void SetContext(ExtensionObject *context) { m_Context = context; }
	
};

static Reflect* temp = NULL;

namespace ExtObj
{
	template<typename T>
	T *GetExtension(ExtensionObject *obj, std::string id)
	{
		if (obj == NULL) return NULL;
		return (T*)obj->GetExtension(id);
	}

	template<typename T>
	T *AddExtension(ExtensionObject *obj, std::string id)
	{
		if (obj == NULL) return NULL;
		//return (T*)obj->AddExtension(id, new T());
		temp = (Reflect*)obj->AddExtension(id, new T());
		temp->SetContext(obj);
		return (T*)temp;
	}

	template<typename T>
	T *Create()
	{
		return new T();
	}

	void DeleteExtension(Extension *ext);

	void Destroy(ExtensionObject *obj);
}


//obj ������  
//extType ���� 
//varName ������

//����obj
#define CreateExtObj(obj) ExtObj::Create<obj>()

//����obj
#define DestroyExtObj(obj) ExtObj::Destroy(obj)

//��ȡ-��������
#define ObjGetExt(obj, extType) ExtObj::GetExtension<extType>(obj, typeid(extType).name())

//����-��������
#define ObjAddExt(obj, extType) ExtObj::AddExtension<extType>(obj, typeid(extType).name())

//�޳�obj�ڵ�����һ��
#define ObjRmExt(obj, extType) obj->RemoveExtension(typeid(extType).name())

//��ȡobj����е���(������)
#define OBJ_GET_EXT(obj, extType, varName)\
    extType *varName = ObjGetExt(obj, extType);\
    varName = (varName ? varName : ObjAddExt(obj, extType));


#endif

