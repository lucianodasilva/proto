#ifdef PROTO_OS_DARWIN

#include "ballistic.input_listener.h"
#include "ballistic.osx_frontend.h"
#import "ballistic.osx_frontend_native.h"
#import <OpenGL/gl.h>

@implementation ballistic_opengl_view

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
	CVReturn result = [(__bridge ballistic_opengl_view*)displayLinkContext getFrameForTime:outputTime];
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
	:(ballistic::game *) game_instance
	:(ballistic::osx_desktop::frontend *)frontend_instance
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
	
	ballistic::point position = {
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
}

-(void)mouseUp:(NSEvent *) event {
	NSPoint mouse_position = [NSEvent mouseLocation];
	
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
}

-(void)mouseMoved:(NSEvent *) event {
	NSPoint mouse_position = [NSEvent mouseLocation];
	
	ballistic::point position = {
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

@implementation ballistic_window_controller

#pragma mark

-(id)initWithGame
	:(NSWindow*)window
	:(ballistic::game *) game_instance
	:(ballistic::osx_desktop::frontend *) frontend_instance
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
		
		_gl_view = [[ballistic_opengl_view alloc] initWithGame:[[_window contentView] frame] : game_instance : frontend_instance ];
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

@implementation ballistic_app_controller

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

#endif