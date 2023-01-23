// Header Files
#import <Foundation/Foundation.h>
#import <cocoa/cocoa.h>

// Global Variable Declarations
FILE *gpFile = NULL;

// Interface Declarations
@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@interface MyView: NSView
@end

// Entry Point Functions
int main(int argc, char* argv[])
{
	// Code
	// Create AutoreleasePool For Refrence Counting
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

	// Create Application Object For This Apploication
	NSApp = [NSApplication sharedApplication];

	// Create AppDelegate Object
	AppDelegate *appDelegate = [[AppDelegate alloc]init];

	// Give Our AppDelegate To NSApp
	[NSApp setDelegate:appDelegate];

	// Start Game Loop
	[NSApp run];

	// Tell AutoreleasePool To Release All Objects Created By This Application
	[pool release];

	return(0);
}

// Implementation Of AppDelegate
@implementation AppDelegate
{
	@private
	NSWindow *window;
	MyView *view;
}

- (void) applicationDidFinishLaunching:(NSNotification *)notification
{
	// Code
	NSBundle *appBundle = [NSBundle mainBundle];

	NSString *appDirPath = [appBundle bundlePath];

	NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];

	NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/log.txt",parentDirPath];

	const char* pszLogFileNameWithPath = [logFileNameWithPath UTF8String];

	gpFile = fopen(pszLogFileNameWithPath,"w");
	if(gpFile == NULL)
	{
		[self release];
		[NSApp terminate:self];
	}

	fprintf(gpFile,"Program Is Started Successfully \n");

	NSRect rect = NSMakeRect(0.0,0.0,800.0,600.0);

	window = [[NSWindow alloc]initWithContentRect:rect 
							  styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
							  backing:NSBackingStoreBuffered
							  defer:NO];  

	[window setTitle:@"macOS Window : PRJ"];
	NSColor *backgroundColor = [NSColor blackColor];
	[window setBackgroundColor:backgroundColor];

	view = [[MyView alloc]initWithFrame:rect];
	[window setContentView:view];

	[window center];

	// Set Windows Delegate To This Object
	[window setDelegate:self];
	[window makeKeyAndOrderFront:self];
}

- (void) applicationWillTerminate:(NSNotification *)notification
{
	// Code

	if(gpFile)
	{
		fprintf(gpFile,"Program Is Finished Successfully \n");
		fclose(gpFile);
		gpFile = NULL;
	}

}

- (void) windowWillClose:(NSNotification *)notification
{
	// Code
	[NSApp terminate:self];
}

- (void) dealloc
{
	// Code
	if(view)
	{
		[view release];
		view = nil;
	}

	if(window)
	{
		[window release];
		window = nil;
	}

	[super dealloc];
}

@end

// Implement MyView
@implementation MyView
{
	@private 
	NSString *string;
}

- (id) initWithFrame:(NSRect)frame
{
	// Code
	self = [super initWithFrame:frame];

	if(self)
	{
		string = @"Hello World !!!";
	}
	return(self);
}

- (void) drawRect:(NSRect)dirtyRect
{
	// Code
	NSFont *font = [NSFont fontWithName:@"Helvetica" size:32];

	NSColor *textColor = [NSColor greenColor];

	NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:font, NSFontAttributeName,
																		  textColor, NSForegroundColorAttributeName,
																		  nil];

	NSSize textSize = [string sizeWithAttributes:dictionary];

	NSPoint point;
	point.x = dirtyRect.size.width/2 - textSize.width/2;
	point.y = dirtyRect.size.height/2 - textSize.height/2;

	[string drawAtPoint:point withAttributes:dictionary];
}

- (BOOL) acceptsFirstResponder
{
	// Code
	[[self window]makeFirstResponder:self];
	return(YES);
}

- (void) keyDown:(NSEvent *)event
{
	// Code
	int key = (int)[[event characters]characterAtIndex:0];

	switch(key)
	{
		case 27:
			[self release];
			[NSApp terminate:self];
		break;

		case 'F':
		case 'f':
			string = @"F Key Is Pressed";
			[[self window]toggleFullScreen:self];
		break;

		default:
		break;
	}
}

- (void) mouseDown:(NSEvent *)event
{
	// Code
	string = @"Mouse Is Clicked";
	[self setNeedsDisplay:YES];   // Repaint
}

- (void) dealloc
{
	// Code
	[super dealloc];
}

@end

