#pragma once

//-----------------------------------------------------------------------------
// Base Header
//-----------------------------------------------------------------------------
#include "Base/EngineVersionConst.h"
#include "Base/DetectCompiler.h"
#include "Base/DetectOS.h"
#include "Base/VersionOSConst.h"
#include "Base/ConfigEngine.h"
#include "Base/BaseMacros.h"

//-----------------------------------------------------------------------------
// 3rd-party Header
//-----------------------------------------------------------------------------
SE_PRAGMA_WARNING_LEVEL(0);

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <stb_image.h>

#include <tiny_obj_loader.h>

SE_PRAGMA_WARNING_POP

//-----------------------------------------------------------------------------
// STL Header
//-----------------------------------------------------------------------------
SE_PRAGMA_WARNING_LEVEL(0);

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>
#include <unordered_map>

SE_PRAGMA_WARNING_POP