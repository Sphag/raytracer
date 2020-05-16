#pragma once
#ifndef __RTPCH_H_INCLUDED
#define __RTPCH_H_INCLUDED

////////////////////////////////////////////////////////////////////////////////
// STL
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <climits>
#include <memory>
#include <algorithm>
#include <cstdio>
#include <cctype>
#include <stack>
#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////
// Vendor
////////////////////////////////////////////////////////////////////////////////
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_image_resize.h>

////////////////////////////////////////////////////////////////////////////////
// Core or platform specific stuff
////////////////////////////////////////////////////////////////////////////////
#include "core/core.h"
#include "core/image.h"
#include "core/math_utils.h"
#include "core/pixel_traits.h"
#include "core/string_utils.h"

#ifdef RT_DEBUG
#include "Windows.h"
#endif

#endif
