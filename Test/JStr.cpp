#include "JObject.h"

int main()
{
    std::string e;
    JStr jstr;

    jstr = JStr();
    if (!jstr.Deserialize("") || !jstr.IsUndefined() || jstr.IsNull() || jstr.HasValue())
    {
        return -1;
    }

    jstr = JStr();
    if (!jstr.Deserialize("null") || jstr.IsUndefined() || !jstr.IsNull() || jstr.HasValue())
    {
        return -1;
    }

    jstr = JStr();
    if (!jstr.Deserialize("\"123\"") || "123" != jstr.Value)
    {
        return -1;
    }

    jstr = JStr();
    if (!jstr.Deserialize("\"\t\n\\\"\"") || "\t\n\"" != jstr.Value)
    {
        return -1;
    }

    jstr = JStr();
    if (!jstr.Deserialize("\"街道\"") || "街道" != jstr.Value)
    {
        return -1;
    }

    jstr = JStr();
    if (!jstr.Deserialize("\"\\u4e2d\\u56fd\"") || "中国" != jstr.Value)
    {
        return -1;
    }

    return 0;
}