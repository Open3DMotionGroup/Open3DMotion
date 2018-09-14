/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_RICHBINARY_H_
#define _ORMPP_RICHBINARY_H_

#include "Open3DMotion/OpenORM/Mappings/MapBinary.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinaryStructure.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactory.h"

namespace Open3DMotion
{
	/** Store binary data which has a repeating event structure, and allow attributes	to be added by derived classes.
	    This is suitable for time sequences (measurements at regular intervals) and event groups (labelled events at irregular intervals). */
	class RichBinary : public MapCompound
	{
	public:

		/** Construct empty - suitable for reading in from tree or direct allocation 
		
				@param structurename the name to give the data structure when outputting to a tree
			*/
		RichBinary(const char* structurename);

	public:

		/** Allocate with known parameters */
		void Allocate(const std::vector<BinaryFieldSpec>& layout, size_t numframes, const BinMemFactory& memfactory);

		/** Perform a deep copy (copy all meta-data and allocate new memory) */
		virtual void DeepCopyFrom(const RichBinary& src, const BinMemFactory& memfactory);

	public:

		/** Name of binary data item as stored on tree (currently always "Data") */
		static const char BinaryName[];

		/** Retrieve data structure of each event */
		const BinaryStructure& DataStructure() const
		{ return Structure; }

		/** Underlying binary object */
		const MapBinary& BinaryContainer() const
		{	return Binary; }

		/** Total size of all events in bytes */
		size_t DataSizeBytes() const
		{ return Binary.SizeBytes(); }

		/** Const access to binary data */
		const UInt8* Data() const
		{ return Binary.Data(); }

		/** Non-const access to binary data */
		UInt8* Data()
		{ return Binary.Data(); }

	private:
		BinaryStructure Structure;
		MapBinary Binary;
	};
}
#endif
