#define BITCOIN_TOOLKIT_VERSION "v0.0.1"
#define MAX_PRIVKEY_WIF_SM "5Km2kuu7vtFDPpxywn4u3NLpbr5jKpTB3jsuDU2KYEqetqj84qw"
#define MAX_PRIVKEY_WIF_CM "L5oLkpV3aqBjhki6LmvChTCV6odsp4SXM6FfU2Gppt5kFLaHLuZ9"
#define MAX_PRIVKEY_WIF_ST "93XfLeifX7KMMtUGa7xouxtnFWSSUyzNPgjrJ6Npsyahfqjy7oJ"
#define MAX_PRIVKEY_WIF_CT "cWALDjUu1tszsCBMjBjL4mhYj2wHUWYDR8Q8aSjLKzjkW5eBtpzu"
#define MAX_PRIVKEY_HEX "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364140"
#define MAX_PRIVKEY_B6  "1021410542201502023034020312354303525141003020114142003134301540433233134222423333133255354344331040"

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