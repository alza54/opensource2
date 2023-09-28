#pragma once

#ifdef _DEBUG
#define OPENSOURCE2_ENABLE_LOGGING 1
#endif  // DEBUG

#ifdef NDEBUG
#define OPENSOURCE2_ENABLE_LOGGING 0
#endif  // NDEBUG

#include "api.hpp"

#ifdef LOAD_SDK_FILES
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#include <glm/glm.hpp>

// Compile-time string encryption. Strings are decoded in run-time.
// In static analysis strings are the simplest trace, easiest to identify.
// Thanks to this small helper no obvious trace of the SDK in strings is left in the compiled file.
#include "utilities/static_string.hpp"

// [SDK]: In-game classes schema helper
#include "schema/schema.hpp"

// [SDK] Virtual function
#include "virtual/virtual.hpp"

// [SDK]: Source 2 SDK definitions
#include "source-sdk/source-sdk.hpp"

// [SDK]: Interface definition
#include "interface/interface.hpp"

// [SDK]: Game modules representation
#include "module/module.hpp"

// Interfaces: game structures grouping necessary methods for a convenient access
#include "interfaces/interfaces.hpp"

// Memory: scans game memory searching for memory patterns of
// essential game functions and internals, by their byte signature

// Finds essential game functions and internals, scanning game memory trying to //
// match signatures defined in the SDK. Signatures are being found
// in the act of reverse engineering the game binaries
#include "memory/memory.hpp"

// [SDK]: Hook utility class
#include "hooks/hook.hpp"

// Function execution manipulation
#include "hooks/hooks.hpp"

// DirectX 11 shaders
// [...]

// Math helpers
#include "math/math.hpp"
#endif
