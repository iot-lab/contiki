/*
 * This file is part of HiKoB Openlab.
 *
 * HiKoB Openlab is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, version 3.
 *
 * HiKoB Openlab is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with HiKoB Openlab. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2011,2012 HiKoB.
 */

/**
 * \file
 *         Configuration for IoT-LAB M3
 *
 * \author
 *         Antoine Fraboulet <antoine.fraboulet.at.hikob.com>
 *         Gaëtan Harter <gaetan.harter.at.inria.fr>
 *
 */

#ifndef CONTIKI_CONF_H_
#define CONTIKI_CONF_H_

/* specific defines */

#define LEDS_GREEN       1
#define LEDS_YELLOW      4
#define LEDS_RED         2
#define LEDS_BLUE        0
#define LEDS_CONF_ALL   (LEDS_GREEN | LEDS_YELLOW | LEDS_RED | LEDS_BLUE)

#define RF2XX_DEVICE rf231
#define SLIP_ARCH_CONF_UART uart_print

#ifndef SLIP_ARCH_CONF_BAUDRATE
#define SLIP_ARCH_CONF_BAUDRATE 500000
#endif

#if SLIP_ARCH_CONF_BAUDRATE != 500000
#warning Only 500000 baudrate works on iotlab testbed.
#warning This warning can be ignored if you are working on a standalone iotlab-m3 board.
#endif

/* CFS defines */
#define XMEM_ERASE_UNIT_SIZE (64*1024L)

/* Openlab common definitions */

#include "../openlab/contiki-openlab-conf.h"

#endif /* CONTIKI_CONF_H_ */
