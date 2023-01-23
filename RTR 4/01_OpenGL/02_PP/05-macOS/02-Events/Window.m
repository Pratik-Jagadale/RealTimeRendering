// Header Files
#import <foundation/foundation.h> 
#import <Cocoa/Cocoa.h>

// Interface Declartions
@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@interface MyView : NSView
@end

// ENTRY POINT FUNCTION
int main(int argc, char* argv[])
{
    // CODE
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    NSApp = [NSApplication sharedApplication];

    // CREATE APP DELEGATE OBJECT
    AppDelegate * appDelegate = [[AppDelegate alloc] init];

    // GIVE OUR APPDELIGATE TO NSAPP
    [NSApp setDelegate: appDelegate];

    // RUN LOOP (START GAME LOOP)
    [NSApp run];

    // TELL AUTO RELEASE TO ALL OBJECTS CREATED BY THIS APPLICATION
    [pool release];

    return 0;
}

// IMPLIMENTATION OF APP DELIGATE
@implementation AppDelegate
{
    @private
        NSWindow* window;   
        MyView* myView;
}

-(void)applicationDidFinishLaunching: (NSNotification *)notification // LIKE WM_CREATE
{
    // CODE
    NSRect rect = NSMakeRect(0.0,0.0,800.0,600.0);

    window =[[NSWindow alloc]  initWithContentRect:rect
                            styleMask:NSWindowStyleMaskTitled|NSWindowStyleMaskClosable|NSWindowStyleMaskMiniaturizable|NSWindowStyleMaskResizable
                            backing:NSBackingStoreBuffered
                            defer:NO];

    // #Set Title to Window #
    [window setTitle: @"macOS Window : PRJ"];

    NSColor* backgroundColor = [NSColor blackColor];

    [window setBackgroundColor:backgroundColor];

    [window center];

    view =[[MyView alloc] initWithFrame]; 

    [window setContaintView:view]

    // SET WINDOWS DELIGATE TO THIS OBJECT
    [window setDelegate:self];

    [window makeKeyAndOrderFront:self];
}

-(void)applicationWillTerminate: (NSNotification*)notification // WM_DETSROY
{
    // CODE
}

-(void)windowWillClose:(NSNotification*)notification // WM_CLOSE
{
    // CODE
    [NSApp terminate:self];
}

-(void)dealloc
{
    // CODE
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

@implementation MyView

-(id)initWithFrame:(NSRect)frame
{
    // CODE
    self = [super initWithFrame:frame];

    if(self)
    {

    }

    return self;
}


@end


// Command 
//#clang -o window window.m -framework cocoa
