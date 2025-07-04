#include "JObject.h"

int main()
{
    std::string e;
    size_t w;
    JTime jtime;

    if (!jtime.Deserialize("", e, w) || !jtime.IsUndefined() || jtime.IsNull() || jtime.HasValue())
    {
        return -1;
    }

    if (!jtime.Deserialize("\"123\"") || !jtime.IsUndefined() || jtime.IsNull() || jtime.HasValue())
    {
        return -1;
    }

    if (!jtime.Deserialize("null", e, w) || jtime.IsUndefined() || !jtime.IsNull() || jtime.HasValue())
    {
        return -1;
    }

    if (!jtime.Deserialize("\"1999-04-23T18:25:43.511Z\"", e, w) || !jtime.HasValue() || (time_t)924863143 != jtime())
    {
        return -1;
    }

    if (!jtime.Deserialize("\"\\/Date(1759388889254)\\/\""))
    {
        return -1;
    }

    return 0;
}