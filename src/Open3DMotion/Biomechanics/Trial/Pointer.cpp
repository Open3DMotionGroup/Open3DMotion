/*--
  Open3DMotion 
  Copyright (c) 2004-2021.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Pointer.h"

namespace Open3DMotion
{
	Pointer::Pointer()
	{
		REGISTER_MEMBER(Generation);
		REGISTER_MEMBER(distance);
		REGISTER_MEMBER(CalX);
		REGISTER_MEMBER(CalY);
		REGISTER_MEMBER(CalZ);
		REGISTER_MEMBER(LandmarkName);
	}

	Pointer pointer;

}
