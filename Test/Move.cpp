#include <JObject.h>

using namespace std;

int main()
{
    JStr stra;
    JStr strb("b");
    stra = move(strb);
    if (!stra.HasValue() || "b" != stra() || strb.IsNull() || !strb.IsUndefined())
    {
        return -1;
    }

    JArr<JInt> arra;
    JArr<JInt> arrb = { 1, 2, 3 };
    arra = move(arrb);
    if (!arra.HasValue() || 1 != arra[0] || arrb.IsNull() || !arrb.IsUndefined())
    {
        return -1;
    }

    JVar vara;
    JVar varb;

    varb = "hello";
    vara = move(varb);
    if (!vara.HasValue() || JType::STR != vara.Subtype() || "hello" != vara.Str() || arrb.IsNull() || !arrb.IsUndefined())
    {
        return -1;
    }

    varb = arra;
    vara = move(varb);
    if (!vara.HasValue() || JType::ARR != vara.Subtype() || 1 != vara[0].Int() || JType::VAR != varb.Subtype() || varb.IsNull() || !varb.IsUndefined())
    {
        return -1;
    }

    varb.Push(1);
    vara = move(varb);
    if (!vara.HasValue() || JType::INT != vara[0].Subtype() || 1 != vara[0].Int() || JType::VAR != varb.Subtype() || varb.IsNull() || !varb.IsUndefined())
    {
        return -1;
    }

    varb["x"] = 1;
    vara = move(varb);
    if (!vara.HasValue() || !vara.HasField("x") || JType::INT != vara["x"].Subtype() || 1 != vara["x"].Int() || JType::VAR != varb.Subtype() || varb.IsNull() || !varb.IsUndefined())
    {
        return -1;
    }

    return 0;
}