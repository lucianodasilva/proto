#ifdef PROTO_OS_DARWIN

#include "proto.window_manager.h"
#include "proto.window.h"

#import <OpenGL/gl.h>

#import <Foundation/Foundation.h>
#import <Foundation/NSObject.h>
#import <Cocoa/Cocoa.h>

namespace proto {
	
	window_manager::window_manager (){}
	
	window_manager & window_manager::instance () {
		static window_manager manager;
		return manager;
	}
	
	void window_manager::main_loop () {
		[NSApplication sharedApplication];
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
		[NSApp activateIgnoringOtherApps:YES];
		[NSApp run];
	}
	
}

/*
@class NSNotification;

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

@implementation proto_opengl_view

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
	CVReturn result = [(__bridge proto_opengl_view*)displayLinkContext getFrameForTime:outputTime];
	return result;
}

- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
	// Add your drawing codes here
	[[self openGLContext] makeCurrentContext];
	
	if (!_game_instance->frame ())
		return kCVReturnLast;
	
	[[self openGLContext] flushBuffer];
	return kCVReturnSuccess;
}

- (void)dealloc
{
	// Release the display link
	CVDisplayLinkRelease(_displayLink);
	[super dealloc];
}

-(void)prepareOpenGL {
	// Synchronize buffer swaps with vertical refresh rate
	GLint swapInt = 1;
	[[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
	
	
	// Create a display link capable of being used with all active displays
	CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
 
	// Set the renderer output callback function
	CVDisplayLinkSetOutputCallback(_displayLink, &MyDisplayLinkCallback, (__bridge void *)(self));
	
	// Set the display link for the current renderer
	CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
	CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
	
	CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(_displayLink, cglContext, cglPixelFormat);
 
	// Activate the display link
	CVDisplayLinkStart(_displayLink);
}

-(id)initWithGame
	:(NSRect)frameRect
	:(proto::osx_desktop::frontend *)frontend_instance
{
	
	// Create pixel format
	NSOpenGLPixelFormat        *windowedPixelFormat;
	NSOpenGLPixelFormatAttribute    attribs[] = {
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 24,
		// Must specify the 3.2 Core Profile to use OpenGL 3.2
		NSOpenGLPFAOpenGLProfile,
		NSOpenGLProfileVersion3_2Core,
		0
	};
	
	windowedPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
	
	if ((self = [super initWithFrame:frameRect pixelFormat:windowedPixelFormat]))
	{
		[self setWantsBestResolutionOpenGLSurface:YES];
	}
	
	_game_instance = game_instance;
	_frontend_instance = frontend_instance;

	return self;
}

-(void)mouseDown:(NSEvent *) event {
	NSPoint mouse_position = [NSEvent mouseLocation];
	
	proto::point position = {
		(int32_t)mouse_position.x,
		(int32_t)mouse_position.y
	};
	
	NSUInteger ns_button = [NSEvent pressedMouseButtons];
	proto::mouse_buttons buttons =
		_frontend_instance->input ()->pressed_mouse_buttons ();
	
	switch (ns_button) {
		case (0):
			buttons = flags_add (buttons, proto::mouse_buttons::left);
			break;
		case (1):
			buttons = flags_add (buttons, proto::mouse_buttons::right);
			break;
		case (2):
			buttons = flags_add (buttons, proto::mouse_buttons::middle);
			break;
		default:
			buttons = flags_add (buttons, proto::mouse_buttons::none);
			break;
	}
	
	_frontend_instance->input ()->send_mouse_button_down (
		position,
		buttons
	);
}

-(void)mouseUp:(NSEvent *) event {
	NSPoint mouse_position = [NSEvent mouseLocation];
	
	proto::point position = {
		(int32_t)mouse_position.x,
		(int32_t)mouse_position.y
	};
	
	NSUInteger ns_button = [NSEvent pressedMouseButtons];
	proto::mouse_buttons buttons =
		_frontend_instance->input ()->pressed_mouse_buttons ();
	
	switch (ns_button) {
		case (0):
			buttons = flags_del (buttons, proto::mouse_buttons::left);
			break;
		case (1):
			buttons = flags_del (buttons, proto::mouse_buttons::right);
			break;
		case (2):
			buttons = flags_del (buttons, proto::mouse_buttons::middle);
			break;
		default:
			buttons = flags_del (buttons, proto::mouse_buttons::none);
			break;
	}
	
	_frontend_instance->input ()->send_mouse_button_up (
		position,
		buttons
	);
}

-(void)mouseMoved:(NSEvent *) event {
	NSPoint mouse_position = [NSEvent mouseLocation];
	
	proto::point position = {
		(int32_t)mouse_position.x,
		(int32_t)mouse_position.y
	};
	
	_frontend_instance->input ()->send_mouse_move (
		position
	);
}

-(BOOL)acceptsFirstResponder
{
	return YES;
}

-(void)viewDidMoveToWindow {
	[[self window] setAcceptsMouseMovedEvents:YES];
	[[self window] makeFirstResponder:self];
}

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

@implementation proto_window_controller

#pragma mark

-(id)initWithGame
	:(NSWindow*)window
	:(proto::game *) game_instance
	:(proto::osx_desktop::frontend *) frontend_instance
{
	if ((self = [super init])){
		_window = nil;
		_gl_view = nil;
		
		_window = window;
		if (_window == nil) {
			// FAIL
			NSLog (@"Window Creation failed loading.");
			return self;
		}
		
		_gl_view = [[proto_opengl_view alloc] initWithGame:[[_window contentView] frame] : game_instance : frontend_instance ];
		if (_gl_view == nil)
		{
			//FAIL
			NSLog (@"Ballistic OpenGL View failed loading.");
			return self;
		}
		
		[_window setContentView:_gl_view];
		
	}
	
	return self;
}

-(void)show_window {
	[[_gl_view openGLContext] makeCurrentContext];
	[_window makeKeyAndOrderFront:nil];
	//[_window setAcceptsMouseMovedEvents:YES];
}

-(void)close_window {
	[_window close];
	[_window setDelegate:nil];
}

@end

@implementation proto_app_controller

- (void)applicationWillFinishLaunching:(NSNotification *) notification
{
	_window = [[NSWindow alloc]
			  initWithContentRect : NSMakeRect (100, 100, 300, 300)
			  styleMask : (NSTitledWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask )
			  backing : NSBackingStoreBuffered
			  defer: YES
			  ];
	
	[_window setTitle : @"Ballistic"];
}

- (void)applicationDidFinishLaunching:(NSNotification *) notification
{
	[_window makeKeyAndOrderFront : self];
}

@end
 */

#endif