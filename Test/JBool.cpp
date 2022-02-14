#include "JObject.h"

int main()
{
    std::string e;
    JBool jbool;

    jbool = JBool();
    if (!jbool.Deserialize("") || !jbool.IsUndefined() || jbool.IsNull() || jbool.HasValue())
    {
        return -1;
    }

    jbool = JBool();
    if (!jbool.Deserialize("null") || jbool.IsUndefined() || !jbool.IsNull() || jbool.HasValue())
    {
        return -1;
    }

    jbool = JBool();
    if (!jbool.Deserialize("true") || !jbool)
    {
        return -1;
    }

    jbool = JBool();
    if (!jbool.Deserialize("false") || !!jbool)
    {
        return -1;
    }
    
    return 0;
}