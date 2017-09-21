#pragma once

#ifdef PROTO_API_WIN32

#ifndef	_proto_gl_window_win32_h_
#define _proto_gl_window_win32_h_

#include "proto.gl.window.h"

#define VC_EXTRALEAN
#include <windowsx.h>

namespace proto {
	namespace gl {

		class application_base;

		class window_win32 : public proto::gl::window {
		public:

			HWND	handle() const;
			HDC		dc() const;
			HGLRC	glrc() const;

			virtual ~window_win32();

			void handle_events();

			// Inherited via window
			virtual void show() override;

			virtual void hide() override;

			virtual void close() override;

			virtual bool is_visible() const override;

			virtual bool is_closed() const override;

			virtual point size() const override;

			virtual void size(const point & p) override;

			application_base * application();

			static expected < std::unique_ptr < window_win32 > > create(
				application_base * application,
				std::string const & title, 
				point const & size_v
			);

		private:

			HWND	_handle = nullptr;
			HDC		_dc = nullptr;
			HGLRC	_glrc = nullptr;

			application_base * _application;
		};

	}
}

#endif
#endif