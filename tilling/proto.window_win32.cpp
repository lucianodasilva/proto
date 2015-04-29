#ifdef PROTO_OS_WINDOWS

#include "proto.window.h"
#include "proto.window_manager.h"
#include "proto.debug.h"

#include <utility>
#include <Windows.h>
#include <windowsx.h>

using namespace std;

namespace proto {

	const LPCSTR	window_class_name	= "PROTOWNDCLASS";
	const DWORD		window_style		= WS_OVERLAPPEDWINDOW;

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
				::wglDeleteContext (glrc);
				::DeleteDC (dc);
				::CloseWindow (handle);
				::DestroyWindow (handle);
			}
		}

	};

	window::window () : _implement (make_unique < window_imp> ())
	{}

	window::~window () {}

	void window::show () const {
		if (_implement)
			::ShowWindow (_implement->handle, true);
	}

	void window::hide () const {
		if (_implement)
			::ShowWindow (_implement->handle, false);
	}

	void window::close () {
		if (_implement) {
			_implement.release ();
		}
	}

	bool window::is_visible () const {
		if (_implement)
			return ::IsWindowVisible (_implement->handle) == TRUE;
		
		return false;
	}

	bool window::is_closed () const {
		return !_implement;
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

	extern LRESULT CALLBACK windows_message_callback (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	std::shared_ptr < window > window::create (const char * title, const point & size_v) {

		auto hinst = ::GetModuleHandle (NULL);

		WNDCLASS wc;
		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= windows_message_callback;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= hinst;
		wc.hIcon			= LoadIcon (0, IDI_APPLICATION);
		wc.hCursor			= LoadCursor (0, IDC_ARROW);
		wc.hbrBackground	= (HBRUSH)GetStockObject (WHITE_BRUSH);
		wc.lpszMenuName		= 0;
		wc.lpszClassName	= window_class_name;

		if (!::RegisterClass (&wc)) {
			debug_print << "window class registry failed";
			return nullptr;
		}

		auto inst = shared_ptr < window > (new window ());

		inst->_implement->handle = ::CreateWindow (
			window_class_name,
			title,
			window_style,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			size_v.x,
			size_v.y,
			0,
			0,
			hinst,
			inst.get ()
		);

		if (!inst->_implement->handle) {
			debug_print << "failed creating window instance";
			return nullptr;
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

		inst->_implement->dc = GetDC (inst->_implement->handle);
		int pixel_format = ChoosePixelFormat (inst->_implement->dc, &pfd);

		if (pixel_format == 0) {
			debug_print << "failed creating pixel format";
			return nullptr;
		}

		if (SetPixelFormat (inst->_implement->dc, pixel_format, &pfd) == FALSE) {
			debug_print << "failed setting pixel format";
			return nullptr;
		}

		inst->_implement->glrc = wglCreateContext (inst->_implement->dc);
		if (inst->_implement->glrc == nullptr) {
			debug_print << "failed creating wgl context";
			return nullptr;
		}

		window_manager::instance ().register_window (inst);
		return inst;
	}

	void *  window::native_handle () const {
		return _implement->handle;
	}

	void * window::native_device () const {
		return _implement->dc;
	}

	void window::make_active () {
		wglMakeCurrent (_implement->dc, _implement->glrc);
	}

}

#endif