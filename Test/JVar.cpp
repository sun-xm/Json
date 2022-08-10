#include "JObject.h"

int main()
{
    std::string e;
    size_t w;
    JVar jvar;

    if (!jvar.Deserialize("", e, w) || !jvar.IsUndefined() || jvar.IsNull() || jvar.HasValue())
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("null") || jvar.IsUndefined() || !jvar.IsNull())
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("true") || JType::BOOL != jvar.Subtype() || !jvar.HasValue() || !jvar.Bool)
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("123") || JType::FLT != jvar.Subtype() || !jvar.HasValue() || 123 != jvar.Flt)
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("\"123\"") || JType::STR != jvar.Subtype() || !jvar.HasValue() || "123" != jvar.Str)
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("{\"one\":1,\"two\":\"two\"}", e, w) || JType::OBJ != jvar.Subtype() || !jvar.HasValue() || !jvar.HasField("one") || !jvar.HasField("two"))
    {
        return -1;
    }

    auto one = jvar.GetField("one");
    if (JType::FLT != one->Subtype() || 1 != one->Flt)
    {
        return -1;
    }

    auto two = jvar.GetField("two");
    if (JType::STR != two->Subtype() || "two" != two->Str)
    {
        return -1;
    }

    if ("{\"one\":1,\"two\":\"two\"}" != jvar.Serialize())
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("[123, \"123\"]", e, w) || JType::ARR != jvar.Subtype() || !jvar.HasValue() || 2 != jvar.Size())
    {
        return -1;
    }

    if (JType::FLT != jvar[0].Subtype() || 123 != jvar[0].Flt)
    {
        return -1;
    }

    if (JType::STR != jvar[1].Subtype() || "123" != jvar[1].Str)
    {
        return -1;
    }

    if ("[123,\"123\"]" != jvar.Serialize())
    {
        return -1;
    }

    return 0;
}