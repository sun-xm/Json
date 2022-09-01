#include "JObject.h"

int main()
{
    JInt jint;

    if (!jint.Deserialize("") || !jint.IsUndefined() || jint.IsNull() || jint.HasValue())
    {
        return -1;
    }

    if (!jint.Deserialize("null") || jint.IsUndefined() || !jint.IsNull() || jint.HasValue())
    {
        return -1;
    }

    if (!jint.Deserialize("123") || 123 != jint)
    {
        return -1;
    }

    if (!jint.Deserialize("-123") || -123 != jint)
    {
        return -1;
    }

    if (!jint.Deserialize("0x1aF") || 0x1AF != jint)
    {
        return -1;
    }

    if (jint.Deserialize("+0X1AF") || jint.Deserialize("1AF") || jint.Deserialize("x1AF") || jint.Deserialize("1.23"))
    {
        return -1;
    }

    return 0;
}