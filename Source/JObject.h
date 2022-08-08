#pragma once

#include "JParser.h"
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define OFFSETOF(t, m)  ((size_t)&reinterpret_cast<char const volatile&>(static_cast<JField&>(((t*)0)->m)))
#define JOBJECT(CLASS)      public:\
                                std::nullptr_t operator=(std::nullptr_t) override\
                                {\
                                    this->undef = false;\
                                    this->null  = true;\
                                    return nullptr;\
                                }\
                                JField* GetField(size_t offset) override\
                                {\
                                    return (JField*)((char*)this + offset);\
                                }\
                                JField* GetField(const std::string& name) override\
                                {\
                                    std::map<std::string, std::size_t>::const_iterator itr;\
                                    itr = FieldOffsets.find(name);\
                                    if(itr != FieldOffsets.end())\
                                    {\
                                        return this->GetField(itr->second);\
                                    }\
                                    return nullptr;\
                                }\
                                void ForEach(const std::function<void(const std::string& name, const JField& field)>& cb) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first.c_str(), field);\
                                        }\
                                    }\
                                }\
                                static const std::map<std::string, std::size_t> FieldOffsets;\
                            private:\
                                typedef CLASS _SELF_

#define JOBJECT_INHERIT(CLASS, BASE)\
                            public:\
                                std::nullptr_t operator=(std::nullptr_t) override\
                                {\
                                    this->undef = false;\
                                    this->null  = true;\
                                    return nullptr;\
                                }\
                                JField* GetField(size_t offset) override\
                                {\
                                    return (JField*)((char*)this + offset);\
                                }\
                                JField* GetField(const std::string& name) override\
                                {\
                                    std::map<std::string, std::size_t>::const_iterator itr;\
                                    itr = FieldOffsets.find(name);\
                                    if(itr != FieldOffsets.end())\
                                    {\
                                        return this->GetField(itr->second);\
                                    }\
                                    return BASE::GetField(name);\
                                }\
                                void ForEach(const std::function<void(const std::string& name, const JField& field)>& cb) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first.c_str(), field);\
                                        }\
                                        BASE::ForEach(cb);\
                                    }\
                                }\
                                static const std::map<std::string, std::size_t> FieldOffsets;\
                            private:\
                                typedef CLASS _SELF_

#define BEG_JFIELDS(CLASS)  const std::map<std::string, std::size_t> CLASS::FieldOffsets = {
#define JFIELD(FIELD)           std::make_pair(std::string(#FIELD), OFFSETOF(_SELF_, FIELD))
#define JFIELD_KEY(FIELD, KEY)  std::make_pair(std::string(KEY),    OFFSETOF(_SELF_, FIELD))
#define END_JFIELDS         };

class JField
{
public:
    JField() : undef(true), null(false) {}

    virtual JType Type() const = 0;

    virtual bool IsUndefined() const
    {
        return this->undef;
    }

    virtual bool IsNull() const
    {
        return this->null;
    }

    virtual bool HasValue() const
    {
        return !(this->IsNull() || this->IsUndefined());
    }

    virtual std::nullptr_t operator=(std::nullptr_t)
    {
        this->undef = false;
        this->null  = true;
        return nullptr;
    }

    std::string Serialize() const
    {
        std::ostringstream json;
        this->Serialize(json);
        return json.str();
    }

    bool Serialize(std::ostream& json) const
    {
        JParser::Serialize(json, *this);
        return !!json;
    }

    bool Deserialize(const std::string& json)
    {
        auto stream = std::istringstream(json);

        try
        {
            JParser::Deserialize(stream, *this);
            return true;

        } catch(const std::exception&)
        {
            return false;
        }
    }

    bool Deserialize(std::istream& json)
    {
        try
        {
            JParser::Deserialize(json, *this);
            return true;

        } catch(const std::exception&)
        {
            return false;
        }
    }

    bool Deserialize(const std::string& json, std::string& error, size_t& where)
    {
        auto stream = std::istringstream(json);

        try
        {
            JParser::Deserialize(stream, *this);
            return true;

        } catch(const std::exception& e)
        {
            where = stream ? (size_t)stream.tellg() : json.length();
            error = e.what();
            return false;
        }
    }

    bool Deserialize(std::istream& json, std::string& error)
    {
        try
        {
            JParser::Deserialize(json, *this);
            return true;

        } catch(const std::exception& e)
        {
            error = e.what();
            return false;
        }
    }

protected:
    bool undef;
    bool null;
};

class JArray : public JField
{
public:
    virtual JField* GetNew() = 0;
    virtual void ForEach(const std::function<void(const JField& field)>& cb) const = 0;

    JType Type() const override
    {
        return JType::ARR;
    }

    void Define() { this->undef = false; }
};

template<typename T>
class JArr : public JArray
{
    static_assert(std::is_base_of<JField, T>::value, "T must inherit from JField");

public:
    T* GetNew() override
    {
        this->Value.push_back(T());
        return &this->Value.back();
    }

    void ForEach(const std::function<void(const JField& field)>& cb) const override
    {
        if (cb)
        {
            for (auto& field : this->Value)
            {
                cb(field);
            }
        }
    }

    void ForEachItem(const std::function<void(const T& item)>& cb) const
    {
        if (cb)
        {
            this->ForEach([&](const JField& field)
            {
                cb((const T&)field);
            });
        }
    }

    T& operator[](size_t index)
    {
        return this->Value[index];
    }

    const T& operator[](size_t index) const
    {
        return this->Value[index];
    }

    std::vector<T>& operator()()
    {
        return this->Value;
    }

    const std::vector<T>& operator()() const
    {
        return this->Value;
    }

    std::vector<T> Value;
};

template<typename T>
class JValue : public JField
{
public:
    JValue() = default;
    JValue(T value) : Value(value)
    {
        this->undef = false;
        this->null  = false;
    }

    JValue<T>& operator=(const T& value)
    {
        this->Value = value;
        this->undef = false;
        this->null  = false;
        return *this;
    }

    bool operator==(const JValue<T>& other)
    {
        if (this->undef && other.undef)
        {
            return true;
        }

        if (this->undef != other.undef)
        {
            return false;
        }

        if (this->null && other.null)
        {
            return true;
        }

        if (this->null != other.null)
        {
            return false;
        }

        return this->Value == other.Value;
    }

    operator const T&() const
    {
        return this->Value;
    }

    const T& operator()() const
    {
        return this->Value;
    }

    T Value;
};

#define JVALUE(C, V, T) C() = default;\
                        C(V v) : JValue<V>(v){}\
                        using JValue<V>::operator=;\
                        JType Type() const override { return JType::T; }

class JInt : public JValue<int64_t>
{
public:
    JVALUE(JInt, int64_t, INT);
};

class JFlt : public JValue<double>
{
public:
    JVALUE(JFlt, double, FLT);
};

class JStr : public JValue<std::string>
{
public:
    JVALUE(JStr, std::string, STR);
};

class JBool : public JValue<bool>
{
public:
    JVALUE(JBool, bool, BOOL);
};

class JDate : public JValue<time_t>
{
public:
    JVALUE(JDate, time_t, DATE);
};

class JObject : public JField
{
public:
    JType Type() const override
    {
        return JType::OBJ;
    }

    void Define() { this->undef = false; }

    virtual JField* GetField(size_t offset) = 0;
    virtual JField* GetField(const std::string&) = 0;
    virtual void ForEach(const std::function<void(const std::string& name, const JField& field)>& field) const = 0;
};