/*--
  Open3DMotion 
  Copyright (c) 2004-2012 Charnwood Dynamics Ltd.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.zip.GZIPOutputStream;

class MakeGZTestData {
	
	static final String rawData = "Hello, I hope this uncompresses ok.\0";
	static final String outputFile = "C:/SVN/codaTree/codaMotionFileTest/GZTestData.gz";
	
	public static void main(String[] args) throws IOException {		
		OutputStream rawStream = new FileOutputStream(outputFile);
		GZIPOutputStream gzStream = new GZIPOutputStream(rawStream);
		gzStream.write(rawData.getBytes());
		gzStream.close();
	}
}
