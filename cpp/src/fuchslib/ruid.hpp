/* Jayson */
#pragma once

#include "shorter.hpp"
#include <string>
#include <vector>

namespace Fuchs {
    using RUID = std::string;

    struct RUIDPattern {
        std::string prefix = "F";
        std::string suffix = "L";
        std::vector<uint8> rows{ 8, 4, 15 };
    };

    RUID generateRUID(const std::string& prefix = "F", const std::string& suffix = "L", const std::vector<uint8>& rows = {8, 4, 15});
    std::string getRUIDPrefix(const RUID& ruid);
    std::string getRUIDSuffix(const RUID& ruid);
    std::vector<std::string> getRUIDRows(const RUID& ruid);
    bool ruidPatternMatches(const RUID& ruid, const RUIDPattern& pattern = {"F", "L", {8, 4, 15}});

    inline bool ruidPatternMatches(const RUID& ruid, const std::string& prefix, const std::string& suffix, const std::vector<uint8>& rows) {
        return ruidPatternMatches(ruid, {prefix, suffix, rows});
    }

    inline RUID generateRUID(const RUIDPattern& pattern) {
        return generateRUID(pattern.prefix, pattern.suffix, pattern.rows);
    }

    class RUIDC {
        std::string content_ = "";
        RUIDPattern pattern_{};
    public:
        bool empty() { return content_.empty(); }
        std::string prefix() { return pattern_.prefix; }
        std::string suffix() { return pattern_.suffix; }
        std::vector<std::string> rows() { return getRUIDRows(content_); }
        bool matchesPattern(const RUIDPattern& pattern) { return ruidPatternMatches(content_, pattern); }
        bool valid() { return matchesPattern(pattern_); }
        void generate() { content_ = generateRUID(pattern_); }
        RUIDPattern& pattern() { return pattern_; }
        std::string content() { return content_; }

        friend std::ostream& operator<<(std::ostream& out, const RUIDC& j) {
            out << j.content_;
            return out;
        }
    };
}