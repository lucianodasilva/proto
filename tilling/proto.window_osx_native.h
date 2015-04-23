//
//  osx_frontend_native.h
//  ballistic
//
//  Created by Luciano da Silva on 17/12/14.
//
//

#ifndef _proto_window_osx_native_h_
#define _proto_window_osx_native_h_

#ifdef PROTO_OS_DARWIN

#import <Foundation/Foundation.h>
#import <Foundation/NSObject.h>
#import <Cocoa/Cocoa.h>

namespace proto {
	class window_controller;

	namespace details {
		class window_osx_native;
	}
}

@class NSNotification;

@protocol proto_window_interface_protocol

-(void)show_window;
-(void)close_window;

@end

@interface proto_opengl_view : NSOpenGLView
{
	CVDisplayLinkRef
		_displayLink;
	
	proto::window_controller *
		_controller_instance;
	
	proto::details::window_osx_native *
		_window_instance;
}

-(id)initWithGame
	:(NSRect)frameRect
	:(proto::window_controller *) controller_instance
	:(proto::details::window_osx_native *)window_instance;

-(BOOL)acceptsFirstResponder;

-(void)viewDidMoveToWindow;

// mouse events
-(void)mouseDown:(NSEvent *) event;
-(void)mouseUp:(NSEvent *) event;
-(void)mouseMoved:(NSEvent *) event;

@end

@interface proto_window_controller : NSResponder < proto_window_interface_protocol, NSWindowDelegate >
{
	NSWindow *		_window;
	NSOpenGLView *	_gl_view;
}

-(id)initWithGame
	:(NSWindow*)window
	:(proto::window_controller *) controller_instance
	:(proto::details::window_osx_native *) window_instance;

@end

@interface proto_app_controller : NSObject
{
	NSWindow * _window;
}

- (void)applicationWillFinishLaunching:(NSNotification *) notification;
- (void)applicationDidFinishLaunching:(NSNotification *) notification;


@end

#endif
#endif