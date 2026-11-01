#pragma once
#include <chrono>
#include <random>

// Shared random engine; seeded once, unlike the old per-call srand(time(0))
// which repeated the same "random" values for a whole second.
inline std::mt19937& rng()
{
    static std::mt19937 engine{std::random_device{}()};
    return engine;
}

// Uniform int in [0, n); 0 when n <= 0.
inline int randInt(int n)
{
    if (n <= 0)
        return 0;
    return std::uniform_int_distribution<int>(0, n - 1)(rng());
}

// Uniform float in [lo, hi).
inline float randFloat(float lo, float hi)
{
    return std::uniform_real_distribution<float>(lo, hi)(rng());
}

inline bool randChance(float p)
{
    return randFloat(0.0f, 1.0f) < p;
}

// Milliseconds since the first call, starting at 1000 so callers can keep using
// 0 as "not set". Replaces tv_sec*1000 epoch math, which overflowed a 32-bit
// long and lost precision in a float.
inline float nowMs()
{
    static const auto start = std::chrono::steady_clock::now();
    return 1000.0f + std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - start).count();
}
