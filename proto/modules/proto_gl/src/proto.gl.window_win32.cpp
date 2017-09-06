
#ifdef PROTO_API_WIN32

#include "proto.gl.window.h"
#include "proto.gl.window_win32.h"
#include "proto.gl.debug.h"

#include "proto.gl.application_win32.h"

#include <memory>
#include <utility>

namespace proto {
	namespace gl {

		const LPCSTR	window_class_name = "PROTOWNDCLASS";
		const DWORD		window_style = WS_OVERLAPPEDWINDOW;

		struct native_event {
			window_win32 *	window;
			WPARAM			wparam;
			LPARAM			lparam;
		};

		inline window_win32 * get_window_userdata(HWND hwnd) {
			return reinterpret_cast <window_win32 *> (GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		inline mask < mouse_buttons > cast_mouse_buttons(native_event const & e) {
			mask < mouse_buttons > buttons;

			buttons |= (e.wparam & MK_LBUTTON ? mouse_buttons::left : mouse_buttons::none);
			buttons |= (e.wparam & MK_RBUTTON ? mouse_buttons::right : mouse_buttons::none);
			buttons |= (e.wparam & MK_MBUTTON ? mouse_buttons::middle : mouse_buttons::none);

			return buttons;
		}

		inline mouse_event_args get_mouse_event_args (native_event const & e) {
			point position = {
				GET_X_LPARAM(e.lparam),
				GET_Y_LPARAM(e.lparam)
			};

			mask < mouse_buttons > buttons = cast_mouse_buttons(e);
				
			return {
				position,
				buttons
			};
		}

		inline void invoke_mouse_event(
			mouse_event & event,
			native_event const & e
		) {
			event.invoke(
				*e.window, 
				get_mouse_event_args (e)
			);
		}

		LRESULT CALLBACK windows_message_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

			window_win32 * win = get_window_userdata(hwnd);

			native_event e{
				win,
				wparam,
				lparam
			};
				
			switch (msg) {
			case WM_CREATE:
			{
				CREATESTRUCT * create = reinterpret_cast <CREATESTRUCT *> (lparam);
				auto instance = reinterpret_cast < window *> (create->lpCreateParams);
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)instance);
				return DefWindowProc(hwnd, msg, wparam, lparam);
			}
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			case WM_SIZE:
			{
				//frontend * instance = reinterpret_cast <frontend *> (GetWindowLongPtr (hWnd, GWLP_USERDATA));
				//instance->on_resize ();
				//return 0;
			}
			case WM_MOUSEMOVE:
			{
				invoke_mouse_event (win->on_mouse_move, e);
				return 0;
			}
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			{
				invoke_mouse_event(win->on_mouse_down, e);
				return 0;
			}
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			{
				invoke_mouse_event(win->on_mouse_up, e);
				return 0;
			}
			case WM_MOUSEWHEEL:
			{
				//send_mouse_message (mouse_event_wheel, hWnd, wParam, lParam);
				return 0;
			}
			case WM_CLOSE:
			{
				auto win = get_window_userdata(hwnd);
				win->on_window_close.invoke(*win);
			}

			default:
				return DefWindowProc(hwnd, msg, wparam, lparam);
			}
		}

		expected < void > create_class() {
			auto hinst = ::GetModuleHandle(NULL);

			WNDCLASSEX wc;

			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = windows_message_callback;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hinst;
			wc.hIcon = LoadIcon(0, IDI_APPLICATION);
			wc.hCursor = LoadCursor(0, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wc.lpszMenuName = 0;
			wc.lpszClassName = window_class_name;
			wc.hIconSm = NULL;

			auto class_reg = ::RegisterClassEx(&wc);
			auto err_reg = ::GetLastError();

			if (class_reg || err_reg == ERROR_CLASS_ALREADY_EXISTS)
				return{};
			else
				return expected_failed < std::runtime_error > ("failed creating window class");
		}

		// window implementation

		HWND window_win32::handle() const { return _handle; }

		HDC window_win32::dc() const { return _dc; }

		HGLRC window_win32::glrc() const { return _glrc; }

		window_win32::~window_win32() {
			::wglDeleteContext(_glrc);
			::DeleteDC(_dc);
			::CloseWindow(_handle);
			::DestroyWindow(_handle);
		}

		void window_win32::handle_windows_events() {
			MSG msg = {};

			while (PeekMessage(&msg, _handle, 0, 0, PM_REMOVE) > 0) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		void window_win32::show()
		{
			scheduler_dispatch(
				_application->scheduler(),
				[this]() {
					::ShowWindow(this->_handle, SW_SHOW);
				}
			);
		}

		void window_win32::hide()
		{
			scheduler_dispatch(
				_application->scheduler(),
				[this] {
					::ShowWindow(this->_handle, SW_HIDE);
				}
			);
		}

		void window_win32::close()
		{
			//TODO: kill window???
		}

		bool window_win32::is_visible() const
		{
			auto f_vis = scheduler_dispatch(
				_application->scheduler(),
				[this] {
					return ::IsWindowVisible(this->_handle) == TRUE;
				}
			);

			return f_vis.get();
		}

		bool window_win32::is_closed() const
		{
			//TODO: how to check this?
			return false;
		}

		point window_win32::size() const
		{
			auto f_size = scheduler_dispatch(
				_application->scheduler(),
				[this] {
					RECT rct;
					GetWindowRect(this->_handle, &rct);
					return point{
						(int32_t)(rct.right - rct.left),
						(int32_t)(rct.bottom - rct.top)
					};
				}
			);

			return f_size.get();
		}

		void window_win32::size(const point & p)
		{
			scheduler_dispatch(
				_application->scheduler(),
				[this, p]() {
					::SetWindowPos(this->_handle, nullptr, 0, 0, p.x, p.y, SWP_NOREPOSITION);
				}
			);
		}

		expected < std::unique_ptr < window_win32 > > window_win32::create(
			application_base * application,
			std::string const & title, 
			point const & size_v
		){

			auto win = std::make_unique < window_win32 > ();

			win->_application = application;

			auto hinst = ::GetModuleHandle(NULL);

			HWND hwnd = ::CreateWindow(
				window_class_name,
				title.c_str(),
				window_style,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				size_v.x,
				size_v.y,
				0,
				0,
				hinst,
				win.get()
			);

			if (!hwnd)
				return expected_failed < std::runtime_error >("window creation failed");

			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),
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

			HDC dc = GetDC(hwnd);

			auto pixel_format = ChoosePixelFormat(dc, &pfd);

			if (!pixel_format)
				return expected_failed < std::runtime_error >("unable to create pixel format");

			if (SetPixelFormat(dc, pixel_format, &pfd) == FALSE)
				return expected_failed < std::runtime_error >("failed setting pixel format");

			HGLRC glrc = wglCreateContext(dc);

			if (!glrc)
				return expected_failed < std::runtime_error >("failed creating wgl context");

			win->_handle = hwnd;
			win->_dc = dc;
			win->_glrc = glrc;

			return std::move(win);
		}

	}

}

#endif