#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "NBDDisk/nbd_control.h"


extern int errno;
const char *dev_path = "/dev/nbdc";

int main(int argc, char *argv[])
{
	int tcp_port;
	int fd;
	struct nbd_control_ip4 msg;
	
	/* TODO getopt for real options; right now just one little int option */
	if(argc != 2)
	{
		fprintf(stderr, "usage: %s port\n", argv[0]);
		return 1;
	}

	tcp_port = atoi(argv[1]);
	if(tcp_port < 1 || tcp_port >= 49152)
	{
		fprintf(stderr, "port must be between 1 and 49151\n");
		return 2;
	}

	fd = open(dev_path, O_RDONLY);
	if(fd == -1)
	{
		fprintf(
			stderr,
			"could not open control dev '%s': %s (%d)\n",
			dev_path,
			strerror(errno),
			errno);
		return 3;
	}

	msg.tcp_port = tcp_port;

	if(-1 == ioctl(
		fd,
		NBD_CTL_CONNECT_TCPIP_LO,
		&msg))
	{
		fprintf(
			stderr,
			"ioctl failed: %s (%d)\n",
			strerror(errno),
			errno);
		return 4;
	}
	
	close(fd);
	
	return 0;
}


