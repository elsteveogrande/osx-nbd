//
//  NBDManager.h
//  NBDDisk
//
//  Created by Steve O'Brien on 12/11/12.
//  Copyright (c) 2012 Steve O'Brien. All rights reserved.
//

#ifndef __NBDDisk__NBDManager__
#define __NBDDisk__NBDManager__

#include <libkern/libkern.h>
#include <mach/kmod.h>
#include <IOKit/IOService.h>


extern "C"
{
// plain C below

#include <sys/types.h>
#include <sys/proc.h>
#include <sys/ioctl.h>

int nbd_open(dev_t dev, int flags, int devtype, proc_t proc);
int nbd_close(dev_t dev, int flags, int devtype, proc_t proc);
int nbd_ioctl(dev_t dev, u_long cmd, caddr_t data, int flags, proc_t proc);
int nbd_read(dev_t dev, uio_t uio, int flags);
int nbd_write(dev_t dev, uio_t uio, int flags);
int nbd_reset(int);
int nbd_select(dev_t, int, void *, proc_t);
int nbd_stop(struct tty *, int);

// plain C above
}


// a single dummy device that accepts a few ioctl calls but no read/write
class cc_obrien_NBDManager : public IOService
{
	OSDeclareDefaultStructors(cc_obrien_NBDManager)

private:
	int major;
	void *dev;

public:
    virtual bool init(OSDictionary *dictionary = 0);
    virtual void free(void);
    virtual IOService *probe(IOService *provider, SInt32 *score);
    virtual bool start(IOService *provider);
    virtual void stop(IOService *provider);
};

#endif /* defined(__NBDDisk__NBDManager__) */
