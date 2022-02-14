#include "JObject.h"

int main()
{
    std::string e;
    JInt jint;

    jint = JInt();
    if (!jint.Deserialize("", e) || !jint.IsUndefined() || jint.IsNull() || jint.HasValue())
    {
        return -1;
    }

    jint = JInt();
    if (!jint.Deserialize("null", e) || jint.IsUndefined() || !jint.IsNull() || jint.HasValue())
    {
        return -1;
    }

    jint = JInt();
    if (!jint.Deserialize("123", e) || 123 != jint)
    {
        return -1;
    }

    jint = JInt();
    if (!jint.Deserialize("-123", e) || -123 != jint)
    {
        return -1;
    }

    jint = JInt();
    if (!jint.Deserialize("0x1aF", e) || 0x1AF != jint)
    {
        return -1;
    }

    return 0;
}