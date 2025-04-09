#include "JObject.h"
#include <cassert>
#include <codecvt>
#include <iomanip>
#include <limits>
#include <sstream>

#define GETFIELD(f, n)  (JType::OBJ == f->Type() ? ((JObject*)f)->GetField(n) : ((JVar*)f)->GetNewField(n))
#define GETNEW(f)       (JType::ARR == f->Type() ? ((JArray*)f)->GetNew() : ((JVar*)f)->GetNewItem())

using namespace std;

wstring_convert<codecvt_utf8<wchar_t>> utf8;

inline std::string to_string(JType type)
{
    switch (type)
    {
        case JType::INT:
            return "JType::INT";

        case JType::NUM:
            return "JType::NUM";

        case JType::STR:
            return "JType::STR";

        case JType::OBJ:
            return "JType::OBJ";

        case JType::ARR:
            return "JType::ARR";

        case JType::DATE:
            return "JType::DATE";

        case JType::BOOL:
            return "JType::BOOL";

        case JType::VAR:
            return "JType::VAR";

        default:
            return "Unknown type";
    }
}

inline std::ostream& operator<<(std::ostream& stream, JType type)
{
    stream << to_string(type);
    return stream;
}

string JField::Serialize() const
{
    ostringstream json;
    this->Serialize(json);
    return json.str();
}

bool JField::Serialize(ostream& json) const
{
    JParser::Serialize(json, *this);
    return !!json;
}

bool JField::Serialize(ostream&& json) const
{
    return this->Serialize((ostream&)json);
}

bool JField::Deserialize(istream& json)
{
    try
    {
        JParser::Deserialize(json, *this);
        return true;

    } catch(const exception&)
    {
        this->Clear();
        return false;
    }
}

bool JField::Deserialize(istream&& json)
{
    return this->Deserialize((istream&)json);
}

bool JField::Deserialize(istream& json, string& error, size_t& where)
{
    try
    {
        where = -1;
        error.clear();
        JParser::Deserialize(json, *this);
        return true;

    } catch(const exception& e)
    {
        if (!json)
        {
            json.clear();
            json.seekg(0, ios::end);
        }
        where = json.tellg();
        error = e.what();
        this->Clear();
        return false;
    }
}

bool JField::Deserialize(istream&& json, string& error, size_t& where)
{
    return this->Deserialize((istream&)json, error, where);
}

bool JField::Deserialize(const string& json)
{
    auto stream = istringstream(json);

    try
    {
        JParser::Deserialize(stream, *this);
        return true;

    } catch(const exception&)
    {
        this->Clear();
        return false;
    }
}

bool JField::Deserialize(string&& json)
{
    return this->Deserialize((const string&)json);
}

bool JField::Deserialize(const string& json, string& error, size_t& where)
{
    return this->Deserialize(istringstream(json), error, where);
}

bool JField::Deserialize(string&& json, string& error, size_t& where)
{
    return this->Deserialize(istringstream(json), error, where);
}

bool JVar::ToArr(JArray& arr, string& err) const
{
    arr.Clear();

    if (this->IsUndefined())
    {
        return true;
    }

    if (this->IsNull())
    {
        (JField&)arr = nullptr;
        return true;
    }

    if (JType::ARR != this->subtype)
    {
        err = "This is not an array";
        return false;
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
                return false;
            }

            if (JType::VAR == item->Type())
            {
                *(JVar*)item = var;
            }
            else if (item->Type() == var.Subtype())
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
                            throw runtime_error(err);
                        }
                        break;
                    }

                    case JType::OBJ:
                    {
                        if (!var.ToObj(*(JObject*)item, err))
                        {
                            throw runtime_error(err);
                        }
                        break;
                    }

                    default: break;
                }
            }
            else if (JType::NUM == item->Type() && JType::INT == var.Subtype())
            {
                *(JNum*)item = (double)var.Int;
            }
            else
            {
                throw runtime_error("Type mismatch. Expecting: " + to_string(item->Type()) + ". Actual: " + to_string(var.Subtype()));
            }

            return false;
        });
    } catch (const exception& e)
    {
        err = e.what();
        return false;
    }

    return true;
}

