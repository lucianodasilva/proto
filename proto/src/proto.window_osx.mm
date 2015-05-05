#ifdef PROTO_OS_DARWIN

#import "proto.window.h"
#import <AppKit/AppKit.h>

#include <memory>

@protocol proto_window_interface_protocol

-(void)show_window;
-(void)hide_window;
-(void)close_window;

@end

@interface proto_opengl_view : NSOpenGLView
{
	CVDisplayLinkRef					_displayLink;
	std::shared_ptr < proto::window >	_proto_window;
}

-(id)initProto
:(NSRect)frameRect
:(std::shared_ptr < proto::window >) proto_window;

-(BOOL)acceptsFirstResponder;

-(void)viewDidMoveToWindow;

// mouse events
-(void)mouseDown:(NSEvent *) event;
-(void)mouseUp:(NSEvent *) event;
-(void)mouseMoved:(NSEvent *) event;

@end

@interface proto_window_controller : NSResponder < proto_window_interface_protocol, NSWindowDelegate >
{
	NSWindow *							_nswindow;
	NSOpenGLView *						_gl_view;
	std::shared_ptr < proto::window >	_proto_window;
}

-(id)									initProto
:(NSWindow*)							nswindow
:(std::shared_ptr < proto::window >)	proto_window;

-(NSOpenGLView *)						getGLView;

@end

namespace proto {
	
	class window_imp {
	public:
		
		NSWindow *					instance;
		proto_window_controller *	controller;
		
	};
	
	window::window () : _implement (new window_imp ()) {}
	
	window::~window () {}
	
	void window::show () {
		if (_implement)
			[_implement->controller show_window];
	}
	
	void window::hide () {
		if (_implement)
			[_implement->controller hide_window];
	}
	
	void window::close () {
		if (_implement) {
			[_implement->controller close_window];
			_implement.release ();
		}
	}
	
	bool window::is_visible () const {
		return !is_closed () && [_implement->instance isVisible];
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
	
	void window::make_active () {
		if (_implement)
			[[[_implement->controller getGLView] openGLContext] makeCurrentContext];
	}
	
	shared_ptr < window > window::create ( const char * title, const point & size_v) {
		
		auto w = make_shared < window > ();
		
		auto native_w = [[NSWindow alloc]
			initWithContentRect: NSMakeRect ( 0, 0, size_v.x, size_v.y )
			styleMask: ( NSTitledWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask )
			backing: NSBackingStoreBuffered
			defer: NO
		 ];
		
		w->_implement->instance = native_w;
		
		[native_w cascadeTopLeftFromPoint:NSMakePoint (20, 20)];
		[native_w setTitle:[NSString stringWithCString:title encoding:NSASCIIStringEncoding]];
		[native_w makeKeyAndOrderFront:nil];
		
		auto native_c = [[proto_window_controller alloc] initProto
			: native_w
			: w
		];
		
		w->_implement->controller = native_c;
		
		return w;
	}
	
}

@implementation proto_opengl_view

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
	CVReturn result = [(__bridge proto_opengl_view*)displayLinkContext getFrameForTime:outputTime];
	return result;
}

- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
	// update logic
	_proto_window->on_window_update.invoke (*_proto_window.get ());
	
	// update graphics
	_proto_window->make_active();
	_proto_window->on_window_render.invoke (*_proto_window.get (), _proto_window->renderer());
	
	[[self openGLContext] makeCurrentContext];
	
	[[self openGLContext] flushBuffer];
	return _proto_window->is_closed () ? kCVReturnLast : kCVReturnSuccess;
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

-(id)initProto
:(NSRect)frameRect
:(std::shared_ptr < proto::window >) proto_window
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
	
	_proto_window = proto_window;
	
	return self;
}

-(void)mouseDown:(NSEvent *) event {
	NSPoint mouse_position = [NSEvent mouseLocation];
	
	// EVENTS HERE
	
	/*
	proto::point position = {
		(int32_t)mouse_position.x,
		(int32_t)mouse_position.y
	};
	
	NSUInteger ns_button = [NSEvent pressedMouseButtons];
	ballistic::mouse_buttons buttons =
	_frontend_instance->input ()->pressed_mouse_buttons ();
	
	switch (ns_button) {
		case (0):
			buttons = flags_add (buttons, ballistic::mouse_buttons::left);
			break;
		case (1):
			buttons = flags_add (buttons, ballistic::mouse_buttons::right);
			break;
		case (2):
			buttons = flags_add (buttons, ballistic::mouse_buttons::middle);
			break;
		default:
			buttons = flags_add (buttons, ballistic::mouse_buttons::none);
			break;
	}
	
	_frontend_instance->input ()->send_mouse_button_down (
														  position,
														  buttons
														  );
	 */
}

-(void)mouseUp:(NSEvent *) event {
	NSPoint mouse_position = [NSEvent mouseLocation];
	
	/*
	ballistic::point position = {
		(int32_t)mouse_position.x,
		(int32_t)mouse_position.y
	};
	
	NSUInteger ns_button = [NSEvent pressedMouseButtons];
	ballistic::mouse_buttons buttons =
	_frontend_instance->input ()->pressed_mouse_buttons ();
	
	switch (ns_button) {
		case (0):
			buttons = flags_del (buttons, ballistic::mouse_buttons::left);
			break;
		case (1):
			buttons = flags_del (buttons, ballistic::mouse_buttons::right);
			break;
		case (2):
			buttons = flags_del (buttons, ballistic::mouse_buttons::middle);
			break;
		default:
			buttons = flags_del (buttons, ballistic::mouse_buttons::none);
			break;
	}
	
	_frontend_instance->input ()->send_mouse_button_up (
														position,
														buttons
														);
	 */
}

-(void)mouseMoved:(NSEvent *) event {
	NSPoint mouse_position = [NSEvent mouseLocation];
	
	/*
	ballistic::point position = {
		(int32_t)mouse_position.x,
		(int32_t)mouse_position.y
	};
	
	_frontend_instance->input ()->send_mouse_move (
												   position
												   );
	 */
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


@implementation proto_window_controller

#pragma mark

-(id)initProto
:(NSWindow*)							nswindow
:(std::shared_ptr < proto::window >)	proto_window
{
	if ((self = [super init])){
		_proto_window = nil;
		_gl_view = nil;
		
		_proto_window = proto_window;
		
		_gl_view = [[proto_opengl_view alloc] initProto:[[nswindow contentView] frame] : proto_window ];
		if (_gl_view == nil)
		{
			//FAIL
			NSLog (@"proto OpenGL View failed loading.");
			return self;
		}
		
		[_nswindow setContentView:_gl_view];
		
	}
	
	return self;
}

-(NSOpenGLView *) getGLView {
	return _gl_view;
}

-(void)show_window {
	[[_gl_view openGLContext] makeCurrentContext];
	[_nswindow makeKeyAndOrderFront: self];
	//[_window setAcceptsMouseMovedEvents:YES];
}

-(void)hide_window {
	[_nswindow orderOut:nil];
}

-(void)close_window {
	[_nswindow close];
	[_nswindow setDelegate:nil];
}

@end

#endif