#include "defs.h"
#include "fdefs.h"

#include <X11/XWDFile.h>
#include <stdlib.h>

extern Pixmap back_xid;
extern Widget canvas;

void
_swapshort (bp, n)
    register char *bp;
    register unsigned n;
{
    register char c;
    register char *ep = bp + n;

    while (bp < ep) {
        c = *bp;
        *bp = *(bp + 1);
        bp++;
        *bp++ = c;
    }
}

void
_swaplong (bp, n)
    register char *bp;
    register unsigned n;
{
    register char c;
    register char *ep = bp + n;
    register char *sp;

    while (bp < ep) {
        sp = bp + 3;
        c = *sp;
        *sp = *bp;
        *bp++ = c;
        sp = bp + 1;
        c = *sp;
        *sp = *bp;
        *bp++ = c;
        bp += 2;
    }
}

/*
Some of this code comes from the xwd program which has the following notice:

Copyright 1987, 1998  The Open Group

All Rights Reserved.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/

#define lowbit(x) ((x) & (~(x) + 1))

static int
ReadColors(vis,cmap,colors)
Visual *vis ;
Colormap cmap ;
XColor **colors ;
{
    int i,ncolors ;

    ncolors = vis->map_entries;

    if (!(*colors = (XColor *) malloc (sizeof(XColor) * ncolors))) {
	printf("Out of memory for colors, %s>\n", title);
	return 0;
	}

    if (vis->class == DirectColor ||
	vis->class == TrueColor) {
	Pixel red, green, blue, red1, green1, blue1;

	red = green = blue = 0;
	red1 = lowbit(vis->red_mask);
	green1 = lowbit(vis->green_mask);
	blue1 = lowbit(vis->blue_mask);
	for (i=0; i<ncolors; i++) {
	  (*colors)[i].pixel = red|green|blue;
	  (*colors)[i].pad = 0;
	  red += red1;
	  if (red > vis->red_mask)
	    red = 0;
	  green += green1;
	  if (green > vis->green_mask)
	    green = 0;
	  blue += blue1;
	  if (blue > vis->blue_mask)
	    blue = 0;
	}
    } else {
	for (i=0; i<ncolors; i++) {
	  (*colors)[i].pixel = i;
	  (*colors)[i].pad = 0;
	}
    }

    XQueryColors(baseframe_dpy, cmap, *colors, ncolors);
    
    return(ncolors);
}

/*
 * Get the XColors of all pixels in image - returns # of colors
 */
int Get_XColors(win_info, colors)
     XWindowAttributes *win_info;
     XColor **colors;
{
    int ncolors;

    ncolors = ReadColors(win_info->visual, default_cmap, colors) ;
    return ncolors ;
}

void
dump_pixmap(name)
char *name;
{
    unsigned long swaptest = 1;
    XImage *image;
    unsigned buffer_size;
    char *win_name;
    int win_name_size;
    XWindowAttributes win_info;
    XWDFileHeader header;
    XWDColor xwdcolor;
    FILE *out;
    XColor *xcolors;
    int ncolors;
    int header_size;
     int i;

    if(!XGetWindowAttributes(baseframe_dpy, currentview_xid, &win_info)){
	printf("<Can't get target window attributes, %s>\n", title);
	return;
    }

    win_name = "tipsy";
    win_name_size = strlen(win_name) + sizeof(char);

    image = XGetImage (baseframe_dpy, back_xid, 0, 0, can_width, can_height, AllPlanes,
	       ZPixmap);
    if (!image) {
	printf("<Can't get image, %s>\n", title);
	return;
    }

    /*
     * Determine the pixmap size.
     */
    buffer_size = image->bytes_per_line*image->height;

    ncolors = Get_XColors(&win_info, &xcolors);

    /*
     * Calculate header size.
     */
    header_size = sz_XWDheader + win_name_size;

    /*
     * Write out header information.
     */
    header.header_size = (CARD32) header_size;
    header.file_version = (CARD32) XWD_FILE_VERSION;
    header.pixmap_format = (CARD32) ZPixmap;
    header.pixmap_depth = (CARD32) image->depth;
    header.pixmap_width = (CARD32) image->width;
    header.pixmap_height = (CARD32) image->height;
    header.xoffset = (CARD32) image->xoffset;
    header.byte_order = (CARD32) image->byte_order;
    header.bitmap_unit = (CARD32) image->bitmap_unit;
    header.bitmap_bit_order = (CARD32) image->bitmap_bit_order;
    header.bitmap_pad = (CARD32) image->bitmap_pad;
    header.bits_per_pixel = (CARD32) image->bits_per_pixel;
    header.bytes_per_line = (CARD32) image->bytes_per_line;
    header.visual_class = (CARD32) win_info.visual->class;
    header.red_mask = (CARD32) win_info.visual->red_mask;
    header.green_mask = (CARD32) win_info.visual->green_mask;
    header.blue_mask = (CARD32) win_info.visual->blue_mask;
    header.bits_per_rgb = (CARD32) win_info.visual->bits_per_rgb;
    header.colormap_entries = (CARD32) win_info.visual->map_entries;
    header.ncolors = ncolors;
    header.window_width = (CARD32) image->width;
    header.window_height = (CARD32) image->height;
    header.window_x = 0;
    header.window_y = 0;
    header.window_bdrwidth = (CARD32) 0;
 if (*(char *) &swaptest) {
        _swaplong((char *) &header, sizeof(header));
        for (i = 0; i < ncolors; i++) {
            _swaplong((char *) &xcolors[i].pixel, sizeof(long));
            _swapshort((char *) &xcolors[i].red, 3 * sizeof(short));
        }
    }

    out = fopen(name, "w");
    if(!out) {
	printf("<Can't open file %s, %s\n", name, title);
	return;
    }

    if (fwrite((char *)&header, sz_XWDheader, 1, out) != 1 ||
        fwrite(win_name, win_name_size, 1, out) != 1) {
        perror("xwd");
        return;
    }

    /*
     * Write out the color maps, if any
     */

    for (i = 0; i < ncolors; i++) {
        xwdcolor.pixel = xcolors[i].pixel;
        xwdcolor.red = xcolors[i].red;
        xwdcolor.green = xcolors[i].green;
        xwdcolor.blue = xcolors[i].blue;
        xwdcolor.flags = xcolors[i].flags;
        if (fwrite((char *) &xwdcolor, sz_XWDColor, 1, out) != 1) {
            perror("xwd");
            return;
        }
    }

    /*
     * Write out the buffer.
     */
    /*
     *    This copying of the bit stream (data) to a file is to be replaced
     *  by an Xlib call which hasn't been written yet.  It is not clear
     *  what other functions of xwd will be taken over by this (as yet)
     *  non-existant X function.
     */
    if (fwrite(image->data, (int) buffer_size, 1, out) != 1) {
        perror("xwd");
        return;
    }
    fclose(out);

    /*
     * free the color buffer.
     */

    if(ncolors > 0) free(xcolors);

    /*
     * Free image
     */
    XDestroyImage(image);
}
