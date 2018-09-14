/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/TimeRange.h"

namespace Open3DMotion
{
	TimeRange::TimeRange()
	{
		REGISTER_MEMBER(Rate);
		REGISTER_MEMBER(Start);
		REGISTER_MEMBER(Frames);
	}

	bool TimeRange::IsSameAs(const TimeRange& other, 
			double start_tolerance/*=1E-9*/,
			double rate_tolerance/*=1E-9*/) const
	{
		return 
			(Frames == other.Frames) &&
			(fabs(Start - other.Start) < start_tolerance) &&
			(fabs(Rate - other.Rate) < rate_tolerance);
	}

}
