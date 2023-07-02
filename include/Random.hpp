#pragma once

#include "pch.hpp"

class Random {
private:
    static thread_local std::default_random_engine rnd;

public:
    Random() = delete;
    Random(Random&) = delete;

    static int uniform_int(int min, int max)
    {
        std::uniform_int_distribution<int> dst(min, max);
        return dst(rnd);
    }

    static float uniform_float(float min, float max)
    {
        std::uniform_real_distribution<float> dst(min, max);
        return dst(rnd);
    }
};
