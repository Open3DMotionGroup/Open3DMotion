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

		// The landmark described (if a pointer trial rather than pointer calibration data)
		MapOptionalString LandmarkName;

		// Generation (style of pointer)
		MapInt32 Generation;
		
		// Generation 1 parameters
		MapOptionalFloat64 distance;
		
		// Generation 2 parameters
		MapOptionalFloat64 CalX;
		MapOptionalFloat64 CalY;
		MapOptionalFloat64 CalZ;
	};

}
#endif