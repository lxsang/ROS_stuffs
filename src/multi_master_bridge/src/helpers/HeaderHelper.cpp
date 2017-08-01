
#include "HeaderHelper.h"


int HeaderHelper::hash()
{
    return _H("std_msgs/Header");
}
void HeaderHelper::rawToRosMsg(uint8_t* data)
{
    std_msgs::Header* ptr = new std_msgs::Header;
    //conversion code here
    ptr->seq = (int)*data;
    int flen;

    memcpy(&(ptr->seq),data,sizeof(int));
    memcpy(&(ptr->stamp.sec),data+sizeof(int),sizeof(ptr->stamp.sec));
    memcpy(&(ptr->stamp.nsec),data+2*sizeof(int),sizeof(ptr->stamp.nsec));
    memcpy(&flen,data+3*sizeof(int),sizeof(int));
    _rawsize = 4*sizeof(int) + flen;
    char* fid = (char*)malloc(flen+1);
    memcpy(fid,data+4*sizeof(int),flen);
    fid[flen] = '\0';
    ptr->frame_id = string(fid);
    if(fid) free(fid);
    _msg = (void*) ptr; 
}
int HeaderHelper::rosMsgToRaw(uint8_t** data)
{
    
    std_msgs::Header* ptr = (std_msgs::Header*)_msg;
    int len=0;
    int flen = ptr->frame_id.size();
    //conversion code here
    len =   4*sizeof(int) + flen;
    *data = (uint8_t*) malloc(len);
    //copy raw data to buffer
    memcpy(*data,&(ptr->seq),sizeof(int));
    memcpy(*data+sizeof(int),&(ptr->stamp.sec),sizeof(ptr->stamp.sec));
    memcpy(*data+2*sizeof(int),&(ptr->stamp.nsec),sizeof(ptr->stamp.nsec));
    memcpy(*data+3*sizeof(int),&flen,sizeof(int));
    memcpy(*data+4*sizeof(int),ptr->frame_id.c_str(),flen);
    return len;
}

