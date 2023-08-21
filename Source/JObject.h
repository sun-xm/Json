#pragma once

#include "JType.h"
#include <cstdint>
#include <functional>
#include <map>
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
                                void Clear() override\
                                {\
                                    for (auto& pair : FieldOffsets)\
                                    {\
                                        auto field = (JField*)((char*)(this) + pair.second);\
                                        field->Clear();\
                                    }\
                                    JField::Clear();\
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
                                void Clear() override\
                                {\
                                    for (auto& pair : FieldOffsets)\
                                    {\
                                        auto field = (JField*)((char*)(this) + pair.second);\
                                        field->Clear();\
                                    }\
                                    BASE::Clear();\
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

    virtual void Clear()
    {
        this->undef = true;
        this->null  = false;
    }

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

    std::string Serialize() const;
    bool Serialize(std::ostream& json) const;
    bool Deserialize(const std::string& json);
    bool Deserialize(std::istream& json);
    bool Deserialize(const std::string& json, std::string& error, size_t& where);
    bool Deserialize(std::istream& json, std::string& error);

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

    void Define()
    {
        this->undef = false;
        this->null  = false;
    }
};

template<typename T>
class JArr : public JArray
{
    static_assert(std::is_base_of<JField, T>::value, "T must inherit from JField");

public:
    void Clear() override
    {
        this->Value.clear();
        JField::Clear();
    }

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

    std::nullptr_t operator=(std::nullptr_t) override
    {
        this->Value.clear();
        return JField::operator=(nullptr);
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

    T& ValueOrDefault(T& defVal)
    {
        return this->HasValue() ? this->Value : defVal;
    }

    const T& ValueOrDefault(const T& defVal)
    {
        return this->HasValue() ? this->Value : defVal;
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

class JNum : public JValue<double>
{
public:
    JVALUE(JNum, double, NUM);
};

class JStr : public JValue<std::string>
{
public:
    JVALUE(JStr, std::string, STR);

    void Clear() override
    {
        this->Value.clear();
        JField::Clear();
    }
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

    void Define()
    {
        this->undef = false;
        this->null  = false;
    }

    virtual JField* GetField(size_t offset) = 0;
    virtual JField* GetField(const std::string&) = 0;
    virtual void ForEach(const std::function<void(const std::string& name, const JField& field)>& cb) const = 0;
};

class JUndVar;

class JVar : public JField
{
public:
    JVar() : subtype(JType::VAR) {}

    void Define(JType type)
    {
        if (JType::ARR == type || JType::OBJ == type)
        {
            if (this->subtype != type)
            {
                this->fields.clear();
                this->Str.clear();
            }

            this->subtype = type;
            this->undef = false;
            this->null  = false;
        }
    }

    void Clear() override
    {
        this->subtype = JType::VAR;
        this->Str.clear();
        this->fields.clear();
        JField::Clear();
    }

    JType Type() const override
    {
        return JType::VAR;
    }

    virtual JType Subtype() const
    {
        return this->subtype;
    }

    virtual size_t Size() const
    {
        return (JType::ARR == this->subtype || JType::OBJ == this->subtype) ? this->fields.size() : 0;
    }

    virtual bool HasField(const std::string& name) const
    {
        return this->fields.end() != this->fields.find(name);
    }

    void ForEachField(const std::function<void(const std::string& name, const JVar& var)>& cb) const
    {
        if (JType::OBJ == this->subtype && cb)
        {
            for (auto& pair : this->fields)
            {
                cb(pair.first, pair.second);
            }
        }
    }

    void ForEachItem(const std::function<void(const JVar& var)>& cb) const
    {
        if (JType::ARR == this->subtype && cb)
        {
            for (size_t i = 0; i < this->fields.size(); i++)
            {
                cb((*this)[i]);
            }
        }
    }

    virtual JVar* GetNewItem();
    virtual JVar* GetNewField(const std::string& name);

    bool ToArr(JArray& arr, std::string& err) const;
    bool ToObj(JObject& obj, std::string& err) const;

    bool ToArr(JArray& arr) const
    {
        std::string e;
        return this->ToArr(arr, e);
    }

    bool ToObj(JObject& obj) const
    {
        std::string err;
        return this->ToObj(obj, err);
    }

    JVar& operator[](size_t index);
    const JVar& operator[](size_t index) const;
    JVar& operator[](const std::string& field);
    const JVar& operator[](const std::string& field) const;

    std::nullptr_t operator=(std::nullptr_t) override
    {
        this->Str.clear();
        this->fields.clear();
        return JField::operator=(nullptr);
    }

    virtual JVar& operator=(int64_t value)
    {
        this->Str.clear();
        this->fields.clear();

        this->subtype = JType::INT;
        this->undef = false;
        this->null  = false;
        this->Int   = value;

        return *this;
    }

    virtual JVar& operator=(int32_t value)
    {
        return (*this = (int64_t)value);
    }

    virtual JVar& operator=(double value)
    {
        this->Str.clear();
        this->fields.clear();

        this->subtype = JType::NUM;
        this->undef = false;
        this->null  = false;
        this->Num   = value;

        return *this;
    }

    virtual JVar& operator=(float value)
    {
        return (*this = (double)value);
    }

    virtual JVar& operator=(bool value)
    {
        this->Str.clear();
        this->fields.clear();

        this->subtype = JType::BOOL;
        this->undef = false;
        this->null  = false;
        this->Bool  = value;

        return *this;
    }

    virtual JVar& operator=(const std::string& value)
    {
        this->fields.clear();

        this->subtype = JType::STR;
        this->undef = false;
        this->null  = false;
        this->Str   = value;

        return *this;
    }

    virtual JVar& operator=(const char* value)
    {
        this->fields.clear();

        this->subtype = JType::STR;
        this->undef = false;
        this->null  = false;
        this->Str   = value;

        return *this;
    }

    virtual JVar& operator=(const JVar& var)
    {
        this->Clear();

        ((JField&)*this) = var;

        this->Int = var.Int;
        this->Str = var.Str;
        this->fields = var.fields;
        this->subtype = var.subtype;

        return *this;
    }

    virtual JVar& operator=(const JField& field);

    union
    {
        int64_t Int;
        double  Num;
        bool    Bool;
    };

    std::string Str;

protected:
    JType subtype;
    std::map<std::string, JVar> fields;

    static JUndVar* UndVar;
};

class JUndVar : public JVar
{
public:
    bool IsUndefined() const override
    {
        return true;
    }

    bool IsNull() const override
    {
        return false;
    }

    bool HasField(const std::string&) const override
    {
        return false;
    }

    JType Subtype() const override
    {
        return JType::VAR;
    }

    size_t Size() const override
    {
        return 0;
    }

    JVar* GetNewField(const std::string& name) override
    {
        return nullptr;
    }

    JVar* GetNewItem() override
    {
        return nullptr;
    }

    JVar& operator=(int64_t) override
    {
        return *this;
    }

    JVar& operator=(int32_t) override
    {
        return *this;
    }

    JVar& operator=(double) override
    {
        return *this;
    }

    JVar& operator=(float) override
    {
        return *this;
    }

    JVar& operator=(bool) override
    {
        return *this;
    }

    JVar& operator=(const std::string&) override
    {
        return *this;
    }

    JVar& operator=(const JField& field) override
    {
        return *this;
    }

    std::nullptr_t operator=(std::nullptr_t) override
    {
        return nullptr;
    }
};