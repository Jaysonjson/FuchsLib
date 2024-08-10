/* Jayson 22.09.21 */
#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <vector>

#if defined(linux) || defined(__linux__)
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <linux/kernel.h>
#endif

#define     dtp               template <typename T>
#define nd

#ifdef _WIN32
#define WINDOWS
#define FUCHS_SUPPORTED
#define OS_IS_WINDOWS
#define architecture_x86
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
#define OS_IS_LINUX
#endif

#if defined(macintosh) || defined(Macintosh) || defined(__MACOSX__) || defined(__APPLE__)
#define MACOS
#define FUCHS_SUPPORTED
#endif

#if defined(__x86_64__) || defined(__x86_64) || defined(_WIN64)
#define architecture_x64
#endif

#ifdef __arm__
#define architecture_ARM_32
#endif

#ifdef __aarch64__
#define architecture_ARM_64
#endif

#ifdef LINUX
struct sysinfo;
    static struct sysinfo sysInfo;
namespace Fuchs {
    extern utsname unameData;
}
#endif

using       int8 =            int8_t; //char
using       int16 =           int16_t; //short
using       int32 =           int32_t; //int
using       uint8 =           uint8_t; //u-char
using       uint16 =          uint16_t; //u-short
using       uint32 =          uint32_t; //u-int

using       fastInt8 =        int_fast8_t;
using       fastInt16 =       int_fast16_t;
using       fastInt32 =       int_fast32_t;

using       ufastInt8 =        uint_fast8_t;
using       ufastInt16 =       uint_fast16_t;
using       ufastInt32 =       uint_fast32_t;

using       char16 =          char16_t;
using       char32 =          char32_t;

#ifdef architecture_x64
using       int64 =           int64_t;
using       uint64 =          uint64_t;
using       dec64 =           double;
using       dec128 =          long double;
using       fastInt64 =       int_fast64_t;
using       ufastInt64 =      uint_fast64_t;
#endif

using       dec32 =           float;

using       intmax =          intmax_t;
using       uintmax =         uintmax_t;

using       cstring =         const char*;
using       notype =          void*;

//https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
template<size_t N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy(str, N, value);
    }

    char value[N];
};

#ifndef FUCHS_SUPPORTED
#error "UNSUPPORTED OS"
#endif

#ifdef WINDOWS
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif
#ifdef LINUX
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif
