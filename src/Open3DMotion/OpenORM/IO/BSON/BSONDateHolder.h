/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BSON_DATE_HOLDER_H_
#define _ORMPP_BSON_DATE_HOLDER_H_

#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"

namespace Open3DMotion
{
	/** Used inside  BSONDateHolder as representation of the components of BSON's date object */
	class BSONDateData : public MapCompound
	{
	public:
		BSONDateData();

	public:
		MapInt32 LSB;
		MapInt32 MSB;
	};

	/** Representation of BSON's 8-byte date object */
	class BSONDateHolder : public MapCompound
	{
	public:
		BSONDateHolder();

	public:
		BSONDateData BSONDate;
	};
}

#endif