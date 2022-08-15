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

class JNum : public JValue<double>
{
public:
    JVALUE(JNum, double, NUM);
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
    virtual void ForEach(const std::function<void(const std::string& name, const JField& field)>& cb) const = 0;
};

class JUndVar;

class JVar : public JField
{
public:
    JVar() : subtype(JType::VAR) {}

    void Define() { this->undef = false; }

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
        return JType::ARR == this->subtype ? this->fields.size() : 0;
    }

    virtual bool HasField(const std::string& name) const
    {
        return this->fields.end() != this->fields.find(name);
    }

    virtual JVar* GetNewField(const std::string& name)
    {
        if (JType::OBJ != this->subtype)
        {
            this->fields.clear();
            this->subtype = JType::OBJ;
        }

        auto itr = this->fields.find(name);
        if (this->fields.end() != itr)
        {
            return &itr->second;
        }

        return &(this->fields[name] = JVar());
    }

    virtual JVar* GetNewItem()
    {
        if (JType::ARR != this->subtype)
        {
            this->fields.clear();
            this->subtype = JType::ARR;
        }

        auto name = std::to_string(this->fields.size());
        return &(this->fields[name] = JVar());
    }

    void ForEachField(const std::function<void(const std::string& name, const JVar& var)>& cb) const
    {
        if (JType::OBJ != this->subtype)
        {
            return;
        }

        if (cb)
        {
            for (auto& pair : this->fields)
            {
                cb(pair.first, pair.second);
            }
        }
    }

    void ForEachItem(const std::function<void(const JVar& var)>& cb) const
    {
        if (JType::ARR != this->subtype)
        {
            return;
        }

        if (cb)
        {
            for (size_t i = 0; i < this->fields.size(); i++)
            {
                cb((*this)[i]);
            }
        }
    }

    bool ToArr(JArray& arr, std::string& err) const
    {
        if (this->undef)
        {
            return true;
        }

        if (this->null)
        {
            (JField&)arr = nullptr;
            return true;
        }

        arr.Define();

        try
        {
            this->ForEachItem([&arr, &err](const JVar& var)
            {
                auto item = arr.GetNew();

                if (var.IsNull())
                {
                    *item = nullptr;
                    return;
                }

                if (item->Type() == var.Subtype())
                {
                    switch (item->Type())
                    {
                        case JType::INT:
                        {
                            *(JInt*)item = var.Int;
                            break;
                        }

                        case JType::NUM:
                        {
                            *(JNum*)item = var.Num;
                            break;
                        }

                        case JType::STR:
                        {
                            *(JStr*)item = var.Str;
                            break;
                        }

                        case JType::ARR:
                        {
                            if (!var.ToArr(*(JArray*)item, err))
                            {
                                throw std::runtime_error(err);
                            }
                            break;
                        }

                        case JType::OBJ:
                        {
                            break;
                        }
                    }
                }
                else if (JType::NUM == item->Type() && JType::INT == var.Subtype())
                {
                    *(JNum*)item = (double)var.Int;
                }
                else
                {
                    throw std::runtime_error("Type mismatch. Expecting: " + to_string(item->Type()) + ". Actual: " + to_string(var.Subtype()));
                }
            });
        } catch (const std::exception& e)
        {
            err = e.what();
            return false;
        }

        return true;
    }

    bool ToArr(JArray& arr) const
    {
        std::string e;
        return this->ToArr(arr, e);
    }

    bool ToObj(JObject& obj, std::string& err) const
    {
        if (this->undef)
        {
            return true;
        }

        if (this->null)
        {
            (JField&)obj = nullptr;
            return true;
        }

        obj.Define();

        try
        {
            this->ForEachField([&obj, &err](const std::string& name, const JVar& var)
            {
                if (var.IsUndefined())
                {
                    return;
                }

                auto field = obj.GetField(name);
                if (!field)
                {
                    return;
                }

                if (var.IsNull())
                {
                    *field = nullptr;
                    return;
                }

                if (field->Type() == var.Subtype())
                {
                    switch (field->Type())
                    {
                        case JType::INT:
                        {
                            *(JInt*)field = var.Int;
                            break;
                        }

                        case JType::NUM:
                        {
                            *(JNum*)field = var.Num;
                            break;
                        }

                        case JType::STR:
                        {
                            *(JStr*)field = var.Str;
                            break;
                        }

                        case JType::ARR:
                        {
                            if (!var.ToArr(*(JArray*)field, err))
                            {
                                throw std::runtime_error(err);
                            }
                            break;
                        }

                        case JType::OBJ:
                        {
                            if (!var.ToObj(*(JObject*)field, err))
                            {
                                throw std::runtime_error(err);
                            }
                            break;
                        }
                    }
                }
                else if (JType::NUM == field->Type() && JType::INT == var.Subtype())
                {
                    *(JNum*)field = (double)var.Int;
                }
                else
                {
                    throw std::runtime_error("Type mismatch: " + name + ". Expecting: " + to_string(field->Type()) + ". Actual: " + to_string(var.Subtype()));
                }
            });
        } catch (const std::exception& e)
        {
            err = e.what();
            return false;
        }

        return true;
    }

    bool ToObj(JObject& obj) const
    {
        std::string err;
        return this->ToObj(obj, err);
    }

    JVar& operator[](size_t index)
    {
        if (JType::ARR != this->Subtype())
        {
            throw std::out_of_range("Is not an array");
        }

        if (index >= this->fields.size())
        {
            throw std::out_of_range("Index is out of range");
        }

        return this->fields.find(std::to_string(index))->second;
    }

    const JVar& operator[](size_t index) const
    {
        if (JType::ARR != this->Subtype())
        {
            throw std::out_of_range("Is not an array");
        }

        if (index >= this->fields.size())
        {
            throw std::out_of_range("Index is out of range");
        }

        return this->fields.find(std::to_string(index))->second;
    }

    JVar& operator[](const std::string& field)
    {
        if (JType::OBJ != this->Subtype())
        {
            throw std::runtime_error("Is not an object");
        }

        if (!this->HasValue() || !this->HasField(field))
        {
            return (JVar&)*UndVar;
        }

        return this->fields.find(field)->second;
    }

    const JVar& operator[](const std::string& field) const
    {
        if (JType::OBJ != this->Subtype())
        {
            throw std::runtime_error("Is not an object");
        }

        if (!this->HasValue() || !this->HasField(field))
        {
            return (JVar&)*UndVar;
        }

        return this->fields.find(field)->second;
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

    JVar& operator=(double value) override
    {
        return *this;
    }

    JVar& operator=(bool value) override
    {
        return *this;
    }

    JVar& operator=(const std::string& value) override
    {
        return *this;
    }

    std::nullptr_t operator=(std::nullptr_t) override
    {
        return nullptr;
    }
};