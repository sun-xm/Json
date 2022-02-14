#include "JObject.h"

struct JInner : public JObject
{
    JInt num;
    JFlt flt;

    JOBJECT(JInner);
};
BEG_JFIELDS(JInner)
    JFIELD_KEY(num, "int"),
    JFIELD(flt)
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
    JOuter outer;

    outer = JOuter();
    if (!outer.Deserialize("", e) || !outer.IsUndefined() || outer.IsNull() || outer.HasValue())
    {
        return -1;
    }

    outer = JOuter();
    if (!outer.Deserialize("null", e) || outer.IsUndefined() || !outer.IsNull() || outer.HasValue())
    {
        return -1;
    }

    outer = JOuter();
    if (!outer.Deserialize("{}", e) || outer.IsUndefined() || outer.IsNull() || !outer.HasValue())
    {
        return -1;
    }

    outer = JOuter();
    if (!outer.Deserialize("{\"str\":\"str\", \"date\":\"1999-04-23T18:25:43.511Z\", \"arr\":[\"1\", \"2\"], \"inner\":{\"int\":123, \"flt\":1.23}}", e))
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

    if (123 != outer.inner.num || 1.23 != outer.inner.flt)
    {
        return -1;
    }

    return 0;
}