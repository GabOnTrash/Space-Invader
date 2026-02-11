#pragma once

#include <random>
#include <type_traits>

inline std::mt19937& GetRNG()
{
	static std::random_device seed;
	static std::mt19937 gen(seed());

	return gen;
}

template <typename T>
T GetRN(T min, T max)
{
	static_assert(std::is_arithmetic<T>::value, "Must pass 2 numbers only.");

	if constexpr (std::is_integral<T>::value)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(GetRNG());
	}
	else if constexpr (std::is_floating_point<T>::value)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(GetRNG());
	}
}