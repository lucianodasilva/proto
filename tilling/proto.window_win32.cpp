#ifdef PROTO_OS_WINDOWS

#include "proto.window.h"
#include "proto.debug.h"

#include <utility>
#include <Windows.h>
#include <windowsx.h>

using namespace std;

namespace proto {

	const LPCSTR	window_class_name	= "PROTOWNDCLASS";
	const DWORD		window_style		= WS_OVERLAPPEDWINDOW;

	inline void get_mouse_info (
		HWND hWnd,
		WPARAM wParam,
		LPARAM lParam,
		mouse_buttons & out_buttons,
		point & out_position
	) {

		out_position.x = GET_X_LPARAM (lParam);
		out_position.y = GET_Y_LPARAM (lParam);

		out_buttons = mouse_buttons::none;

		out_buttons = out_buttons | (wParam & MK_LBUTTON ? mouse_buttons::left : mouse_buttons::none);
		out_buttons = out_buttons | (wParam & MK_RBUTTON ? mouse_buttons::right : mouse_buttons::none);
		out_buttons = out_buttons | (wParam & MK_MBUTTON ? mouse_buttons::middle : mouse_buttons::none);
	}

	inline window * get_window_userdata (HWND hWnd) {
		return reinterpret_cast < window  *> (GetWindowLongPtr (hWnd, GWLP_USERDATA));
	}

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

	void window::do_event_loop (
		function < bool () > update_callback,
		function < void () > render_callback
	) {

		while (update_callback ()) {

			MSG msg = {};
			while (PeekMessage (&msg, _implement->handle, 0, 0, PM_REMOVE) > 0) {
				::TranslateMessage (&msg);
				::DispatchMessage (&msg);
			}

			render_callback ();

			SwapBuffers (_implement->dc);
		}
	}

	LRESULT CALLBACK windows_message_callback (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
		case WM_CREATE:
		{
			CREATESTRUCT * create = reinterpret_cast <CREATESTRUCT *> (lParam);
			auto instance = reinterpret_cast <window *> (create->lpCreateParams);
			SetWindowLongPtr (hWnd, GWLP_USERDATA, (LONG_PTR)instance);
			return DefWindowProc (hWnd, msg, wParam, lParam);
		}
		case WM_DESTROY:
			PostQuitMessage (0);
			return 0;
		case WM_SIZE:
		{
			//frontend * instance = reinterpret_cast <frontend *> (GetWindowLongPtr (hWnd, GWLP_USERDATA));
			//instance->on_resize ();
			//return 0;
		}
		case WM_MOUSEMOVE:
		{
			mouse_buttons	mouse_buttons;
			point			mouse_position;

			get_mouse_info (hWnd, wParam, lParam, mouse_buttons, mouse_position);

			auto win = get_window_userdata (hWnd);
			auto args = mouse_move_event_args{ mouse_position };
			win->on_mouse_move.invoke (*win, args);

			return 0;
		}
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		{
			mouse_buttons	mouse_buttons;
			point			mouse_position;

			get_mouse_info (hWnd, wParam, lParam, mouse_buttons, mouse_position);

			auto win = get_window_userdata (hWnd);
			auto args = mouse_down_event_args { mouse_position, mouse_buttons };

			win->on_mouse_down.invoke (*win, args);

			return 0;
		}
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		{
			mouse_buttons	mouse_buttons;
			point			mouse_position;

			get_mouse_info (hWnd, wParam, lParam, mouse_buttons, mouse_position);

			auto win = get_window_userdata (hWnd);
			auto args = mouse_up_event_args{ mouse_position, mouse_buttons };

			win->on_mouse_up.invoke (*win, args);

			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			//send_mouse_message (mouse_event_wheel, hWnd, wParam, lParam);
			return 0;
		}
		case WM_CLOSE:
		{
			auto win = get_window_userdata (hWnd);
			win->on_window_close.invoke (*win);
		}
		default:
			return DefWindowProc (hWnd, msg, wParam, lParam);
		}
	}

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

		wglMakeCurrent (inst->_implement->dc, inst->_implement->glrc);

		return inst;
	}

}

#endif