#pragma once

#include "JType.h"
#include <cstdint>
#include <exception>
#include <iostream>
#include <string>

class JArray;
class JDate;
class JField;
class JObject;
class JStr;
class JVar;

class JParser
{
public:
    static void         Serialize(std::ostream& json, const JField& field);
    static void         Deserialize(std::istream& json, JField& field);

private:
    static void         GetVal(std::istream& json, const std::string& name, JField* field);
    static void         GetArr(std::istream& json, const std::string& name, JField* arr);
    static void         GetObj(std::istream& json, JField* obj);
    static double       GetFlt(std::istream& json);
    static std::string  GetStr(std::istream& json);
    static int64_t      GetInt(std::istream& json);
    static int64_t      GetInt(const std::string& json, std::string::size_type& off);
    static std::string  GetName(std::istream& json);
    static time_t       GetDate(std::istream& json);
    static bool         GetBool(std::istream& json);
    static uint64_t     GetUint(std::istream& json);
    static uint64_t     GetUint(const std::string& json, std::string::size_type& off);

    static void GetJson(const std::string& name, const JField& field, std::ostream& json);
    static void GetJson(const JObject& obj, std::ostream& json);
    static void GetJson(const JArray& arr, std::ostream& json);
    static void GetJson(const JDate& date, std::ostream& json);
    static void GetJson(const JStr& str, std::ostream& json);
    static void GetJson(const JVar& var, std::ostream& json);
};