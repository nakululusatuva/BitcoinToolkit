#define BITCOIN_TOOLKIT_VERSION "v0.0.1"

#ifndef STD_TYPE
#define STD_TYPE

typedef signed char int8_t;
typedef short       int16_t;
typedef int         int32_t;
typedef long int    int64_t;

typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
typedef unsigned long int uint64_t;

typedef uint8_t  BYTE;
typedef uint64_t size_t;

typedef enum{true, false} bool;
#endif