bool JVar::ToObj(JObject& obj, string& err) const
{
    obj.Clear();

    if (this->IsUndefined())
    {
        return true;
    }

    if (this->IsNull())
    {
        (JField&)obj = nullptr;
        return true;
    }

    if (JType::OBJ != this->subtype)
    {
        err = "This is not an object";
        return false;
    }

    try
    {
        this->ForEachField([&obj, &err](const string& name, const JVar& var)
        {
            if (var.IsUndefined())
            {
                return false;
            }

            auto field = obj.GetField(name);
            if (!field)
            {
                return false;
            }

            if (var.IsNull())
            {
                *field = nullptr;
                return false;
            }

            if (JType::VAR == field->Type())
            {
                *(JVar*)field = var;
            }
            else if (field->Type() == var.Subtype())
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
                            throw runtime_error(err);
                        }
                        break;
                    }

                    case JType::OBJ:
                    {
                        if (!var.ToObj(*(JObject*)field, err))
                        {
                            throw runtime_error(err);
                        }
                        break;
                    }

                    default: break;
                }
            }
            else if (JType::NUM == field->Type() && JType::INT == var.Subtype())
            {
                *(JNum*)field = (double)var.Int;
            }
            else
            {
                throw runtime_error("Type mismatch: " + name + ". Expecting: " + to_string(field->Type()) + ". Actual: " + to_string(var.Subtype()));
            }

            return false;
        });
    } catch (const exception& e)
    {
        err = e.what();
        return false;
    }

    return true;
}

JVar& JVar::operator=(const JField& field)
{
    this->Clear();

    if (field.IsUndefined())
    {
        return *this;
    }

    if (field.IsNull())
    {
        *this = nullptr;
        return *this;
    }

    switch (field.Type())
    {
        case JType::INT:
        {
            *this = ((JInt&)field)();
            break;
        }

        case JType::NUM:
        {
            *this = ((JNum&)field)();
            break;
        }

        case JType::STR:
        {
            *this = ((JStr&)field)();
            break;
        }

        case JType::BOOL:
        {
            *this = ((JBool&)field)();
            break;
        }

        case JType::DATE:
        {
            *this = field.Serialize();
            break;
        }

        case JType::ARR:
        {
            ((JArray&)field).ForEach([this](const JField& field)
            {
                auto item = this->GetNewItem();
                *item = field;
                return false;
            });

            break;
        }

        case JType::OBJ:
        {
            ((JObject&)field).ForEach([this](const string& name, const JField& field)
            {
                auto f = this->GetNewField(name);
                *f = field;
                return false;
            });

            break;
        }

        case JType::VAR:
        {
            *this = (JVar&)field;
            break;
        }

        default: break;
    }

    return *this;
}


inline runtime_error TypeMismatch(const string& name, JType expected, JType actual)
{
    return runtime_error(("Type mismatch: " + name + ". Expecting: " + to_string(expected) + ". Actual: " + to_string(actual)).c_str());
}

inline runtime_error ExpectMore()
{
    return runtime_error("Expecting more charactors");
}

inline runtime_error Unexpected()
{
    return runtime_error("Unexpected charactor");
}

inline bool IsNumber(char c)
{
    return c >= '+' && c <= '9' && c != ',' && c != '/';
}

inline char GetChar(istream& json)
{
    char c;
    json.read(&c, 1);

    if (json.eof())
    {
        throw ExpectMore();
    }

    return c;
}

inline char FirstNotSpace(istream& json)
{
    char c;

    do
    {
        c = GetChar(json);

    } while (' ' == c || '\r' == c || '\n' == c || '\t' == c);

    return c;
}

void JParser::Serialize(ostream& json, const JField& field)
{
    GetJson("", field, json);
}

