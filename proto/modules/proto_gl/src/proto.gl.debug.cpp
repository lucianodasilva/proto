//#include "proto.gl.debug.h"
//
//namespace proto {
//	namespace debug {
//
//		std::string gl_error_to_string (GLenum error) {
//
//#			define auto_case(x) \
//				case x: \
//					return #x;
//
//			switch (error) {
//				auto_case (GL_INVALID_ENUM)
//				auto_case (GL_INVALID_VALUE)
//				auto_case (GL_INVALID_OPERATION)
//				auto_case (GL_INVALID_FRAMEBUFFER_OPERATION)
//				auto_case (GL_OUT_OF_MEMORY)
//				auto_case (GL_STACK_UNDERFLOW)
//				auto_case (GL_STACK_OVERFLOW)
//				auto_case (GL_NO_ERROR)
//			default:
//				return "Unknown error code!";
//			};
//
//#			undef auto_case
//
//		}
//
//	}
//}