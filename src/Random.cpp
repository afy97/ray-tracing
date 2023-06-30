#include "Random.hpp"

thread_local std::random_device Random::dev;
thread_local std::default_random_engine Random::rnd(Random::dev());
