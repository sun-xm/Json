#include "JObject.h"

int main()
{
    std::string e;
    size_t w;
    JNum jnum;

    jnum = JNum();
    if (!jnum.Deserialize("", e, w) || !jnum.IsUndefined() || jnum.IsNull() || jnum.HasValue())
    {
        return -1;
    }

    jnum = JNum();
    if (!jnum.Deserialize("null", e, w) || jnum.IsUndefined() || !jnum.IsNull() || jnum.HasValue())
    {
        return -1;
    }

    jnum = JNum();
    if (!jnum.Deserialize("1.23", e, w) || 1.23 != jnum)
    {
        return -1;
    }

    jnum = JNum();
    if (!jnum.Deserialize("-1.23", e, w) || -1.23 != jnum)
    {
        return -1;
    }

    return 0;
}