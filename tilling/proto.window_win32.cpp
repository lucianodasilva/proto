#ifdef PROTO_OS_WINDOWS

#include "proto.window.h"
#include "proto.debug.h"

#include <utility>
#include <Windows.h>

namespace proto {

	const DWORD window_style = WS_OVERLAPPEDWINDOW;
	const BOOL	window_menu = FALSE;

	class window_imp {
	public:
		
		HWND	handle;
		HDC		dc;
		HGLRC	glrc;

		window_imp () :
			handle (nullptr),
			dc (nullptr),
			glrc (nullptr)
		{}

		~window_imp () {
			if (handle) {
				wglDeleteContext (glrc);
				DeleteDC (dc);
				CloseWindow (handle);
				DestroyWindow (handle);
			}
		}

	};

	void window::swap (window & w) {
		std::swap (_implement, w._implement);
	}

	window::window () {}

	window::window (window && w) {
		swap (w);
	}

	window::~window () {
	}

	window & window::operator = (window && w) {
		swap (w);
		return *this;
	}

	void window::show () const {
		if (_implement)
			::ShowWindow (_implement->handle, true);
	}

	void window::hide () const {
		if (_implement)
			::ShowWindow (_implement->handle, false);
	}

	point window::size () const {
		if (_implement) {
			RECT rct;
			GetWindowRect (_implement->handle, &rct);

			return {(int32_t)(rct.right - rct.left), (int32_t)(rct.bottom - rct.top)};
		}

		return{ 0,0 };
	}

	void window::size (const point & p) {
		if (_implement) {
			::SetWindowPos (_implement->handle, nullptr, 0, 0, p.x, p.y, SWP_NOREPOSITION);
		}
	}

	void window::do_event_loop (window_controller * controller) {
		if (!_implement)
			return;

		while (controller->update ()) {
			MSG msg = {};
			while (PeekMessage (&msg, _implement->handle, 0, 0, PM_REMOVE) > 0) {
				::TranslateMessage (&msg);
				::DispatchMessage (&msg);
			}

			SwapBuffers (_implement->dc);
		}
	}

	LRESULT CALLBACK windows_message_callback (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		return 0;
	}

	window window::create (const char * title, const point & size_v) {

		auto hinst = ::GetModuleHandle (NULL);

		const char * class_name = "PROTO_WND_CLASS";
		WNDCLASS wc = { 0 };
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = windows_message_callback;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinst;
		wc.hIcon = LoadIcon (0, IDI_APPLICATION);
		wc.hCursor = LoadCursor (0, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
		wc.lpszMenuName = 0;
		wc.lpszClassName = class_name;

		if (!::RegisterClass (&wc)) {
			debug_print << "window class registry failed";
			return window ();
		}

		auto imp = std::make_shared < window_imp > ();

		imp->handle = ::CreateWindow (
			class_name,
			title,
			window_style,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			size_v.x,
			size_v.y,
			window_menu,
			0,
			hinst,
			0 // can be window instance
		);

		if (!imp->handle) {
			debug_print << "failed creating window instance";
			return window ();
		}

		PIXELFORMATDESCRIPTOR pfd = {
			sizeof (PIXELFORMATDESCRIPTOR),
			1, // version number
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			24, // color depth
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			32, // 32 bit z buffer
			0,	// no stencil buffer
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		imp->dc = GetDC (imp->handle);
		int pixel_format = ChoosePixelFormat (imp->dc, &pfd);

		if (pixel_format == 0) {
			debug_print << "failed creating pixel format";
			return window ();
		}

		if (SetPixelFormat (imp->dc, pixel_format, &pfd) == FALSE) {
			debug_print << "failed setting pixel format";
			return window ();
		}

		imp->glrc = wglCreateContext (imp->dc);
		if (imp->glrc == nullptr) {
			debug_print << "failed creating wgl context";
			return window ();
		}

		wglMakeCurrent (imp->dc, imp->glrc);

		window w;

		w._implement = imp;
		return w;
	}

}

#endif