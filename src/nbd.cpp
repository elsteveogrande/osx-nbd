#include "device.h"


NBDBlockStorageDevice * instance = 0;


void startup(kmod_info_t *info, void *data)
{
}


void shutdown(kmod_info_t *info, void *data)
{
	instance->stop();
}


extern "C"
{

#include <libkern/libkern.h>
#include <mach/mach_types.h>

kern_return_t _start(kmod_info_t *info, void *data)
{
	startup(info, data);
	return KERN_SUCCESS;
}

kern_return_t _stop(kmod_info_t *info, void *data)
{
	shutdown(info, data);
	return KERN_SUCCESS;
}

KMOD_EXPLICIT_DECL(cc.obrien.nbd.NBDBlockDevice, "0.0.5", _start, _stop)
__private_extern__ kmod_start_func_t *_realmain = _start;
__private_extern__ kmod_stop_func_t *_antimain = _stop;
__private_extern__ int _kext_apple_cc = __APPLE_CC__;

};
