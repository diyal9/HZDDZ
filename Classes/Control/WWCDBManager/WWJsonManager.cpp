/*
 * WWJsonManager.cpp
 *
 *  Created on: 2014-8-4
 *      Author: lenovo
 */

#include "WWJsonManager.h"

USING_NS_CC;


WWJsonManager::WWJsonManager():
		_root(rapidjson::kObjectType)
{
	// TODO Auto-generated constructor stub
}

WWJsonManager::~WWJsonManager() {
	// TODO Auto-generated destructor stub

}

std::string WWJsonManager::getContent()
{
	StringBuffer _strbuf;
	Writer<StringBuffer> writer(_strbuf);
	_root.Accept(writer);
	std::string result = _strbuf.GetString();
    return result;
}

JsonStatus WWJsonManager::setContent(const char* content, std::string& error)
{
    if (content && *content) {
        _doc.Parse<rapidjson::kParseDefaultFlags>(content);         // DOM解析
        if (_doc.HasParseError()) {
        	error = _doc.GetParseError();
        	return kJsonParseErr;
        }
    }
    return kJsonSuccess;
}

JsonStatus WWJsonManager::readInt(const char* name, int& value)
{

    if (_doc.HasMember(name)) {
        const rapidjson::Value &item = _doc[name];

        if (item.IsInt()) {
        	value = item.GetInt();
        }
        else if (item.IsUint()) {
        	value = item.GetUint();
        }
        else {
        	return kJsonDecodeErr;
        }
    }
    else
	{
		return kJsonDecodeErr;
	}

    return kJsonSuccess;
}

JsonStatus WWJsonManager::readBool(const char* name, bool& value)
{

    if (_doc.HasMember(name)) {
        const rapidjson::Value& item = _doc[name];

        if (item.IsBool()) {
        	value = item.GetBool();
        }
        else {
        	return kJsonDecodeErr;
        }
    }
    else
	{
		return kJsonDecodeErr;
	}

    return kJsonSuccess;
}

JsonStatus WWJsonManager::readString(const char* name, std::string& value)
{

    if (_doc.HasMember(name))
    {
        const rapidjson::Value &item = _doc[name];

        if (item.IsString())
        {
        	value = item.GetString();
        }
        else
        {
        	return kJsonDecodeErr;
        }
    }
    else
    {
    	return kJsonDecodeErr;
    }

    return kJsonSuccess;
}


JsonStatus WWJsonManager::readStringArray(const char* name, std::vector<std::string>& value)
{
	if (_doc.HasMember(name))
	{
		rapidjson::Value &item = _doc[name];

		if (item.IsArray())
		{
			value.resize(item.Size());
			for(int i=0; i<item.Size(); i++)
			{
				rapidjson::Value& eleItem = item[i];
				if(eleItem.IsString())
				{
//					log("jsonparse===%d", eleItem.GetInt());
					value[i] = eleItem.GetString();
				}
				else
				{
					return kJsonDecodeErr;
				}
			}
		}
		else
		{
			return kJsonDecodeErr;
		}
	}
	else
	{
		return kJsonDecodeErr;
	}

	return kJsonSuccess;
}

JsonStatus WWJsonManager::readObjectArray(const char* name, std::vector<std::map<std::string , std::string>>& vectors)
{

	if (_doc.HasMember(name))
	{
		rapidjson::Value &item = _doc[name];
		vectors.resize(item.Size());
		if (item.IsArray())
		{
			for(int i=0; i<item.Size(); i++)
			{
				rapidjson::Value& eleItem = item[i];
				if(eleItem.IsObject())
				{
					for (auto m= eleItem.MemberonBegin(); m!=eleItem.MemberonEnd(); m++)
					{
						rapidjson::Value& name = m->name;
						rapidjson::Value& value = m->value;
//						log("WWJsonManager::readObjectArray###MemberonBegin=%s", name.GetString());
						if(value.IsInt())
						{
//							log("WWJsonManager::readObjectArray###MemberonBegin=%d",value.GetInt());
							std::string valueStr = __String::createWithFormat("%d",value.GetInt() )->_string;
							vectors[i].insert(std::pair<std::string,std::string>(name.GetString(), valueStr));

						}
						else if(value.IsString())
						{
							vectors[i].insert(std::pair<std::string,std::string>(name.GetString(), value.GetString()));
//							log("WWJsonManager::readObjectArray###MemberonBegin=%s",value.GetString());
						}
					}
				}
				else
				{
					return kJsonDecodeErr;
				}
			}
		}
		else
		{
			return kJsonDecodeErr;
		}
	}
	else
	{
		return kJsonDecodeErr;
	}

	return kJsonSuccess;
}

