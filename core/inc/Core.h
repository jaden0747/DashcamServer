#pragma once

#define DASHCAM_ASSERT(x, ...) { if (!(x)) {DASHCAM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define BIT(x) (1 << x)