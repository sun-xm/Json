#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <map>
#include <string>
#include <vector>

#define OFFSETOF(t, m)  ((std::size_t)&reinterpret_cast<char const volatile&>(static_cast<JField&>(((t*)0)->m)))
#define JOBJECT(CLASS)      public:\
                                CLASS& operator=(std::nullptr_t) override\
                                {\
                                    return (CLASS&)JField::operator=(nullptr);\
                                }\
                                bool IsUndefined() const override\
                                {\
                                    if (this->nul)\
                                    {\
                                        return false;\
                                    }\
                                    for (auto& pair : FieldOffsets)\
                                    {\
                                        auto field = (JField*)((char*)(this) + pair.second);\
                                        if (!field->IsUndefined())\
                                        {\
                                            return false;\
                                        }\
                                    }\
                                    return true;\
                                }\
                                JField* GetField(const std::string& name) override\
                                {\
                                    std::map<std::string, std::size_t>::const_iterator itr;\
                                    itr = FieldOffsets.find(name);\
                                    if(itr != FieldOffsets.end())\
                                    {\
                                        return JObject::GetField(this, itr->second);\
                                    }\
                                    return nullptr;\
                                }\
                                const JField* GetField(const std::string& name) const override\
                                {\
                                    std::map<std::string, std::size_t>::const_iterator itr;\
                                    itr = FieldOffsets.find(name);\
                                    if(itr != FieldOffsets.end())\
                                    {\
                                        return JObject::GetField(this, itr->second);\
                                    }\
                                    return nullptr;\
                                }\
                                bool ForEach(const std::function<bool(const std::string& name, const JField& field)>& cb) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            const auto& field = *(const JField*)((char*)(this) + pair.second);\
                                            if (cb(pair.first, field))\
                                            {\
                                                return true;\
                                            }\
                                        }\
                                    }\
                                    return false;\
                                }\
                                bool ForEach(const std::function<bool(const std::string& name, const JField& field, void* param)>& cb, void* param) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            const auto& field = *(const JField*)((char*)(this) + pair.second);\
                                            if (cb(pair.first, field, param))\
                                            {\
                                                return true;\
                                            }\
                                        }\
                                    }\
                                    return false;\
                                }\
                                bool ForEach(const std::function<bool(const std::string& name, JField& field)>& cb) override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            if (cb(pair.first, field))\
                                            {\
                                                return true;\
                                            }\
                                        }\
                                    }\
                                    return false;\
                                }\
                                bool ForEach(const std::function<bool(const std::string& name, JField& field, void* param)>& cb, void* param) override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            if (cb(pair.first, field, param))\
                                            {\
                                                return true;\
                                            }\
                                        }\
                                    }\
                                    return false;\
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
                                CLASS& operator=(std::nullptr_t) override\
                                {\
                                    return (CLASS&)JField::operator=(nullptr);\
                                }\
                                bool IsUndefined() const override\
                                {\
                                    if (this->nul)\
                                    {\
                                        return false;\
                                    }\
                                    for (auto& pair : FieldOffsets)\
                                    {\
                                        auto field = (JField*)((char*)(this) + pair.second);\
                                        if (!field->IsUndefined())\
                                        {\
                                            return false;\
                                        }\
                                    }\
                                    return BASE::IsUndefined();\
                                }\
                                JField* GetField(const std::string& name) override\
                                {\
                                    std::map<std::string, std::size_t>::const_iterator itr;\
                                    itr = FieldOffsets.find(name);\
                                    if(itr != FieldOffsets.end())\
                                    {\
                                        return JObject::GetField(this, itr->second);\
                                    }\
                                    return BASE::GetField(name);\
                                }\
                                const JField* GetField(const std::string& name) const override\
                                {\
                                    std::map<std::string, std::size_t>::const_iterator itr;\
                                    itr = FieldOffsets.find(name);\
                                    if(itr != FieldOffsets.end())\
                                    {\
                                        return JObject::GetField(this, itr->second);\
                                    }\
                                    return BASE::GetField(name);\
                                }\
                                bool ForEach(const std::function<bool(const std::string& name, const JField& field)>& cb) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            const auto& field = *(const JField*)((char*)(this) + pair.second);\
                                            if (cb(pair.first, field))\
                                            {\
                                                return true;\
                                            }\
                                        }\
                                        if (BASE::ForEach(cb))\
                                        {\
                                            return true;\
                                        }\
                                    }\
                                    return false;\
                                }\
                                bool ForEach(const std::function<bool(const std::string& name, const JField& field, void* param)>& cb, void* param) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            const auto& field = *(const JField*)((char*)(this) + pair.second);\
                                            if (cb(pair.first, field, param))\
                                            {\
                                                return true;\
                                            }\
                                        }\
                                        if (BASE::ForEach(cb, param))\
                                        {\
                                            return true;\
                                        }\
                                    }\
                                    return false;\
                                }\
                                bool ForEach(const std::function<bool(const std::string& name, JField& field)>& cb) override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            if (cb(pair.first, field))\
                                            {\
                                                return true;\
                                            }\
                                        }\
                                        if (BASE::ForEach(cb))\
                                        {\
                                            return true;\
                                        }\
                                    }\
                                    return false;\
                                }\
                                bool ForEach(const std::function<bool(const std::string& name, JField& field, void* param)>& cb, void* param) override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            if (cb(pair.first, field, param))\
                                            {\
                                                return true;\
                                            }\
                                        }\
                                        if (BASE::ForEach(cb, param))\
                                        {\
                                            return true;\
                                        }\
                                    }\
                                    return false;\
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

