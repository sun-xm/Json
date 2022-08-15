#include "JObject.h"

struct JInner : public JObject
{
    JStr str;
    JOBJECT(JInner);
};
BEG_JFIELDS(JInner)
    JFIELD(str)
END_JFIELDS

struct JOuter : public JObject
{
    JNum num;
    JArr<JInt> ints;
    JInner inner;
    JOBJECT (JOuter);
};
BEG_JFIELDS(JOuter)
    JFIELD(num),
    JFIELD(ints),
    JFIELD(inner)
END_JFIELDS

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
    if (!jvar.Deserialize("-123") || JType::INT != jvar.Subtype() || !jvar.HasValue() || -123 != jvar.Int)
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("0x123") || JType::INT != jvar.Subtype() || !jvar.HasValue() || 0x123 != jvar.Int)
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("1.23") || JType::NUM != jvar.Subtype() || !jvar.HasValue() || 1.23 != jvar.Num)
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("\"123\"") || JType::STR != jvar.Subtype() || !jvar.HasValue() || "123" != jvar.Str)
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("{\"one\":1,\"two\":\"two\"}", e, w) || JType::OBJ != jvar.Subtype() || !jvar.HasValue() || !jvar["one"].HasValue() || !jvar["two"].HasValue())
    {
        return -1;
    }

    auto& one = jvar["one"];
    if (JType::INT != one.Subtype() || 1 != one.Int)
    {
        return -1;
    }

    auto& two = jvar["two"];
    if (JType::STR != two.Subtype() || "two" != two.Str)
    {
        return -1;
    }

    auto& three = jvar["three"];
    if (JType::VAR != three.Subtype() || !three.IsUndefined() || three.IsNull() || three.HasValue())
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("[123., \"123\", false, {\"var\":\"a\\\\b\\\"c\"}]", e, w) || JType::ARR != jvar.Subtype() || !jvar.HasValue() || 4 != jvar.Size())
    {
        return -1;
    }

    if (JType::NUM != jvar[0].Subtype() || 123.0 != jvar[0].Num)
    {
        return -1;
    }

    if (JType::STR != jvar[1].Subtype() || "123" != jvar[1].Str)
    {
        return -1;
    }

    if (JType::BOOL != jvar[2].Subtype() || jvar[2].Bool)
    {
        return -1;
    }

    if (JType::OBJ != jvar[3].Subtype())
    {
        return -1;
    }

    auto& var = jvar[3]["var"];
    if (JType::STR != var.Subtype() || "a\\b\"c" != var.Str)
    {
        return -1;
    }

    if ("[123,\"123\",false,{\"var\":\"a\\\\b\\\"c\"}]" != jvar.Serialize())
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("[1, 2, 3]"))
    {
        return -1;
    }

    JArr<JInt> jints;
    if (!jvar.ToArr(jints, e))
    {
        return -1;
    }

    if (!jints.HasValue() || 3 != jints().size() || 1 != jints[0] || 2 != jints[1] || 3 != jints[2])
    {
        return -1;
    }

    JArr<JNum> jnums;
    if (!jvar.ToArr(jnums, e))
    {
        return -1;
    }

    if (!jnums.HasValue() || 3 != jnums().size() || 1.0 != jnums[0] || 2.0 != jnums[1] || 3.0 != jnums[2])
    {
        return -1;
    }

    jvar = JVar();
    if (!jvar.Deserialize("{\"num\":1.0, \"ints\":[1, 2, null], \"inner\":{\"str\":\"123\"}}", e, w))
    {
        return -1;
    }

    JOuter jouter;
    if (!jvar.ToObj(jouter))
    {
        return -1;
    }

    if (!jouter.HasValue() || !jouter.num.HasValue() || !jouter.ints.HasValue() || !jouter.inner.HasValue())
    {
        return -1;
    }

    if (1.0 != jouter.num)
    {
        return -1;
    }

    auto& ints = jouter.ints;
    if (3 != ints().size() || 1 != ints[0] || 2 != ints[1] || !ints[2].IsNull())
    {
        return -1;
    }

    auto& inner = jouter.inner;
    if (!inner.str.HasValue() || "123" != inner.str())
    {
        return -1;
    }

    JUndVar juv;
    if (!juv.Deserialize("123") || juv.HasValue())
    {
        return -1;
    }

    if (!juv.Deserialize("{\"n\":123}") || juv.HasValue())
    {
        return -1;
    }

    if (!juv.Deserialize("[1, 2, 3]") || juv.HasValue())
    {
        return -1;
    }

    return 0;
}