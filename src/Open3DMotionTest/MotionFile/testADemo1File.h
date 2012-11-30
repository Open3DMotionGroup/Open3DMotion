/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/MotionFile/MotionFileHandler.h"

/*
	Summary
	Compare data loaded from specified file with the ADemo1 data set
	@param strict Force strict numerical equivalence (disable when using integer C3D's which have rounding issues)
	@param forcedecimation If reading from C3D, force will have been supersampled
												and must be decimated by factor 2 to get back to original
@param checkmarkerid Verify that marker hardware ID's are as expected (disable for C3D's which don't store marker ID)
	*/
void testADemo1File(Open3DMotion::MotionFileHandler& handler, const char* filename, bool strict, Open3DMotion::UInt32 forcedecimation, bool checkmarkerid);

