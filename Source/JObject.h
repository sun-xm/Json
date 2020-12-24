#pragma once

#include "JParser.h"
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <string>
#include <vector>

#define OFFSETOF(t, m)  ((size_t)&reinterpret_cast<char const volatile&>(static_cast<JField&>(((t*)0)->m)))
#define JOBJECT(CLASS)      public:\
                                JField* GetField(const std::string& name) override\
                                {\
                                    std::map<std::string, std::size_t>::const_iterator itr;\
                                    itr = fieldOffsets.find(name);\
                                    if(itr != fieldOffsets.end())\
                                    {\
                                        return (JField*)((char*)this + itr->second);\
                                    }\
                                    return nullptr;\
                                }\
                                void ForEach(const std::function<void(const std::string& name, const JField& field)>& cb) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : fieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first.c_str(), field);\
                                        }\
                                    }\
                                }\
                            private:\
                                static const std::map<std::string, std::size_t> fieldOffsets;\
                                typedef CLASS _SELF_

#define JOBJECT_INHERIT(CLASS, BASE)\
                            public:\
                                JField* GetField(const std::string& name) override\
                                {\
                                    std::map<std::string, std::size_t>::const_iterator itr;\
                                    itr = fieldOffsets.find(name);\
                                    if(itr != fieldOffsets.end())\
                                    {\
                                        return (JField*)((char*)this + itr->second);\
                                    }\
                                    return BASE::GetField(name);\
                                }\
                                void ForEach(const std::function<void(const std::string& name, const JField& field)>& cb) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : fieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first.c_str(), field);\
                                        }\
                                        BASE::ForEach(cb);\
                                    }\
                                }\
                            protected:\
                                static const std::map<std::string, std::size_t> fieldOffsets;\
                                typedef CLASS _SELF_

#define BEG_JFIELDS(CLASS)  const std::map<std::string, std::size_t> CLASS::fieldOffsets = {
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
        return JParser::Serialize(*this);
    }

    bool Deserialize(const std::string& json)
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

    bool Deserialize(const std::string& json, JParserError& error)
    {
        try
        {
            JParser::Deserialize(json, *this);
            return true;

        } catch(const JParserError& e)
        {
            error = e;
            return false;

        }
    }

protected:
    bool undef;
    bool null;
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
    operator const T&() const
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

class JUint : public JValue<uint64_t>
{
public:
    JVALUE(JUint, uint64_t, UINT);
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

class JFloat : public JValue<double>
{
public:
    JVALUE(JFloat, double, FLT);
};

class JString : public JValue<std::string>
{
public:
    JVALUE(JString, std::string, STR);
};

class JObject : public JField
{
public:
    JType Type() const override
    {
        return JType::OBJ;
    }

    bool IsUndefined() const override
    {
        if (this->IsNull())
        {
            return false;
        }

        bool undef = true;

        this->ForEach([&undef](const std::string&, const JField& field)
        {
            if (!field.IsUndefined())
            {
                undef = false;
                return;
            }
        });

        return undef;
    }

    virtual JField* GetField(const std::string&) = 0;
    virtual void ForEach(const std::function<void(const std::string& name, const JField& field)>& field) const = 0;
};

class JArr : public JField
{
public:
    virtual JField* GetNew() = 0;
    virtual size_t Size() const = 0;
    virtual void ForEach(const std::function<void(const JField& field)>& cb) const = 0;

    JType Type() const override
    {
        return JType::ARR;
    }

    void Define() { this->undef = false; }
};

template<typename T>
class JList : public JArr
{
public:
    JField* GetNew() override
    {
        this->Value.push_back(T());
        return &this->Value.back();
    }

    size_t Size() const override
    {
        return this->Value.size();
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

    std::list<T> Value;
};

template<typename T>
class JArray : public JArr
{
public:
    JField* GetNew() override
    {
        this->Value.push_back(T());
        return &this->Value.back();
    }

    size_t Size() const override
    {
        return this->Value.size();
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

    T& operator[](size_t index)
    {
        return this->Value[index];
    }
    
    const T& operator[](size_t index) const
    {
        return this->Value[index];
    }
    
    std::vector<T> Value;
};