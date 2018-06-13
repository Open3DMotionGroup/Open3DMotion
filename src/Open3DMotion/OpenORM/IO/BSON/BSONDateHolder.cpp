#include "BSONDateHolder.h"

namespace Open3DMotion
{
	BSONDateData::BSONDateData()
	{
		REGISTER_MEMBER(LSB);
		REGISTER_MEMBER(MSB);
	}

	BSONDateHolder::BSONDateHolder()
	{
		REGISTER_MEMBER(BSONDate);
	}
}