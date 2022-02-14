#include "JObject.h"

int main()
{
    std::string e;
    JArr<JInt> jarr;

    jarr = JArr<JInt>();
    if (!jarr.Deserialize("") || !jarr.IsUndefined() || jarr.IsNull() || jarr.HasValue())
    {
        return -1;
    }

    jarr = JArr<JInt>();
    if (!jarr.Deserialize("null") || jarr.IsUndefined() || !jarr.IsNull() || jarr.HasValue())
    {
        return -1;
    }

    jarr = JArr<JInt>();
    if (!jarr.Deserialize("[]") || jarr.IsUndefined() || jarr.IsNull() || !jarr.HasValue() || 0 != jarr.Size())
    {
        return -1;
    }

    jarr = JArr<JInt>();
    if (!jarr.Deserialize("[0, 1, 2]") || 0 != jarr[0] || 1 != jarr[1] || 2 != jarr[2])
    {
        return -1;
    }

    JArr<JArr<JInt>> jarrarr;
    if (!jarrarr.Deserialize("[[0, 1], [2, 3]]") || 0 != jarrarr[0][0] || 1 != jarrarr[0][1] || 2 != jarrarr[1][0] || 3 != jarrarr[1][1])
    {
        return false;
    }

    return 0;
}