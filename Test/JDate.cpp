#include "JObject.h"

int main()
{
    std::string e;
    JDate jdate;

    jdate = JDate();
    if (!jdate.Deserialize("", e) || !jdate.IsUndefined() || jdate.IsNull() || jdate.HasValue())
    {
        return -1;
    }

    jdate = JDate();
    if (!jdate.Deserialize("null", e) || jdate.IsUndefined() || !jdate.IsNull() || jdate.HasValue())
    {
        return -1;
    }

    jdate = JDate();
    if (!jdate.Deserialize("\"1999-04-23T18:25:43.511Z\"", e) || 924863143 != jdate.Value)
    {
        return -1;
    }

    return 0;
}