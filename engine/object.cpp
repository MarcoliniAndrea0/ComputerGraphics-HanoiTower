#include "object.h"

int Object::id_counter = 0;

Object::Object(const std::string& name_, const std::string& type_)
	: id(++id_counter), name(name_), type(type_)
{
}

int Object::getID() const 
{
	return id;
}

const std::string& Object::getName() const 
{
	return name;
}

const std::string& Object::getType() const 
{ 
	return type; 
}


void Object::setName(const std::string& newName) {
	name = newName;
}

void Object::setType(const std::string& newType) 
{ 
	type = newType; 
}