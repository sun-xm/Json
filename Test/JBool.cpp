#include "JObject.h"

int main()
{
    std::string e;
    size_t w;
    JBool jbool;

    if (!jbool.Deserialize("", e, w) || !jbool.IsUndefined() || jbool.IsNull() || jbool.HasValue())
    {
        return -1;
    }

    if (!jbool.Deserialize("null", e, w) || jbool.IsUndefined() || !jbool.IsNull() || jbool.HasValue())
    {
        return -1;
    }

    if (!jbool.Deserialize("true", e, w) || !jbool)
    {
        return -1;
    }

    if (!jbool.Deserialize("false", e, w) || !!jbool)
    {
        return -1;
    }

    return 0;
}