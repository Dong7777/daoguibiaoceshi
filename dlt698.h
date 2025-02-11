#ifndef DLT698_H
#define DLT698_H

class DLT698
{
public:
    DLT698();
};
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

extern u8 globalAddress[6];

u16 cjdeal_698_get_request(u8 *SendBuf, u16 *DataLen, u8 *APDU, u16 APDUDataLen);

#endif // DLT698_H
