#pragma once

#include "shorter.hpp"
#include "data.hpp"
#include "utility.hpp"

struct OSPlatformLinux {
    Fuchs::Data::LINUX_DESKTOP_ENVIRONMENT environment = Fuchs::Data::UNKNOWN;

    inline void reload() {
#ifdef LINUX
        environment = Fuchs::Utility::getDesktopEnvironment();
#endif
    }
};

struct OSPlatformWindows {
    inline void reload() {
#ifdef WINDOWS
#endif
    }
};

class OSPlatform {
    OSPlatformLinux linuxData{};
    OSPlatformWindows windowsData{};
public:

    inline uintmax getPhysicalRAM() { Fuchs::Utility::getPhysicalRAM(); }
    inline std::string getGPUName(uintmax monitor) { Fuchs::Utility::getGPUName(monitor); }
    inline uintmax getCurrentMemoryUsage() { return Fuchs::Utility::getCurrentMemoryUsage(); }
    inline uintmax getTotalMemory() { Fuchs::Utility::getTotalMemory(); }

    OSPlatformWindows& windows() {
        return windowsData;
    }

    OSPlatformLinux& linux() {
        return linuxData;
    }

    inline bool isLinux() {
        #ifdef LINUX
        return true;
        #endif
        return false;
    }

    inline bool isWindows() {
        #ifdef WINDOWS
        return true;
        #endif
        return false;
    }

    inline bool isMac() {
        #ifdef MACOS
        return true;
        #endif
        return false;
    }

    inline bool isUnix() {
        #ifdef UNIX
        return true;
        #endif
        return false;
    }

    inline void create() {
        #ifdef OS_IS_LINUX
        linux().reload();
        #endif
        #ifdef OS_IS_WINDOWS 
        windows().reload();
        #endif
    }
};