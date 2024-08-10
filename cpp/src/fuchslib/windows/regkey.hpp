/* Jayson */
#pragma once
#include "../shorter.hpp"
#ifdef WINDOWS
#include "../utility.hpp"
#include <windows.h>
#include <winreg.h>

namespace Fuchs::Reg {


    class HKey {
        HKEY key_;
        HKEY original_;
        const std::string subKey_;

    public:
        HKey(HKEY key, std::string subKey) : original_(key), subKey_(std::move(subKey)) {}

    public:
        std::string getSubKey() const { return subKey_; }
        HKEY getHKey() const { return key_; }
        HKEY getOriginal() const { return original_; }

        LONG create() {
            DWORD pos;
            LONG result = RegCreateKeyEx(original_, subKey_.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key_, &pos);
            close();
            return result;
        }

        void close() {
            RegCloseKey(key_);
        }

        LONG open(DWORD access = KEY_ALL_ACCESS) {
            return RegOpenKeyEx(
                original_,
                subKey_.c_str(),
                0,
                access,
                &key_
            );
        }

    };

    /*
     * https://docs.microsoft.com/en-us/windows/win32/sysinfo/registry-key-security-and-access-rights
     * https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-reggetvaluea?redirectedfrom=MSDN
     * */
    template <typename T, DWORD type, DWORD typeRestriction>
    class Key {
    protected:
        const std::string value_;
        T t_;
        DWORD size_;
        HKey* key_;

    public:
        Key(HKey* key, std::string value) : key_(key), value_(std::move(value)), size_(sizeof(T)) {}
        virtual ~Key() = default;
    public:
        DWORD getSize() const { return size_; }
        std::string getValue() const { return value_; }
        Key* getHKey() const { return key_; }

        virtual LONG refresh() {
            return RegGetValue(key_->getOriginal(), key_->getSubKey().c_str(), value_.c_str(), typeRestriction, nullptr, &t_, &size_);
        }

        T get() const { return t_; }

        virtual LONG set(T t, DWORD access = KEY_ALL_ACCESS) {
            key_->open(access);
            LONG result = RegSetValueEx(
                key_->getHKey(),
                value_.c_str(),
                0,
                type,
                ((BYTE*)(&t)),
                size_);
            key_->close();
            return result;
        }
    };

    class StringKey : public Key<std::string, REG_SZ, RRF_RT_ANY> {
    public:
        StringKey(HKey* key, std::string value) : Key(key, value) {}

    public:
        LONG set(std::string t, DWORD access = KEY_ALL_ACCESS) override {
            key_->open(access);
            LONG result = RegSetValueEx(
                key_->getHKey(),
                value_.c_str(),
                0,
                REG_SZ,
                (LPBYTE)(t.c_str()),
                (t.size() + 1) * sizeof(wchar_t));
            key_->close();
            return result;
        }

        LONG refresh() {
            DWORD dwType = REG_SZ;
            char value[1024];
            DWORD value_length = 1024;
            key_->open();
            RegQueryValueEx(key_->getHKey(), value_.c_str(), NULL, &dwType, (LPBYTE)&value, &value_length);
            t_ = std::string(value);
            key_->close();
            return 0;
        }
    };

    using DWORDKey = Key<DWORD, REG_DWORD, RRF_RT_DWORD>;
    using STRINGKey = StringKey;
};
#endif