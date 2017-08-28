
#include "proto.gl.window.h"
#include "proto.gl.window_manager.h"
#include "proto.gl.debug.h"

#include <memory>
#include <utility>

using namespace std;

namespace proto {
	namespace gl {

		class window_imp {
		public:

			//SDL_Window *	window;
			//SDL_Renderer *	sdl_renderer;
			//SDL_GLContext	gl_context;
			//int32_t			window_id;
			//
			//window_imp () :
			//	window (nullptr),
			//	sdl_renderer(nullptr),
			//	window_id (0)
			//{}
			//
			//~window_imp () {
			//	if (window) {
			//		SDL_DestroyWindow(window);
			//	}
			//}

		};

		void * window::context() const {
			//if (_implement)
			//	return _implement->gl_context;
			//else
			return nullptr;
		}

		void window::swap_context() const {
			//if (_implement)
				//SDL_GL_SwapWindow(_implement->window);
		}

		inline mouse_event_args convert_sdl_mouse_state() {
			//int x, y;
			//auto button_mask = SDL_GetMouseState(&x, &y);
			//
			//auto buttons = mouse_buttons::none;
			//
			//if (button_mask & SDL_BUTTON(SDL_BUTTON_LEFT))
			//	buttons = buttons | mouse_buttons::left;
			//
			//if (button_mask & SDL_BUTTON(SDL_BUTTON_RIGHT))
			//	buttons = buttons | mouse_buttons::right;
			//
			//if (button_mask & SDL_BUTTON(SDL_BUTTON_MIDDLE))
			//	buttons = buttons | mouse_buttons::middle;
			//
			//return{
			//	{ x, y},
			//	buttons
			//};
			return{
				{ 0, 0 },
				mouse_buttons::none
			};
		}

		void window::handle_event(window::external_event e) {
			//auto sdl_e = static_cast <SDL_Event *> (e);
			//
			//// check if this event is for us
			//if (
			//	!_implement ||
			//	sdl_e->window.windowID != _implement->window_id
			//	)
			//	return;
			//
			//switch (sdl_e->type) {
			//case SDL_WINDOWEVENT:
			//	switch (sdl_e->window.event) {
			//	case SDL_WINDOWEVENT_SHOWN:
			//		on_window_show.invoke(*this);
			//		break;
			//	case SDL_WINDOWEVENT_HIDDEN:
			//		on_window_hide.invoke(*this);
			//		break;
			//	case SDL_WINDOWEVENT_SIZE_CHANGED:
			//		{
			//			auto p = point{ sdl_e->window.data1, sdl_e->window.data2 };
			//			on_window_resize.invoke(*this, p);
			//			break;
			//		}
			//		//	//Repaint on expose
			//		//case SDL_WINDOWEVENT_EXPOSED:
			//		//	SDL_RenderPresent(mRenderer);
			//		//	break;
			//
			//		//	//Mouse enter
			//		//case SDL_WINDOWEVENT_ENTER:
			//		//	mMouseFocus = true;
			//		//	updateCaption = true;
			//		//	break;
			//
			//		//	//Mouse exit
			//		//case SDL_WINDOWEVENT_LEAVE:
			//		//	mMouseFocus = false;
			//		//	updateCaption = true;
			//		//	break;
			//
			//		//	//Keyboard focus gained
			//		//case SDL_WINDOWEVENT_FOCUS_GAINED:
			//		//	mKeyboardFocus = true;
			//		//	updateCaption = true;
			//		//	break;
			//
			//		//	//Keyboard focus lost
			//		//case SDL_WINDOWEVENT_FOCUS_LOST:
			//		//	mKeyboardFocus = false;
			//		//	updateCaption = true;
			//		//	break;
			//
			//		//	//Window minimized
			//		//case SDL_WINDOWEVENT_MINIMIZED:
			//		//	mMinimized = true;
			//		//	break;
			//
			//		//	//Window maxized
			//		//case SDL_WINDOWEVENT_MAXIMIZED:
			//		//	mMinimized = false;
			//		//	break;
			//
			//		//	//Window restored
			//		//case SDL_WINDOWEVENT_RESTORED:
			//		//	mMinimized = false;
			//		//	break;
			//
			//		//Hide on close
			//	case SDL_WINDOWEVENT_CLOSE:
			//		on_window_close.sync_invoke(*this);
			//		close();
			//		break;
			//	}
			//	break;
			//case SDL_MOUSEBUTTONDOWN:
			//{
			//	auto args = convert_sdl_mouse_state();
			//	on_mouse_down.invoke(*this, args);
			//	break;
			//}
			//case SDL_MOUSEBUTTONUP:
			//{
			//	auto args = convert_sdl_mouse_state();
			//	on_mouse_up.invoke(*this, args);
			//	break;
			//}
			//case SDL_MOUSEMOTION:
			//{
			//	auto args = convert_sdl_mouse_state();
			//	on_mouse_move.invoke(*this, args);
			//	break;
			//}
			//}
		}

