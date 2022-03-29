#pragma once

#include <JObject.h>

struct Address : public JObject
{
    JStr country;
    JStr city;
    JStr street;
    JStr zipcode;

    JOBJECT(Address);
};

struct Person : public JObject
{
    JInt    age;
    JStr    name;
    JDate   birth;
    JBool   married;
    JFlt    weight;
    Address address;

    JArr<JStr> skills;

    JOBJECT(Person);
};

struct Employee : public Person
{
    JInt id;
    JStr department;

    JOBJECT_INHERIT(Employee, Person);
};