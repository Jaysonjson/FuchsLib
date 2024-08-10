#include "fuchslib/ruid.hpp"
#include "fuchslib/shorter.hpp"
#include "fuchslib/utility.hpp"

#include <cassert>
#include <cstddef>
#include <ios>
#include <random>
#include <sstream>
#include <string>
#include <thread>

namespace Fuchs {
    static std::random_device rd;
    static std::mt19937_64  gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);

    RUID generateRUID(const std::string& prefix, const std::string& suffix, const std::vector<uint8>& rows) {
        #if __cplusplus > 202002L
            assert(!prefix.contains("-"));
            assert(!suffix.contains("-"));
        #else
            assert(std::count(prefix.begin(), prefix.end(), '-') == 0);
            assert(std::count(suffix.begin(), suffix.end(), '-') == 0);
        #endif
        std::stringstream sstream;
        sstream << std::hex;
        if(!prefix.empty()) sstream << prefix << "-";

        for (int i = 0; i < rows.size(); ++i) {
            for(int j = 0; j < rows[i]; ++j) sstream << dis(gen);
            if(i != rows.size() - 1) sstream << "-";
        }

        if(!suffix.empty()) sstream << "-" << suffix;
        return sstream.str();
    }

    std::string getRUIDPrefix(const RUID& ruid) {
        #if __cplusplus > 202002L
            if(!ruid.contains("-")) return "";
        #else
            if (std::count(ruid.begin(), ruid.end(), '-') == 0) return "";
        #endif
        return std::move(ruid.substr(0, ruid.find_first_of("-")));
    }

    std::string getRUIDSuffix(const RUID& ruid) {
        #if __cplusplus > 202002L
            if(!ruid.contains("-")) return "";
        #else
            if (std::count(ruid.begin(), ruid.end(), '-') == 0) return "";
        #endif
        return std::move(ruid.substr(ruid.find_last_of("-") + 1, ruid.length()));
    }

    std::vector<std::string> getRUIDRows(const RUID& ruid) {
        std::vector<std::string> rows{};
        std::string prefix = getRUIDPrefix(ruid);
        std::string suffix = getRUIDSuffix(ruid);
        std::string rowSequence = ruid.substr(prefix.length() + 1, ruid.length() - (prefix.length() + suffix.length() + 2));
        std::string currentRow;
        for(;;) {
            size_t indPos = rowSequence.find_first_of("-");
            if(indPos == std::string::npos) break;
            currentRow = rowSequence.substr(0, indPos);
            rowSequence = rowSequence.substr(currentRow.length() + 1, rowSequence.length());
            rows.push_back(currentRow);
        }
        return rows;
    }

    bool ruidPatternMatches(const RUID& ruid, const RUIDPattern& pattern) {
        if(ruid.empty()) return false;

        int length = pattern.prefix.length() + pattern.suffix.length() + 1;
        for(uint8 row : pattern.rows) length += row + 1;
        if(ruid.length() != length) return false;

        std::string prefix = getRUIDPrefix(ruid);
        if(pattern.prefix != prefix) return false;
        
        std::string suffix = getRUIDSuffix(ruid);
        if(pattern.suffix != suffix) return false;

        std::string rowSequence = ruid.substr(prefix.length() + 1, ruid.length() - (prefix.length() + suffix.length() + 2));
        std::string currentRow;
        int validRows = 0;
        
        for(int i = 0; i < pattern.rows.size() - 1; ++i) {
            size_t indPos = rowSequence.find_first_of("-");
            if(indPos == std::string::npos) return false;
            currentRow = rowSequence.substr(0, indPos);
            if(currentRow.length() == pattern.rows[i]) ++validRows;
            rowSequence = rowSequence.substr(currentRow.length() + 1, rowSequence.length());
        }
        return validRows == pattern.rows.size() - 1;
    }
}
