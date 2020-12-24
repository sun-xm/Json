#include "JParser.h"
#include "JObject.h"
#include <codecvt>
#include <ctime>
#include <iomanip>
#include <limits>
#include <locale>
#include <sstream>

#define GETCAPTURE(capture, index, type)   (*(type*)(((void**)capture)[index]))
#define EXPECTMORE  "Expecting more charactors"
#define UNEXPECTED  "Unexpected charactor"

using namespace std;

const string Numbers("+-.0123456789");

wstring_convert<codecvt_utf8<wchar_t>> utf8;

string JParser::Serialize(const JField& field)
{
    ostringstream oss;
    GetJson("", field, oss);
    return oss.str();
}

void JParser::Deserialize(const string& json, JField& field)
{
    string::size_type off = 0;
    GetVal(json, off, "", &field);

    if (string::npos != (off = json.find_first_not_of(" \r\n", off)))
    {
        throw JParserError(json, off, "Extra following charactors");
    }
}

void JParser::GetVal(const string& json, string::size_type& off, const string& name, JField* field)
{
    off = FindFirstNotSpace(json, off);
    switch (json[off])
    {
    case '{':
        if (field && JType::OBJ != field->Type())
        {
            throw TypeMismatch(json, off, name, field->Type(), JType::OBJ);
        }
        GetObj(json, off, (JObject*)field);
        break;

    case '[':
        if (field && JType::ARR != field->Type())
        {
            throw TypeMismatch(json, off, name, field->Type(), JType::ARR);
        }
        GetArr(json, off, name, (JArr*)field);
        break;

    case '\"':
        if (field)
        {
            if (JType::STR == field->Type())
            {
                *(JString*)field = GetStr(json, off).c_str();
            }
            else if (JType::DATE == field->Type())
            {
                *(JDate*)field = GetDate(json, off);
            }
            else
            {
                throw TypeMismatch(json, off, name, field->Type(), JType::STR);
            }
        }
        else
        {
            GetStr(json, off);
        }
        break;

    default:
        if ('t' == json[off] || 'f' == json[off])
        {
            if (field)
            {
                if (JType::BOOL != field->Type())
                {
                    throw TypeMismatch(json, off, name, field->Type(), JType::BOOL);
                }
                *(JBool*)field = GetBool(json, off);
            }
            else
            {
                GetBool(json, off);
            }
        }
        else if ('n' == json[off])
        {
            if ("null" == json.substr(off, 4))
            {
                if (field)
                {
                    *field = nullptr;
                }

                off += 4;
            }
            else
            {
                throw JParserError(json, off, UNEXPECTED);
            }
        }
        else if (string::npos != Numbers.find_first_of(json[off]))
        {
            if (field)
            {
                switch (field->Type())
                {
                case JType::INT:
                    *(JInt*)field = GetInt(json, off);
                    break;

                case JType::UINT:
                    *(JUint*)field = GetUint(json, off);
                    break;

                case JType::FLT:
                    *(JFloat*)field = GetFlt(json, off);
                    break;

                default:
                    throw TypeMismatch(json, off, name, field->Type(), JType::INT);
                }
            }
            else
            {
                GetFlt(json, off);
            }
        }
        else
        {
            throw JParserError(json, off, UNEXPECTED);
        }
        break;
    }
}

void JParser::GetArr(const string& json, string::size_type& off, const string& name, JArr* arr)
{
    arr->Define();

    off = FindFirstNotSpace(json, ++off);
    if (']' == json[off])
    {
        off++;
        return;
    }

    do
    {
        off = FindFirstNotSpace(json, off);

        if (arr)
        {
            GetVal(json, off, name, arr->GetNew());
        }
        else
        {
            GetVal(json, off, name, nullptr);
        }
    } while([&]
    {
        off = FindFirstNotSpace(json, off);

        switch (json[off++])
        {
            case ',': return true;
            case ']': return false;
            default: throw JParserError(json, off - 1, UNEXPECTED);
        }
    }());
}

