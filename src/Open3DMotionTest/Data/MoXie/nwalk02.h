/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

/* nwalk02.h
   
	 Summary
	 Data set for nwalk02.mox [ See http://www.smalll.nl/VUMC/REV/MoXie/Downloads.htm ]

	 Description
	 Taken from file placed in My Documents\MoXie data\example data by MoXie View 2.0.0 installer.
	 File was imported into MS Excel and mapped as XML, then manipulated in Excel to get comma-delimited
   channel data which was pasted into nwalk02.cpp.
 */

#ifndef _OPEN3DMOTIONTEST_NWALK02_H_
#define _OPEN3DMOTIONTEST_NWALK02_H_

#include <stdlib.h>

namespace TestData
{
	static const int nwalk02_numcameras = 2;

	static const int nwalk02_numframes_force = 1129;

	static const int moxie_numforcechannels = 6;

	extern const char* moxie_forcechannel_name[moxie_numforcechannels];

	extern const double nwalk02_camera_dlt[nwalk02_numcameras][12];

	const double nwalk02_force_rateHz = 100.0;

	extern const double nwalk02_force[moxie_numforcechannels][nwalk02_numframes_force];
}

#endif