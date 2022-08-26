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
    JVar jvar;

    if (!jvar.Deserialize("") || !jvar.IsUndefined() || jvar.IsNull() || jvar.HasValue())
    {
        return -1;
    }

    if (!jvar.Deserialize("0X1aB") || JType::INT != jvar.Subtype() || !jvar.HasValue() || 0x1AB != jvar.Int)
    {
        return -1;
    }

    if (jvar.Deserialize("1AB"))
    {
        return -1;
    }

    if (!jvar.Deserialize("123e-2") || JType::NUM != jvar.Subtype() || !jvar.HasValue() || 1.23 != jvar.Num)
    {
        return -1;
    }

    if (!jvar.Deserialize(R"(
        [
            123,
            123.,
            "123",
            false,
            {
                "var": "a\\b\"c"
            },
            null
        ]
    )"))
    {
        return -1;
    }

    if (JType::ARR != jvar.Subtype() || !jvar.HasValue() || 6 != jvar.Size())
    {
        return -1;
    }

    if (JType::INT != jvar[0].Subtype() || 123 != jvar[0].Int)
    {
        return -1;
    }

    if (JType::NUM != jvar[1].Subtype() || 123.0 != jvar[1].Num)
    {
        return -1;
    }

    if (JType::STR != jvar[2].Subtype() || "123" != jvar[2].Str)
    {
        return -1;
    }

    if (JType::BOOL != jvar[3].Subtype() || jvar[3].Bool)
    {
        return -1;
    }

    if (JType::OBJ != jvar[4].Subtype())
    {
        return -1;
    }

    auto& var = jvar[4]["var"];
    if (JType::STR != var.Subtype() || "a\\b\"c" != var.Str)
    {
        return -1;
    }

    if (!jvar[5].IsNull())
    {
        return -1;
    }

    if ("[123,123,\"123\",false,{\"var\":\"a\\\\b\\\"c\"},null]" != jvar.Serialize())
    {
        return -1;
    }

    if (!jvar.Deserialize("[1, 2, 3]"))
    {
        return -1;
    }

    JArr<JInt> jints;
    if (!jvar.ToArr(jints))
    {
        return -1;
    }

    if (!jints.HasValue() || 3 != jints().size() || 1 != jints[0] || 2 != jints[1] || 3 != jints[2])
    {
        return -1;
    }

    JArr<JNum> jnums;
    if (!jvar.ToArr(jnums))
    {
        return -1;
    }

    if (!jnums.HasValue() || 3 != jnums().size() || 1.0 != jnums[0] || 2.0 != jnums[1] || 3.0 != jnums[2])
    {
        return -1;
    }

    if (!jvar.Deserialize("{\"a.b\":1}") || !jvar.HasValue() || !jvar["a"].HasValue() || !jvar["a"]["b"].HasValue() || 1 != jvar["a"]["b"].Int)
    {
        return -1;
    }

    if (!jvar.Deserialize(R"(
        {
            "num": 1.0,
            "ints": [
                1,
                2,
                null
            ],
            "inner": {
                "str": "123"
            }
        }
    )"))
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

    JVar jv;
    jv = jouter;
    if (!jv["num"].HasValue() || !jv["ints"].HasValue() || !jv["inner"].HasValue())
    {
        return -1;
    }

    if (1.0 != jv["num"].Num)
    {
        return -1;
    }

    if (!jv["ints"][2].IsNull())
    {
        return -1;
    }

    if (!jv["inner"]["str"].HasValue() || "123" != jv["inner"]["str"].Str)
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