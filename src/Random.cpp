#include "Random.hpp"

thread_local std::default_random_engine Random::rnd(std::random_device{}());
