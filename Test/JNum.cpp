#include "JObject.h"

int main()
{
    JNum jnum;

    if (!jnum.Deserialize("") || !jnum.IsUndefined() || jnum.IsNull() || jnum.HasValue())
    {
        return -1;
    }

    if (!jnum.Deserialize("null") || jnum.IsUndefined() || !jnum.IsNull() || jnum.HasValue())
    {
        return -1;
    }

    if (!jnum.Deserialize("1.23") || 1.23 != jnum)
    {
        return -1;
    }

    if (!jnum.Deserialize("-1.23") || -1.23 != jnum)
    {
        return -1;
    }

    if (!jnum.Deserialize("+1.23") || 1.23 != jnum)
    {
        return -1;
    }

    if (!jnum.Deserialize("-123e4") || -1230000 != jnum)
    {
        return -1;
    }

    if (!jnum.Deserialize("1.23E-4") || 0.000123 != jnum)
    {
        return -1;
    }

    return 0;
}