//
//  nbd_control.h
//  NBDDisk
//
//  Created by Steve O'Brien on 12/13/12.
//  Copyright (c) 2012 Steve O'Brien. All rights reserved.
//

#ifndef NBDDisk_nbd_control_h
#define NBDDisk_nbd_control_h

#include <sys/ioctl.h>


typedef struct nbd_control_loopback
{
	unsigned short tcp_port;
} nbd_control_loopback;


typedef struct nbd_control_ip4
{
	unsigned short tcp_port;
	unsigned char addr[4];
} nbd_control_ip4;


typedef struct nbd_control_ip6
{
	unsigned short tcp_port;
	unsigned char addr[16];
} nbd_control_ip6;


#define NBD_CTL_CONNECT_TCPIP_LO (_IOW(0xaa, 1, nbd_control_loopback))
#define NBD_CTL_CONNECT_TCPIP4   (_IOW(0xaa, 4, nbd_control_ip4))
#define NBD_CTL_CONNECT_TCPIP6   (_IOW(0xaa, 6, nbd_control_ip6))


#endif