JsonStatus WWJsonManager::readIntArray(const char* name, std::vector<int>& value)
{
	if (_doc.HasMember(name))
	{
		rapidjson::Value &item = _doc[name];

		if (item.IsArray())
		{
			value.resize(item.Size());
			for(int i=0; i<item.Size(); i++)
			{
				rapidjson::Value& eleItem = item[i];
				if(eleItem.IsInt())
				{
//					log("jsonparse===%d", eleItem.GetInt());
					value[i] = eleItem.GetInt();
				}
				else
				{
					return kJsonDecodeErr;
				}
			}
		}
		else
		{
			return kJsonDecodeErr;
		}
	}
	else
	{
		return kJsonDecodeErr;
	}

	return kJsonSuccess;
}

void WWJsonManager::writeInt(const char* name, int value)
{
    _root.AddMember(name, value, _doc.GetAllocator());
}

void WWJsonManager::writeBool(const char* name, bool value)
{
    _root.AddMember(name, value, _doc.GetAllocator());
}

void WWJsonManager::writeString(const char* name, const char* value)
{
	rapidjson::Value stringNode(kStringType);
	stringNode.SetString(value,_doc.GetAllocator());
    _root.AddMember(name, stringNode, _doc.GetAllocator());
}

void WWJsonManager::writeStringArray(const char* name, std::vector<std::string> &values)
{
	rapidjson::Value node(kArrayType);
	for(int i=0; i<values.size(); i++)
	{
		rapidjson::Value stringNode(kStringType);
		stringNode.SetString(values[i].c_str());

		node.PushBack(stringNode, _doc.GetAllocator());

	}
	_root.AddMember(name, node, _doc.GetAllocator());
}

void WWJsonManager::writeIntArray(const char* name, std::vector<int> &values)
{
	rapidjson::Value node(kArrayType);
	for(int i=0; i<values.size(); i++)
	{
		log("json writeIntArray===name=%s value=%d",name,values[i]);
		node.PushBack((int)values[i], _doc.GetAllocator());
	}
	_root.AddMember(name, node, _doc.GetAllocator());
}

void WWJsonManager::traverseJson()
{
	if(_doc.IsNull() || _doc.Size() == 0){
		log("jsonparse===empty json");
		return;
	}
	for (int i = 0; i < _doc.Size(); i++)
	{
		log("jsonparse===index:%d", i);
		rapidjson::Value& eleName = _doc[i*2];
		log("jsonparse===name:%s", eleName.GetString());
		rapidjson::Value& ele = _doc[i*2 + 1];
		if (ele.IsInt())
		{
			log("jsonparse===%d", ele.GetInt());
		}
		else if (ele.IsString())
		{
			log("jsonparse===%s", ele.GetString());
		}
		else if (ele.IsBool())
		{
			if (ele.GetBool())
			{
				log("jsonparse===%s", "true");
			}
			else
			{
				log("jsonparse===%s", "false");
			}
		}
		else if (ele.IsArray())
		{
			log("jsonparse===%s:%d", "array", ele.Size());

			for(int j=0; j<ele.Size(); j++){
				rapidjson::Value& eleItem = ele[j];
				if(eleItem.IsInt())
				{
					log("jsonparse===%d", eleItem.GetInt());
				}
				else if(eleItem.IsString())
				{
					log("jsonparse===%s", eleItem.GetString());
				}
				else
				{
					log("jsonparse===unkown element");
				}
			}
		}
		else if (ele.IsObject())
		{
			log("jsonparse===%s", "object");
		}
		else
		{
			log("jsonparse===%s", "flag error");
		}
	}
}

JsonStatus WWJsonManager::getJsonFileStr(std::string filename, std::string& json)
{
	if(!CCFileUtils::getInstance()->isFileExist(filename))
	{
//		log("jsonparse===json file is not find [%s]", filename.c_str());
		return kJsonFileOpenErr;
	}

	Data data = CCFileUtils::getInstance()->getDataFromFile(filename.c_str());

	json = std::string((const char*)data.getBytes(), data.getSize());
	return kJsonFileOpenSuccess;
}
