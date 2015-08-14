#pragma once
#ifndef _proto_gl_renderer_h_
#define _proto_gl_renderer_h_

#include "proto.gl.deps.h"
#include "proto.gl.framebuffer.h"
#include "proto.gl.mesh.h"
#include "proto.gl.program.h"
#include "proto.gl.texture.h"

namespace proto {
	namespace gl {

		enum class clear_mask : uint32_t {
			color = GL_COLOR_BUFFER_BIT,
			depth = GL_DEPTH_BUFFER_BIT,
			stencil = GL_STENCIL_BUFFER_BIT
		};

		inline bool operator & (clear_mask v1, clear_mask v2) {
			return ((uint32_t)v1 & (uint32_t)v2) != 0;
		}

		inline clear_mask operator | (clear_mask v1, clear_mask v2) {
			return (clear_mask)((uint32_t)v1 | (uint32_t)v2);
		}

		enum class blend_function : uint32_t {
			zero = GL_ZERO,
			one = GL_ONE,
			src_color = GL_SRC_COLOR,
			one_minus_src_color = GL_ONE_MINUS_SRC_COLOR,
			dst_color = GL_DST_COLOR,
			one_minus_dst_color = GL_ONE_MINUS_DST_COLOR,
			src_alpha = GL_SRC_ALPHA,
			one_minus_src_alpha = GL_ONE_MINUS_SRC_ALPHA,
			dst_alpha = GL_DST_ALPHA,
			one_minus_dst_alpha = GL_ONE_MINUS_DST_ALPHA,
			constant_color = GL_CONSTANT_COLOR,
			one_minus_constant_color = GL_ONE_MINUS_CONSTANT_COLOR,
			constant_alpha = GL_CONSTANT_ALPHA,
			one_minus_constant_alpha = GL_ONE_MINUS_CONSTANT_ALPHA,
			src_alpha_saturate = GL_SRC_ALPHA_SATURATE,
			src1_color = GL_SRC1_COLOR,
			one_minus_src1_color = GL_ONE_MINUS_SRC1_COLOR,
			src1_alpha = GL_SRC1_ALPHA,
			one_minus_src1_alpha = GL_ONE_MINUS_SRC1_ALPHA
		};

		class renderer {
		private:

		public:

			void set_framebuffer(framebuffer & f);
			void reset_framebuffer();

			void set_texture(texture & t);
			void set_mesh(mesh & m);

			void render_mesh(mesh & m);

			void set_program(program & p);

			void set_viewport(int x, int y, int width, int height);

			void enable_depth(blend_function sfactor = blend_function::src_alpha, blend_function dfactor = blend_function::one_minus_src_alpha) const;
			void disable_depth() const;

			void clear(const color & c, clear_mask mask = clear_mask::color);

			void present();

		};
	}
}

#endif