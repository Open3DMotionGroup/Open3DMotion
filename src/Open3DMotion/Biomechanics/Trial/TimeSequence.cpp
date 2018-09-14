/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/TimeSequence.h"

namespace Open3DMotion
{
	const char TimeSequence::StructureName[] = "FrameStructure";

	TimeSequence::TimeSequence() :
		RichBinary(StructureName)
	{
		// optional descriptors
		REGISTER_MEMBER(Group);
		REGISTER_MEMBER(Channel);
		REGISTER_MEMBER(HardwareID);
		REGISTER_MEMBER(Units);
		REGISTER_MEMBER(Scale);
		REGISTER_MEMBER(Offset);
		REGISTER_MEMBER(OffsetStdDev);		
		REGISTER_MEMBER(ScaleUnits);

		// required descriptors
		REGISTER_MEMBER(Rate);
		REGISTER_MEMBER(Start);

		// store number of frames
		// this is currently redundant but would allow data to contain something 
		// after the frame data in future
		REGISTER_MEMBER(Frames);

		// Initialise required
		Frames = 0;
		Rate = 0.0;
		Start = 0;

		// base class will register data members
	}

	void TimeSequence::Allocate(const std::vector<BinaryFieldSpec>& layout, const TimeRange& t, const BinMemFactory& memfactory)
	{
		// copy descriptor info
		Frames = t.Frames;
		Rate = t.Rate;
		Start = t.Start;

		// do allocation
		RichBinary::Allocate(layout, Frames.Value(), memfactory);
	}


	void TimeSequence::GetTimeRange(TimeRange& t) const
	{
		t.Start = Start;
		t.Rate = Rate;
		t.Frames = Frames;
	}


	bool TimeSequence::TimeRangeIsSameAs(const TimeSequence& other, double start_tolerance/*=1E-9*/, double rate_tolerance/*=1E-9*/) const
	{
		TimeRange tr_this, tr_other;
		GetTimeRange(tr_this);
		other.GetTimeRange(tr_other);
		return tr_this.IsSameAs(tr_other, start_tolerance, rate_tolerance);
	}

}
