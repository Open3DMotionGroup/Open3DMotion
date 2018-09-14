/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_TS_FACTORY_H_
#define _OPEN3DMOTION_TS_FACTORY_H_

#include "Open3DMotion/Biomechanics/Trial/TimeSequence.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinIter.h"

namespace Open3DMotion
{
	class TSFactory
	{
	protected:
		TSFactory()
		{
		}

	public:

		TimeSequence* New(const TimeRange& tr, const BinMemFactory& memfactory) const;

		TimeSequence& Allocate(TimeSequence& ts, const TimeRange& tr, const BinMemFactory& memfactory) const;

		TimeSequence& AllocateSameTimeRange(TimeSequence& ts, const TimeSequence& predicate, const BinMemFactory& memfactory) const;

		const std::vector<BinaryFieldSpec>& Layout() const
		{ return layout; }

	protected:
		void AddField(const BinaryFieldSpec& field);

	private:
		std::vector<BinaryFieldSpec> layout;
	};

	class TSFactoryValue : public TSFactory
	{
	public:
		static const char fieldname_value[];

	public:
		TSFactoryValue(Int32 dimension);
	};

	class TSFactoryOccValue : public TSFactoryValue
	{
	public:
		static const char fieldname_occluded[];

	public:
		TSFactoryOccValue(Int32 dimension);
	};

	class TSFactoryOccConfValue : public TSFactoryOccValue
	{
	public:
		static const char fieldname_confidence[];

	public:
		TSFactoryOccConfValue(Int32 dimension);
	};

	class TSScalarConstIter : public BinConstIter1<double>
	{
	public:
		TSScalarConstIter(const TimeSequence& ts) throw(NoSuchFieldException)  :
				BinConstIter1<double>(ts, TSFactoryValue::fieldname_value, 1)
		{
		}

	public:
		const double& Value() const
		{ return *ValuePtr0(); }
	};

	class TSScalarIter : public BinIter1<double>
	{
	public:
		TSScalarIter(TimeSequence& ts) throw(NoSuchFieldException)  :
				BinIter1<double>(ts, TSFactoryValue::fieldname_value, 1)
		{
		}

	public:
		double& Value() const
		{ return *ValuePtr0(); }
	};

	class TSOccScalarConstIter : public BinConstIter2<double, UInt8>
	{
	public:
		TSOccScalarConstIter(const TimeSequence& ts) throw(NoSuchFieldException)  :
				BinConstIter2<double, UInt8>(ts, TSFactoryValue::fieldname_value, 1, TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		const double& Value() const
		{ return *ValuePtr0(); }

		const UInt8& Occluded() const
		{ return *ValuePtr1(); }
	};

	class TSOccScalarIter : public BinIter2<double, UInt8>
	{
	public:
		TSOccScalarIter(TimeSequence& ts) throw(NoSuchFieldException)  :
				BinIter2<double, UInt8>(ts, TSFactoryValue::fieldname_value, 1, TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		double& Value() const
		{ return *ValuePtr0(); }

		UInt8& Occluded() const
		{ return *ValuePtr1(); }
	};

	class TSVector3ConstIter : public BinConstIter1<double>
	{
	public:
			TSVector3ConstIter(const TimeSequence& ts) throw(NoSuchFieldException) :
				BinConstIter1<double>(ts, TSFactoryValue::fieldname_value, 3)
		{
		}

	public:
		const double* Value() const
		{ return ValuePtr0(); }
	};

	class TSVector3Iter : public BinIter1<double>
	{
	public:
		TSVector3Iter(TimeSequence& ts) throw(NoSuchFieldException)  :
				BinIter1<double>(ts, TSFactoryValue::fieldname_value, 3)
		{
		}

	public:
		double* Value() const
		{ return ValuePtr0(); }
	};

	class TSOccVector3ConstIter : public BinConstIter2<double, UInt8>
	{
	public:
			TSOccVector3ConstIter(const TimeSequence& ts) throw(NoSuchFieldException) :
				BinConstIter2<double, UInt8>(ts, TSFactoryValue::fieldname_value, 3, TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		const double* Value() const
		{ return ValuePtr0(); }

		const UInt8& Occluded() const
		{ return *ValuePtr1(); }
	};

