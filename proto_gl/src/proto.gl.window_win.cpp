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

		// windows native specific helper functions
		expected < void > create_native_class();

		expected < void > create_native_window(
			const string & title,
			const point & size_v,
			window * winst,
			HWND & handle,
			HDC & dc,
			HGLRC & glrc
		);

		void handle_native_events(HWND handle);

		using window_task = function < void() >;
		// ---------------------------------------

		class window::native_proxy {
		public:

			HWND			handle;
			HDC				dc;
			HGLRC			glrc;

			thread			running_thread;
			atomic < bool >	running;

			queue < window_task >
							task_queue;
			spin_mutex		task_queue_mutex;

			native_proxy () :
				handle(nullptr),
				dc(nullptr),
				glrc(nullptr),
				running (false)
			{}

			~native_proxy() {
				if (handle) {
					running = false;
					running_thread.join();
				}
			}

			inline void enqueue_task(window_task && task) {
				unique_lock < spin_mutex > lock(task_queue_mutex);
				task_queue.emplace(task);
			}

			static void thread_run(window * w, const string & title, const point & size_v) {

				unique_lock < spin_mutex > starting_lock(w->_native->task_queue_mutex);

				auto native = w->_native.get();
				atomic < bool > & r_running = native->running;
				r_running = true;

				queue < window_task > local_task_queue;

				if (!create_native_class())
					return;

				if (!create_native_window(
						title, 
						size_v,
						w, 
						native->handle, 
						native->dc, 
						native->glrc
				))
					return;

				wglMakeCurrent(native->dc, native->glrc);

				// release initialization lock
				starting_lock.unlock();

				w->on_window_load.sync_invoke(*w);

				while (r_running) {

					handle_native_events(native->handle);

					// switch task queues
					{
						unique_lock < spin_mutex > lock(native->task_queue_mutex);
						swap(native->task_queue, local_task_queue);
					}

					while (!local_task_queue.empty()) {
						local_task_queue.front()();
						local_task_queue.pop();
					}

					w->on_window_render.sync_invoke(*w);
					SwapBuffers(native->dc);
				}

				::wglDeleteContext(native->glrc);
				::DeleteDC(native->dc);
				::CloseWindow(native->handle);
				::DestroyWindow(native->handle);;
			}

			inline void create_and_run(window * w, const string & title, const point & size_v) {
				running_thread = thread(native_proxy::thread_run, w, title, size_v);
			}

		};

		window::window() : _native (make_unique < native_proxy > ())
		{}

		window::~window() {}

		void window::show() {
			if (_native->running && !is_visible()) {
				_native->enqueue_task([this] {
					::ShowWindow(_native->handle, SW_SHOW);
				});
			}
		}

		void window::hide() {
			if (_native->running) {
				_native->enqueue_task([this] {
					::ShowWindow(_native->handle, SW_HIDE);
				});
			}
		}

		void window::close() {
			if (_native->running) {
				_native->running = false;

				if (_native->running_thread.get_id() != this_thread::get_id())
					_native->running_thread.join();
			}
		}

		bool window::is_visible() const {
			return _native->running && ::IsWindowVisible(_native->handle) == TRUE;
		}

		bool window::is_closed() const {
			return !_native->running;
		}

		point window::size() const {
			if (_native->running) {
				RECT rct;
				GetWindowRect(_native->handle, &rct);
				return{ (int32_t)(rct.right - rct.left), (int32_t)(rct.bottom - rct.top) };
			}

			return{ 0,0 };
		}

		void window::size(const point & p) {
			if (_native->running) {
				_native->enqueue_task([this, p] {
					::SetWindowPos(_native->handle, nullptr, 0, 0, p.x, p.y, SWP_NOREPOSITION);
				});
			}
		}

		std::shared_ptr < window > window::create(const string & title, const point & size_v) {

			auto inst = shared_ptr < window >(new window());

			// respond to operating system close
			inst->on_window_close += [](window & sender) {
				sender.close();
			};

			inst->_native->create_and_run(inst.get(), title, size_v);

			return inst;
		}

	}
}

#endif