void JParser::Deserialize(istream& json, JField& field)
{
    field.Clear();

    char c;
    do
    {
        json.read(&c, 1);
        if (json.eof())
        {
            return;
        }
    } while (' ' == c || '\r' == c || '\n' == c || '\t' == c);

    json.seekg(-1, ios::cur);

    GetVal(json, "", &field);

    json.read(&c, 1);

    while (!json.eof())
    {
        if ('\t' != c && '\r' != c && '\n' != c && ' ' != c)
        {
            json.seekg(-1, ios::cur);
            throw Unexpected();
        }

        json.read(&c, 1);
    }
}

void JParser::GetVal(istream& json, const string& name, JField* field)
{
    auto c = FirstNotSpace(json);
    switch (c)
    {
        case '{':
        {
            if (field && JType::OBJ != field->Type() && JType::VAR != field->Type())
            {
                json.seekg(-1, ios::cur);
                throw TypeMismatch(name, field->Type(), JType::OBJ);
            }
            GetObj(json, (JObject*)field);
            break;
        }

        case '[':
        {
            if (field && JType::ARR != field->Type() && JType::VAR != field->Type())
            {
                json.seekg(-1, ios::cur);
                throw TypeMismatch(name, field->Type(), JType::ARR);
            }
            GetArr(json, name, field);
            break;
        }

        case '\"':
        {
            if (field)
            {
                if (JType::STR == field->Type())
                {
                    *(JStr*)field = GetStr(json);
                }
                else if (JType::VAR == field->Type())
                {
                    *(JVar*)field = GetStr(json);
                }
                else if (JType::DATE == field->Type())
                {
                    *(JDate*)field = GetDate(json);
                }
                else
                {
                    json.seekg(-1, ios::cur);
                    throw TypeMismatch(name, field->Type(), JType::STR);
                }
            }
            else
            {
                GetStr(json);
            }
            break;
        }

        default:
        {
            json.seekg(-1, ios::cur);

            switch (c)
            {
                case 't':
                case 'f':
                {
                    if (field)
                    {
                        if (JType::BOOL == field->Type())
                        {
                            *(JBool*)field = GetBool(json);
                        }
                        else if (JType::VAR == field->Type())
                        {
                            *(JVar*)field = GetBool(json);
                        }
                        else
                        {
                            throw TypeMismatch(name, field->Type(), JType::BOOL);
                        }
                    }
                    else
                    {
                        GetBool(json);
                    }
                    break;
                }

                case 'n':
                {
                    if ('n' == GetChar(json) &&
                        'u' == GetChar(json) &&
                        'l' == GetChar(json) &&
                        'l' == GetChar(json))
                    {
                        if (field)
                        {
                            *field = nullptr;
                        }
                    }
                    else
                    {
                        json.seekg(-1, ios::cur);
                        throw Unexpected();
                    }
                    break;
                }

                default:
                {
                    if (IsNumber(c))
                    {
                        if (field)
                        {
                            switch (field->Type())
                            {
                                case JType::INT:
                                {
                                    *(JInt*)field = GetInt(json);
                                    break;
                                }

                                case JType::NUM:
                                {
                                    *(JNum*)field = GetFlt(json);
                                    break;
                                }

                                case JType::VAR:
                                {
                                    auto num = GetNum(json);
                                    if (IsFloat(num))
                                    {
                                        *(JVar*)field = GetFlt(num);
                                    }
                                    else
                                    {
                                        *(JVar*)field = GetInt(num);
                                    }
                                    break;
                                }

                                default:
                                {
                                    throw TypeMismatch(name, field->Type(), JType::NUM);
                                }
                            }
                        }
                        else
                        {
                            GetFlt(json);
                        }
                    }
                    else
                    {
                        throw Unexpected();
                    }
                    break;
                }
            }

            break;
        }
    }
}

