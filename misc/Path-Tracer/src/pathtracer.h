#pragma once

#include "glog/logging.h"


#ifdef __GNUG__
#define RUN_ON_LINUX
#else
#endif  // __GNUG__

typedef float Float;

#define Infinity std::numeric_limits<Float>::infinity()

#if defined(_WIN32) || defined(_WIN64)
#define OS_IS_WINDOWS
	#if defined(__MINGW32__)  // Defined for both 32 bit/64 bit MinGW
    #define PBRT_IS_MINGW
	#elif defined(_MSC_VER)
	#define PBRT_IS_MSVC
	#endif
#elif defined(__linux__)
#define OS_IS_LINUX
#elif defined(__APPLE__)
#define OS_IS_OSX
#elif defined(__OpenBSD__)
#define OS_IS_OPENBSD
#elif defined(__FreeBSD__)
#define OS_IS_FREEBSD
#endif

static const Float Pi = 3.14159265358979323846;

#ifdef _MSC_VER
#define MachineEpsilon (std::numeric_limits<Float>::epsilon() * 0.5)
#else
static const Float MachineEpsilon =
std::numeric_limits<Float>::epsilon() * 0.5;
#endif

inline Float gamma(int n) {
	return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
}

inline int Log2Int(uint32_t v) {
#if defined(PBRT_IS_MSVC)
	unsigned long lz = 0;
	if (_BitScanReverse(&lz, v)) return lz;
	return 0;
#else
	return 31 - __builtin_clz(v);
#endif
}

inline int Log2Int(int32_t v) { return Log2Int((uint32_t)v); }

inline int Log2Int(uint64_t v) {
#if defined(PBRT_IS_MSVC)
	unsigned long lz = 0;
//	if (_BitScanReverse64(&lz, v)) return lz;
	return 0;
#else
	return 63 - __builtin_clzll(v);
#endif
}

inline int Log2Int(int64_t v) { return Log2Int((uint64_t)v); }

inline uint32_t FloatToBits(float f) {
	uint32_t ui;
	memcpy(&ui, &f, sizeof(float));
	return ui;
}

inline float BitsToFloat(uint32_t ui) {
	float f;
	memcpy(&f, &ui, sizeof(uint32_t));
	return f;
}

inline uint64_t FloatToBits(double f) {
	uint64_t ui;
	memcpy(&ui, &f, sizeof(double));
	return ui;
}

inline double BitsToFloat(uint64_t ui) {
	double f;
	memcpy(&f, &ui, sizeof(uint64_t));
	return f;
}

inline float NextFloatUp(float v) {
	// Handle infinity and negative zero for _NextFloatUp()_
	if (std::isinf(v) && v > 0.) return v;
	if (v == -0.f) v = 0.f;

	// Advance _v_ to next higher float
	uint32_t ui = FloatToBits(v);
	if (v >= 0)
		++ui;
	else
		--ui;
	return BitsToFloat(ui);
}

inline float NextFloatDown(float v) {
	// Handle infinity and positive zero for _NextFloatDown()_
	if (std::isinf(v) && v < 0.) return v;
	if (v == 0.f) v = -0.f;
	uint32_t ui = FloatToBits(v);
	if (v > 0)
		--ui;
	else
		++ui;
	return BitsToFloat(ui);
}

inline double NextFloatUp(double v, int delta = 1) {
	if (std::isinf(v) && v > 0.) return v;
	if (v == -0.f) v = 0.f;
	uint64_t ui = FloatToBits(v);
	if (v >= 0.)
		ui += delta;
	else
		ui -= delta;
	return BitsToFloat(ui);
}

inline double NextFloatDown(double v, int delta = 1) {
	if (std::isinf(v) && v < 0.) return v;
	if (v == 0.f) v = -0.f;
	uint64_t ui = FloatToBits(v);
	if (v > 0.)
		ui -= delta;
	else
		ui += delta;
	return BitsToFloat(ui);
}



template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
	if (val < low)
		return low;
	else if (val > high)
		return high;
	else
		return val;
}