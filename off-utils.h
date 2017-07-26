/* off-utils.h
 *
 * Copyright (C) 2017 bindh3x <os@bindh3x.io>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 3, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _NG_OFF_UTILS
#define _NG_OFF_UTILS

#include "off.h"

char *offutil_extract_string(char*, offtok_t*, char*);
void chrclone(char*, char*, size_t);

#endif /* _NG_OFF_UTILS */