enum class JType
{
    INT,
    NUM,
    STR,
    OBJ,
    ARR,
    DATE,
    BOOL,
    VAR
};

class JField
{
public:
    JField() : und(true), nul(false) {}
    JField(std::nullptr_t) : und(false), nul(true) {}

    virtual JType Type() const = 0;

    virtual void Clear()
    {
        this->und = true;
        this->nul = false;
    }

    virtual bool IsUndefined() const
    {
        return this->und;
    }

    virtual bool IsNull() const
    {
        return this->nul;
    }

    virtual bool IsObj() const
    {
        return false;
    }

    virtual bool HasValue() const
    {
        return !(this->IsNull() || this->IsUndefined());
    }

    virtual JField& operator=(std::nullptr_t)
    {
        this->Clear();
        this->und = false;
        this->nul  = true;
        return *this;
    }

    std::string Serialize() const;
    bool Serialize(std::ostream&  json) const;
    bool Serialize(std::ostream&& json) const;
    bool Deserialize(std::istream&  json);
    bool Deserialize(std::istream&& json);
    bool Deserialize(std::istream&  json, std::string& error, std::size_t& where);
    bool Deserialize(std::istream&& json, std::string& error, std::size_t& where);
    bool Deserialize(const std::string& json);
    bool Deserialize(std::string&& json);
    bool Deserialize(const std::string& json, std::string& error, std::size_t& where);
    bool Deserialize(std::string&& json, std::string& error, std::size_t& where);

protected:
    bool und;
    bool nul;
};

class JArray : public JField
{
public:
    virtual JField* NewItem() = 0;
    virtual bool ForEach(const std::function<bool(const JField& field)>& cb) const = 0;

    JType Type() const override
    {
        return JType::ARR;
    }
};

template<typename T>
class JArr : public JArray
{
    static_assert(std::is_base_of<JField, T>::value, "T must inherit from JField");

public:
    JArr() {}
    JArr(const std::initializer_list<T>& list)
    {
        this->Value = list;
    }

    void Clear() override
    {
        this->Value.clear();
        JArray::Clear();
    }

    bool IsUndefined() const override
    {
        return !this->nul && this->Value.empty();
    }