		window::window() : _implement(unique_ptr < window_imp >(new window_imp()))
		{}

		window::~window() {}

		void window::show() {
			//if (_implement && !is_visible()) {
			//	SDL_ShowWindow(_implement->window);
			//	on_window_show.invoke(*this);
			//}
		}

		void window::hide() {
			//if (_implement)
			//	SDL_HideWindow(_implement->window);
		}

		void window::close() {
			//if (_implement) {
			//	_implement.reset ();
			//}
		}

		bool window::is_visible() const {
			//if (_implement)
			//	return (SDL_GetWindowFlags (_implement->window) & SDL_WINDOW_SHOWN) == SDL_WINDOW_SHOWN;
			//
			return false;
		}

		bool window::is_closed() const {
			//return !_implement;
			return false;
		}

		point window::size() const {
			//if (_implement) {
			//	point p;
			//	SDL_GetWindowSize(_implement->window, &p.x, &p.y);
			//	return p;
			//}

			return{ 0,0 };
		}

		void window::size(const point & p) {
			//if (_implement)
			//	SDL_SetWindowSize(_implement->window, p.x, p.y);
		}

		void window::make_current() const {
			//if (_implement)
			//	SDL_GL_MakeCurrent(_implement->window, _implement->gl_context);
		}

		//std::shared_ptr < window > window::create_window_instance(const char * title, const point & size_v) {

			//auto inst = make_shared < window >();
			//
			//inst->_implement->window = SDL_CreateWindow(
			//	title, 
			//	SDL_WINDOWPOS_UNDEFINED, 
			//	SDL_WINDOWPOS_UNDEFINED, 
			//	size_v.x, 
			//	size_v.y, 
			//	SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE	| SDL_WINDOW_HIDDEN
			//);
			//
			//if (!inst->_implement->window) {
			//	debug_print << "failed to create window instance";
			//	return nullptr;
			//}
			//
			//inst->_implement->sdl_renderer = SDL_CreateRenderer(
			//	inst->_implement->window,
			//	-1,
			//	SDL_WINDOW_OPENGL |
			//	SDL_RENDERER_ACCELERATED
			//);
			//
			//if (!inst->_implement->sdl_renderer) {
			//	debug_print << "failed to create sdl renderer instance";
			//	return nullptr;
			//}
			//
			//inst->_implement->gl_context = SDL_GL_CreateContext(inst->_implement->window);
			//
			//if (!inst->_implement->gl_context) {
			//	debug_print << "failed to create window gl context instance";
			//	return nullptr;
			//}
			//
			//inst->_implement->window_id = SDL_GetWindowID(inst->_implement->window);
			//
			//return inst;
			//return nullptr;
		//}

		// glfw event handling
		//void char_callback(GLFWwindow* w, unsigned int char_v) {
			
		//}

		//std::shared_ptr < window > window::create(const string & title, const point & size_v) {
			//auto & w_manager = window_manager::instance(); // force initialization of the singleton
			//
			//auto inst = create_window_instance(title, size_v);
			//w_manager.register_window(inst);
			//
			//return inst;

			//auto inst = new window();
			//auto glfw_window = glfwCreateWindow(size_v.x, size_v.y, title.c_str(), nullptr, nullptr);

			//glfwSetCharCallback(glfw_window, [inst](GLFWwindow * w, unsigned int c) {
			//	inst->on_key_down.invoke(*inst, key_event_args{});
			//});



			//glfwSetCharModsCallback
			//glfwSetCursorEnterCallback
			//glfwSetCursorPosCallback
			//glfwSetDropCallback
			//glfwSetFramebufferSizeCallback
			//glfwSetKeyCallback
			//glfwSetMouseButtonCallback
			//glfwSetScrollCallback
			//glfwSetWindowCloseCallback
			//glfwSetWindowFocusCallback
			//glfwSetWindowPosCallback
			//glfwSetWindowRefreshCallback
			//glfwSetWindowSizeCallback

			//auto inst = new window();
			
			// register glfwevents
			
			//return nullptr;
		//}
	}
}
