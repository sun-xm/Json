#pragma once

#include "JType.h"
#include <cstdint>
#include <exception>
#include <string>

class JArr;
class JDate;
class JField;
class JObject;
class JString;

class JParserError : public std::exception
{
public:
    JParserError() = default;
    JParserError(const std::string& json, std::string::size_type off, const std::string what) : Json(json), Off(off), What(what)
    {
    }

public:
    std::string What;
    std::string Json;
    std::string::size_type Off;
};

class JParser
{
public:
    static std::string  Serialize(const JField& field);
    static void         Deserialize(const std::string& json, JField& field);

private:
    static void         GetVal(const std::string& json, std::string::size_type& off, const std::string& name, JField* field);
    static void         GetArr(const std::string& json, std::string::size_type& off, const std::string& name, JArr* arr);
    static void         GetObj(const std::string& json, std::string::size_type& off, JObject* obj);
    static double       GetFlt(const std::string& json, std::string::size_type& off);
    static std::string  GetStr(const std::string& json, std::string::size_type& off);
    static int64_t      GetInt(const std::string& json, std::string::size_type& off);
    static std::string  GetName(const std::string& json, std::string::size_type& off);
    static time_t       GetDate(const std::string& json, std::string::size_type& off);
    static bool         GetBool(const std::string& json, std::string::size_type& off);
    static uint64_t     GetUint(const std::string& json, std::string::size_type& off);

    static void GetJson(const std::string& name, const JField& field, std::ostringstream& oss);
    static void GetJson(const JArr& arr, std::ostringstream& oss);
    static void GetJson(const JDate& date, std::ostringstream& oss);
    static void GetJson(const JObject& obj, std::ostringstream& oss);
    static void GetJson(const JString& str, std::ostringstream& oss);

    static std::string::size_type FindFirstNotSpace(const std::string& json, std::string::size_type off);
    static JParserError TypeMismatch(const std::string& json, size_t off, const std::string& name, JType expect, JType actual);
};