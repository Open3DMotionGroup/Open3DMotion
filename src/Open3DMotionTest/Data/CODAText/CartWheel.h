/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

/* CartWheel.h
   
	 Summary
	 Access to the CartWheel data set, equivalent to Data/TXT/CartWheel.txt

	 Description
	 Motion has been converted a comma-delimited array and hence imported to a C++ variable, 
	 defined in the corresponding .CPP file. This means it can be used within C++ as 
	 ground truth against which to compare other file imports.

	 Software used to export data was Codamotion Analysis (some unspecified early version).
	 MS Excel was used to strip out title rows and save as comma delimited values,
	 which were copied into CartWheel.cpp
 */

#ifndef _OPEN3DMOTION_CARTWHEEL_H_
#define _OPEN3DMOTION_CARTWHEEL_H_

namespace TestData
{
	static const int CartWheel_nummarkers = 20;
	static const int CartWheel_numframes = 348;
	static const double CartWheel_rateHz = 200.0;
	static const double CartWheel_starttime = 0.190;
	static const double CartWheel_markertol = 0.1;

	extern const char* CartWheel_comment;
	extern const char* CartWheel_markername[CartWheel_nummarkers];
	extern const int CartWheel_markerid[CartWheel_nummarkers];
	extern const double CartWheel_markertimeoffset[CartWheel_nummarkers];
	extern const double CartWheel[CartWheel_numframes*CartWheel_nummarkers*4];
}

#endif
