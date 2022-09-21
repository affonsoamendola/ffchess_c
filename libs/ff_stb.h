#ifndef FF_STB
#define FF_STB

#define true 1
#define false 0

#ifndef __cplusplus
typedef int bool;
#endif

typedef unsigned char  uint8 ;
typedef   signed char   int8 ;
typedef unsigned short uint16;
typedef   signed short  int16;
typedef unsigned int   uint32;
typedef   signed int    int32;

typedef unsigned char  uchar ;
typedef unsigned short ushort;
typedef unsigned int   uint  ;
typedef unsigned long  ulong ;

// Produce compile errors if the sizes aren't correct, stole straight from STB.
typedef char ff__testsize16[sizeof(int16)==2];
typedef char ff__testsize32[sizeof(int32)==4];
#endif