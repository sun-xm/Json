#include "JObject.h"

int main()
{
    JStr jstr;

    if (!jstr.Deserialize("") || !jstr.IsUndefined() || jstr.IsNull() || jstr.HasValue())
    {
        return -1;
    }

    if (!jstr.Deserialize("null") || jstr.IsUndefined() || !jstr.IsNull() || jstr.HasValue())
    {
        return -1;
    }

    if (!jstr.Deserialize("\"123\"") || "123" != jstr.Value)
    {
        return -1;
    }

    if (!jstr.Deserialize("\"\t\\t \n\\n \\\" nrt\"") || "\t\t \n\n \" nrt" != jstr.Value)
    {
        return -1;
    }

    if (!jstr.Deserialize("\"街道\"") || "街道" != jstr.Value)
    {
        return -1;
    }

    if (!jstr.Deserialize("\"\\u4e2d\\u56fd\"") || "中国" != jstr.Value)
    {
        return -1;
    }

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