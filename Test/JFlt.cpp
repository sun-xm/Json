#include "JObject.h"

int main()
{
    std::string e;
    size_t w;
    JFlt jflt;

    jflt = JFlt();
    if (!jflt.Deserialize("", e, w) || !jflt.IsUndefined() || jflt.IsNull() || jflt.HasValue())
    {
        return -1;
    }

    jflt = JFlt();
    if (!jflt.Deserialize("null", e, w) || jflt.IsUndefined() || !jflt.IsNull() || jflt.HasValue())
    {
        return -1;
    }

    jflt = JFlt();
    if (!jflt.Deserialize("1.23", e, w) || 1.23 != jflt)
    {
        return -1;
    }

    jflt = JFlt();
    if (!jflt.Deserialize("-1.23", e, w) || -1.23 != jflt)
    {
        return -1;
    }

    return 0;
}