void JParser::GetObj(const string& json, string::size_type& off, JObject* obj)
{
    off = FindFirstNotSpace(json, ++off);
    if ('}' == json[off])
    {
        off++;
        return;
    }

    do
    {
        off = FindFirstNotSpace(json, off);

        if ('\"' != json[off])
        {
            throw JParserError(json, off, UNEXPECTED);
        }

        string name = GetName(json, off);
        auto field = obj ? obj->GetField(name.c_str()) : nullptr;

        while ('.' == json[off])
        {
            if ('\"' == json[off - 1])
            {
                throw JParserError(json, off - 1, UNEXPECTED);
            }

            if (field && JType::OBJ != field->Type())
            {
                throw TypeMismatch(json, off, name, field->Type(), JType::OBJ);
            }

            name = GetName(json, off);
            field = field ? ((JObject*)field)->GetField(name.c_str()) : nullptr;
        }

        off = FindFirstNotSpace(json, off);
        if (':' != json[off])
        {
            throw JParserError(json, off, UNEXPECTED);
        }

        GetVal(json, ++off, name, field);

    } while ([&]
    {
        off = FindFirstNotSpace(json, off);

        switch (json[off++])
        {
            case ',': return true;
            case '}': return false;
            default: throw JParserError(json, off - 1, UNEXPECTED);
        }
    }());
}

double JParser::GetFlt(const string& json, string::size_type& off)
{
    char* end;
    auto value = strtod(json.c_str() + off, &end);

    if (end == json.c_str() + off)
    {
        throw JParserError(json, off, "Failed to parse float");
    }

    off = end - json.c_str();

    return value;
}

string JParser::GetStr(const string& json, string::size_type& off)
{
    if (json.length() == off)
    {
        throw JParserError(json, off, EXPECTMORE);
    }

    off = FindFirstNotSpace(json, off);
    if ('\"' != json[off])
    {
        throw JParserError(json, off, UNEXPECTED);
    }

    ostringstream oss;
    while ([&]{ if (json.length() == ++off) throw JParserError(json, off, EXPECTMORE); return '\"' != json[off]; }())
    {
        switch (json[off])
        {
            case '\\':
            {
                if (json.length() == ++off)
                {
                    throw JParserError(json, off, EXPECTMORE);
                }

                switch (json[off])
                {
                    case '\\':
                    case '\"':
                        oss << json[off];
                        break;

                    case 'u':
                    {
                        if (json.length() <= off + 4)
                        {
                            throw JParserError(json, off + 4, EXPECTMORE);
                        }

                        oss << utf8.to_bytes((wchar_t)stoi(json.substr(off + 1, off + 5), nullptr, 16));
                        off += 4;
                        break;
                    }

                    default:
                        throw JParserError(json, off, UNEXPECTED);
                }
                break;
            }

            default:
            {
                oss << json[off];
                break;
            }
        }
    }

    off++;

    return oss.str();
}

int64_t JParser::GetInt(const string& json, string::size_type& off)
{
    char* end;
    int64_t value;
    
    if (json.length() > off + 2 && '0' == json[off] && ('x' == json[off + 1] || 'X' == json[off + 1]))
    {
        value = std::strtoll(json.c_str() + off, &end, 16);
    }
    else
    {
        value = std::strtoll(json.c_str() + off, &end, 10);
    }

    if (end == json.c_str() + off)
    {
        throw JParserError(json, off, "Failed to parse int");
    }

    off = end - json.c_str();

    return value;
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
        throw JParserError(json, off, "Failed to parse uint");
    }

    off = end - json.c_str();

    return value;
}

string JParser::GetName(const string& json, string::size_type& off)
{
    off = FindFirstNotSpace(json, off);
    if ('\"' != json[off] && '.' != json[off])
    {
        throw JParserError(json, off, UNEXPECTED);
    }

    auto beg = off + 1;

    do
    {
        if (++off == json.length())
        {
            throw JParserError(json, off, EXPECTMORE);
        }

        off = json.find_first_of(".\"", off);

        if (string::npos == off)
        {
            throw JParserError(json, beg - 1, "Not enclosed");
        }
    } while ('\\' == json[off - 1]);

    if ('.' == json[off])
    {
        return json.substr(beg, off - beg);
    }
    else
    {
        return json.substr(beg, off++ - beg);
    }
}

