#ifdef PROTO_OS_DARWIN

#import "proto.window.h"
#import <AppKit/AppKit.h>

#include <memory>

namespace proto {
	
	class window_imp {
	public:
		
		NSWindow *	instance;
		
	};
	
	window::window () {}
	
	void window::show () const {
		
	}
	
	void window::hide () const {
	}
	
	void window::close () {}
	
	bool window::is_visible () const {
		return [_implement->instance isVisible];
	}
	
	bool window::is_closed () const {
		return !_implement;
	}
	
	point window::size () const {
		if (!_implement)
			return { 0, 0 };
		
		NSRect frame = [_implement->instance frame];
		return { (int)frame.size.width, (int)frame.size.height };
	}
	
	void window::size (const point & p) {
		if (!_implement)
			return;
		
		NSRect frame = [_implement->instance frame];
		
		frame.size.width = p.x;
		frame.size.height = p.y;
		
		[_implement->instance setFrame : frame
			display : YES
			animate : NO
		 ];
	}
	
	void * window::native_handle () const {
		if (_implement)
			return (void *)_implement->instance;
		
		return nullptr;
	}
	
	void * window::native_device () const {
		return nullptr;
	}
	
	void window::make_active () {}
	
	shared_ptr < window > window::create ( const char * title, const point & size_v) {
		return nullptr;
	}
	
}

/*
namespace proto {
	namespace details {
		
		struct window_internals {
			NSWindow *						window_instance;
			window_controller *				window_controller;
			input_listener *				input;
		};
		
		void frontend::input (input_listener * input_instance) {
			_internals->input = input_instance;
		}
		
		input_listener * frontend::input () const {
			return _internals->input;
		}
		
		frontend::frontend ( ) :
			_internals (new frontend_internals ())
		{
			_internals->window_instance = nil;
			_internals->window_controller = nil;
			_internals->input = new input_listener ();
			_internals->game_instance = nullptr;
		}
		
		frontend::~frontend () {
			delete _internals;
		}
		
		bool frontend::initialize(ballistic::game * game_instance, const point & window_size ) {
			
			_internals->game_instance = game_instance;
			
			_internals->window_instance = [[NSWindow alloc]
							initWithContentRect: NSMakeRect ( 10, 10, window_size.x, window_size.y )
							styleMask : (NSTitledWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask )
							backing:NSBackingStoreBuffered
							defer:NO
						 ];
			
			[_internals->window_instance cascadeTopLeftFromPoint:NSMakePoint (20, 20)];
			[_internals->window_instance setTitle:@""];
			[_internals->window_instance makeKeyAndOrderFront:nil];
			
			if (_internals->window_instance == nil)
				return false;
			
			_internals->window_controller = [[ballistic_window_controller alloc] initWithGame : _internals->window_instance : _internals->game_instance : this];
			
			if (_internals->window_controller == nil)
				return false;
			
			[_internals->window_controller show_window];
			
			return true;
		}
		
		void frontend::terminate () {
			[_internals->window_instance dealloc];
			_internals->window_instance = nil;
			[_internals->window_controller close_window];
		}
		
		void frontend::do_event_loop() {
			[NSApplication sharedApplication];
			[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
			[NSApp activateIgnoringOtherApps:YES];
			[NSApp run];
		}
		
	}
}
*/

#endif