    bool IsNull() const override
    {
        return this->nul && !this->Length();
    }

    std::size_t Length() const
    {
        return this->Value.size();
    }

    void Insert(const T& value, std::size_t before)
    {
        auto itr = this->Value.begin() + (before > this->Value.size() ? this->Value.size() : before);
        this->Value.insert(itr, value);
    }

    void Insert(const JArr<T>& values, std::size_t before)
    {
        auto itr = this->Value.begin() + (before > this->Value.size() ? this->Value.size() : before);
        this->Value.insert(itr, values.Value.begin(), values.Value.end());
    }

    void Push(const T& value)
    {
        this->Value.push_back(value);
    }

    void Unshift(const T& value)
    {
        this->Value.insert(this->Value.begin(), value);
    }

    void RemoveAt(size_t index)
    {
        if (index < this->Value.size())
        {
            this->Value.erase(this->Value.begin() + index);
        }
    }

    void RemoveIf(const std::function<bool(size_t index, const T& value)>& pred)
    {
        if (pred)
        {
            size_t index = 0;
            this->Value.erase(std::remove_if(this->Value.begin(), this->Value.end(), [&index, &pred](const T& value)
            {
                return pred(index++, value);
            }));
        }
    }

    T* NewItem() override // Caution: Returned pointer may become invalid after subsequent call due to reallocation.
    {
        this->Value.push_back(T());
        return &this->Value.back();
    }

