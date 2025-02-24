#pragma once

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
                                    if (!this->und)\
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
                                JField* GetField(std::size_t offset) override\
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
                                void ForEachConst(const std::function<void(const std::string& name, const JField& field)>& cb) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first, field);\
                                        }\
                                    }\
                                }\
                                void ForEachConst(const std::function<void(const std::string& name, const JField& field, void* param)>& cb, void* param) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first, field, param);\
                                        }\
                                    }\
                                }\
                                void ForEach(const std::function<void(const std::string& name, JField& field)>& cb) override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first, field);\
                                        }\
                                    }\
                                }\
                                void ForEach(const std::function<void(const std::string& name, JField& field, void* param)>& cb, void* param) override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first, field, param);\
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
                                CLASS& operator=(std::nullptr_t) override\
                                {\
                                    return (CLASS&)JField::operator=(nullptr);\
                                }\
                                bool IsUndefined() const override\
                                {\
                                    if (!this->und)\
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
                                JField* GetField(std::size_t offset) override\
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
                                void ForEachConst(const std::function<void(const std::string& name, const JField& field)>& cb) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first, field);\
                                        }\
                                        BASE::ForEach(cb);\
                                    }\
                                }\
                                void ForEachConst(const std::function<void(const std::string& name, const JField& field, void* param)>& cb, void* param) const override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first, field, param);\
                                        }\
                                        BASE::ForEach(cb);\
                                    }\
                                }\
                                void ForEach(const std::function<void(const std::string& name, JField& field)>& cb) override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first, field);\
                                        }\
                                        BASE::ForEach(cb);\
                                    }\
                                }\
                                void ForEach(const std::function<void(const std::string& name, JField& field, void* param)>& cb, void* param) override\
                                {\
                                    if (cb)\
                                    {\
                                        for (auto& pair : FieldOffsets)\
                                        {\
                                            auto& field = *(JField*)((char*)(this) + pair.second);\
                                            cb(pair.first, field, param);\
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
    virtual JField* GetNew() = 0;
    virtual void ForEach(const std::function<void(const JField& field)>& cb) const = 0;

    JType Type() const override
    {
        return JType::ARR;
    }

    void Define()
    {
        this->und = false;
        this->nul = false;
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
        JArray::Clear();
    }

    bool IsUndefined() const override
    {
        return this->und && !this->Length();
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

    void Insert(const std::initializer_list<T>& list, std::size_t before)
    {
        auto itr = this->Value.begin() + (before > this->Value.size() ? this->Value.size() : before);
        this->Value.insert(itr, list);
    }

    void Push(const T& value)
    {
        this->Value.push_back(value);
    }

    void Unshift(const T& value)
    {
        this->Value.insert(this->Value.begin(), value);
    }

    T* GetNew() override // Caution: Returned pointer may become invalid after subsequent call due to reallocation.
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

    T& operator[](std::size_t index)
    {
        return this->Value[index];
    }

    const T& operator[](std::size_t index) const
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

    JArr& operator=(std::nullptr_t) override
    {
        this->Value.clear();
        JField::operator=(nullptr);
        return *this;
    }

    JArr& operator=(const std::initializer_list<T>& list)
    {
        this->Value = list;
        this->Define();
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

    void Define()
    {
        this->und = false;
        this->nul = false;
    }

    virtual JField* GetField(std::size_t offset) = 0;
    virtual JField* GetField(const std::string&) = 0;
    virtual void ForEachConst(const std::function<void(const std::string& name, const JField& field)>& cb) const = 0;
    virtual void ForEachConst(const std::function<void(const std::string& name, const JField& field, void* param)>& cb, void*) const = 0;
    virtual void ForEach(const std::function<void(const std::string& name, JField& field)>& cb) = 0;
    virtual void ForEach(const std::function<void(const std::string& name, JField& field, void* param)>& cb, void*) = 0;
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
            this->und = false;
            this->nul = false;
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

    virtual std::size_t Size() const
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
            for (std::size_t i = 0; i < this->fields.size(); i++)
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

    JVar& operator[](std::size_t index);
    const JVar& operator[](std::size_t index) const;
    JVar& operator[](const std::string& field);
    const JVar& operator[](const std::string& field) const;

    virtual JVar& operator=(int64_t value)
    {
        this->Str.clear();
        this->fields.clear();

        this->subtype = JType::INT;
        this->und = false;
        this->nul = false;
        this->Int = value;

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
        this->und = false;
        this->nul = false;
        this->Num = value;

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
        this->und = false;
        this->nul = false;
        this->Bool = value;

        return *this;
    }

    virtual JVar& operator=(const std::string& value)
    {
        this->fields.clear();

        this->subtype = JType::STR;
        this->und = false;
        this->nul = false;
        this->Str = value;

        return *this;
    }

    virtual JVar& operator=(const char* value)
    {
        this->fields.clear();

        this->subtype = JType::STR;
        this->und = false;
        this->nul = false;
        this->Str = value;

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

    JVar& operator=(std::nullptr_t) override
    {
        return (JVar&)JField::operator=(nullptr);
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

    std::size_t Size() const override
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

    JVar& operator=(std::nullptr_t) override
    {
        return *this;
    }
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
    static void GetJson(const JObject& obj, std::ostream& json);
    static void GetJson(const JArray& arr, std::ostream& json);
    static void GetJson(const JDate& date, std::ostream& json);
    static void GetJson(const JStr& str, std::ostream& json);
    static void GetJson(const JVar& var, std::ostream& json);
};