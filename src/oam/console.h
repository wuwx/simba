/**
 * @file oam/console.h
 * @version 2.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __OAM_CONSOLE_H__
#define __OAM_CONSOLE_H__

#include "simba.h"

/**
 * Initialize the console module.
 */
int console_module_init(void);

/**
 * Initialize the console.
 *
 * @return zero(0) or negative error code.
 */
int console_init(void);

/**
 * Start the console.
 *
 * @return zero(0) or negative error code.
 */
int console_start(void);

/**
 * Stop the console.
 *
 * @return zero(0) or negative error code.
 */
int console_stop(void);

/**
 * Get the pointer to the input channel.
 *
 * @return Input channel or NULL.
 */
chan_t *console_get_input_channel(void);

/**
 * Get the pointer to the output channel.
 *
 * @return Output channel or NULL.
 */
chan_t *console_get_output_channel(void);

#endif