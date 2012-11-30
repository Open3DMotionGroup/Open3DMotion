/*--
  OpenMove 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

/* EB01PI.h
   
	 Summary
	 Access to the EB01PI data set, equivalent to all C3D files in Data/C3D/sample08/

	 Description
	 Motion has been converted a comma-delimited array and hence imported to a C++ variable, 
	 defined in the corresponding .CPP file. This means it can be used within C++ as 
	 ground truth against which to compare other file imports.

	 Software used to export data from EB01PI.c3d was C-motion Visual3D 3.99.33.1 Beta.
	 Visual3D allowed individual marker and analog channels to be saved as separate text files.
	 MS Excel was used to combine these into a single .XLS file, which was then 
	 converted to comma-separated-values and imported into EP01PI.cpp.

 */

#pragma once

#include <string.h>

namespace TestData
{
	// EB01PI is the example C3D file in sample08
	static const int EB01PI_numframes = 450;
	static const int EB01PI_nummarkers = 26;
	static const int EB01PI_numanalog = 16;
	static const int EB01PI_analogpermarker = 4;
	static const int EB01PI_valuesperframe = EB01PI_nummarkers*4+EB01PI_numanalog*EB01PI_analogpermarker;
	extern const char* EB01PI_markername[EB01PI_nummarkers];
	extern const char* EB01PI_analogname[EB01PI_numanalog];
	extern const double EB015_markertol;
	extern const double EB015_analogtol[EB01PI_numanalog];	
	extern const double EB01PI[EB01PI_numframes*EB01PI_valuesperframe];
	extern bool EB01PI_isforce[EB01PI_numanalog];
}
