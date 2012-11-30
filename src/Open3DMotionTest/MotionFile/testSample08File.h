/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "stdlib.h"
#include "Open3DMotionTest/Data/C3D/sample08/EB01PI.h"
#include "Open3DMotion/MotionFile/MotionFileHandler.h"

void testSample08File(Open3DMotion::MotionFileHandler& handler, const char* filename, double markertol = TestData::EB015_markertol, const double* analogtol = TestData::EB015_analogtol, bool allow_analog_reorder = false, bool allow_analog_removal = false, bool exclude_force = false);
