#include "BaseHelper.h"

BaseHelper::~BaseHelper()
{
    //if(_msg)
    //    free(_msg);
}

portal_data_t BaseHelper::getPortalDataFor(const char* s)
{
    portal_data_t d;
    int sz = strlen(s);
    memcpy(d.from,s,sz);
    d.from[sz] = '\0';
    d.size = raw(&(d.data));
    return d;
}
void* BaseHelper::msg()
{
    return _msg;
}
int BaseHelper::raw(uint8_t**data)
{
    if(_msg)
        return this->rosMsgToRaw(data);
    return -1;
}

