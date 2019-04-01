// x11.c --- X11 routines used by the TV and KBD interfaces

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>

#include "usim.h"
#include "tv.h"
#include "kbd.h"
#include "mouse.h"

typedef struct DisplayState {
	unsigned char *data;
	int linesize;
	int depth;
	int width;
	int height;
} DisplayState;

static Display *display;
static Window window;
static int bitmap_order;
static int color_depth;
static Visual *visual = NULL;
static GC gc;
static XImage *ximage;

#define USIM_EVENT_MASK ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask

#define MOUSE_EVENT_LBUTTON 1
#define MOUSE_EVENT_MBUTTON 2
#define MOUSE_EVENT_RBUTTON 4

#define X_SHIFT 1
#define X_CAPS 2
#define X_CTRL 4
#define X_ALT 8
#define X_META 16

static unsigned long Black;
static unsigned long White;

static int old_run_state;

static XComposeStatus status;

// Takes E, converts it into a LM keycode and sends it to the
// IOB KBD.
static void
process_key(XEvent *e, int keydown)
{
	KeySym keysym;
	unsigned char buffer[5];
	int extra;
	int lmcode;
	int newkbd = 0;

	extra = 0;
	if (e->xkey.state & X_META)
		extra |= 3 << 12;

	if (e->xkey.state & X_ALT)
		extra |= 3 << 12;

	if (e->xkey.state & X_SHIFT)
		extra |= 3 << 6;

	if (e->xkey.state & X_CAPS)
		extra ^= 3 << 6;

	if (e->xkey.state & X_CTRL)
		extra |= 3 << 10;

	if (keydown) {
		XLookupString(&e->xkey, (char *) buffer, 5, &keysym, &status);

		if (keysym == XK_Shift_L ||
		    keysym == XK_Shift_R ||
		    keysym == XK_Control_L ||
		    keysym == XK_Control_R ||
		    keysym == XK_Alt_L ||
		    keysym == XK_Alt_R)
			return;

		switch (keysym) {
		case XK_F1:	// Terminal.
			lmcode = 1;
			break;
		case XK_F2:	// System.
			lmcode = 1 | (3 << 8);
			break;
		case XK_F3:	// Network.
			lmcode = 0 | (3 << 8);
			break;
		case XK_F4:	// Abort.
			lmcode = 16 | (3 << 8);
			break;
		case XK_F5:	// Clear.
			lmcode = 17;
			break;
		case XK_F6:	// Help.
			lmcode = 44 | (3 << 8);
			break;
		case XK_F11:	// End.
			lmcode = 50 | (3 << 8);
			break;
		case XK_F7:	// Call.
			lmcode = 16;
			break;
		case XK_F12:	// Break.
		case XK_Break:
			lmcode = 0;
			break;
		case XK_BackSpace:	// Rubout.
			lmcode = 046;
			break;
		case XK_Return:	// Return.
			lmcode = 50;
			break;
		case XK_Tab:
			lmcode = 18;
			break;
		case XK_Escape:
			lmcode = 1;
			break;
		case XK_Left:
			newkbd = 1;
			lmcode = 0117;
			break;
		case XK_Right:
			newkbd = 1;
			lmcode = 017;
			break;
		case XK_Up:
			newkbd = 1;
			lmcode = 0106;
			break;
		case XK_Down:
			newkbd = 1;
			lmcode = 0176;
			break;
		default:
			if (keysym > 255) {
				fprintf(stderr, "unknown keycode: %lu\n", keysym);
				return;
			}
			lmcode = okb_to_scancode[keysym][(extra & (3 << 6)) ? 1 : 0];
			break;
		}

		// Keep Control and Meta bits, Shift is in the scancode table.
		lmcode |= extra & ~(3 << 6);
		// ... but if Control or Meta, add in Shift.
		if (extra & (17 << 10))
			lmcode |= extra;

		if (newkbd)
			lmcode |= 1 << 16;
		else
			lmcode |= 0xffff0000;

		kbd_key_event(lmcode, keydown);
	}
}

static int u_minh = 0x7fffffff;
static int u_maxh;
static int u_minv = 0x7fffffff;
static int u_maxv;

void
accumulate_update(int h, int v, int hs, int vs)
{
	if (h < u_minh)
		u_minh = h;
	if (h + hs > u_maxh)
		u_maxh = h + hs;
	if (v < u_minv)
		u_minv = v;
	if (v + vs > u_maxv)
		u_maxv = v + vs;
}

