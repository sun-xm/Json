#include "JObject.h"

int main()
{
    std::string e;
    JFlt jflt;

    jflt = JFlt();
    if (!jflt.Deserialize("") || !jflt.IsUndefined() || jflt.IsNull() || jflt.HasValue())
    {
        return -1;
    }

    jflt = JFlt();
    if (!jflt.Deserialize("null") || jflt.IsUndefined() || !jflt.IsNull() || jflt.HasValue())
    {
        return -1;
    }

    jflt = JFlt();
    if (!jflt.Deserialize("1.23") || 1.23 != jflt)
    {
        return -1;
    }

    jflt = JFlt();
    if (!jflt.Deserialize("-1.23") || -1.23 != jflt)
    {
        return -1;
    }

    return 0;
}