void JParser::GetArr(istream& json, const string& name, JField* arr)
{
    if (arr)
    {
        if (JType::ARR == arr->Type())
        {
            ((JArray*)arr)->Define();
        }
        else
        {
            ((JVar*)arr)->Subtype(JType::ARR);
        }
    }

    auto c = FirstNotSpace(json);
    if (']' == c)
    {
        return;
    }

    json.seekg(-1, ios::cur);

    do
    {
        GetVal(json, name, arr ? GETNEW(arr) : nullptr);

    } while ([&]
    {
        switch (FirstNotSpace(json))
        {
            case ',': return true;

            case ']': return false;

            default:
            {
                json.seekg(-1, ios::cur);
                throw Unexpected();
            }
        }
    }());
}

void JParser::GetObj(istream& json, JField* obj)
{
    if (obj && JType::VAR == obj->Type())
    {
        ((JVar*)obj)->Subtype(JType::OBJ);
    }

    auto c = FirstNotSpace(json);
    if ('}' == c)
    {
        return;
    }

    do
    {
        if ('\"' != c)
        {
            json.seekg(-1, ios::cur);
            throw Unexpected();
        }

        auto n = GetName(json);
        auto f = obj ? GETFIELD(obj, n) : nullptr;

        c = GetChar(json);
        while ('.' == c)
        {
            if (f)
            {
                switch (f->Type())
                {
                    case JType::OBJ:
                    case JType::VAR:
                    {
                        break;
                    }

                    default:
                    {
                        json.seekg(-1, ios::cur);
                        throw TypeMismatch(n, f->Type(), JType::OBJ);
                    }
                }
            }

            n = GetName(json);
            f = f ? GETFIELD(f, n) : nullptr;

            c = GetChar(json);
        }
        assert('\"' == c);

        c = FirstNotSpace(json);

        if (':' != c)
        {
            json.seekg(-1, ios::cur);
            throw Unexpected();
        }

        GetVal(json, n, f);

    } while ([&]
    {
        switch (FirstNotSpace(json))
        {
            case ',':
            {
                c = FirstNotSpace(json);
                return true;
            }

            case '}':
            {
                return false;
            }

            default:
            {
                json.seekg(-1, ios::cur);
                throw Unexpected();
            }
        }
    }());
}

double JParser::GetFlt(istream& json)
{
    double v;
    json >> v;
    return v;
}

double JParser::GetFlt(const string& num)
{
    char* end;
    double v = strtod(num.c_str(), &end);

    if (num.c_str() == end)
    {
        throw runtime_error("Failed to parse double");
    }

    return v;
}

string JParser::GetStr(istream& json)
{
    ostringstream oss;

    char c = 0;
    char p;
    do
    {
        p = c;
        c = GetChar(json);

    } while ([&]
    {
        switch (c)
        {
            case '\\':
            {
                if ('\\' == p)
                {
                    oss << c;
                    c = 0;
                }
                break;
            }

            case '\"':
            {
                if ('\\' != p)
                {
                    return false;
                }

                oss << c;
                break;
            }

            case 'n':
            {
                oss << ('\\' == p ? '\n' : 'n');
                break;
            }

            case 'r':
            {
                oss << ('\\' == p ? '\r' : 'r');
                break;
            }

            case 't':
            {
                oss << ('\\' == p ? '\t' : 't');
                break;
            }

            case 'u':
            {
                if ('\\' == p)
                {
                    char buf[5];
                    for (int i = 0; i < 4; i++)
                    {
                        c = tolower(GetChar(json));
                        if (('0' <= c && '9' >= c) ||
                            ('a' <= c && 'f' >= c))
                        {
                            buf[i] = c;
                            continue;
                        }

                        json.seekg(-1, ios::cur);
                        throw Unexpected();
                    }
                    buf[4] = '\0';
                    oss << utf8.to_bytes((wchar_t)strtol(buf, nullptr, 16));
                }
                else
                {
                    oss << c;
                }
                break;
            }

            default:
            {
                if ('\\' == p)
                {
                    json.seekg(-1, ios::cur);
                    throw Unexpected();
                }
                oss << c;
                break;
            }
        }

        return true;
    }());

    return oss.str();
}

