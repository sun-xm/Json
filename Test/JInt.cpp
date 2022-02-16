#include "JObject.h"

int main()
{
    std::string e;
    size_t w;
    JInt jint;

    jint = JInt();
    if (!jint.Deserialize("", e, w) || !jint.IsUndefined() || jint.IsNull() || jint.HasValue())
    {
        return -1;
    }

    jint = JInt();
    if (!jint.Deserialize("null", e, w) || jint.IsUndefined() || !jint.IsNull() || jint.HasValue())
    {
        return -1;
    }

    jint = JInt();
    if (!jint.Deserialize("123", e, w) || 123 != jint)
    {
        return -1;
    }

    jint = JInt();
    if (!jint.Deserialize("-123", e, w) || -123 != jint)
    {
        return -1;
    }

    jint = JInt();
    if (!jint.Deserialize("0x1aF", e, w) || 0x1AF != jint)
    {
        return -1;
    }

    return 0;
}