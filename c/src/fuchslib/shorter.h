#ifndef __SHORTER_H__
#define __SHORTER_H__

#include <cstdint>

#ifdef _WIN32 
#define WINDOWS
#define FUCHS_SUPPORTED
#endif 

#if defined(unix) || defined(__unix__)
#define UNIX
#endif

#ifdef __OpenBSD__
#define OPENBSD
#endif

#if defined(linux) || defined(__linux__)
#define LINUX
#define FUCHS_SUPPORTED
#endif

#if defined(macintosh) || defined(Macintosh) || defined(__MACOSX__) || defined(__APPLE__)
#define MACOS
#endif

#if defined(__x86_64__) || defined(__x86_64) || defined(_WIN64)
#define x64
#endif

typedef int8_t          int8;
typedef int16_t         int16;
typedef int32_t         int32;
typedef uint8_t         uint8;
typedef uint16_t        uint16;
typedef uint32_t        uint32;

typedef int_fast8_t     fastInt8; 
typedef int_fast16_t    fastInt16; 
typedef int_fast32_t    fastInt32; 

typedef uint_fast8_t     ufastInt8; 
typedef uint_fast16_t    ufastInt16; 
typedef uint_fast32_t    ufastInt32; 

#ifdef x64
typedef int64_t         int64;
typedef uint64_t        uint64;
typedef int_fast64_t    fastInt64;
typedef uint_fast64_t    ufastInt64;
#endif

typedef intmax_t        intmax;
typedef uintmax_t       uintmax;

typedef const char*      cstring;
typedef void*            notype;

#endif // __SHORTER_H__