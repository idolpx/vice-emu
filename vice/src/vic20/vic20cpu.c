/*
 * vic20cpu.c - Emulation of the VIC 6502 processor.
 *
 * Written by
 *  Hannu Nuotio <hannu.nuotio@tut.fi>
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

#include <stdio.h>

#include "monitor.h"
#include "vic-cycle.h"

/* ------------------------------------------------------------------------- */

/* Global clock counter.  */
CLOCK maincpu_clk = 0L;
/* if != 0, exit when this many cycles have been executed */
CLOCK maincpu_clk_limit = 0L;

#define REWIND_FETCH_OPCODE(clock) /*clock-=2*/

#define CLK_INC() maincpu_clk++; vic_cycle()

#define CLK_ADD(clock, amount) nosuchfunction(&clock, amount)

/* Skip cycle implementation */

#define SKIP_CYCLE 0

/* opcode_t etc */

#if !defined WORDS_BIGENDIAN && defined ALLOW_UNALIGNED_ACCESS

#define opcode_t uint32_t

#define p0 (opcode & 0xff)
#define p1 ((opcode >> 8) & 0xff)
#define p2 (opcode >> 8)

#define SET_OPCODE(o) (opcode) = o;

#else /* WORDS_BIGENDIAN || !ALLOW_UNALIGNED_ACCESS */

#define opcode_t          \
    struct {              \
        uint8_t ins;         \
        union {           \
            uint8_t op8[2];  \
            uint16_t op16;    \
        } op;             \
    }

#define p0 (opcode.ins)
#define p2 (opcode.op.op16)

#ifdef WORDS_BIGENDIAN

#define p1 (opcode.op.op8[1])

#define SET_OPCODE(o)                          \
    do {                                       \
        opcode.ins = (o) & 0xff;               \
        opcode.op.op8[1] = ((o) >> 8) & 0xff;  \
        opcode.op.op8[0] = ((o) >> 16) & 0xff; \
    } while (0)

#else /* !WORDS_BIGENDIAN */

#define p1 (opcode.op.op8[0])

#define SET_OPCODE(o)                          \
    do {                                       \
        opcode.ins = (o) & 0xff;               \
        opcode.op.op8[0] = ((o) >> 8) & 0xff;  \
        opcode.op.op8[1] = ((o) >> 16) & 0xff; \
    } while (0)

#endif

#endif /* WORDS_BIGENDIAN || !ALLOW_UNALIGNED_ACCESS */


/* HACK: memmap updates for the reg_pc < bank_limit case */
#ifdef FEATURE_CPUMEMHISTORY
#define MEMMAP_UPDATE(addr) memmap_mem_update(addr, 0, 0)
#else
#define MEMMAP_UPDATE(addr)
#endif


/* FETCH_OPCODE implementation(s) */
/* FIXME: update last_read on "< bank_limit" case */
#if !defined WORDS_BIGENDIAN && defined ALLOW_UNALIGNED_ACCESS
#define FETCH_OPCODE(o) \
    do { \
        if (((int)reg_pc) < bank_limit) {                       \
            o = (*((uint32_t *)(bank_base + reg_pc)) & 0xffffff);  \
            MEMMAP_UPDATE(reg_pc);                              \
            CLK_INC();                                          \
            CLK_INC();                                          \
            if (fetch_tab[o & 0xff]) {                          \
                CLK_INC();                                      \
            }                                                   \
        } else {                                                \
            o = LOAD(reg_pc);                                   \
            CLK_INC();                                          \
            o |= LOAD(reg_pc + 1) << 8;                         \
            CLK_INC();                                          \
            if (fetch_tab[o & 0xff]) {                          \
                o |= (LOAD(reg_pc + 2) << 16);                  \
                CLK_INC();                                      \
            }                                                   \
        }                                                       \
    } while (0)

#else /* WORDS_BIGENDIAN || !ALLOW_UNALIGNED_ACCESS */
#define FETCH_OPCODE(o) \
    do { \
        if (((int)reg_pc) < bank_limit) {                         \
            (o).ins = *(bank_base + reg_pc);                      \
            MEMMAP_UPDATE(reg_pc);                                \
            CLK_INC();                                            \
            (o).op.op16 = *(bank_base + reg_pc + 1);              \
            CLK_INC();                                            \
            if (fetch_tab[(o).ins]) {                             \
                (o).op.op16 |= (*(bank_base + reg_pc + 2) << 8);  \
                CLK_INC();                                        \
            }                                                     \
        } else {                                                  \
            (o).ins = LOAD(reg_pc);                               \
            CLK_INC();                                            \
            (o).op.op16 = LOAD(reg_pc + 1);                       \
            CLK_INC();                                            \
            if (fetch_tab[(o).ins]) {                             \
                (o).op.op16 |= (LOAD(reg_pc + 2) << 8);           \
                CLK_INC();                                        \
            }                                                     \
        }                                                         \
    } while (0)

#endif /* WORDS_BIGENDIAN || !ALLOW_UNALIGNED_ACCESS */


#include "../mainviccpu.c"
