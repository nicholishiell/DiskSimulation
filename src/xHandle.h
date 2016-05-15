#ifndef XHANDLE
#define XHANDLE

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/Xdbe.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <vector>
#include <sstream>

#include "displayable.h"

using namespace std;

class xHandle{  

public:
  
  xHandle();
  ~xHandle();
  
  Bool process();

  void UpdateScene(vector<Displayable*> displayList, int timeStep);

  void End();

  Display *display;
  Atom	wmProtocols[3];
  Atom	wmDeleteWindow;
  Atom	wmTakeFocus;
  int screen;
  Window root;
  Window win;

  XSetWindowAttributes attributes;
  unsigned int width, height;
  
  int major, minor; 
  XdbeBackBuffer backBuffer;	
  XdbeBackBufferAttributes *backAttr;	
  XdbeSwapInfo swapInfo;		
  
  GC gc;
  XGCValues gcv;
   
};


xHandle::xHandle(){
  // Open display 
  display = XOpenDisplay(NULL);
  
  // We'll be using default screen 
  screen = DefaultScreen(display);
  
  // The window width and height will be the quarter of the display dimension
  width  = 800;//XDisplayWidth(display,screen) / 4;
  height = 600;//XDisplayHeight(display,screen) / 4; 
  
  // List of events we want to handle 
  attributes.event_mask = 
    KeyPressMask         | KeyReleaseMask      | KeymapStateMask        | ButtonPressMask          |
    ButtonReleaseMask    | EnterWindowMask     | LeaveWindowMask        | ExposureMask             |
    VisibilityChangeMask | StructureNotifyMask | SubstructureNotifyMask | SubstructureRedirectMask |
    FocusChangeMask;
		
  // Various window attributes 
  attributes.backing_store	 = Always;
  attributes.save_under		 = True;
  attributes.override_redirect = False;
  attributes.background_pixel  = WhitePixel(display, screen);

  // Get defaul root window 
  root = DefaultRootWindow(display);
  
  // Create our window 
  win = XCreateWindow( display, root, 0, 0, width, height, 0,
		       CopyFromParent, InputOutput, CopyFromParent,
		       CWBackingStore|CWOverrideRedirect|CWEventMask|CWSaveUnder|CWBackPixel,
		       &attributes );
  
  // Set WM protocols to handle (delete window and focus acquisition 
  wmProtocols[0]  =                  XInternAtom(display, "WM_PROTOCOLS", True);
  wmProtocols[1]  = wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
  wmProtocols[2]  = wmTakeFocus    = XInternAtom(display, "WM_TAKE_FOCUS", False);
  
  XSetWMProtocols(display, win, wmProtocols, 3);
  
  // Create gc 
  gcv.foreground = WhitePixel(display, screen);
  gcv.background = BlackPixel(display, screen);
  
  gc = XCreateGC(display, win, GCForeground | GCBackground, &gcv);
  
  // Get DBE version 
  if(!XdbeQueryExtension(display, &major, &minor)) {
    fprintf(stderr, "Error : unable to fetch Xdbe Version.\n");
  }
  
  printf("Xdbe version %d.%d\n",major,minor);	
  
// Allocate back buffer 
// From XdbeSwapBuffers man page :
//		XdbeUndefined	
//			The contents of the new back buffer become undefined.  This  may 
//			be  the most efficient action since it allows the implementation 
//			to discard the contents of the buffer if it needs to. 
// ... so we'll have to do the cleanup by hand. And as we didn't specify any 
// background that' ok :) 
  
  backBuffer = XdbeAllocateBackBufferName(display, win, XdbeUndefined);
  
  // Get back buffer attributes (used for swapping) 
  backAttr = XdbeGetBackBufferAttributes(display, backBuffer);
  swapInfo.swap_window = backAttr->window;
  swapInfo.swap_action = XdbeUndefined;
  
  XFree(backAttr);
  
  // Map and raise window 
  XMapWindow(display, win);
  XRaiseWindow(display, win);
};

void xHandle::End(){
  if(!XdbeDeallocateBackBufferName(display, backBuffer)) {
    fprintf(stderr,"Error : unable to deallocate back buffer.\n");
  } 
}


void xHandle::UpdateScene(vector<Displayable*> displayList, int timeStep){

  // Clear 
  XSetForeground(display, gc, WhitePixel(display, screen));
  XFillRectangle(display, backBuffer, gc, 0, 0, width, height);
  
  // Draw something 
  XSetForeground(display, gc, BlackPixel(display, screen));
  //XFillRectangle(display, backBuffer, gc, x, y, 32, 32);
  for(unsigned int i = 0; i < displayList.size(); i++){
    Displayable *d = displayList[i];
    d->paint(this);
  }
 
  ostringstream os;
  os << "Time: "<<timeStep;
  string timeStamp = os.str();
  int length = timeStamp.size();
   
  XDrawString(display, backBuffer, gc, 10, 25, timeStamp.c_str(), length);

  // Swap buffers
  XdbeSwapBuffers(display, &swapInfo, 1);
};

// Event handling 
Bool xHandle::process() {
	XEvent	event;
	Bool    run  = True;
	
	if(!XPending(display)) return True;
	
	XNextEvent(display, &event);

	switch(event.type) {
		// Well this is just a test program, so a lot of event aren't processed 
		case ButtonPress:
		case ButtonRelease:
		case KeyPress:
		case KeyRelease:
		case KeymapNotify:
		case EnterNotify:
		case LeaveNotify:
		case ReparentNotify:
		break;

		case FocusIn:
		case FocusOut:
			switch(event.xfocus.mode) {
				case NotifyNormal:
				case NotifyGrab:
				case NotifyUngrab:
				break;
			}
		break;

		case VisibilityNotify:
		case ConfigureNotify:
		break;

		// The window received a destroy notification 
		case DestroyNotify:
			run  = False;
		break;
		
		case ClientMessage:
		  if (event.xclient.message_type == wmProtocols[0]) {
		    // The user may have pushed the close button. Or more generally 
		    // the WM wants the window to be closed.	
		    if(((Atom)event.xclient.data.l[0]) == wmDeleteWindow) {
		      run  = False;
		    }
		    else if(event.xclient.data.l[0] == wmTakeFocus) {
		      // The WM wants the window to gain focus. 
		      XSetInputFocus(display, event.xclient.window, RevertToParent, CurrentTime);
		    }
		  }
		  break;
		  
	default:
	  break;
	}

	return run;
	};
#endif
