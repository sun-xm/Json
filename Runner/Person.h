#pragma once

#include <JObject.h>

struct Address : public JObject
{
    JString country;
    JString city;
    JString street;
    JString zipcode;

    JOBJECT(Address);
};

struct Person : public JObject
{
    JUint   age;
    JString name;
    JDate   birth;
    JBool   married;
    JFloat  weight;
    Address address;

    JArray<JString> skills;

    JOBJECT(Person);
};

struct Employee : public Person
{
    JUint   id;
    JString department;
    
    JOBJECT_INHERIT(Employee, Person);
};