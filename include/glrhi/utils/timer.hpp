#pragma once

#include <chrono>

namespace glrhi {
    class timer {
    public:

        timer() {
            reset();
        }
        ~timer() = default;

        void reset() {
            m_begin = std::chrono::high_resolution_clock::now();
        }
        double get() {
            auto current = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diffrence = current - m_begin;
            return diffrence.count();
        }

    private:

        // jebes to
        std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> m_begin;

    };
}