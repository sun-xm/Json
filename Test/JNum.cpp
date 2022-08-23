#include "JObject.h"

int main()
{
    std::string e;
    size_t w;
    JNum jnum;

    if (!jnum.Deserialize("", e, w) || !jnum.IsUndefined() || jnum.IsNull() || jnum.HasValue())
    {
        return -1;
    }

    if (!jnum.Deserialize("null", e, w) || jnum.IsUndefined() || !jnum.IsNull() || jnum.HasValue())
    {
        return -1;
    }

    if (!jnum.Deserialize("1.23", e, w) || 1.23 != jnum)
    {
        return -1;
    }

    if (!jnum.Deserialize("-1.23", e, w) || -1.23 != jnum)
    {
        return -1;
    }

    return 0;
}