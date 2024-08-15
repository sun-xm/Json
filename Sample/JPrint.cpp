#include "JPrint.h"
#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>

using namespace std;

void Indent(ostream& stream, int indent)
{
    for (auto i = 0; i < indent; i++)
    {
        stream << "  ";
    }
}

void JPrint(ostream& stream, const JField& field, const string& name, int indent)
{
    if (field.HasValue())
    {
        switch (field.Type())
        {
        case JType::OBJ:
            JPrint(stream, (const JObject&)field, name, indent);
            break;

        case JType::ARR:
            JPrint(stream, (const JArray&)field, name, indent);
            break;

        default:
            Indent(stream, indent);
            if (!name.empty())
            {
                stream << name << ": ";
            }

            stream << field.Serialize();
        }
    }
    else
    {
        Indent(stream, indent);

        if (!name.empty())
        {
            stream << name << ": ";
        }

        stream << (field.IsNull() ? "null" : "undefined");
    }
}

void JPrint(ostream& stream, const JArray& arr, const string& name, int indent)
{
    Indent(stream, indent);

    if (!name.empty())
    {
        stream << name << ": [" << endl;
    }
    else
    {
        stream << '[' << endl;
    }

    arr.ForEach([&](const JField& field)
    {
        JPrint(stream, field, "", indent + 1);
        stream << ',' << endl;
    });

    Indent(stream, indent);
    stream << ']';
}

void JPrint(ostream& stream, const JObject& object, const string& name, int indent)
{
    Indent(stream, indent);
    if (!name.empty())
    {
        stream << name << ": ";
    }

    if (object.IsUndefined())
    {
        stream << "undefined";
        return;
    }

    stream << '{' << endl;

    object.ForEach([&](const string& name, const JField& field)
    {
        JPrint(stream, field, name, indent + 1);
        stream << ',' << endl;
    });

    Indent(stream, indent);
    stream << '}';
}

string JError(istream& json, const string& error, size_t where)
{
    ostringstream oss;

    json.clear();
    json.seekg(0, ios::end);
    auto n = min((size_t)json.tellg(), where + 1);
    json.seekg(0, ios::beg);

    copy_n(istreambuf_iterator<char>(json), n, ostreambuf_iterator<char>(oss));
    oss << "\033[1;31m" << "<--(" << error << ")\033[0m";

    return oss.str();
}

ostream& operator<<(ostream& stream, const JDate& date)
{
    tm tm;
    time_t dt = date;
#ifdef WIN32
    localtime_s(&tm, &dt);
#else
    tm = *localtime(&dt);
#endif

    auto flags = stream.flags();
    stream << to_string(tm.tm_year + 1900) << '-'
           << setfill('0') << setw(2) << tm.tm_mon + 1 << '-'
           << setfill('0') << setw(2) << tm.tm_mday << 'T'
           << setfill('0') << setw(2) << tm.tm_hour << ':'
           << setfill('0') << setw(2) << tm.tm_min << ':'
           << setfill('0') << setw(2) << tm.tm_sec << 'Z';
    stream.flags(flags);
    return stream;
}

ostream& operator<<(ostream& stream, const JStr& str)
{
    return stream << str.Value;
}

ostream& operator<<(ostream& stream, const JField& field)
{
    JPrint(stream, field, "", 0);
    return stream;
}