time_t JParser::GetDate(const string& json, string::size_type& off)
{
    auto beg = off;

    auto date = GetStr(json, off);
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
            throw JParserError(json, beg + pos, UNEXPECTED);
        }

        tm.tm_mon = (int)GetUint(date, ++pos) - 1;
        if ('-' != date[pos])
        {
            throw JParserError(json, beg + pos, UNEXPECTED);
        }

        tm.tm_mday = (int)GetUint(date, ++pos);
        if ('T' != date[pos])
        {
            throw JParserError(json, beg + pos, UNEXPECTED);
        }

        tm.tm_hour = (int)GetUint(date, ++pos);
        if (':' != date[pos])
        {
            throw JParserError(json, beg + pos, UNEXPECTED);
        }

        tm.tm_min = (int)GetUint(date, ++pos);
        if (':' != date[pos])
        {
            throw JParserError(json, beg + pos, UNEXPECTED);
        }

        tm.tm_sec = (int)GetUint(date, ++pos);

        if ('.' == date[pos])
        {
            auto milliseconds = GetUint(date, ++pos);
        }

        auto time = mktime(&tm);
        if (-1 == time)
        {
            throw JParserError(json, beg, "Failed to parse data");
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

bool JParser::GetBool(const string& json, string::size_type& off)
{
    switch (tolower(json[off]))
    {
        case 't':
            if ("true" == json.substr(off, 4))
            {
                off += 4;
                return true;
            }
            break;

        case 'f':
            if ("false" == json.substr(off, 5))
            {
                off += 5;
                return false;
            }

        default:
            break;
    }
    throw JParserError(json, off, "Failed to parse bool");
}

void JParser::GetJson(const string& name, const JField& field, ostringstream& oss)
{
    if (field.IsUndefined())
    {
        return;
    }

    if (!name.empty())
    {
        oss << '\"' << name << "\":" ;
    }

    if (field.IsNull())
    {
        oss << "null";
        return;
    }

    switch (field.Type())
    {
    case JType::INT:
        oss << (JInt&)field;
        break;

    case JType::UINT:
        oss << (JUint&)field;
        break;

    case JType::FLT:
        oss << setprecision(numeric_limits<double>::digits10 + 1) << (JFloat&)field;
        break;

    case JType::BOOL:
        oss << ((JBool&)field ? "true" : "false");
        break;

    case JType::DATE:
        GetJson((JDate&)field, oss);
        break;

    case JType::STR:
        GetJson((JString&)field, oss);
        break;

    case JType::OBJ:
        GetJson((JObject&)field, oss);
        break;

    case JType::ARR:
        GetJson((JArr&)field, oss);
        break;

    default:
        throw runtime_error(name + ": unknown json type");
    }
}

void JParser::GetJson(const JArr& arr, ostringstream& oss)
{
    oss << '[';

    bool first = true;
    arr.ForEach([&](const JField& field)
    {
        if (!first)
        {
            oss << ',';
        }
        first = false;

        GetJson("", field, oss);
    });

    oss << ']';
}

void JParser::GetJson(const JDate& date, ostringstream& oss)
{
    tm tm;
    time_t dt = date;
#ifdef WIN32
    localtime_s(&tm, &dt);
#else
    tm = *localtime(&dt);
#endif
    auto flags = oss.flags();
    oss << '\"' << to_string(tm.tm_year + 1900) << '-'
        << setfill('0') << setw(2) << tm.tm_mon + 1 << '-'
        << setfill('0') << setw(2) << tm.tm_mday << 'T'
        << setfill('0') << setw(2) << tm.tm_hour << ':'
        << setfill('0') << setw(2) << tm.tm_min  << ':'
        << setfill('0') << setw(2) << tm.tm_sec  << "Z\"";
    oss.flags(flags);
}

void JParser::GetJson(const JObject& obj, ostringstream& oss)
{
    oss << '{';

    bool first = true;
    obj.ForEach([&](const string& name, const JField& field)
    {
        if (field.IsUndefined())
        {
            return;
        }

        if (!first)
        {
            oss << ',';
        }
        first = false;

        GetJson(name, field, oss);
    });

    oss << '}';
}

void JParser::GetJson(const JString& str, ostringstream& oss)
{
    oss << '\"' << (const string&)str << '\"';
}

string::size_type JParser::FindFirstNotSpace(const string& json, string::size_type off)
{
    auto pos = json.find_first_not_of(" \r\n", off);
    if (std::string::npos == pos)
    {
        throw JParserError(json, off, EXPECTMORE);
    }

    return pos;
}

JParserError JParser::TypeMismatch(const std::string& json, size_t off, const std::string& name, JType expect, JType actual)
{
    return JParserError(json, off, "Type mismatch: " + name + ". Expecting: " + to_string(expect) + ". Actual: " + to_string(actual));
}