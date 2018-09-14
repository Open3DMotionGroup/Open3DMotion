/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_TIME_SEQUENCE_H_
#define _OPEN3DMOTION_TIME_SEQUENCE_H_

#include "Open3DMotion/OpenORM/Mappings/RichBinary/RichBinary.h"
#include "Open3DMotion/Biomechanics/Trial/TimeRange.h"

namespace Open3DMotion
{
	class TimeSequence : public RichBinary
	{
	public:

		TimeSequence();

	public:

		void Allocate(const std::vector<BinaryFieldSpec>& layout, const TimeRange& t, const BinMemFactory& memfactory);

		void GetTimeRange(TimeRange& t) const;

		bool TimeRangeIsSameAs(const TimeSequence& other, double start_tolerance=1E-9, double rate_tolerance=1E-9) const;

	public:

		/** Name to give data structure on the tree ("FrameStructure") */
		static const char StructureName[];

	public:		

		// optional descriptors
		MapOptionalString Group;
		MapOptionalString Channel;
		MapOptionalInt32 HardwareID;
		MapOptionalString Units;
		MapOptionalFloat64 Scale;
		MapOptionalFloat64 Offset;
		MapOptionalFloat64 OffsetStdDev;		
		MapOptionalString ScaleUnits;

		// the essential part to describe the data
		MapFloat64 Rate;
		MapFloat64 Start;

	public:

		/** Immutable access to frames count */
		size_t NumFrames() const
		{ return (size_t) Frames.Value(); }

	private:
		MapInt32 Frames;
	};
}
#endif