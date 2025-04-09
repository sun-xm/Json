#include "JObject.h"

struct JInner : public JObject
{
    JInt num;
    JNum flt;
    JVar var;

    JOBJECT(JInner);
};
BEG_JFIELDS(JInner)
    JFIELD_KEY(num, "int"),
    JFIELD(flt),
    JFIELD(var)
END_JFIELDS

struct JOuter : public JObject
{
    JStr  str;
    JDate date;
    JArr<JStr> arr;

    JInner inner;

    JOBJECT(JOuter);
};
BEG_JFIELDS(JOuter)
    JFIELD(str),
    JFIELD(date),
    JFIELD(arr),
    JFIELD(inner)
END_JFIELDS

int main()
{
    std::string e;
    size_t w;
    JOuter outer;

    if (!outer.Deserialize("", e, w) || !outer.IsUndefined() || outer.IsNull() || outer.HasValue())
    {
        return -1;
    }

    if (!outer.Deserialize("{}", e, w) || !outer.IsUndefined() || outer.IsNull() || outer.HasValue())
    {
        return -1;
    }

    if (!outer.Deserialize("null", e, w) || outer.IsUndefined() || !outer.IsNull() || outer.HasValue())
    {
        return -1;
    }

    if (!outer.Deserialize("{\"str\":\"str\", \"date\":\"1999-04-23T18:25:43.511Z\", \"arr\":[\"1\", \"2\"], \"inner\":{\"int\":123, \"flt\":1.23, \"var\":true}}", e, w))
    {
        return -1;
    }

    if ("str" != outer.str.Value)
    {
        return -1;
    }

    if (924863143 != outer.date.Value)
    {
        return -1;
    }

    if ("1" != outer.arr[0].Value || "2" != outer.arr[1].Value)
    {
        return -1;
    }

    if (123 != outer.inner.num || 1.23 != outer.inner.flt || !outer.inner.var.Bool)
    {
        return -1;
    }

    outer.Clear();
    if (!outer.IsUndefined())
    {
        return -1;
    }

    if (!outer.str.IsUndefined() || outer.str().length() || !outer.arr.IsUndefined() || outer.arr().size() || !outer.date.IsUndefined() || !outer.inner.IsUndefined())
    {
        return -1;
    }

    auto& inner = outer.inner;
    if (!inner.num.IsUndefined() || !inner.flt.IsUndefined() || !inner.var.IsUndefined())
    {
        return -1;
    }

    outer.Deserialize(R"(
        {
            "inner.int": 1
        }
    )");
    if (!outer.HasValue() || !outer.inner.HasValue() || 1 != outer.inner.num)
    {
        return -1;
    }

    outer.Clear();
    if (!outer.IsUndefined())
    {
        return -1;
    }

    if (!outer.str.IsUndefined() || !outer.date.IsUndefined() || !outer.arr.IsUndefined() || !inner.IsUndefined())
    {
        return -1;
    }

    outer.str = "123";
    if (outer.IsUndefined())
    {
        return -1;
    }

    outer = nullptr;
    if (outer.IsUndefined() || !outer.IsNull())
    {
        return -1;
    }

    return 0;
}