//
//  NBDManager.h
//  NBDDisk
//
//  Created by Steve O'Brien on 12/11/12.
//  Copyright (c) 2012 Steve O'Brien. All rights reserved.
//

#ifndef __NBDDisk__NBDManager__
#define __NBDDisk__NBDManager__

#include <IOKit/storage/IOMediaBSDClient.h>


class cc_obrien_NBDManager : public IOMediaBSDClient
{
	OSDeclareDefaultStructors(cc_obrien_NBDManager);
	
	
};

#endif /* defined(__NBDDisk__NBDManager__) */
