/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"

namespace Open3DMotion
{
	using namespace std;

	const char TSFactoryValue::fieldname_value[] = "value";
	const char TSFactoryOccValue::fieldname_occluded[] = "occluded";
	const char TSFactoryOccConfValue::fieldname_confidence[] = "confidence";

	TimeSequence* TSFactory::New(const TimeRange& tr, const BinMemFactory& memfactory) const
	{
		TimeSequence* ts = new TimeSequence;
		ts->Allocate(layout, tr, memfactory);
		return ts;
	}

	TimeSequence& TSFactory::Allocate(TimeSequence& ts, const TimeRange& tr, const BinMemFactory& memfactory) const
	{
		ts.Allocate(layout, tr, memfactory);
		return ts;
	}

	TimeSequence& TSFactory::AllocateSameTimeRange(TimeSequence& ts, const TimeSequence& predicate, const BinMemFactory& memfactory) const
	{
		TimeRange tr;
		predicate.GetTimeRange(tr);
		return Allocate(ts, tr, memfactory);
	}

	void TSFactory::AddField(const BinaryFieldSpec& field)
	{
		layout.push_back(field);
	}

	TSFactoryValue::TSFactoryValue(Int32 dimension)
	{
		AddField( BinaryFieldSpec::FromType<double>( fieldname_value, dimension ) );
	}

	TSFactoryOccValue::TSFactoryOccValue(Int32 dimension) :
		TSFactoryValue(dimension)
	{
		AddField( BinaryFieldSpec::FromType<UInt8>( fieldname_occluded, 1 ) );
	}

	TSFactoryOccConfValue::TSFactoryOccConfValue(Int32 dimension) :
		TSFactoryOccValue(dimension)
	{
		AddField(BinaryFieldSpec::FromType<double>(fieldname_confidence, dimension*dimension));
	}
}
