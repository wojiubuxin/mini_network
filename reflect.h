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

//包含反射类集合
class ExtensionObject
{
public:
	ExtensionList m_Extensions;
	Extension *GetExtension(std::string id);
	Extension *AddExtension(std::string id, Extension *);
	void RemoveExtension(std::string id);

	virtual ~ExtensionObject();
};

//如需反射,任何类继承Reflect即可
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


//obj 主对象  
//extType 类名 
//varName 变量名

//创建obj
#define CreateExtObj(obj) ExtObj::Create<obj>()

//销毁obj
#define DestroyExtObj(obj) ExtObj::Destroy(obj)

//获取-不主动调
#define ObjGetExt(obj, extType) ExtObj::GetExtension<extType>(obj, typeid(extType).name())

//增加-不主动调
#define ObjAddExt(obj, extType) ExtObj::AddExtension<extType>(obj, typeid(extType).name())

//剔除obj内的其中一类
#define ObjRmExt(obj, extType) obj->RemoveExtension(typeid(extType).name())

//获取obj里存有的类(主动调)
#define OBJ_GET_EXT(obj, extType, varName)\
    extType *varName = ObjGetExt(obj, extType);\
    varName = (varName ? varName : ObjAddExt(obj, extType));


#endif

