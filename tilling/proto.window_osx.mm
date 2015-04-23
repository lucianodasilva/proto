#ifdef PROTO_OS_DARWIN

#import "proto.window.h"
#import "proto.window_osx_native.h"
#import <AppKit/AppKit.h>

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


#endif