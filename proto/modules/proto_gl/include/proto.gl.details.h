#pragma once

#ifndef	_proto_gl_details_h_
#define _proto_gl_details_h_

// opengl and math
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cinttypes>

// minor convenience definitions
using point		= glm::ivec2;

using vec2		= glm::vec2;
using vec3		= glm::vec3;
using vec4		= glm::vec4;

using mat4		= glm::mat4;

using color		= glm::vec3;
using color4	= glm::vec4;

namespace proto {
	namespace gl {

		inline uint32_t color_to_int(color const & c) {
			return color_to_int(color4(c, 1.0F));
		}

		inline uint32_t color_to_int(color4 const & c) {
			return
				(uint32_t(c.r * 255.0F) << 24) +
				(uint32_t(c.g * 255.0F) << 16) +
				(uint32_t(c.b * 255.0F) << 8) +
				uint32_t(c.a * 255.0F);
		}

	}
}

#endif