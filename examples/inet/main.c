/**
 * @file main.c
 * @version 0.3.0
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

#include "simba.h"
#include "inet.h"

#include "esp_common.h"

/* Ports. */
#define UDP_PORT   30303
#define TCP_PORT   40404
#define SHELL_PORT 50505

static char packet[512];
static struct uart_driver_t uart;
static struct fs_command_t cmd_udp_send;
static struct shell_args_t shell_args;

static int cmd_udp_send_cb(int argc,
                           const char *argv[],
                           chan_t *out_p,
                           chan_t *in_p,
                           void *arg_p,
                           void *call_arg_p)
{
    int i;
    struct socket_t udp;
    struct socket_addr_t addr;

    /* Open an UDP socket. */
    socket_open(&udp, SOCKET_DOMAIN_AF_INET, SOCKET_TYPE_DGRAM, 0);

    addr.ip = 0x6701a8c0;
    addr.port = UDP_PORT;
    socket_bind(&udp, &addr, sizeof(addr));

    addr.ip = 0x6a01a8c0;
    addr.port = UDP_PORT;

    for (i = 0; i < sizeof(packet); i++) {
        packet[i] = i;
    }
    
    /* Send a packet to the server. */
    socket_sendto(&udp,
                  packet,
                  sizeof(packet),
                  0,
                  &addr,
                  sizeof(addr));
        
    std_printf(FSTR("Packet sent.\r\n"));

    /* Close the socket. */
    socket_close(&udp);

    return (0);
}

static int init()
{
    struct station_config sta_config;
    struct ip_info ip_config;
    
    sys_start();

    uart_module_init();
    uart_init(&uart, &uart_device[0], 38400, NULL, 0);
    uart_start(&uart);
    sys_set_stdout(&uart.chout);

    fs_command_init(&cmd_udp_send,
                    FSTR("/udp_send"),
                    cmd_udp_send_cb,
                    NULL);
    fs_command_register(&cmd_udp_send);

    /* Start WiFi in station mode. */
    wifi_set_opmode_current(STATION_MODE);

    memset(&sta_config, 0, sizeof(sta_config));
    std_sprintf((char *)sta_config.ssid, FSTR("%s"), STRINGIFY(SSID));
    std_sprintf((char *)sta_config.password, FSTR("%s"), STRINGIFY(PASSWORD));

    wifi_station_set_config(&sta_config);

    wifi_get_ip_info(STATION_IF, &ip_config);

    while (ip_config.ip.addr == 0) {
        thrd_usleep(50000);
        wifi_get_ip_info(STATION_IF, &ip_config);
    }

    std_printf(FSTR("Connected to AP. Got IP 0x%x\r\n"),
               ip_config.ip.addr);

    return (0);
}

int main()
{
    init();

    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_args.username_p = NULL;
    shell_args.password_p = NULL;
    shell_main(&shell_args);
    
    return (0);
}
