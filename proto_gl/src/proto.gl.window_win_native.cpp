#ifdef PROTO_API_WIN32

#include <proto.h>

#include "proto.gl.window.h"
#include "proto.gl.debug.h"
#include "proto.gl.deps.h"

#include <memory>
#include <utility>
#include <thread>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <windowsx.h>

using namespace std;

namespace proto {

	namespace gl {

		const LPCSTR	window_class_name = "PROTOWNDCLASS";
		const DWORD		window_style = WS_OVERLAPPEDWINDOW;

		inline void get_mouse_info(
			HWND hWnd,
			WPARAM wParam,
			LPARAM lParam,
			mouse_buttons & out_buttons,
			point & out_position
			) {

			out_position.x = GET_X_LPARAM(lParam);
			out_position.y = GET_Y_LPARAM(lParam);

			out_buttons = mouse_buttons::none;

			out_buttons = out_buttons | (wParam & MK_LBUTTON ? mouse_buttons::left : mouse_buttons::none);
			out_buttons = out_buttons | (wParam & MK_RBUTTON ? mouse_buttons::right : mouse_buttons::none);
			out_buttons = out_buttons | (wParam & MK_MBUTTON ? mouse_buttons::middle : mouse_buttons::none);
		}

		inline window * get_window_userdata(HWND hWnd) {
			return reinterpret_cast < window  *> (GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		template < class _event_t >
		inline void call_mouse_event(
			const _event_t & event_address,
			HWND hWnd,
			WPARAM wParam,
			LPARAM lParam
			) {
			point			mouse_position;
			mouse_buttons	mouse_buttons;

			get_mouse_info(hWnd, wParam, lParam, mouse_buttons, mouse_position);

			auto win = get_window_userdata(hWnd);
			auto args = mouse_event_args{ mouse_position, mouse_buttons };

			(win->*event_address).sync_invoke(*win, args);
		}

		LRESULT CALLBACK windows_message_callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg) {
			case WM_CREATE:
			{
				CREATESTRUCT * create = reinterpret_cast <CREATESTRUCT *> (lParam);
				auto instance = reinterpret_cast < window *> (create->lpCreateParams);
				SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)instance);
				return DefWindowProc(hWnd, msg, wParam, lParam);
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
				call_mouse_event(&window::on_mouse_move, hWnd, wParam, lParam);
				return 0;
			}
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			{
				call_mouse_event(&window::on_mouse_down, hWnd, wParam, lParam);
				return 0;
			}
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			{
				call_mouse_event(&window::on_mouse_up, hWnd, wParam, lParam);
				return 0;
			}
			case WM_MOUSEWHEEL:
			{
				//send_mouse_message (mouse_event_wheel, hWnd, wParam, lParam);
				return 0;
			}
			case WM_CLOSE:
			{
				auto win = get_window_userdata(hWnd);
				win->on_window_close.sync_invoke(*win);
			}
			default:
				return DefWindowProc(hWnd, msg, wParam, lParam);
			}
		}

		expected < void > create_native_class () {

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
				return expected_failed < runtime_error >("failed creating window class");

		}

		expected < void > create_native_window(
			const string & title,
			const point & size_v,
			window * winst,
			HWND & handle,
			HDC & dc,
			HGLRC & glrc
		) {
			auto hinst = ::GetModuleHandle(NULL);

			handle = ::CreateWindow(
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
				winst
			);

			if (!handle)
				return expected_failed < runtime_error > ("window creation failed");

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

			dc = GetDC(handle);
			auto pixel_format = ChoosePixelFormat(dc, &pfd);

			if (!pixel_format)
				return expected_failed < runtime_error >("unable to create pixel format");

			if (SetPixelFormat(dc, pixel_format, &pfd) == FALSE)
				return expected_failed < runtime_error >("failed setting pixel format");

			glrc = wglCreateContext(dc);

			if (!glrc)
				return expected_failed < runtime_error >("failed creating wgl context");

			return {};
		}

		void handle_native_events(HWND handle) {
			MSG msg = {};
			while (PeekMessage(&msg, handle, 0, 0, PM_REMOVE) > 0) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

	}
}

#endif