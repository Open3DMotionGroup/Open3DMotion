/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/IO/BSON/BSONInputStream.h"

namespace Open3DMotion
{
  /**
   * Utility to read BSON from a memory buffer
   * - could move into Open3DMotion when convenient
	 * 
	 * @ingroup OdinMongoDB
   */
	class BSONInputStreamBuffer : public BSONInputStream
	{
	public:
		BSONInputStreamBuffer(const UInt8* ndata, UInt32 nlength);

	public:

		virtual void SkipBytes(UInt32 count) throw(BSONReadException);

		virtual void ReadBinary(void* binary, UInt32 size) throw(BSONReadException);

		virtual bool HaveMore();

	protected:
		const UInt8* data;
    UInt32 length;
    UInt32 offset;
	};
}
