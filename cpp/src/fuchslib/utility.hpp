/* Jayson */
#pragma once

#include "shorter.hpp"
#include "data.hpp"
#include <fstream>
#include <filesystem>
#include "limits.h"
#include "algorithm"
#include "json.hpp"
using namespace nlohmann;

#define BUILD_NLOHMANNJSON
#ifdef WINDOWS
#include <Windows.h>
#include <psapi.h>
#endif

#ifdef LINUX
#include <linux/kernel.h>
#include "unistd.h"
#endif

namespace Fuchs::Utility
{

#ifdef BUILD_NLOHMANNJSON
    namespace js
    {
        inline bool hasKey(const json &js, const std::string &key) { return js.contains(key); }

        dtp inline void getIfHas(const json &js, const std::string &key, T &t)
        {
            if (hasKey(js, key))
                js.at(key).get_to(t);
        }

        dtp inline void getIfHas(const json &js, const std::string &key, const std::function<void(T)> &setFunc)
        {
            if (hasKey(js, key))
                setFunc(js.at(key));
        }
    }
#endif

    inline std::string wStringToString(const std::wstring &wstringIn)
    {
        return std::string{wstringIn.begin(), wstringIn.end()};
    }

    inline std::wstring stringToWString(const std::string &stringIn)
    {
        return std::wstring{stringIn.begin(), stringIn.end()};
    }

    inline void openDirectoryInFileManager(const std::string &dir)
    {
#ifdef UNIX
        system(std::string{"gio open " + dir}.c_str());
#endif
#ifdef WINDOWS
        // system(string{"cd " + dir}.c_str());
        system(std::string{"explorer \"" + dir + "\""}.c_str());
#endif
#ifdef MACOS
        system(std::string{"open \"" + dir + "\""}.c_str());
#endif
    }

    inline void openURLInBrowser(const std::string &url)
    {
#ifdef UNIX
        system(std::string{"xdg-open " + url}.c_str());
#endif
#ifdef WINDOWS
        system(std::string{"start " + url}.c_str());
#endif
#ifdef MACOS
        system(std::string{"open " + url}.c_str());
#endif
    }

    inline bool createDirectories(const std::string &path)
    {
        try
        {
            std::filesystem::create_directories(path);
            return true;
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
            return false;
        }
    }

    inline std::string toLowerCase(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        return str;
    }

    inline std::string toUpperCase(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                       { return std::toupper(c); });
        return str;
    }

    inline void toLowerCaseRef(std::string &str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                       { return std::tolower(c); });
    }

    inline void toUpperCaseRef(std::string &str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                       { return std::toupper(c); });
    }

#if __cplusplus < 202003L
    template <typename T, typename J>
    inline bool contains(const T &array, const J &content)
    {
        return std::find(array.begin(), array.end(), content) != array.end();
    }
#endif

    dtp
        nd inline std::array<char, sizeof(T)>
        serialize(const T &t)
    {
        std::array<char, sizeof(T)> buffer;
        memcpy(&buffer, &t, sizeof(T));
        return std::move(buffer);
    }

    dtp
        nd inline T *
        deserialize(auto buffer)
    {
        return (T *)buffer;
    }

    nd uintmax injectLibrary(const std::string &path, void *data);

    nd std::string getCMDOutput(const std::string &cmd);

    nd std::string getOSName();

#ifdef LINUX
    nd inline std::string readLink(const std::string &link)
    {
        char buffer[PATH_MAX];
        ssize_t len = readlink(link.c_str(), buffer, sizeof(buffer) - 1);
        if (len != -1)
        {
            buffer[len] = '\0';
            return std::string(buffer);
        }
        return "";
    }
#endif

    nd inline std::string getExecutableFile()
    {
#ifdef WINDOWS
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        return std::string(buffer);
#endif

#ifdef LINUX
        return readLink("/proc/self/exe");
#endif
        return "";
    }

    nd inline std::string getExecutableDirectory()
    {
#ifdef WINDOWS
        std::string cache = getExecutableFile();
        std::string::size_type pos = std::string(cache).find_last_of("\\/");
        return std::string(cache).substr(0, pos + 1);
#endif

#ifdef LINUX
        return getExecutableFile().substr(0, getExecutableFile().find_last_of("/"));
#endif
        return "";
    }

    nd inline std::string getArchitecture()
    {
#ifdef x64
        return "x64";
#endif
#ifdef ARM_32
        return "ARM-aarch32";
#endif
#ifdef ARM_64
        return "ARM-aarch64";
#endif
        return "x86";
    }

    nd inline uintmax getPhysicalRAM()
    {
#ifdef WINDOWS
        uintmax ram = 0;
        GetPhysicallyInstalledSystemMemory(&ram);
        return ram * 1024;
#endif
#ifdef LINUX
        // https://stackoverflow.com/questions/349889/how-do-you-determine-the-amount-of-linux-system-ram-in-c
#endif
        return -1;
    }

    nd inline std::string getGPUName(uintmax monitor)
    {
#ifdef WINDOWS
        DISPLAY_DEVICE dd = {sizeof(dd), 0};
        if (!EnumDisplayDevices(NULL, monitor, &dd, EDD_GET_DEVICE_INTERFACE_NAME))
            return "UNKNOWN";
        return dd.DeviceString;
#endif

#ifdef LINUX
        std::string output = getCMDOutput("lshw");
        if(output.contains("*-display")) {
            std::string displayOutput = output.substr(output.find("*-display"));
            std::string gpuName = displayOutput.substr(displayOutput.find("product:") + 9);
            return gpuName.substr(0, gpuName.find("\n"));
        }
#endif
        return "UNKNOWN";
    }

#ifdef LINUX
    // int setResourceLimit(const Data::Unit<0>& unit);
    int setResourceLimit(uint8 megabytes);

    nd inline std::string getKernelRelease()
    {
        return unameData.release;
    }

    nd Data::LINUX_DESKTOP_ENVIRONMENT getDesktopEnvironment();

#endif

    inline uintmax getCurrentMemoryUsage()
    {
#ifdef WINDOWS
        PROCESS_MEMORY_COUNTERS counters;
        HANDLE process = GetCurrentProcess();
        uintmax bytes = 0;
        if (K32GetProcessMemoryInfo(process, &counters, sizeof(counters)))
        {
            bytes = counters.PagefileUsage;
        }
        CloseHandle(process);
        return bytes;
#endif

#ifdef LINUX
        long rss = 0L;
        FILE *fp = NULL;
        if ((fp = fopen("/proc/self/statm", "r")) == NULL)
            return (size_t)0L;
        if (fscanf(fp, "%*s%ld", &rss) != 1)
        {
            fclose(fp);
            return (size_t)0L;
        }
        fclose(fp);
        return (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);
#endif
    }

    inline uintmax getTotalMemory()
    {
#ifdef WINDOWS
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullTotalPhys;
#endif
#ifdef LINUX
        return sysInfo.totalram;
#endif
        return 0;
    }

}