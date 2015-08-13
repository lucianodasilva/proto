#pragma once

#ifndef _proto_formats_h_
#define _proto_formats_h_

#include <cinttypes>
#include "proto.gl.h"

namespace proto {

    enum class texture_wrap : uint32_t {
		none					= GL_NONE,
		clamp_to_edge			= GL_CLAMP_TO_EDGE,
		clamp_to_border			= GL_CLAMP_TO_BORDER,
		mirrored_repeat			= GL_MIRRORED_REPEAT,
		repeat					= GL_REPEAT
    };

	enum class texture_filter : uint32_t {
		nearest					= GL_NEAREST,
		linear					= GL_LINEAR,
		linear_mipmap_nearest	= GL_LINEAR_MIPMAP_NEAREST,
		linear_mipmap_linear	= GL_LINEAR_MIPMAP_LINEAR
	};

	enum class texture_format : uint32_t {
		rgb						= GL_RGB,
		rgba					= GL_RGBA
	};

}

#endif