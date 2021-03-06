#ifndef KERNEL_LIB_INT_H
#define KERNEL_LIB_INT_H

#define MIN_INT16 (-32768)
#define MAX_INT16 32767

#define MIN_INT32 (-2147483648)
#define MAX_INT32 2147483647

#define MIN_UINT16 0
#define MAX_UINT16 65535

#define MIN_UINT32 0
#define MAX_UINT32 4294967295

typedef signed char             int8_t;
typedef signed short            int16_t;
typedef signed int              int32_t;
typedef signed long long        int64_t;

typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long      uint64_t;

#endif
