/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/IO/BSON/BSONOutputStream.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONWriteException.h"

namespace Open3DMotion
{
  /**
   * Utility to write BSON to a memory buffer
   * - could move into Open3DMotion when convenient
	 * 
	 * @ingroup OdinMongoDB
   */
	class BSONOutputStreamBuffer : public BSONOutputStream
	{
	public:
		BSONOutputStreamBuffer(UInt8* ndata, UInt32 nlength);

	public:

		virtual void WriteBinary(const void* binary, UInt32 size)  throw(BSONWriteException);

	protected:
		UInt8* data;
    UInt32 length;
    UInt32 offset;
	};
}
