//
//  nbd_control.h
//  NBDDisk
//
//  Created by Steve O'Brien on 12/13/12.
//  Copyright (c) 2012 Steve O'Brien. All rights reserved.
//

#ifndef NBDDisk_nbd_control_h
#define NBDDisk_nbd_control_h


#define NBD_CTL_CONNECT_TCPIP4  (0x04)
#define NBD_CTL_CONNECT_TCPIP6  (0x06)


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


#endif
