#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "../../include/RenderToy/rendertoy.h"

Display *dis;
int screen;
Window win;
GC gc;

/* here are our X routines declared! */
void init_x();
void close_x();
void redraw();

using namespace RenderToy;

int main()
{
	XEvent event;	/* the XEvent declaration !!! */
	KeySym key;		/* a dealie-bob to handle KeyPress Events */
	char text[255]; /* a char buffer for KeyPress Events */

	init_x();

	/* look for events forever... */
	while (1)
	{
		/* get the next event and stuff it into our event variable.
		   Note:  only events we set the mask for are detected!
		*/
		XNextEvent(dis, &event);

		if (event.type == Expose && event.xexpose.count == 0)
		{
			/* the window was exposed redraw it! */
			redraw();
		}
		if (event.type == KeyPress &&
			XLookupString(&event.xkey, text, 255, &key, 0) == 1)
		{
			/* use the XLookupString routine to convert the invent
			   KeyPress data into regular text.  Weird but necessary...
			*/
			if (text[0] == 'q')
			{
				close_x();
			}
			printf("You pressed the %c key!\n", text[0]);
		}
		if (event.type == ButtonPress)
		{
			/* tell where the mouse Button was Pressed */
			int x = event.xbutton.x,
				y = event.xbutton.y;

			strcpy(text, "X is FUN!");
			XSetForeground(dis, gc, rand() % event.xbutton.x % 255);
			XDrawString(dis, win, gc, x, y, text, strlen(text));
		}
	}
}

void init_x()
{
	/* get the colors black and white (see section for details) */
	unsigned long black, white;

	dis = XOpenDisplay((char *)0);
	screen = DefaultScreen(dis);
	black = BlackPixel(dis, screen),
	white = WhitePixel(dis, screen);
	win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0,
							  300, 300, 5, black, white);
	XSetStandardProperties(dis, win, "Howdy", "Hi", None, NULL, 0, NULL);
	XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);
	gc = XCreateGC(dis, win, 0, 0);
	XSetBackground(dis, gc, white);
	XSetForeground(dis, gc, black);
	XClearWindow(dis, win);
	XMapRaised(dis, win);
};

void close_x()
{
	XFreeGC(dis, gc);
	XDestroyWindow(dis, win);
	XCloseDisplay(dis);
	exit(1);
};

void redraw()
{
	XClearWindow(dis, win);
};