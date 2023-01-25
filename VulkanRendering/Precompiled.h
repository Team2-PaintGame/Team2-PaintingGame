#pragma once
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "vulkan/vulkan.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <iosfwd>
#include <set>

#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Quaternion.h"