	class TSOccVector3Iter : public BinIter2<double, UInt8>
	{
	public:
		TSOccVector3Iter(TimeSequence& ts) throw(NoSuchFieldException)  :
			BinIter2<double, UInt8>(ts, TSFactoryValue::fieldname_value, 3, TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		double* Value() const
		{ return ValuePtr0(); }

		UInt8& Occluded() const
		{ return *ValuePtr1(); }
	};

	class TSOccConstIter : public BinConstIter1<UInt8>
	{
	public:
		TSOccConstIter(const TimeSequence& ts) throw(NoSuchFieldException)  :
				BinConstIter1<UInt8>(ts, TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		const UInt8& Occluded() const
		{ return *ValuePtr0(); }
	};

	class TSOccIter : public BinIter1<UInt8>
	{
	public:
		TSOccIter(TimeSequence& ts) throw(NoSuchFieldException)  :
				BinIter1<UInt8>(ts, TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		UInt8& Occluded() const
		{ return *ValuePtr0(); }
	};

	class TSOccMatrix3x3ConstIter : public Open3DMotion::BinConstIter2<double, Open3DMotion::UInt8>
	{
	public:
			TSOccMatrix3x3ConstIter(const Open3DMotion::TimeSequence& ts) throw(Open3DMotion::NoSuchFieldException) :
				BinConstIter2<double, Open3DMotion::UInt8>(ts, Open3DMotion::TSFactoryValue::fieldname_value, 9, Open3DMotion::TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		const double* Value() const
		{ return ValuePtr0(); }

		const Open3DMotion::UInt8& Occluded() const
		{ return *ValuePtr1(); }
	};

	class TSOccMatrix3x3Iter : public Open3DMotion::BinIter2<double, Open3DMotion::UInt8>
	{
	public:
			TSOccMatrix3x3Iter(Open3DMotion::TimeSequence& ts) throw(Open3DMotion::NoSuchFieldException) :
				BinIter2<double, Open3DMotion::UInt8>(ts, Open3DMotion::TSFactoryValue::fieldname_value, 9, Open3DMotion::TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		double* Value() const
		{ return ValuePtr0(); }

		Open3DMotion::UInt8& Occluded() const
		{ return *ValuePtr1(); }
	};

	class TSOccConfValueIter : public Open3DMotion::BinIter3<double, Open3DMotion::UInt8, double>
	{
	public:
		TSOccConfValueIter(Open3DMotion::TimeSequence& ts, UInt32 dimension) throw(Open3DMotion::NoSuchFieldException) :
			BinIter3<double, Open3DMotion::UInt8, double>(ts, 
				Open3DMotion::TSFactoryValue::fieldname_value, dimension,
				Open3DMotion::TSFactoryOccValue::fieldname_occluded, 1,
				Open3DMotion::TSFactoryOccConfValue::fieldname_confidence, dimension*dimension)
		{
		}

	public:

		double* Value() const
		{
			return ValuePtr0();
		}

		Open3DMotion::UInt8& Occluded() const
		{
			return *ValuePtr1();
		}

		double* Confidence() const
		{
			return ValuePtr2();
		}
	};

	class TSOccConfValueConstIter : public Open3DMotion::BinConstIter3<double, Open3DMotion::UInt8, double>
	{
	public:
		TSOccConfValueConstIter(const Open3DMotion::TimeSequence& ts, UInt32 dimension) throw(Open3DMotion::NoSuchFieldException) :
			BinConstIter3<double, Open3DMotion::UInt8, double>(ts,
				Open3DMotion::TSFactoryValue::fieldname_value, dimension,
				Open3DMotion::TSFactoryOccValue::fieldname_occluded, 1,
				Open3DMotion::TSFactoryOccConfValue::fieldname_confidence, dimension*dimension)
		{
		}

	public:

		const double* Value() const
		{
			return ValuePtr0();
		}

		const Open3DMotion::UInt8& Occluded() const
		{
			return *ValuePtr1();
		}

		const double* Confidence() const
		{
			return ValuePtr2();
		}
	};

};

#endif
