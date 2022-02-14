#include "JObject.h"

int main()
{
    std::string e;
    JUint juint;

    juint = JUint();
    if (!juint.Deserialize("") || !juint.IsUndefined() || juint.IsNull() || juint.HasValue())
    {
        return -1;
    }

    juint = JUint();
    if (!juint.Deserialize("null") || juint.IsUndefined() || !juint.IsNull() || juint.HasValue())
    {
        return -1;
    }

    juint = JUint();
    if (!juint.Deserialize("123", e) || 123 != juint)
    {
        return -1;
    }

    juint = JUint();
    if (!juint.Deserialize("-123", e) || (unsigned long long)-123 != juint)
    {
        return -1;
    }

    juint = JUint();
    if (!juint.Deserialize("0x1aF", e) || 0x1AF != juint)
    {
        return -1;
    }

    return 0;
}