void
send_accumulated_updates(void)
{
	int hs;
	int vs;

	hs = u_maxh - u_minh;
	vs = u_maxv - u_minv;
	if (u_minh != 0x7fffffff && u_minv != 0x7fffffff && u_maxh && u_maxv) {
		XPutImage(display, window, gc, ximage, u_minh, u_minv, u_minh, u_minv, hs, vs);
		XFlush(display);
	}

	u_minh = 0x7fffffff;
	u_maxh = 0;
	u_minv = 0x7fffffff;
	u_maxv = 0;
}

void
x11_event(void)
{
	XEvent e;

	send_accumulated_updates();
	kbd_dequeue_key_event();

	while (XCheckWindowEvent(display, window, USIM_EVENT_MASK, &e)) {
		switch (e.type) {
		case Expose:
			XPutImage(display, window, gc, ximage, 0, 0, 0, 0, tv_width, tv_height);
			XFlush(display);
			break;
		case KeyPress:
			process_key(&e, 1);
			break;
		case KeyRelease:
			process_key(&e, 0);
			break;
		case MotionNotify:
		case ButtonPress:
		case ButtonRelease:
			mouse_event(e.xbutton.x, e.xbutton.y, e.xbutton.button);
			break;
		default:
			break;
		}
	}
	if (old_run_state != run_ucode_flag) {
		old_run_state = run_ucode_flag;
	}
}

void
x11_init(void)
{
	char *displayname;
	unsigned long bg_pixel = 0L;
	int xscreen;
	Window root;
	XEvent e;
	XGCValues gcvalues;
	XSetWindowAttributes attr;
	XSizeHints *size_hints;
	XTextProperty windowName;
	XTextProperty *pWindowName = &windowName;
	XTextProperty iconName;
	XTextProperty *pIconName = &iconName;
	XWMHints *wm_hints;
	char *window_name = (char *) "CADR";
	char *icon_name = (char *) "CADR";

	displayname = getenv("DISPLAY");
	display = XOpenDisplay(displayname);
	if (display == NULL) {
		fprintf(stderr, "usim: failed to open display.\n");
		exit(-1);
	}

	bitmap_order = BitmapBitOrder(display);
	xscreen = DefaultScreen(display);
	color_depth = DisplayPlanes(display, xscreen);

	Black = BlackPixel(display, xscreen);
	White = WhitePixel(display, xscreen);

	root = RootWindow(display, xscreen);
	attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
	window = XCreateWindow(display, root, 0, 0, tv_width, tv_height, 0, color_depth, InputOutput, visual, CWBorderPixel | CWEventMask, &attr);
	if (window == None) {
		fprintf(stderr, "usim: failed to open window.\n");
		exit(-1);
	}

	if (!XStringListToTextProperty(&window_name, 1, pWindowName)) {
		pWindowName = NULL;
	}

	if (!XStringListToTextProperty(&icon_name, 1, pIconName)) {
		pIconName = NULL;
	}

	size_hints = XAllocSizeHints();
	if (size_hints != NULL) {
		// The window will not be resizable.
		size_hints->flags = PMinSize | PMaxSize;
		size_hints->min_width = size_hints->max_width = tv_width;
		size_hints->min_height = size_hints->max_height = tv_height;
	}

	wm_hints = XAllocWMHints();
	if (wm_hints != NULL) {
		wm_hints->initial_state = NormalState;
		wm_hints->input = True;
		wm_hints->flags = StateHint | InputHint;
	}

	XSetWMProperties(display, window, pWindowName, pIconName, NULL, 0, size_hints, wm_hints, NULL);
	XMapWindow(display, window);

	gc = XCreateGC(display, window, 0, &gcvalues);

	// Fill window with the specified background color.
	bg_pixel = 0;
	XSetForeground(display, gc, bg_pixel);
	XFillRectangle(display, window, gc, 0, 0, tv_width, tv_height);

	// Wait for first Expose event to do any drawing, then flush.
	do {
		XNextEvent(display, &e);
	}
	while (e.type != Expose || e.xexpose.count);

	XFlush(display);
	ximage = XCreateImage(display, visual, (unsigned) color_depth, ZPixmap, 0, (char *) tv_bitmap, tv_width, tv_height, 32, 0);
	ximage->byte_order = LSBFirst;
}
