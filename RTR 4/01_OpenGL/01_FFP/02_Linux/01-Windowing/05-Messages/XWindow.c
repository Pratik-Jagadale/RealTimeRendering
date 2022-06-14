// Standard Headers
#include <stdio.h>		// For Standard IO
#include <stdlib.h>		// For exit()
#include <memory.h> 	// For memset()
#include <string.h> 	// For strlen()

// X11 Headers (11 Is 11 th Version of X)
#include <X11/Xlib.h>
#include <X11/Xutil.h> 	// For XVisualInfo
#include <X11/XKBlib.h>

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
Display *display = NULL; 	// 77 memebrs
XVisualInfo visualInfo; 	// 10 members
Colormap colormap;
Window window;
XEvent event;
KeySym keysym;
Bool fullscreen = False;

// Entry Point Function
int main(void)
{
	// Function declartions
	void uninitiallize(void);
	void toogleFullscreen(void);

	// Local Variables
	int defaultScreen;
	int defaultDepth;
	Status status;
	XSetWindowAttributes windowAttribute;
	int styleMask;
	Atom wm_delete_window_atom;
	int screenWidth;
	int screenHeight;
	char keys[26];

	// Code
	display = XOpenDisplay(NULL);
	if(display == NULL)
	{
		printf("Error - XOpenDisplay() Failed...\n");
		uninitiallize();
		exit(1);
	}

	defaultScreen = XDefaultScreen(display);

	defaultDepth = XDefaultDepth(display,defaultScreen);

	status = XMatchVisualInfo(display,defaultScreen, defaultDepth, TrueColor,&visualInfo);

	if(status == 0)
	{
		printf("Error - XMatchVisualInfo() Failed...\n");
		uninitiallize();
		exit(1);
	}


	memset(&windowAttribute,0,sizeof(XSetWindowAttributes));

	windowAttribute.border_pixel = 0;
	windowAttribute.background_pixel = XBlackPixel(display,defaultScreen);	
	windowAttribute.background_pixmap = 0;
	windowAttribute.colormap = XCreateColormap(	display,
												RootWindow(display,visualInfo.screen),
												visualInfo.visual,
												AllocNone);
	windowAttribute.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | FocusChangeMask;

	colormap = windowAttribute.colormap;

	styleMask = CWBorderPixel | CWBackPixel | CWColormap| CWEventMask;

	window = XCreateWindow(	display,
							RootWindow(display,visualInfo.screen),
							100,
							100,
							WIN_WIDTH, 
							WIN_HEIGHT,
							0,
							visualInfo.depth,
							InputOutput,
							visualInfo.visual,
							styleMask,
							&windowAttribute);

	if(!window)
	{
		printf("Error - XCreateWindow() Failed...\n");
		uninitiallize();
		exit(1);
	}

	XStoreName(display,window,"PRJ - XWINDOW");

	wm_delete_window_atom = XInternAtom(display,"WM_DELETE_WINDOW",True);

	XSetWMProtocols(display,window,&wm_delete_window_atom,1);

	XMapWindow(display,window);

	// Centering of Window
	screenWidth = XWidthOfScreen(XScreenOfDisplay(display,defaultScreen));
	screenHeight = XHeightOfScreen(XScreenOfDisplay(display, defaultScreen));
	
	XMoveWindow(display,window, (screenWidth/2) - (WIN_WIDTH / 2) ,(screenHeight / 2 ) - (WIN_HEIGHT/ 2));

	// Message Loop
	while(1)
	{
		XNextEvent(display,&event);

		switch (event.type)
		{
		case MapNotify:
			break;


		
		case KeyPress:
			keysym = XkbKeycodeToKeysym(display,event.xkey.keycode,0,0); // like WM_KEYDOWN
			switch (keysym)
			{
			case XK_Escape:
				uninitiallize();
				exit(0);
				break;

			}

			XLookupString(&event.xkey,keys,sizeof(keys),NULL,NULL); // like WM_CHAR
			switch(keys[0])
			{
			case 'F':
			case 'f':
				if(fullscreen == False)
				{
					toogleFullscreen();
					fullscreen = True;
				}
				else{
					toogleFullscreen();
					fullscreen = False;
				}
				break;
			}
			break;

		case ButtonPress:
			switch(event.xbutton.button)
			{
				case 1:
					printf("Button left mouse button is Clicked...\n");
				 	break;
				case 2:
					printf("Button middle mouse button is Clicked...\n");
					break;
				case 3:
					printf("Button right mouse button is Clicked...\n");
					break;
			}	
			break;
		
		case FocusIn:
			printf("Window has got focuse.\n");
			break;;

		case FocusOut:
			printf("Window hase lose focuse.\n");
			break;

		case 33:
			uninitiallize();
			exit(0);
			break;

		default:
			break;
		}
	}

	uninitiallize();

	return (0);
}

void uninitiallize(void)
{
	if(window)
	{
		XDestroyWindow(display,window);
	
	}

	if(colormap)
	{
		XFreeColormap(display,colormap);
	}
	
	if(display)
	{
		XCloseDisplay(display);
		display = NULL;
	}

}

void toogleFullscreen(void)
{
	// Local Variables
	Atom wm_current_state_atom;
	Atom wm_fullscreen_state_atom;
	XEvent event;
	
					printf("ekjr\n");

	// code
	wm_current_state_atom = XInternAtom(display,"_NET_WM_STATE", False);

	wm_fullscreen_state_atom = XInternAtom(display,"_NET_WM_STATE_FULLSCREEN",False);

	memset(&event, 0 , sizeof(XEvent));

	event.type = ClientMessage;
	event.xclient.window = window;
	event.xclient.message_type = wm_current_state_atom;
	event.xclient.format = 32;
	event.xclient.data.l[0] = fullscreen ? 0 : 1;
	event.xclient.data.l[1] = wm_fullscreen_state_atom;

	XSendEvent(display,RootWindow(display,visualInfo.screen),False,SubstructureNotifyMask,&event);
	
}
