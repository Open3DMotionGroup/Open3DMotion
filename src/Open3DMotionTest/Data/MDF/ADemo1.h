/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

/* ADemo1.h
   
	 Summary
	 Data set for ADemo1 file (originally HA011)

	 Description
	 This was saved as text using a modified version of Codamotion Analysis 6.70
	 Modified because default output seemed to be wrong due to a rounding error
	 in GetEpoch(...).  The TXT file was imported to MS Excel and separate
	 CSV files saved for marker and force data.  These were then imported into
	 ADemo1.cpp as arrays (ADemo1_marker and ADemo1 force).  
	 EMG data was copied to the clipboard from a graph of all EMG channels, 
	 pasted into MS Excel, saved as CSV then pasted into ADemo1.cpp as
	 ADemo1_EMG.  This had to be done via a graph since the EMG sample rate
	 is twice that of the markers so only half the samples appear in the TXT save.

 */

#ifndef _OPEN3DMOTION_ADEMO1_H_
#define _OPEN3DMOTION_ADEMO1_H_

#ifdef __GNUC__
#include <stddef.h>
#endif

namespace TestData
{
	static const size_t ADemo1_nummarkers = 24;
	static const size_t ADemo1_numforceplates = 1;
	static const size_t ADemo1_numEMG = 8;
	static const double ADemo1_rateHz_marker = 200.0;
	static const double ADemo1_rateHz_force = 200.0;
	static const double ADemo1_rateHz_EMG = 400.0;
	static const size_t ADemo1_numframes_marker = 512;
	static const size_t ADemo1_numframes_force = 511;
	static const size_t ADemo1_numframes_EMG = 1023;
	static const double ADemo1_forcetol = 0.05;
	extern const double ADemo1_marker[ADemo1_numframes_marker*ADemo1_nummarkers*4];
	extern const double ADemo1_force[ADemo1_numframes_force*ADemo1_numforceplates*8];
	extern const double ADemo1_EMG[ADemo1_numframes_EMG*ADemo1_numEMG];
	extern const char* ADemo1_markername[ADemo1_nummarkers];
	extern const char* ADemo1_EMGname[ADemo1_nummarkers];
}

#endif
