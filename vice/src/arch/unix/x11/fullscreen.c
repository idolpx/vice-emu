/*
 * fullscreen.c
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *  Martin Pottendorfer <pottendo@utanet.at>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "vice.h"

#include "fullscreen.h"
#include "video.h"

#ifdef USE_XF86_EXTENSIONS

#ifdef USE_XF86_VIDMODE_EXT
#include "vidmode.h"
#endif
#ifdef USE_XF86_DGA2_EXTENSIONS
#include "dga2.h"
#endif

int fullscreen_is_enabled;

int fullscreen_available(void) 
{
#ifdef USE_XF86_VIDMODE_EXT
    if (vidmode_available())
        return 1;
#endif
#ifdef USE_XF86_DGA2_EXTENSIONS
    if (dga2_available())
	return 1;
#endif    
    return 0;
}

void fullscreen_create_menus(void)
{
#ifdef USE_XF86_VIDMODE_EXT
    vidmode_create_menus();
#endif
#ifdef USE_XF86_DGA2_EXTENSIONS
    dga2_create_menus();
#endif
}

int fullscreen_init(void)
{
#ifdef USE_XF86_VIDMODE_EXT
    if (vidmode_init() < 0)
        return -1;
#endif
#ifdef USE_XF86_DGA2_EXTENSIONS
    if (dga2_init() < 0)
        return -1;
#endif
    return 0;
}

static int fullscreen_enable(struct video_canvas_s *canvas, int enable)
{
    /*printf("FS EN%i\n",enable);*/
    return 0;
}

static int fullscreen_device(struct video_canvas_s *canvas, const char *device)
{
    /*printf("FS DEV %s\n",device);*/
    return 0;
}

#ifdef USE_XF86_VIDMODE_EXT
static int fullscreen_mode_vidmode(struct video_canvas_s *canvas, int mode)
{
    /*printf("FS VM%i\n",mode);*/
    return 0;
}
#endif

#ifdef USE_XF86_DGA2_EXTENSIONS
static int fullscreen_mode_dga2(struct video_canvas_s *canvas, int mode)
{
    /*printf("FS DM%i\n",mode);*/
    return 0;
}
#endif

#endif

void fullscreen_capability(cap_fullscreen_t *cap_fullscreen)
{
    cap_fullscreen->device_num = 0;

#ifdef USE_XF86_VIDMODE_EXT
    cap_fullscreen->device_name[cap_fullscreen->device_num] = "Vidmode";
    cap_fullscreen->enable = fullscreen_enable;
    cap_fullscreen->device = fullscreen_device;
    cap_fullscreen->mode[cap_fullscreen->device_num] = fullscreen_mode_vidmode;
    cap_fullscreen->device_num += 1;
#endif
#ifdef USE_XF86_DGA2_EXTENSIONS
    cap_fullscreen->device_name[cap_fullscreen->device_num] = "DGA2";
    cap_fullscreen->enable = fullscreen_enable;
    cap_fullscreen->device = fullscreen_device;
    cap_fullscreen->mode[cap_fullscreen->device_num] = fullscreen_mode_dga2;
    cap_fullscreen->device_num += 1;
#endif
}

