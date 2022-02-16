#include "JObject.h"

int main()
{
    std::string e;
    size_t w;
    JDate jdate;

    jdate = JDate();
    if (!jdate.Deserialize("", e, w) || !jdate.IsUndefined() || jdate.IsNull() || jdate.HasValue())
    {
        return -1;
    }

    jdate = JDate();
    if (!jdate.Deserialize("null", e, w) || jdate.IsUndefined() || !jdate.IsNull() || jdate.HasValue())
    {
        return -1;
    }

    jdate = JDate();
    if (!jdate.Deserialize("\"1999-04-23T18:25:43.511Z\"", e, w) || 924863143 != jdate.Value)
    {
        return -1;
    }

    return 0;
}