/*--
  Open3DMotion 
  Copyright (c) 2004-2021.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_POINTER_H_
#define _OPEN3DMOTION_POINTER_H_

#include "Open3DMotion/OpenORM/Mappings/MapFloat64.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"
#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"

namespace Open3DMotion
{
	class Pointer : public MapCompound
	{
	public:
		Pointer();

	public: 
		MapInt32 Generation;
		MapOptionalString LandmarkName;
		MapOptionalFloat64 CalX;
		MapOptionalFloat64 CalY;
		MapOptionalFloat64 CalZ;
	};

}
#endif