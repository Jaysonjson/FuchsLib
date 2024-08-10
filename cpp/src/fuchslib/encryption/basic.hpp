/* Jayson */
#pragma once

#include "../shorter.hpp"

namespace Fuchs {

    class Cryptor {

    public:
        virtual ~Cryptor() = default;

        virtual std::string encrypt(std::string input) = 0;
        virtual std::string decrypt(std::string input) = 0;
    };

    /*
    * Uses a single key which counts up (encryption) or down (decryption)
    */
    class BasicCryptor : public Cryptor {
        const int key_;

    public:
        BasicCryptor() = default;
        constexpr BasicCryptor(int key) : key_(key) {}

    public:
        std::string encrypt(std::string input) override {
            for (auto& item : input) item = item + key_;
            return input;
        }

        std::string decrypt(std::string input) override {
            for (auto& item : input) item = item - key_;
            return input;
        }
    };

    /*
    * Uses a sequence of keys which gets iterated over while increasing (encryption)
    * or decreasing (decryption) another iteration of the input
    */
    class SequencedBasicCryptor : public Cryptor {
        const std::string sequence_;

    public:
        SequencedBasicCryptor() = default;
        SequencedBasicCryptor(std::string sequence) : sequence_(std::move(sequence)) {}

    public:
        std::string encrypt(std::string input) override {
            int i = 0;
            for (auto& item : input) {
                item = item + sequence_[i];
                if (i == sequence_.length()) i = 0;
                ++i;
            }
            return input;
        }

        std::string decrypt(std::string input) override {
            int i = 0;
            for (auto& item : input) {
                item = item - sequence_[i];
                if (i == sequence_.length()) i = 0;
                ++i;
            }
            return input;
        }
    };
}
