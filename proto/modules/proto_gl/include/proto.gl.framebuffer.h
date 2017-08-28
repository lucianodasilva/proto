//#pragma once
//
//#ifndef _proto_gl_framebuffer_h_
//#define _proto_gl_framebuffer_h_
//
//#include "proto.gl.deps.h"
//#include "proto.gl.texture.h"
//
//namespace proto {
//	namespace gl {
//
//		class framebuffer {
//		private:
//
//			GLuint	_framebuffer_id;
//			GLuint	_auxiliar_id;
//			bool	_is_active;
//			bool	_has_renderbuffer;
//			bool	_has_no_backbuffer;
//
//
//		public:
//
//			void swap(framebuffer & v);
//
//			framebuffer();
//
//			framebuffer(framebuffer && v);
//			~framebuffer();
//
//			framebuffer & operator = (framebuffer && v);
//
//			void bind() const;
//
//			static framebuffer create(const texture & color_buffer);
//			static framebuffer create(const texture & color_buffer, const texture & depth_buffer);
//			static framebuffer create_depth_only(const texture & depth_buffer);
//
//		};
//	}
//}
//
//#endif