string JParser::GetNum(istream& json)
{
    ostringstream oss;

    bool digit = false;
    bool sign  = false;
    bool hex   = false;
    bool exp   = false;

    auto c = GetChar(json);
    auto p = json.peek();

    if ('0' == c && ('x' == p || 'X' == p))
    {
        oss << c << GetChar(json);

        c = GetChar(json);

        digit = true;
        sign  = true;
        hex   = true;
    }

    const auto isnum = [hex](char c)
    {
        if (c >= '0' && c <= '9')
        {
            return true;
        }

        if (hex && ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
        {
            return true;
        }

        return false;
    };

    if ('+' != c && '-' != c && '.' != c && !isnum(c))
    {
        throw Unexpected();
    }

    do
    {
        if ('+' == c || '-' == c)
        {
            if (sign)
            {
                throw Unexpected();
            }

            sign = true;
        }
        else if ('E' == c || 'e' == c)
        {
            if (exp)
            {
                throw Unexpected();
            }

            exp  = true;
            sign = false;
        }
        else if ('.' == c)
        {
            if (digit)
            {
                throw Unexpected();
            }

            digit = true;
        }

        oss << c;

    } while ([&json, &c, &isnum]
    {
        auto p = json.peek();

        if ('.' == p || '+' == p || '-' == p || 'e' == p || 'E' == p || isnum(p))
        {
            c = GetChar(json);
            return true;
        }

        return false;
    }());

    return oss.str();
}

int64_t JParser::GetInt(istream& json)
{
    int64_t v;

    auto c = GetChar(json);
    auto p = json.peek();
    if ('0' == c && ('x' == p || 'X' == p))
    {
        json.seekg(1, ios::cur);
        json >> hex >> v;
    }
    else
    {
        if (json.eof())
        {
            json.clear();
            json.seekg(-1, ios::end);
        }
        else
        {
            json.seekg(-1, ios::cur);
        }

        json >> v;
    }

    return v;
}

int64_t JParser::GetInt(const string& num)
{
    int64_t v;
    char* end;

    if (num.length() > 2 && '0' == num[0] && ('x' == num[1] || 'X' == num[1]))
    {
        v = strtoll(num.c_str(), &end, 16);
    }
    else
    {
        v = strtoll(num.c_str(), &end, 10);
    }

    if (num.c_str() == end)
    {
        throw runtime_error("Failed to parse int");
    }

    return v;
}

int64_t JParser::GetInt(const string& json, string::size_type& off)
{
    char* end;
    int64_t value;

    if (json.length() > off + 2 && '0' == json[off] && ('x' == json[off + 1] || 'X' == json[off + 1]))
    {
        value = strtoll(json.c_str() + off, &end, 16);
    }
    else
    {
        value = strtoll(json.c_str() + off, &end, 10);
    }

    if (end == json.c_str() + off)
    {
        throw runtime_error("Failed to parse int");
    }

    off = end - json.c_str();

    return value;
}

uint64_t JParser::GetUint(istream& json)
{
    uint64_t v;

    auto c = GetChar(json);
    if ('0' == c && 'x' == json.peek())
    {
        json.seekg(1, ios::cur);
        json >> hex >> v;
    }
    else
    {
        json.seekg(-1, ios::cur);
        json >> v;
    }

    return v;
}

uint64_t JParser::GetUint(const string& json, string::size_type& off)
{
    char* end;
    uint64_t value;

    if (json.length() > off + 2 && '0' == json[off] && ('x' == json[off + 1] || 'X' == json[off + 1]))
    {
        value = std::strtoull(json.c_str() + off, &end, 16);
    }
    else
    {
        value = std::strtoull(json.c_str() + off, &end, 10);
    }

    if (end == json.c_str() + off)
    {
        throw runtime_error("Failed to parse uint");
    }

    off = end - json.c_str();

    return value;
}

string JParser::GetName(istream& json)
{
    ostringstream oss;

    char c = 0;
    char p;
    do
    {
        p = c;
        c = GetChar(json);

    } while ([&]
    {
        switch (c)
        {
            case '\\':
            {
                if ('\\' == p)
                {
                    oss << c;
                    c = 0;
                }
                break;
            }

            case '\"':
            {
                if ('\\' != p)
                {
                    json.seekg(-1, ios::cur);
                    return false;
                }

                oss << c;
                break;
            }

            default:
            {
                if ('\\' == p)
                {
                    json.seekg(-1, ios::cur);
                    throw Unexpected();
                }

                if ('.' == c)
                {
                    json.seekg(-1, ios::cur);
                    return false;
                }

                oss << c;
                break;
            }
        }

        return true;
    }());

    return oss.str();
}

time_t JParser::GetDate(istream& json)
{
    auto beg = json.tellg();
    auto date = GetStr(json);

    if (0 == date.find("\\/Date("))
    {
        string::size_type pos = 0;
        time_t time = GetInt(date, pos);

        if ('+' == date[pos] || '-' == date[pos])
        {
            auto zone = GetInt(date, pos);
            auto hours = zone / 100;
            auto minites = zone - hours * 100;

            time -= hours * 60 * 60;
            time -= minites * 60;
        }

        return time;
    }
    else
    {
        tm tm = { 0 };
        string::size_type pos = 0;

        tm.tm_year = (int)GetUint(date, pos) - 1900;
        if ('-' != date[pos])
        {
            json.seekg(beg);
            throw Unexpected();
        }

        tm.tm_mon = (int)GetUint(date, ++pos) - 1;
        if ('-' != date[pos])
        {
            json.seekg(beg);
            throw Unexpected();
        }

        tm.tm_mday = (int)GetUint(date, ++pos);
        if ('T' != date[pos])
        {
            json.seekg(beg);
            throw Unexpected();
        }

        tm.tm_hour = (int)GetUint(date, ++pos);
        if (':' != date[pos])
        {
            json.seekg(beg);
            throw Unexpected();
        }

        tm.tm_min = (int)GetUint(date, ++pos);
        if (':' != date[pos])
        {
            json.seekg(beg);
            throw Unexpected();
        }

        tm.tm_sec = (int)GetUint(date, ++pos);

        if ('.' == date[pos])
        {
            GetUint(date, ++pos);
        }

        auto time = mktime(&tm);
        if (-1 == time)
        {
            json.seekg(beg);
            throw runtime_error("Failed to parse date");
        }

        if ('z' != tolower(date[pos]))
        {
            auto zone = GetInt(date, pos);
            auto hours = zone / 100;
            auto minites = zone - hours * 100;

            time -= hours * 60 * 60;
            time -= minites * 60;
        }

        return time;
    }
}

bool JParser::GetBool(istream& json)
{
    switch (GetChar(json))
    {
        case 't':
        {
            if ('r' == GetChar(json) &&
                'u' == GetChar(json) &&
                'e' == GetChar(json))
            {
                return true;
            }

            break;
        }

        case 'f':
        {
            if ('a' == GetChar(json) &&
                'l' == GetChar(json) &&
                's' == GetChar(json) &&
                'e' == GetChar(json))
            {
                return false;
            }

            break;
        }

        default: break;
    }

    json.seekg(-1, ios::cur);
    throw Unexpected();
}

bool JParser::IsFloat(const string& num)
{
    if (num.length() >= 2 && '0' == num[0] && ('x' == num[1] || 'X' == num[1]))
    {
        return false;
    }

    for (auto c : num)
    {
        if ('.' == c || 'e' == c || 'E' == c)
        {
            return true;
        }
    }

    return false;
}

void JParser::GetJson(const string& name, const JField& field, ostream& json)
{
    if (field.IsUndefined())
    {
        return;
    }

    if (!name.empty())
    {
        json << '\"' << name << "\":" ;
    }

    if (field.IsNull())
    {
        json << "null";
        return;
    }

    switch (field.Type())
    {
    case JType::INT:
        json << (JInt&)field;
        break;

    case JType::NUM:
        json << setprecision(numeric_limits<double>::digits10 + 1) << (JNum&)field;
        break;

    case JType::BOOL:
        json << ((JBool&)field ? "true" : "false");
        break;

    case JType::DATE:
        GetJson((JDate&)field, json);
        break;

    case JType::STR:
        GetJson((JStr&)field, json);
        break;

    case JType::OBJ:
        GetJson((JObject&)field, json);
        break;

    case JType::ARR:
        GetJson((JArray&)field, json);
        break;

    case JType::VAR:
        GetJson((JVar&)field, json);
        break;

    default:
        throw runtime_error(name + ": unknown json type");
    }
}

void JParser::GetJson(const JObject& obj, ostream& json)
{
    json << '{';

    bool first = true;
    obj.ForEach([&](const string& name, const JField& field)
    {
        if (field.IsUndefined())
        {
            return false;
        }

        if (!first)
        {
            json << ',';
        }
        first = false;

        GetJson(name, field, json);

        return false;
    });

    json << '}';
}

void JParser::GetJson(const JArray& arr, ostream& json)
{
    json << '[';

    bool first = true;
    arr.ForEach([&](const JField& field)
    {
        if (!first)
        {
            json << ',';
        }
        first = false;

        GetJson("", field, json);

        return false;
    });

    json << ']';
}

void JParser::GetJson(const JDate& date, ostream& json)
{
    tm tm;
    time_t dt = date;
#if defined(WIN32) || defined(_WIN32)
    localtime_s(&tm, &dt);
#else
    tm = *localtime(&dt);
#endif
    auto flags = json.flags();
    json << '\"' << to_string(tm.tm_year + 1900) << '-'
        << setfill('0') << setw(2) << tm.tm_mon + 1 << '-'
        << setfill('0') << setw(2) << tm.tm_mday << 'T'
        << setfill('0') << setw(2) << tm.tm_hour << ':'
        << setfill('0') << setw(2) << tm.tm_min  << ':'
        << setfill('0') << setw(2) << tm.tm_sec  << "Z\"";
    json.flags(flags);
}

void JParser::GetJson(const JStr& str, ostream& json)
{
    json << '\"';

    for (auto c : str.Value)
    {
        switch (c)
        {
            case '\\':
            {
                json << "\\\\";
                break;
            }

            case '"':
            {
                json << "\\\"";
                break;
            }

            default:
            {
                json << c;
                break;
            }
        }
    }

    json << '\"';
}

void JParser::GetJson(const JVar& var, ostream& json)
{
    switch (var.Subtype())
    {
        case JType::BOOL:
        {
            json << (var.Bool ? "true" : "false");
            break;
        }

        case JType::INT:
        {
            json << var.Int;
            break;
        }

        case JType::NUM:
        {
            json << setprecision(numeric_limits<double>::digits10 + 1) << var.Num;
            break;
        }

        case JType::STR:
        {
            json << '\"';

            for (auto c : var.Str)
            {
                switch (c)
                {
                    case '\\':
                    {
                        json << "\\\\";
                        break;
                    }

                    case '"':
                    {
                        json << "\\\"";
                        break;
                    }

                    default:
                    {
                        json << c;
                        break;
                    }
                }
            }

            json << '\"';
            break;
        }

        case JType::OBJ:
        {
            json << '{';

            bool first = true;
            var.ForEachField([&](const string& name, const JVar& var)
            {
                if (var.IsUndefined())
                {
                    return false;
                }

                if (!first)
                {
                    json << ',';
                }
                first = false;

                GetJson(name, var, json);

                return false;
            });

            json << '}';
            break;
        }

        case JType::ARR:
        {
            json << '[';

            bool first = true;
            var.ForEachItem([&](const JVar& var)
            {
                if (!first)
                {
                    json << ',';
                }
                first = false;

                GetJson("", var, json);

                return false;
            });

            json << ']';
            break;
        }

        default: break;
    }
}