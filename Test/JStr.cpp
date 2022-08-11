#include "JObject.h"

int main()
{
    std::string e;
    size_t w;
    JStr jstr;

    jstr = JStr();
    if (!jstr.Deserialize("", e, w) || !jstr.IsUndefined() || jstr.IsNull() || jstr.HasValue())
    {
        return -1;
    }

    jstr = JStr();
    if (!jstr.Deserialize("null", e, w) || jstr.IsUndefined() || !jstr.IsNull() || jstr.HasValue())
    {
        return -1;
    }

    jstr = JStr();
    if (!jstr.Deserialize("\"123\"", e, w) || "123" != jstr.Value)
    {
        return -1;
    }

    jstr = JStr();
    if (!jstr.Deserialize("\"\t\\t \n\\n \\\" nrt\"", e, w) || "\t\t \n\n \" nrt" != jstr.Value)
    {
        return -1;
    }

    jstr = JStr();
    if (!jstr.Deserialize("\"街道\"", e, w) || "街道" != jstr.Value)
    {
        return -1;
    }

    jstr = JStr();
    if (!jstr.Deserialize("\"\\u4e2d\\u56fd\"", e, w) || "中国" != jstr.Value)
    {
        return -1;
    }

    jstr = JStr();
    if (!jstr.Deserialize("\"a\\\\b\\\"c\""))
    {
        return -1;
    }

    if ("a\\b\"c" != jstr.Value)
    {
        return -1;
    }

    if ("\"a\\\\b\\\"c\"" != jstr.Serialize())
    {
        return -1;
    }

    return 0;
}