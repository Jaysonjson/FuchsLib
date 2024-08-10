/* Jayson Hartmann - 8.2.2022 */
#pragma once

#include "shorter.hpp"
#include <regex>

namespace Fuchs {

    /* Minecraft-Like ID 
    * ID's of Objects, Contents, etc. 
    * Key should be the name, as example: example_object
    * Namespace should be the mod's id, as example: fuchslib
    * A whole FuchsKey is built using namespace:key, as example: fuchslib:example_object
    */
    class FuchsKey {
        std::string key_ = "";
        std::string namespace_ = "";
    public:
        FuchsKey() = default;
        FuchsKey(std::string key, std::string namespaceIn): key_(std::move(key)), namespace_(std::move(namespaceIn)) {}

        constexpr static cstring keyPattern_ = "[a-z0-9/._-]{2,31}";
        constexpr static cstring fullPattern_ = "[a-z0-9/._-]{2,31}:[a-z0-9/._-]{2,31}";
        constexpr static int AR_NAMESPACE = 0;
        constexpr static int AR_KEY = 1;

    template <typename T, StringLiteral name>
    friend class Registry;
    public:
        std::string getKey() const { return key_; }
        std::string getNameSpace() const { return namespace_; }

        bool empty() {
            return key_.empty() && namespace_.empty();
        }

        bool valid() const {
            return valid(asString());
        }

        static bool valid(const std::string& str) {
            return std::regex_match(str, std::regex{fullPattern_});
        }

        std::string asString() const {
            return getNameSpace() + ":" + getKey();
        }

        /* Returns an Empty FuchsKey if given string is not valid */
        static FuchsKey fromString(const std::string& str) {
            if(valid(str)) {
                auto delimeter = str.find(':'); //No need to check if delimeter is NPOS, due to valid check
                return FuchsKey{str.substr(0, delimeter), str.substr(delimeter)};
            }         
            return FuchsKey{};
        }

        friend std::ostream& operator<<(std::ostream& os, const FuchsKey& key) {
            os << key.asString();
            return os;
        }

        friend bool operator==(const FuchsKey& key0, const FuchsKey& key) {
            return key.asString() == key0.asString();
        }

        std::string operator[](int i) {
            if (i == 0) return getNameSpace();
            if (i == 1) return getKey();
        }
    };

}