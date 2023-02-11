/** \file   src/diskimage/fsimage-check.h
 *
 * \brief   Validation functions for disk images - header
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
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

#ifndef VICE_FSIMAGE_CHECK_H
#define VICE_FSIMAGE_CHECK_H


/** \brief  Error code for illegal track numbers
 */
#define FSIMAGE_BAD_TRKNUM  -2

/** \brief  Error code for illegal sector numbers
 */
#define FSIMAGE_BAD_SECNUM  -3


struct disk_image_s;

int fsimage_check_sector(const struct disk_image_s *image, unsigned int track, unsigned int sector);

#endif
