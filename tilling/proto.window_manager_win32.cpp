#ifdef PROTO_OS_WINDOWS

#include "proto.window_manager.h"
#include "proto.window.h"
#include "proto.debug.h"

#include <Windows.h>
#include <windowsx.h>

using namespace std;

namespace proto {

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
			auto args = mouse_down_event_args{ mouse_position, mouse_buttons };

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

	window_manager::window_manager () {}

	window_manager & window_manager::instance () {
		static window_manager manager;
		return manager;
	}

	void window_manager::main_loop () {
		window_manager & manager = instance ();

		while (manager._windows.size ()) {

			MSG msg = {};
			for (auto & w_ptr : manager._windows) {

				while (PeekMessage (&msg, (HWND)w_ptr->native_handle (), 0, 0, PM_REMOVE) > 0) {
					::TranslateMessage (&msg);
					::DispatchMessage (&msg);
				}

				w_ptr->on_window_update.invoke (*w_ptr.get ());

				if (w_ptr->is_visible ()) {
					w_ptr->make_active ();
					w_ptr->on_window_render.invoke (*w_ptr.get (), w_ptr->renderer ());

					SwapBuffers ((HDC)w_ptr->native_device ());
				}
			}

			{
				lock_guard < mutex > l (manager._accessory_mutex);

				for (auto & w_it : manager._windows) {
					if (w_it->is_closed ())
						manager._old.push_back (w_it.get ());
				}

				for (auto & w_it : manager._old) {
					auto it = std::find (
						manager._windows.begin (),
						manager._windows.end (),
						w_it
					);

					if (it != manager._windows.end ()) {
						manager._windows.erase (it);
					}
				}

				for (auto & w_it : manager._new) {
					manager._windows.push_back (w_it);
				}

				manager._new.clear ();
				manager._old.clear ();
			}

		}
	}

	void window_manager::add_window (const shared_ptr < window > & w) {
		lock_guard < mutex > lock (_accessory_mutex);
		_new.emplace_back (w);
	}

	void window_manager::remove_window (window * w) {
		lock_guard < mutex > lock (_accessory_mutex);
		_old.push_back (w);
	}

}

#endif