    bool ForEach(const std::function<bool(const JField& field)>& cb) const override
    {
        if (cb)
        {
            for (auto& field : this->Value)
            {
                if (cb(field))
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool ForEachItem(const std::function<void(const T& item)>& cb) const
    {
        if (cb)
        {
            return this->ForEach([&](const JField& field)
            {
                return cb((const T&)field);
            });
        }
        return false;
    }

    const std::vector<T>& ValueOrDefault(const std::vector<T>& defVal) const
    {
        return this->HasValue() ? this->Value : defVal;
    }

    T& operator[](std::size_t index)
    {
        return this->Value[index];
    }

    const T& operator[](std::size_t index) const
    {
        return this->Value[index];
    }

    const std::vector<T>& operator()() const
    {
        return this->Value;
    }

    const std::vector<T>& operator()(const std::vector<T>& defVal) const
    {
        return this->ValueOrDefault(defVal);
    }

    JArr& operator=(std::nullptr_t) override
    {
        this->Value.clear();
        JField::operator=(nullptr);
        return *this;
    }

    std::vector<T> Value;
};

template<typename T, typename U>
class JValue : public JField
{
public:
    JValue() = default;
    JValue(const T& value) : Value(value)
    {
        this->und = false;
        this->nul = false;
    }
    explicit JValue(std::nullptr_t) : JField(nullptr) {}

    virtual U& operator=(const T& value)
    {
        this->Value = value;
        this->und = false;
        this->nul = false;
        return (U&)*this;
    }

    JValue& operator=(std::nullptr_t) override
    {
        return (JValue&)JField::operator=(nullptr);
    }

    const T& ValueOrDefault(const T& defVal) const
    {
        return this->HasValue() ? this->Value : defVal;
    }

    bool operator==(const JValue<T, U>& other)
    {
        if (this->und && other.und)
        {
            return true;
        }

        if (this->und != other.und)
        {
            return false;
        }

        if (this->nul && other.nul)
        {
            return true;
        }

        if (this->nul != other.nul)
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

    const T& operator()(const T& defVal) const
    {
        return this->ValueOrDefault(defVal);
    }

    T Value;
};

#define JVALUE(T, U, V) U() = default;\
                        U(T t) : JValue<T, U>(t) {}\
                        explicit U(std::nullptr_t) : JValue<T, U>(nullptr) {}\
                        JType Type() const override { return JType::V; }\
                        using JValue<T, U>::operator=;\
                        U& operator=(std::nullptr_t) override { return (U&)JValue<T, U>::operator=(nullptr); }

class JInt : public JValue<int64_t, JInt>
{
public:
    JVALUE(int64_t, JInt, INT);

    JInt& operator=(int32_t v)
    {
        *this = (int64_t)v;
        return *this;
    }

    JInt& operator=(uint32_t v)
    {
        *this = (int64_t)v;
        return *this;
    }
};

class JNum : public JValue<double, JNum>
{
public:
    JVALUE(double, JNum, NUM);
};

class JStr : public JValue<std::string, JStr>
{
public:
    JVALUE(std::string, JStr, STR);

    void Clear() override
    {
        this->Value.clear();
        JValue<std::string, JStr>::Clear();
    }
};

class JBool : public JValue<bool, JBool>
{
public:
    JVALUE(bool, JBool, BOOL);
};

class JDate : public JValue<time_t, JDate>
{
public:
    JVALUE(time_t, JDate, DATE);
};

class JObject : public JField
{
public:
    JType Type() const override
    {
        return JType::OBJ;
    }

    bool IsObj() const override
    {
        return true;
    }

    virtual JField* GetField(const std::string&) = 0;
    virtual const JField* GetField(const std::string&) const = 0;

    // ForEach() stops and returns true if any callback returns true.
    virtual bool ForEach(const std::function<bool(const std::string& name, const JField& field)>& cb) const = 0;
    virtual bool ForEach(const std::function<bool(const std::string& name, const JField& field, void*)>& cb, void*) const = 0;
    virtual bool ForEach(const std::function<bool(const std::string& name, JField& field)>& cb) = 0;
    virtual bool ForEach(const std::function<bool(const std::string& name, JField& field, void*)>& cb, void*) = 0;

protected:
    static JField* GetField(JObject* object, std::size_t offset)
    {
        return (JField*)((char*)object + offset);
    }
    static const JField* GetField(const JObject* object, std::size_t offset)
    {
        return (const JField*)((char*)object + offset);
    }
};

class JVariant : public JField
{
public:
    virtual void  Subtype(JType subtype) = 0;
    virtual JType Subtype() const = 0;

    virtual JVariant* NewItem() = 0;
    virtual JVariant* GetField(const std::string&) = 0;

    virtual bool ToArr(JArray&, std::string&) const = 0;
    virtual bool ToObj(JObject&, std::string&) const = 0;

    virtual void ForEach(const std::function<void(const std::string&, const JVariant&)>&) const = 0;
    virtual void ForEach(const std::function<void(const JVariant&)>&) const = 0;

    virtual JVariant& operator=(int64_t) = 0;
    virtual JVariant& operator=(int32_t) = 0;
    virtual JVariant& operator=(double) = 0;
    virtual JVariant& operator=(float) = 0;
    virtual JVariant& operator=(bool) = 0;
    virtual JVariant& operator=(const std::string&) = 0;
    virtual JVariant& operator=(const char*) = 0;
    virtual JVariant& operator=(const JField&) = 0;
    virtual JVariant& operator=(std::nullptr_t) = 0;

    virtual int64_t Int() const = 0;
    virtual double  Num() const = 0;
    virtual bool    Bool() const = 0;
    virtual const std::string& Str() const = 0;
};

class JVar : public JVariant
{
public:
    JVar() : subtype(JType::VAR) {}

    bool IsUndefined() const override
    {
        if (this->nul)
        {
            return false;
        }
        else if (JType::ARR == this->subtype ||
                 JType::OBJ == this->subtype)
        {
            for (auto& var : this->fields)
            {
                if (!var.second.IsUndefined())
                {
                    return false;
                }
            }

            return true;
        }
        else
        {
            return this->und;
        }
    }

    void Clear() override
    {
        this->subtype = JType::VAR;
        this->str.clear();
        this->fields.clear();
        JField::Clear();
    }

    JType Type() const override
    {
        return JType::VAR;
    }

    bool IsObj() const override
    {
        return JType::OBJ == this->subtype;
    }

    void Subtype(JType subtype) override
    {
        if (this->subtype != subtype)
        {
            this->Clear();
        }
        this->subtype = subtype;
    }

    virtual JType Subtype() const override
    {
        return this->subtype;
    }

    virtual std::size_t Size() const
    {
        return (JType::ARR == this->subtype || JType::OBJ == this->subtype) ? this->fields.size() : 0;
    }

    virtual bool HasField(const std::string& name) const
    {
        return this->fields.end() != this->fields.find(name);
    }

    // ForEachField()/ForEachItem() stops and returns true if any callback returns true
    bool ForEachField(const std::function<bool(const std::string& name, const JVar& var)>& cb) const
    {
        if (JType::OBJ == this->subtype && cb)
        {
            for (auto& pair : this->fields)
            {
                if (cb(pair.first, pair.second))
                {
                    return true;
                }
            }
        }

        return false;
    }
    bool ForEachField(const std::function<bool(const std::string& name, const JVar& var, void* param)>& cb, void* param) const
    {
        if (JType::OBJ == this->subtype && cb)
        {
            for (auto& pair : this->fields)
            {
                if (cb(pair.first, pair.second, param))
                {
                    return true;
                }
            }
        }

        return false;
    }
    bool ForEachField(const std::function<bool(const std::string& name, JVar& var)>& cb)
    {
        if (JType::OBJ == this->subtype && cb)
        {
            for (auto& pair : this->fields)
            {
                if (cb(pair.first, pair.second))
                {
                    return true;
                }
            }
        }

        return false;
    }
    bool ForEachField(const std::function<bool(const std::string& name, JVar& var, void* param)>& cb, void* param)
    {
        if (JType::OBJ == this->subtype && cb)
        {
            for (auto& pair : this->fields)
            {
                if (cb(pair.first, pair.second, param))
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool ForEachItem(const std::function<bool(const JVar& var)>& cb) const
    {
        if (JType::ARR == this->subtype && cb)
        {
            for (std::size_t i = 0; i < this->fields.size(); i++)
            {
                if (cb((*this)[i]))
                {
                    return true;
                }
            }
        }

        return false;
    }
    bool ForEachItem(const std::function<bool(JVar& var)>& cb)
    {
        if (JType::ARR == this->subtype && cb)
        {
            for (std::size_t i = 0; i < this->fields.size(); i++)
            {
                if (cb((*this)[i]))
                {
                    return true;
                }
            }
        }

        return false;
    }

    JVar* NewItem() override
    {
        this->Subtype(JType::ARR);

        auto name = std::to_string(this->fields.size());
        return &(this->fields[name] = JVar());
    }
    JVar* GetField(const std::string& name) override
    {
        this->Subtype(JType::OBJ);

        auto itr = this->fields.find(name);
        if (this->fields.end() != itr)
        {
            return &itr->second;
        }

        return &(this->fields[name] = JVar());
    }

    bool ToArr(JArray& arr, std::string& err) const override;
    bool ToObj(JObject& obj, std::string& err) const override;

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

    void ForEach(const std::function<void(const std::string&, const JVariant&)>& cb) const override
    {
        if (JType::OBJ == this->subtype && cb)
        {
            for (auto& pair : this->fields)
            {
                cb(pair.first, pair.second);
            }
        }
    }
    void ForEach(const std::function<void(const JVariant&)>& cb) const override
    {
        if (JType::ARR == this->subtype && cb)
        {
            for (auto& pair : this->fields)
            {
                cb(pair.second);
            }
        }
    }

    JVar& operator[](std::size_t index)
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
    const JVar& operator[](std::size_t index) const
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
        if (JType::OBJ != this->Subtype() && JType::VAR != this->Subtype())
        {
            throw std::runtime_error("Is not an object");
        }

        return *this->GetField(field);
    }
    const JVar& operator[](const std::string& field) const
    {
        if (JType::OBJ != this->Subtype())
        {
            throw std::runtime_error("Is not an object");
        }

        if (!this->HasValue() || !this->HasField(field))
        {
            return UndVar;
        }

        return this->fields.find(field)->second;
    }

    JVar& operator=(int64_t value) override
    {
        this->Subtype(JType::INT);
        this->und = false;
        this->nul = false;
        this->val.i = value;
        return *this;
    }

    JVar& operator=(int32_t value) override
    {
        return (*this = (int64_t)value);
    }

    JVar& operator=(double value) override
    {
        this->Subtype(JType::NUM);
        this->und = false;
        this->nul = false;
        this->val.n = value;
        return *this;
    }

    JVar& operator=(float value) override
    {
        return (*this = (double)value);
    }

    JVar& operator=(bool value) override
    {
        this->Subtype(JType::BOOL);
        this->und = false;
        this->nul = false;
        this->val.b = value;
        return *this;
    }

    JVar& operator=(const std::string& value) override
    {
        this->Subtype(JType::STR);
        this->und = false;
        this->nul = false;
        this->str = value;
        return *this;
    }

    JVar& operator=(const char* value) override
    {
        this->Subtype(JType::STR);
        this->und = false;
        this->nul = false;
        this->str = value;
        return *this;
    }

    JVar& operator=(const JField& field) override;

    JVar& operator=(std::nullptr_t) override
    {
        return (JVar&)JField::operator=(nullptr);
    }

    JVar& operator=(const JVar& var)
    {
        this->Clear();

        ((JField&)*this) = var;

        this->str = var.str;
        this->val = var.val;
        this->fields = var.fields;
        this->subtype = var.subtype;

        return *this;
    }

    int64_t Int() const override
    {
        return this->val.i;
    }

    int64_t Int(int64_t defVal) const
    {
        return (JType::INT == this->subtype && this->HasValue()) ? this->Int() : defVal;
    }

    double Num() const override
    {
        return this->val.n;
    }

    double Num(double defVal) const
    {
        return (JType::NUM == this->subtype && this->HasValue()) ? this->Num() : defVal;
    }

    bool Bool() const override
    {
        return this->val.b;
    }

    bool Bool(bool defVal) const
    {
        return (JType::BOOL == this->subtype && this->HasValue()) ? this->Bool() : defVal;
    }

    const std::string& Str() const override
    {
        return this->str;
    }

    const std::string& Str(const std::string& defVal) const
    {
        return (JType::STR == this->subtype && this->HasValue()) ? this->Str() : defVal;
    }

protected:
    JType subtype;

    union
    {
        int64_t i;
        double  n;
        bool    b;
    } val;
    std::string str;

    std::map<std::string, JVar> fields;
    static const JVar UndVar;
};

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
    static double       GetFlt(const std::string& num);
    static std::string  GetStr(std::istream& json);
    static std::string  GetNum(std::istream& json);
    static int64_t      GetInt(std::istream& json);
    static int64_t      GetInt(const std::string& num);
    static int64_t      GetInt(const std::string& json, std::string::size_type& off);
    static std::string  GetName(std::istream& json);
    static time_t       GetDate(std::istream& json);
    static bool         GetBool(std::istream& json);
    static uint64_t     GetUint(std::istream& json);
    static uint64_t     GetUint(const std::string& json, std::string::size_type& off);
    static bool         IsFloat(const std::string& num);

    static void GetJson(const std::string& name, const JField& field, std::ostream& json);
    static void GetJson(const JVariant& var, std::ostream& json);
    static void GetJson(const JObject& obj, std::ostream& json);
    static void GetJson(const JArray& arr, std::ostream& json);
    static void GetJson(const JDate& date, std::ostream& json);
    static void GetJson(const JStr& str, std::ostream& json);
};