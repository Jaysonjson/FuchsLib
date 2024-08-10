#pragma once

#include "shorter.hpp"
#include "structs.hpp"
#include <json.hpp>

using namespace nlohmann;

namespace Fuchs {

    extern m_range<int8> INT8_RANGE;
    extern m_range<uint8> UINT8_RANGE;
    extern m_range<int16> INT16_RANGE;
    extern m_range<uint16> UINT16_RANGE;
    extern m_range<int32> INT32_RANGE;
    extern m_range<uint32> UINT32_RANGE;
    extern m_range<intmax> INTMAX_RANGE;
    extern m_range<uintmax> UINTMAX_RANGE;

    template <typename T, T max, T min>
    class Stack {
        T amount_ = 0;

    public:
        Stack() = default;
        explicit Stack(T amount) {
            setAmount(amount);
        }
        
        /*template<typename I, I max, I min>
        explicit operator Stack<I, max, min>() {
            return Stack<I, max, min>(static_cast<I, max, min>(this->amount_));
        }*/

    public:
        nd T getAmount() { return this->amount_; }
        
        /* Returns the inner amount_ value as reference for direct editing [UNSAFE] */
        nd T& referenceAmount() { return this->amount_; }

        /* Sets the amount safely, checks for MIN/MAX */
        void setAmount(T amountIn) {
            if (amountIn >= max) {
                forceSetAmount(max);
            } 
            else {
                if (amountIn <= min) {
                    forceSetAmount(min);
                } 
                else {
                    forceSetAmount(amountIn);
                }
            }
        }

        /* Force Sets the amount, ignores MIN/MAX */
        void forceSetAmount(T amountIn) { this->amount_ = amountIn; }
    };
}
