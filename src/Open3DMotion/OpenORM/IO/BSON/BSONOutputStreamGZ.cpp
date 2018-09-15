#include "BSONOutputStreamGZ.h"
#include <zlib.h>
#include <memory.h>
#include <stdlib.h>

namespace Open3DMotion
{
	BSONOutputStreamGZ::BSONOutputStreamGZ(std::ostream& _output) :
		output(_output),
		strm(new z_stream_s),
		output_buffer(4096)
	{
		memset(strm.get(), 0, sizeof(z_stream_s));
		const int GZIP_ENCODING = 0x10;
		deflateInit2(strm.get(), 9, Z_DEFLATED, MAX_WBITS | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY);
	}
	
	BSONOutputStreamGZ::~BSONOutputStreamGZ()
	{
		strm->avail_in = 0;
		strm->next_in = NULL;
		do
		{ 
			strm->avail_out = output_buffer.size();
			strm->next_out = &output_buffer[0];		
			deflate(strm.get(), Z_FINISH);
			UInt32 have = output_buffer.size() - strm->avail_out;
			output.write((char*)&output_buffer[0], have);
		}
		while (strm->avail_out == 0);
		deflateEnd(strm.get());
	}
	
	void BSONOutputStreamGZ::WriteBinary(const void* binary, UInt32 size)  throw(BSONWriteException)
	{
		strm->avail_in = size;
		strm->next_in = (z_const Bytef *)binary;
		do
		{
			strm->avail_out = output_buffer.size();
			strm->next_out = &output_buffer[0];
			UInt32 ret = deflate(strm.get(), Z_NO_FLUSH);
			if (ret != 0)
				throw (BSONWriteException("compression error"));
			UInt32 have = output_buffer.size() - strm->avail_out;
			output.write((char*)&output_buffer[0], have);
		}
		while (strm->avail_out == 0);
	}
}