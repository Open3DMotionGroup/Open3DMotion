/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "nwalk02.h"

namespace TestData
{
	const double nwalk02_camera_dlt[nwalk02_numcameras][12] =
	{
		{ -0.164036, 0.068132, 0.000666, 119.884916, 0.004198, 0.052368, -0.166773, 256.425021, 0.000038, 0.000345, 0.000000, 1.0 },
		{  0.048805, 0.175859, 0.001817, 263.671102, 0.035007, 0.000510, -0.170733, 262.519716, 0.000189, 0.000014, 0.000000, 1.0 }
	};

	const double nwalk02_force[moxie_numforcechannels][nwalk02_numframes_force] = 
	{
		{ -0.49,1.01,-0.19,-0.89,-0.19,-0.19,-0.49,1.31,-0.19,0.61,-0.19,0.61,-0.89,0.61,-0.89,0.61,-0.49,0.21,-0.49,0.61,-0.19,0.61,-1.29,0.21,0.21,0.61,-0.49,1.01,-0.49,1.01,-0.19,0.61,-0.19,1.01,-0.19,0.21,-0.19,1.01,-0.19,0.21,-0.19,1.01,-0.19,0.61,0.21,0.21,-0.49,0.61,-0.49,1.01,0.21,0.61,-0.19,0.61,-0.89,0.61,-0.19,0.61,-0.19,-0.19,-0.19,0.21,-0.49,0.61,-0.19,1.01,-0.19,1.01,-0.19,1.01,-0.19,1.01,-0.89,0.61,-0.19,0.61,-0.49,1.01,-0.19,0.61,-0.89,1.01,0.21,0.61,-0.19,0.61,0.21,0.61,-1.59,1.01,-0.19,0.61,-0.49,0.61,1.01,0.61,-0.19,0.61,-0.49,0.61,-0.19,0.61,-1.29,0.61,-0.19,0.21,-0.49,-0.49,-0.49,0.61,-0.49,0.21,-0.49,0.61,-0.49,0.61,-0.19,1.01,-0.49,0.21,-0.19,0.21,-0.49,0.61,-0.49,1.31,-0.19,0.61,0.21,0.21,-0.49,0.61,-0.19,0.61,-0.89,0.61,-0.19,0.61,-0.89,-0.49,-0.19,0.61,-0.89,0.61,-0.49,0.61,-1.29,0.61,-0.49,0.61,-0.49,0.61,-0.49,0.61,-0.19,0.21,-0.89,0.61,-0.49,-0.49,-0.49,0.21,-0.49,0.61,-0.19,0.21,-0.49,1.01,-0.19,0.61,-0.19,0.21,-0.49,0.21,-0.19,0.61,1.01,0.21,-0.19,0.21,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.49,0.21,-0.19,-0.19,-0.19,0.61,-0.49,0.21,-0.49,0.21,-0.49,0.61,-0.19,0.61,-0.19,0.21,-0.49,0.21,-0.19,0.61,-0.49,0.61,-0.49,0.61,-0.19,0.61,-0.49,0.61,-0.19,0.61,-0.89,0.61,-0.19,0.61,-0.49,-0.89,-0.19,-1.99,-0.19,1.71,-0.19,0.61,-0.19,-0.49,-0.89,-0.19,-0.19,0.21,-0.49,1.01,-0.19,0.61,-0.89,0.61,-0.19,0.21,-0.89,0.61,-0.19,0.21,0.21,0.21,-0.19,0.21,-0.19,0.61,-0.89,0.61,-0.49,0.61,-0.19,0.61,-0.19,0.61,-0.49,0.61,-0.49,0.61,-0.49,1.31,-0.49,0.21,-0.49,0.21,-0.49,0.21,-0.49,0.61,-0.19,0.61,0.21,0.61,-0.49,0.61,0.21,0.61,-0.19,0.61,-0.89,0.21,-0.49,0.21,0.61,0.61,-0.89,0.21,-0.49,0.21,-0.49,0.21,0.21,-0.19,-0.49,0.21,-0.19,2.11,-0.19,0.61,-2.69,0.21,1.71,0.61,-0.49,0.21,-0.19,0.21,-0.49,0.61,-0.49,0.61,-0.49,0.21,-0.19,0.21,1.01,0.61,-0.19,1.71,-0.49,-0.49,-0.49,-1.29,-0.19,-0.19,-0.19,-0.19,-0.19,0.21,-0.19,-0.19,-0.19,0.21,-0.49,1.01,-0.19,1.31,0.61,-0.19,-0.49,0.21,-0.49,0.21,-0.19,0.21,-0.19,-0.19,-0.19,0.21,-0.19,-0.19,0.21,0.21,-0.19,0.21,-0.49,-0.19,-0.19,0.21,-0.19,0.21,0.21,-0.49,-0.49,0.21,-0.19,0.21,-0.19,-0.19,0.21,0.21,0.21,0.61,0.21,0.61,-0.19,0.61,-0.19,0.21,0.21,0.21,-0.19,-0.19,-0.89,0.21,0.21,0.21,-0.49,0.21,-0.19,0.21,-0.19,0.21,-0.49,0.21,-0.89,0.61,-0.19,0.61,-0.19,-2.69,0.21,-0.19,-0.19,0.21,-0.19,0.21,-0.19,0.21,-0.49,0.61,-0.19,0.21,-0.19,-0.49,1.01,-0.19,-0.19,-0.19,-0.89,-0.19,-0.49,0.21,-0.49,0.21,-0.19,0.21,-0.19,0.21,-0.49,0.21,-0.19,0.61,-0.19,0.21,-0.19,-1.29,-0.19,0.61,-0.19,0.21,-0.49,0.21,-0.49,0.21,-0.19,-0.49,-0.49,0.21,-0.49,-0.19,-0.19,-0.19,-0.19,0.61,-0.19,0.61,-0.19,0.21,-0.19,0.61,-0.49,0.21,-0.49,0.21,-0.19,-0.19,-0.19,0.21,-0.49,0.21,-1.59,0.61,-0.19,1.71,-0.19,1.01,-0.49,0.61,-0.19,-0.19,-0.19,0.21,-0.19,0.61,-0.49,0.21,-0.49,0.61,-0.49,0.21,-0.19,0.61,-0.19,0.21,-0.19,0.61,0.21,0.61,-0.49,-0.19,-0.19,0.21,0.21,0.21,-0.19,-0.19,-0.19,0.61,-0.49,0.21,-0.19,0.21,-0.19,-0.89,-0.19,0.61,-0.19,0.21,-0.19,0.61,-0.19,0.21,-0.49,0.21,-0.19,0.61,-0.89,0.61,-0.49,0.21,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.19,0.21,-0.19,0.21,-0.19,0.21,-0.49,0.21,-0.19,0.21,-0.19,0.21,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.19,0.21,-0.49,0.61,-0.19,0.61,-0.49,0.61,-0.19,0.61,-0.19,0.61,-0.19,0.21,0.21,0.61,0.61,0.61,-0.49,0.61,-0.19,0.21,-0.49,0.21,-0.49,0.21,-0.49,0.21,-0.49,0.21,-0.49,0.21,-0.89,0.21,-0.49,0.21,-0.49,1.31,-0.19,0.61,-0.89,0.21,-0.19,0.21,-0.49,0.21,-0.19,0.21,-0.49,0.21,-0.19,0.21,-0.49,0.21,-0.49,0.61,-0.49,0.21,-0.19,0.61,-0.89,0.21,-0.19,0.21,-0.89,1.01,-0.19,0.61,-0.19,0.21,-0.49,1.01,-0.49,0.61,-0.49,-0.19,-0.19,0.61,-0.19,0.61,-0.49,0.61,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.49,0.61,-0.19,0.61,0.61,0.61,-0.49,0.61,0.21,0.61,-0.89,0.61,-0.19,0.21,-0.19,0.21,-0.19,0.61,-0.19,0.61,-0.19,0.21,-0.19,0.61,-0.19,0.21,-0.49,0.21,-0.19,0.61,-0.19,0.61,-0.49,0.61,-0.19,0.21,0.21,0.61,-0.19,0.61,-0.49,0.61,-0.49,0.61,-0.19,0.61,-0.19,0.61,-0.19,0.61,0.21,0.61,-0.19,1.01,-0.19,1.01,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.19,1.01,-0.19,0.61,-0.49,0.61,-0.49,0.61,0.21,1.01,-0.19,1.01,-0.19,1.01,0.21,-0.49,12.41,37.81,-3.49,-56.49,-94.09,-66.79,-67.89,-108.79,-127.89,-134.89,-147.49,-153.29,-154.09,-148.19,-141.89,-132.39,-125.69,-117.59,-110.99,-102.49,-95.19,-87.79,-80.79,-73.39,-67.49,-60.19,-57.19,-51.29,-48.39,-43.99,-41.39,-37.69,-35.49,-30.69,-27.79,-23.69,-20.39,-15.59,-11.89,-6.39,-1.99,4.31,7.91,12.41,14.91,20.11,24.51,31.11,37.01,44.41,51.01,59.91,68.71,77.91,87.51,98.21,109.21,121.41,131.71,143.11,151.61,160.41,170.31,178.81,184.31,190.21,192.11,189.51,176.61,150.81,104.41,73.91,43.71,29.71,17.91,7.61,1.01,-0.19,-0.19,1.01,0.21,1.01,-0.19,1.01,-0.19,0.61,0.21,0.61,-0.19,0.61,0.21,0.21,-0.49,0.61,0.21,0.61,-0.19,0.21,-0.49,1.01,-0.19,0.61,-0.19,0.61,-0.19,0.21,-0.19,0.61,0.21,0.61,-0.19,0.61,-0.49,0.61,-0.19,0.61,-0.19,1.01,0.21,1.31,0.21,0.21,-0.19,0.61,0.21,0.61,0.21,0.61,-0.19,0.21,-0.19,0.21,-0.19,0.61,-1.29,0.61,-0.19,-0.19,-0.19,1.01,-0.19,0.61,-0.49,1.01,-0.19,0.61,-0.19,0.21,-0.19,0.61,-0.19,0.21,-0.19,0.61,-0.19,0.21,-0.49,0.21,0.21,0.21,-0.19,0.21,-0.19,0.61,-0.19,0.21,-0.19,0.61,-0.19,0.61,-0.49,0.21,-0.19,0.61,-0.49,-0.49,-0.49,-0.89,-0.19,-0.19,-0.19,0.61,-0.49,0.61,-0.49,0.61,-0.19,0.61,-0.19,0.21,-0.19,0.21,-0.89,-0.19,-0.19,0.21,-0.49,1.01,-0.49,0.61,-0.19,0.21,-0.89,0.61,-0.49,0.21,0.21,0.61,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.19,2.11,-0.49,0.61,-0.19,0.61,-0.19,0.61,-0.49,0.21,-0.19,0.21,-0.19,0.21,-0.19,0.61,-0.49,-0.19,-0.19,0.21,0.21,0.61,-0.49,0.21,-0.19,0.61,-0.49,0.21,-0.19,0.21,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.19,0.61,-0.19,1.71,0.21,1.31,0.21,-0.49,-0.19,-0.19,-0.19,0.21,-0.49,-0.19,-0.49,0.21,-0.19,-0.19,-0.49,0.61,-0.19,-0.19,-0.19,0.21,-0.19,0.61,0.21,0.61,-0.49,0.21,-0.19,0.21,0.21,0.21,-0.19,-0.19,-0.19,0.21,-0.19,0.61,-0.19,0.61,-0.19,0.21,-0.19,-0.19,-0.19,-0.19,-0.19,0.21,-0.19,0.21,-0.19,0.61,0.21,0.21,0.21,0.21,-0.19,0.21,1.01,0.21,0.61,0.21,-0.49,0.61,1.01,0.21,0.21,-0.19,-0.19,0.21,-0.19,0.21,-0.19,-0.19,-0.19,0.61,0.21,-0.19,-0.19,-0.89,0.21,0.61,0.21,0.61,0.61,-0.19,0.21,0.21,-0.19,0.21,0.21,-0.19,-0.19,-0.19,-0.19,0.21,0.21,0.21,-0.19,0.21,-0.19,0.21,0.21,0.21,-0.19,-0.19,0.21,0.21,0.21,0.21,-0.19,-0.19,-0.19,0.21,0.21,-0.19,-0.19,-0.19,-0.89,-0.89,1.31,0.21,0.21,0.21,0.21,-0.19,-0.19,0.21,-0.19,1.31,-0.19,-0.19,-0.19,-0.19,0.21,-0.19,0.21,-0.49,-0.19,0.21,0.21,0.21,-0.19 },
		{ -0.52,0.98,-0.52,0.98,-0.52,0.28,-0.12,-0.12,-0.52,0.28,-0.12,0.58,0.28,0.58,-0.82,0.28,-0.12,0.28,-0.52,0.58,-1.62,0.28,0.28,0.58,-0.12,-0.12,-0.12,-0.12,-0.52,0.98,-0.52,0.28,-0.52,-0.52,-0.52,0.28,-0.82,0.98,-0.12,0.98,-0.52,0.28,-0.52,-0.12,-0.52,0.58,-0.12,0.28,-0.82,0.28,0.28,0.58,-0.52,1.38,0.28,0.98,-0.12,0.58,-1.22,0.28,-0.12,0.28,-0.52,0.58,-0.52,0.28,-0.52,0.28,-0.52,0.28,-0.52,0.28,-0.12,0.58,-0.12,0.98,-0.12,-0.12,-0.52,-0.52,-0.12,0.28,0.28,0.58,-0.52,0.28,-0.52,0.28,-0.52,0.58,-0.82,0.58,-0.82,0.28,-1.22,0.98,-0.82,0.28,-0.12,0.28,-0.52,0.58,0.98,0.58,-0.52,0.28,-0.52,0.28,-0.52,0.98,-0.52,-0.12,-0.52,-0.82,-0.52,0.58,-0.52,0.28,-0.52,0.58,-0.12,0.28,-0.52,0.58,-0.12,0.58,-0.82,0.58,0.28,0.28,-0.52,0.58,-0.12,0.58,-0.52,0.28,-0.82,0.58,-0.12,0.58,-0.12,0.58,-0.52,0.28,-0.52,0.58,-0.12,0.28,-0.52,-0.12,-0.12,-0.12,-0.12,0.28,-0.12,0.58,-0.12,0.58,-0.52,0.98,-0.12,0.58,-0.52,0.28,-1.92,0.58,-0.52,0.28,-0.52,0.28,-0.12,0.58,0.28,0.58,0.28,0.98,-0.52,0.58,-0.12,0.28,-0.12,0.58,-0.52,0.58,-0.12,0.58,-0.82,-0.12,-0.52,-0.12,-0.52,-0.12,-0.52,-0.12,-0.12,0.28,-0.12,0.28,-0.52,0.28,-0.52,0.28,-0.12,0.28,-0.12,0.28,-0.12,0.28,0.28,0.28,-0.82,0.28,-0.12,0.58,-0.12,0.28,-0.12,0.28,-0.52,0.28,-0.52,-0.52,-0.12,-0.52,-0.52,0.28,-0.12,0.98,-0.52,0.28,-0.12,0.28,-0.12,0.58,-0.12,0.28,-0.12,0.28,-0.52,1.38,-0.52,0.28,-0.12,0.58,-0.12,0.58,-0.52,0.58,-0.52,0.58,-0.12,1.68,-0.12,0.28,-0.12,0.58,-1.22,0.58,-0.12,0.58,-1.22,0.58,-0.12,0.58,-0.52,0.58,-0.52,1.68,-0.52,0.28,-0.82,0.58,-0.12,0.28,-0.82,0.28,-0.82,0.28,-0.12,0.28,-0.52,0.28,-0.52,0.58,-0.52,0.28,-0.52,0.28,-0.82,0.28,0.28,0.28,-2.72,0.28,-0.12,0.28,-0.12,0.28,0.98,-0.12,-0.52,0.28,-0.12,0.98,-0.52,0.28,-0.82,0.28,0.28,0.28,0.28,0.28,-0.52,0.28,-0.82,0.28,-0.52,0.28,-0.12,-0.52,-0.52,-0.82,-0.12,0.58,-0.12,0.98,-0.12,-0.52,-0.12,-0.12,-0.52,0.58,-0.12,0.58,-0.12,0.28,-0.12,1.38,-0.52,0.28,-0.12,-0.52,0.98,-0.12,-0.82,0.28,-0.82,0.28,-0.12,-0.12,-0.12,0.28,-0.12,0.28,-0.12,0.28,-0.12,0.28,-0.52,0.28,-0.12,-0.82,0.28,0.28,-0.12,0.28,-0.12,-0.12,0.28,0.28,-0.12,0.28,-0.12,0.28,-0.12,0.28,-0.52,0.28,-0.52,-0.12,-0.12,0.28,-0.82,-1.22,-0.12,0.28,-0.52,0.28,-0.52,0.28,-0.82,-0.12,-0.12,0.28,0.28,-0.12,-0.12,0.28,-0.12,0.98,0.28,-0.12,-0.52,0.98,-0.52,0.28,-0.52,-0.52,-0.12,0.28,-0.52,-0.12,-0.12,-0.12,-0.12,-0.12,-0.12,-0.12,-0.12,0.28,-1.22,0.28,0.98,0.28,-0.12,0.28,-0.12,0.28,-0.12,0.58,-0.12,0.28,-0.12,0.28,-0.12,0.28,-0.12,1.38,-0.52,-0.12,-0.12,0.58,-0.12,0.28,-0.12,0.28,-0.12,-0.12,-0.12,0.28,-0.12,0.28,-0.12,-0.12,-0.52,0.58,-0.12,0.28,-2.72,0.28,0.28,0.28,0.28,0.28,-0.12,0.28,-0.52,0.58,-0.12,0.58,-0.52,0.28,0.28,0.58,-0.82,0.28,-0.12,-0.82,-0.82,0.58,-0.52,0.58,-0.12,-0.12,-0.52,0.28,-0.12,0.28,-0.52,0.28,-0.12,0.58,-0.52,0.28,-0.12,0.58,1.38,0.58,-0.12,0.28,-0.12,0.58,-0.52,0.28,-0.52,0.28,-0.52,0.28,-0.12,0.28,-0.52,0.28,-0.52,0.28,-0.12,0.28,-0.12,-1.22,-0.12,0.28,-0.12,0.98,-0.52,0.28,-0.12,0.58,-0.12,0.58,-0.12,0.58,-0.12,0.28,-0.52,0.28,-0.52,0.28,0.58,0.28,-0.12,1.38,-0.52,0.58,-0.82,0.28,-0.12,0.28,-0.52,0.58,-0.52,0.28,-0.12,0.28,-0.52,0.28,-0.52,0.58,-0.52,0.28,-0.12,0.28,-0.52,0.28,-0.12,0.28,-0.52,0.28,-0.52,0.28,-0.12,0.28,-0.52,0.28,-0.12,0.28,-0.12,0.28,-0.82,0.28,-0.52,0.28,-0.82,0.58,-0.52,0.28,-0.52,0.28,-0.52,0.28,-0.52,0.28,-0.52,0.28,-0.12,-0.12,-0.52,-0.12,-0.52,1.38,-0.82,0.28,-0.52,-0.12,-0.52,0.28,-0.12,0.28,-0.82,0.58,-0.12,0.28,-0.12,0.28,-0.12,0.28,0.28,0.28,-0.82,0.28,-0.52,0.28,-0.52,0.28,-0.12,0.58,-0.12,0.28,-0.52,0.28,-0.12,0.28,-0.12,0.28,-0.12,-0.12,-0.52,0.58,-0.12,0.28,-0.82,0.28,-0.12,0.28,-0.52,0.28,-0.52,-0.52,-0.12,0.28,-0.12,-0.12,-0.52,0.28,-0.52,0.28,-0.12,0.28,-0.82,0.28,-0.12,-0.12,-0.52,0.28,-0.12,0.28,-0.12,0.28,-0.12,0.28,-0.12,0.28,-0.52,-0.12,-0.52,-0.52,-1.22,0.98,-0.12,0.58,-0.12,0.28,-0.12,0.28,-0.82,0.28,-0.52,0.28,-0.12,0.28,-0.52,0.28,-0.52,-0.12,-0.12,0.28,-0.12,0.28,-0.12,0.28,-0.82,-0.52,-0.52,0.28,-0.52,0.28,-0.52,-0.12,-0.52,0.28,-0.52,0.28,-0.82,0.28,-0.82,-0.82,-0.52,0.58,-0.12,-0.12,-1.62,0.28,-0.52,0.28,-0.52,-0.12,-0.82,-0.12,-0.82,0.28,-0.52,-0.12,-0.52,0.28,0.28,1.68,5.08,42.18,41.08,30.08,14.98,12.38,-9.32,-20.42,-28.52,-28.82,-28.82,-35.12,-41.72,-40.62,-40.22,-42.82,-44.72,-43.22,-43.92,-43.62,-44.72,-45.42,-45.42,-43.62,-42.12,-39.52,-38.82,-36.92,-36.62,-34.72,-34.02,-32.12,-33.22,-32.52,-34.02,-34.02,-34.72,-34.72,-35.82,-36.22,-37.32,-37.32,-39.12,-40.22,-41.72,-43.22,-45.02,-45.82,-48.02,-49.52,-47.62,-47.62,-48.02,-48.02,-48.72,-49.52,-50.62,-51.32,-52.02,-50.92,-49.52,-47.62,-45.42,-41.72,-37.72,-32.12,-28.12,-19.62,-10.82,-1.92,2.48,4.28,0.58,-0.12,-1.92,-3.82,-4.22,-0.12,-1.22,0.28,0.28,0.28,-0.12,0.28,-0.12,0.28,-0.52,-0.12,-0.52,-0.12,-0.12,-0.12,-1.22,0.28,-0.52,0.58,-0.52,0.28,-0.52,0.28,-0.52,-0.52,-0.52,0.28,-0.52,0.58,-0.12,0.58,-0.52,0.58,-0.12,0.58,-0.12,-0.12,0.28,0.28,-0.12,0.58,-1.22,0.28,-0.52,0.58,-0.82,0.28,-0.12,0.58,-0.52,0.28,-0.52,0.28,-0.12,0.28,0.28,0.58,-0.52,-1.62,-0.52,-0.52,-0.12,0.58,-0.52,0.28,-0.12,0.58,-0.52,0.28,-0.12,0.28,-0.12,0.28,-0.52,0.28,0.28,-0.12,0.28,0.28,-0.12,0.28,-0.82,-0.12,-0.82,-0.12,-0.82,0.28,-0.12,0.28,-0.12,0.28,-0.12,-0.12,-0.12,0.28,-0.12,-0.12,-0.12,-0.12,-0.12,0.58,-0.12,0.28,-0.12,0.28,-0.12,-0.12,-0.52,-0.12,-0.12,-0.12,-0.12,-0.12,-0.12,-0.12,-1.62,0.28,-0.12,0.28,-0.52,0.28,0.28,0.28,-0.12,0.28,-0.12,0.28,-0.12,0.28,-0.12,-0.12,-0.52,-0.12,-0.12,-0.12,0.28,1.38,-0.12,0.58,-0.12,0.28,-0.52,-0.12,-0.12,0.28,-0.12,-0.12,-0.12,0.28,0.28,0.28,-0.52,-0.52,0.28,0.28,-0.12,0.28,1.68,0.28,-0.12,-0.12,-0.12,-0.12,-0.12,0.28,-0.12,0.28,-0.52,-0.12,-0.12,0.28,-0.12,0.28,-0.52,-0.52,-0.12,0.28,-0.12,-0.12,-0.12,-0.12,-0.12,0.28,-0.12,-0.12,-0.12,-0.12,-0.52,0.28,-0.12,-0.12,0.58,0.28,0.28,-0.52,0.28,0.28,0.28,0.28,-0.52,-0.12,0.28,-0.12,-0.52,0.28,0.28,-0.12,-0.52,-0.12,-0.52,-0.52,-0.12,-0.12,-0.12,-0.52,-0.12,-0.12,-0.12,-0.12,0.28,0.58,0.28,-0.12,-0.12,-0.12,-0.12,0.28,-0.82,-0.12,0.28,-0.12,-0.12,-0.12,1.68,-0.12,-0.52,0.28,-0.82,-0.12,-0.52,0.28,-0.12,-0.12,0.28,-0.12,-0.12,-0.12,0.28,-0.12,-0.12,-0.52,-0.12,-1.22,-0.52,-0.82,0.28,-0.12,-0.12,0.98,-0.12,0.28,0.28,-0.12,-0.12,0.28,0.28,-1.62,-0.12,-0.52,0.28,0.28,-0.12,-0.12,0.28,-0.52,0.28,0.28,0.28,0.28,0.28,-0.12,-0.12,0.28,0.28,0.28,0.28,-0.12,0.28,0.58,0.28,-0.52,0.28,-1.22,0.58,1.38,-0.52,-0.12,0.28,-0.12,-0.12,-0.12,-0.12,-0.12,0.28,-0.12,0.58,-0.12,-0.12,-0.12,-1.92,-0.12,-0.52,-0.52,0.28,-0.12,-0.12,-0.12,-0.12 },
		{ -1.44,-1.44,-1.44,-0.04,-1.44,1.46,-2.94,1.46,-1.44,1.46,-1.44,1.46,-0.04,-0.04,-1.44,1.46,-1.44,-0.04,-1.44,-0.04,-0.04,-0.04,-0.04,-0.04,1.46,-0.04,-0.04,-0.04,-1.44,-0.04,-1.44,1.46,-0.04,-2.94,-2.94,-4.34,-1.44,-1.44,1.46,1.46,-1.44,-0.04,-1.44,1.46,-4.34,-0.04,-1.44,-0.04,2.86,1.46,-0.04,-0.04,-0.04,-1.44,-1.44,1.46,1.46,1.46,-0.04,-4.34,-0.04,-0.04,-0.04,-0.04,1.46,1.46,-0.04,1.46,-0.04,1.46,-0.04,1.46,-1.44,1.46,-1.44,1.46,-1.44,-0.04,-0.04,1.46,-0.04,4.36,-0.04,1.46,1.46,1.46,-1.44,-1.44,-1.44,-0.04,-0.04,1.46,-5.84,1.46,-0.04,-2.94,-1.44,1.46,-1.44,1.46,-1.44,1.46,-4.34,1.46,-0.04,-0.04,-1.44,-0.04,-0.04,-1.44,-1.44,1.46,-1.44,1.46,-1.44,2.86,-1.44,2.86,-1.44,1.46,-1.44,1.46,-2.94,-0.04,-4.34,1.46,-1.44,-1.44,-0.04,-1.44,-0.04,1.46,-0.04,1.46,-1.44,1.46,1.46,1.46,-1.44,-1.44,-0.04,-0.04,-0.04,1.46,1.46,-0.04,-2.94,1.46,-1.44,-1.44,-0.04,1.46,-0.04,-0.04,-1.44,1.46,-0.04,2.86,-1.44,-0.04,-0.04,1.46,-0.04,1.46,-0.04,-0.04,-4.34,2.86,-0.04,1.46,-1.44,1.46,1.46,1.46,-7.24,7.26,1.46,-1.44,-1.44,-5.84,-1.44,1.46,-1.44,1.46,-1.44,1.46,-1.44,2.86,-0.04,-0.04,-1.44,-1.44,-1.44,1.46,-0.04,-4.34,-0.04,1.46,-0.04,1.46,1.46,1.46,7.26,-0.04,2.86,1.46,-1.44,1.46,-0.04,1.46,-0.04,1.46,-0.04,1.46,-1.44,-0.04,-0.04,1.46,-2.94,1.46,-0.04,1.46,-1.44,1.46,-0.04,-0.04,-0.04,2.86,-0.04,-5.84,-0.04,1.46,-0.04,1.46,-0.04,-0.04,-0.04,-0.04,-1.44,-0.04,-1.44,1.46,-1.44,1.46,-1.44,-0.04,-0.04,1.46,1.46,-0.04,-1.44,4.36,-0.04,1.46,-0.04,1.46,-1.44,1.46,-1.44,1.46,1.46,-0.04,-1.44,2.86,-1.44,4.36,-0.04,1.46,-1.44,2.86,-1.44,-1.44,-2.94,-0.04,-0.04,1.46,-1.44,1.46,-1.44,1.46,-2.94,-1.44,-0.04,-0.04,-1.44,-1.44,-1.44,-0.04,-0.04,2.86,-0.04,1.46,-2.94,1.46,-1.44,1.46,-0.04,1.46,1.46,-0.04,-1.44,4.36,-0.04,1.46,-1.44,-0.04,-4.34,1.46,-1.44,1.46,-1.44,1.46,-0.04,1.46,-1.44,-1.44,-0.04,-0.04,-2.94,-4.34,-1.44,-0.04,-0.04,1.46,-1.44,1.46,-1.44,-2.94,-2.94,1.46,-0.04,-0.04,-0.04,1.46,-1.44,1.46,-1.44,-4.34,-1.44,-0.04,-0.04,-0.04,-1.44,1.46,-1.44,1.46,-0.04,-0.04,1.46,-0.04,-0.04,-0.04,-0.04,1.46,-0.04,-0.04,-1.44,-0.04,-2.94,-1.44,-0.04,-0.04,-0.04,2.86,-0.04,1.46,-0.04,-0.04,-0.04,-0.04,-0.04,-0.04,-0.04,-1.44,-0.04,-0.04,-2.94,-0.04,-7.24,1.46,-0.04,1.46,1.46,-1.44,-1.44,-0.04,-0.04,-0.04,-0.04,-0.04,-0.04,1.46,-5.84,-0.04,-0.04,2.86,-0.04,1.46,-0.04,4.36,-0.04,1.46,-0.04,-0.04,-0.04,1.46,1.46,-1.44,-0.04,1.46,-0.04,1.46,-0.04,1.46,-0.04,1.46,-2.94,1.46,-4.34,-0.04,1.46,1.46,-1.44,1.46,-0.04,1.46,-1.44,1.46,-1.44,1.46,-1.44,1.46,-0.04,-0.04,-1.44,-4.34,-1.44,4.36,-1.44,-0.04,-0.04,-2.94,-0.04,1.46,-0.04,-2.94,-1.44,1.46,-0.04,1.46,-0.04,-4.34,-0.04,1.46,5.76,1.46,4.36,1.46,-2.94,1.46,-0.04,1.46,7.26,1.46,-1.44,-0.04,-1.44,-0.04,-0.04,1.46,-2.94,1.46,-0.04,1.46,-0.04,1.46,-0.04,2.86,-2.94,5.76,-0.04,1.46,-1.44,-0.04,-1.44,1.46,-1.44,1.46,-1.44,-0.04,-0.04,1.46,-1.44,1.46,-1.44,2.86,4.36,1.46,-0.04,1.46,-0.04,1.46,-1.44,1.46,-1.44,-0.04,-1.44,-0.04,-1.44,1.46,-0.04,1.46,-2.94,1.46,-1.44,-0.04,-1.44,-1.44,-1.44,-0.04,-1.44,1.46,-1.44,-0.04,-0.04,1.46,-1.44,1.46,-1.44,1.46,-1.44,1.46,-1.44,1.46,-4.34,-0.04,-1.44,1.46,-0.04,-0.04,-1.44,-0.04,-1.44,1.46,-1.44,1.46,-1.44,1.46,-0.04,1.46,-0.04,-0.04,-1.44,5.76,-0.04,1.46,-1.44,-1.44,-1.44,1.46,-1.44,1.46,-1.44,1.46,-1.44,-0.04,-1.44,-0.04,-1.44,2.86,-0.04,1.46,-0.04,-0.04,-1.44,-0.04,-2.94,1.46,-1.44,1.46,-2.94,-0.04,-1.44,-0.04,-2.94,-0.04,-1.44,-0.04,-1.44,-1.44,-1.44,-1.44,-1.44,-0.04,-1.44,1.46,-2.94,-0.04,-1.44,-0.04,-1.44,-0.04,-1.44,-0.04,-1.44,1.46,-1.44,1.46,-1.44,-0.04,-0.04,-0.04,-1.44,1.46,-2.94,-0.04,-2.94,1.46,-0.04,1.46,-1.44,1.46,-0.04,1.46,-2.94,1.46,-1.44,1.46,-1.44,-0.04,-0.04,1.46,-0.04,5.76,-0.04,1.46,-1.44,1.46,-0.04,1.46,1.46,2.86,-1.44,1.46,-0.04,1.46,-1.44,2.86,-0.04,1.46,4.36,1.46,-0.04,1.46,-2.94,1.46,-1.44,1.46,-0.04,1.46,-1.44,1.46,-1.44,1.46,-0.04,1.46,-0.04,1.46,-0.04,-1.44,-2.94,-0.04,-0.04,-0.04,-1.44,-1.44,-0.04,4.36,-1.44,1.46,-0.04,-1.44,-1.44,-0.04,-1.44,-0.04,-0.04,-0.04,-4.34,-0.04,-0.04,-0.04,-1.44,1.46,-0.04,1.46,-0.04,1.46,-0.04,2.86,-1.44,1.46,-0.04,1.46,-0.04,1.46,-0.04,1.46,-0.04,7.26,-1.44,4.36,-1.44,-1.44,-0.04,-0.04,-1.44,1.46,-1.44,1.46,-0.04,1.46,-0.04,1.46,-1.44,1.46,-1.44,-2.94,-4.34,1.46,-1.44,1.46,-2.94,4.36,116.26,446.16,340.06,387.96,387.96,408.36,465.06,491.16,511.56,571.06,620.46,680.06,744.06,791.96,823.96,851.56,867.56,883.56,886.46,883.56,870.46,855.96,837.06,818.16,800.66,786.16,770.16,760.06,748.36,741.16,733.86,730.96,728.06,728.06,726.56,728.06,722.26,733.86,739.66,754.16,762.96,775.96,791.96,810.86,822.46,842.86,855.96,876.26,895.16,919.86,941.66,963.46,983.86,999.76,1011.46,1023.06,1023.06,1021.56,1012.86,1001.26,979.46,954.76,922.76,884.96,847.16,790.56,716.46,636.46,539.16,433.06,322.56,226.66,161.26,101.66,56.66,27.56,8.66,-0.04,-0.04,1.46,-1.44,-0.04,-0.04,-0.04,-1.44,1.46,-1.44,-0.04,1.46,1.46,-0.04,-0.04,-2.94,-2.94,1.46,2.86,-0.04,1.46,-0.04,-1.44,-1.44,-5.84,-1.44,-0.04,-1.44,1.46,-1.44,-0.04,-1.44,-0.04,-1.44,1.46,-4.34,-0.04,1.46,1.46,-1.44,-1.44,2.86,-0.04,2.86,-0.04,-1.44,1.46,-0.04,-0.04,-0.04,1.46,1.46,1.46,-0.04,1.46,1.46,-0.04,-1.44,1.46,-0.04,1.46,-1.44,-0.04,-0.04,1.46,-1.44,-0.04,-0.04,-1.44,-0.04,-1.44,-4.34,-0.04,-4.34,-1.44,-0.04,1.46,2.86,-0.04,-0.04,-0.04,-1.44,-0.04,-1.44,-0.04,-1.44,-0.04,-0.04,1.46,-0.04,-0.04,1.46,-1.44,-1.44,-0.04,-1.44,-0.04,-1.44,-2.94,-1.44,-1.44,-1.44,-1.44,1.46,-0.04,-2.94,-0.04,-0.04,-0.04,-1.44,-1.44,-1.44,-0.04,-0.04,-0.04,-5.84,-1.44,-1.44,-0.04,-1.44,-0.04,-0.04,-1.44,2.86,-1.44,-1.44,-0.04,-0.04,-0.04,-0.04,-4.34,2.86,-0.04,-0.04,-0.04,-0.04,2.86,-1.44,1.46,1.46,-0.04,-0.04,-0.04,1.46,-0.04,-0.04,-0.04,-0.04,1.46,-0.04,-0.04,1.46,-1.44,7.26,-1.44,-0.04,-0.04,4.36,-0.04,-0.04,-0.04,-1.44,1.46,-0.04,-0.04,-1.44,-0.04,1.46,-4.34,-0.04,-1.44,-0.04,-1.44,-0.04,1.46,-0.04,1.46,-0.04,-1.44,-0.04,-1.44,-0.04,-1.44,-1.44,-2.94,-0.04,-0.04,1.46,-0.04,2.86,-1.44,1.46,-1.44,-2.94,-1.44,-1.44,-1.44,-0.04,-2.94,-0.04,-0.04,-1.44,1.46,-0.04,-1.44,-1.44,-0.04,7.26,-0.04,-1.44,2.86,-0.04,-1.44,-0.04,-0.04,-1.44,-1.44,1.46,-1.44,-1.44,-2.94,1.46,-1.44,-1.44,-2.94,-0.04,-1.44,-0.04,-0.04,-0.04,-1.44,-0.04,-1.44,2.86,-0.04,1.46,-0.04,-0.04,-0.04,1.46,-0.04,-0.04,-0.04,-0.04,-1.44,-0.04,-0.04,-0.04,-0.04,-0.04,4.36,1.46,4.36,1.46,1.46,-0.04,-1.44,1.46,-0.04,-0.04,-0.04,1.46,-0.04,1.46,-0.04,-0.04,-1.44,1.46,4.36,-2.94,-0.04,-1.44,-0.04,-0.04,-0.04,1.46,-1.44,-1.44,-0.04,-0.04,-0.04,-0.04,-1.44,-0.04,-1.44,-1.44,-0.04,-0.04,-0.04,-0.04,1.46,1.46,-1.44,-1.44,-2.94,4.36,-0.04,1.46,-0.04,1.46,-0.04,-0.04,-0.04,-0.04,-0.04,1.46,-0.04,1.46,1.46,2.86,-1.44,-5.84,1.46,-0.04,-0.04,-0.04,-1.44,1.46 },
		{ 0.1,0.1,0.1,0.1,0.1,0.1,0.8,0.1,0.8,0.1,0.1,0.1,-0.6,0.8,0.1,-2.1,-0.6,0.1,-0.6,-1.4,1.5,0.1,0.1,0.1,0.8,0.1,0.1,0.1,-0.6,1.5,0.8,0.8,0.1,0.8,0.1,-0.6,0.1,1.5,0.1,0.1,0.1,-0.6,-0.6,0.1,0.1,0.1,0.1,0.1,-1.4,-0.6,0.8,0.1,0.8,-0.6,0.1,0.1,0.1,0.1,0.8,-2.1,0.1,-0.6,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,-0.6,0.8,1.5,0.1,2.3,0.1,0.1,0.1,-1.4,0.1,0.1,3,0.1,1.5,0.1,0.8,0.1,0.1,0.1,0.1,0.1,1.5,-0.6,0.1,-0.6,0.1,0.1,0.1,-0.6,0.1,2.3,0.8,0.1,-0.6,0.1,0.1,0.1,0.1,0.1,0.8,-0.6,-0.6,-1.4,-0.6,0.1,0.1,0.1,-1.4,0.1,-0.6,0.1,0.1,0.1,-0.6,0.8,0.1,0.1,1.5,-0.6,-0.6,0.1,0.8,0.1,0.1,0.1,-0.6,0.1,-0.6,0.1,0.1,0.8,0.1,0.8,0.1,0.8,-0.6,0.1,0.8,0.8,0.1,0.1,0.1,0.1,0.1,-0.6,-0.6,0.1,0.8,0.8,-2.8,0.8,0.1,0.1,-1.4,0.1,-0.6,0.1,0.8,0.1,0.1,0.1,2.3,0.1,0.1,1.5,0.1,0.1,0.1,-0.6,0.1,0.8,0.1,-0.6,0.1,-2.1,0.1,0.1,0.1,-0.6,0.1,0.8,0.1,0.1,0.1,-0.6,0.8,0.1,0.1,0.1,-0.6,-0.6,0.8,0.1,-1.4,-0.6,3.7,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,-0.6,-0.6,0.1,-0.6,0.8,0.1,0.1,-0.6,0.1,-0.6,0.1,0.1,0.8,-0.6,0.1,0.8,0.8,0.1,-0.6,-0.6,0.1,0.1,-0.6,0.1,0.8,0.1,0.1,-0.6,0.1,0.1,0.1,0.1,0.1,-0.6,0.1,0.1,-0.6,0.1,0.8,-0.6,0.1,-0.6,-0.6,0.1,0.8,2.3,0.1,0.1,0.1,-0.6,0.1,-0.6,0.1,-0.6,0.1,0.8,0.8,-0.6,0.8,-0.6,-1.4,0.1,0.1,0.8,0.1,0.8,0.1,0.1,0.1,0.1,0.1,0.1,-1.4,0.8,0.1,0.1,-0.6,0.8,-0.6,0.1,0.1,-0.6,-0.6,0.1,0.8,-0.6,0.1,0.8,0.1,0.1,2.3,0.1,-0.6,0.1,0.1,2.3,-0.6,0.1,0.1,-0.6,-0.6,1.5,0.1,0.1,0.1,0.1,0.1,-0.6,0.1,-0.6,0.1,0.1,0.1,0.1,0.1,0.1,-2.8,0.1,-0.6,0.1,0.8,-0.6,0.1,-0.6,0.1,0.1,-0.6,0.1,2.3,0.1,-4.3,-0.6,0.1,-0.6,0.1,0.1,0.1,0.1,0.8,0.1,0.8,-1.4,0.1,0.1,0.1,0.1,0.1,-0.6,0.1,0.1,0.1,0.1,0.1,2.3,0.1,0.1,-1.4,0.1,-0.6,0.1,0.1,-0.6,0.8,0.1,0.1,-0.6,0.1,0.1,0.8,-0.6,0.1,0.1,-0.6,-1.4,-0.6,0.1,0.1,-0.6,0.1,-1.4,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,-0.6,0.8,-0.6,0.1,0.1,0.1,0.1,-2.1,0.1,0.1,-0.6,0.1,-0.6,0.1,-0.6,0.1,0.1,0.1,0.1,0.1,-0.6,0.1,-1.4,0.8,-3.6,-0.6,0.8,0.1,0.8,0.1,1.5,0.1,-0.6,0.8,-0.6,0.1,-1.4,0.1,-0.6,0.1,-0.6,0.1,1.5,0.1,0.1,0.1,-0.6,-2.8,0.1,-0.6,0.1,0.1,0.1,-1.4,0.1,0.8,0.1,0.8,0.1,-1.4,0.1,0.8,1.5,0.1,0.1,0.1,0.1,0.1,0.1,0.1,-0.6,0.1,-0.6,0.1,0.1,0.1,0.1,-0.6,0.1,0.1,0.1,0.1,0.1,-2.1,0.1,0.1,0.1,0.1,0.1,0.8,0.1,-0.6,0.1,0.8,0.1,0.8,0.1,0.1,0.1,0.1,0.8,0.1,0.1,0.1,0.8,-0.6,0.1,0.1,-0.6,0.1,0.1,0.1,0.1,0.1,0.1,-0.6,-0.6,0.1,0.1,-0.6,-0.6,0.1,0.1,0.1,0.1,1.5,0.1,0.8,0.1,0.1,-0.6,-0.6,0.1,-0.6,0.1,-0.6,-0.6,0.1,-0.6,0.1,-0.6,-0.6,0.1,0.1,0.1,0.1,0.1,0.1,0.8,0.1,0.1,0.1,0.1,0.8,0.1,0.1,0.1,0.1,-0.6,0.8,-1.4,0.8,0.1,-0.6,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,-0.6,0.1,0.8,1.5,0.1,-0.6,-0.6,0.1,0.1,-0.6,0.1,0.1,0.1,-0.6,-0.6,0.1,-0.6,-0.6,-0.6,0.1,0.1,-0.6,-0.6,0.1,-0.6,0.8,-1.4,0.1,-0.6,0.1,0.1,0.1,-0.6,0.8,0.1,0.1,0.1,0.1,0.1,0.1,-0.6,0.8,-0.6,-0.6,0.1,-1.4,0.1,-0.6,-0.6,0.1,0.8,-0.6,0.1,0.1,0.1,-0.6,0.1,-0.6,-0.6,-0.6,0.1,0.1,-0.6,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,-0.6,0.1,-0.6,0.1,0.1,0.1,-0.6,0.1,0.1,0.1,-1.4,0.1,0.1,0.1,0.1,0.1,-0.6,0.1,0.1,2.3,-1.4,-2.8,0.1,0.8,0.1,0.1,0.8,0.1,-0.6,0.1,0.1,-0.6,0.1,0.1,0.1,0.1,-0.6,-0.6,-0.6,-0.6,0.1,0.1,0.8,0.1,-0.6,0.1,-0.6,0.1,-0.6,0.1,-0.6,0.1,-0.6,0.8,0.1,0.8,0.1,0.8,0.1,-0.6,-0.6,0.1,0.1,2.3,0.1,0.1,0.1,3.7,13.2,10.3,11.8,11,8.8,7.4,6.7,3.7,4.5,5.9,3,5.9,3.7,7.4,8.1,6.7,6.7,6.7,5.2,5.2,3.7,3,3,0.8,0.1,-0.6,-1.4,-2.8,-2.1,-0.6,-4.3,-5,-5.8,-7.2,-8,-8.7,-9.4,-10.1,-9.4,-10.1,-12.3,-13.1,-13.8,-13.8,-16,-16,-16.7,-16.7,-17.4,-16.7,-18.2,-16.7,-18.2,-16,-16,-15.3,-16.7,-16,-15.3,-15.3,-13.8,-12.3,-13.1,-10.9,-10.1,-8.7,-8,-6.5,-5,-1.4,-2.1,-2.1,-1.4,-0.6,-0.6,-0.6,0.1,0.1,0.1,0.8,0.1,0.1,0.1,0.8,0.1,0.1,0.1,0.1,2.3,0.1,2.3,0.1,1.5,0.1,0.1,0.8,-0.6,0.1,-0.6,0.1,0.1,0.1,0.8,0.8,-0.6,-0.6,-0.6,-2.1,0.1,-0.6,0.8,0.8,0.1,0.1,0.1,0.8,0.1,-0.6,-0.6,1.5,0.1,0.1,0.1,-2.8,0.1,0.1,2.3,0.1,0.1,0.8,-0.6,-0.6,-1.4,0.8,-0.6,-0.6,-0.6,0.8,0.1,0.1,-0.6,0.1,0.1,-0.6,0.1,-0.6,0.1,-2.1,0.1,-1.4,0.1,0.8,0.1,0.8,0.1,0.1,-0.6,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.8,-1.4,1.5,0.8,0.1,-2.1,0.8,0.1,0.8,0.1,0.1,0.1,-0.6,0.1,0.8,0.1,0.1,0.1,0.1,-0.6,2.3,0.1,0.1,0.1,0.8,-0.6,0.1,-0.6,0.8,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,-2.8,0.8,-2.1,1.5,0.1,0.8,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,-0.6,0.1,0.1,0.1,-0.6,0.1,-1.4,0.8,-0.6,0.1,-0.6,-0.6,0.1,0.1,0.1,0.8,0.1,0.8,-0.6,0.1,0.1,0.8,0.1,0.8,0.8,0.1,-1.4,0.8,0.8,0.8,0.1,0.8,0.1,0.8,0.1,0.1,-0.6,0.1,-0.6,0.1,0.1,0.8,-1.4,0.1,-2.1,-0.6,-0.6,0.1,0.1,1.5,-0.6,0.8,0.1,0.1,-0.6,0.8,0.1,0.1,-0.6,0.8,-0.6,0.1,0.1,0.8,3,0.1,3.7,0.1,0.8,0.1,2.3,-0.6,-1.4,0.1,-3.6,0.8,-0.6,0.8,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.8,0.1,0.1,0.1,0.8,-0.6,0.8,-0.6,0.1,0.1,0.8,-0.6,0.1,-0.6,0.1,0.1,0.1,0.8,0.1,-1.4,0.1,-0.6,0.1,-0.6,0.1,-1.4,0.1,-0.6,0.1,0.1,0.8,0.8,0.8,-0.6,-0.6,0.1,1.5,0.1,0.1,-1.4,1.5,-0.6,1.5,-0.6,0.1,-0.6,-2.1,0.8,0.1,0.1,0.1,0.1,0.1,0.1,1.5,-1.4,0.1,0.1,0.1,0.1,0.1,0.8,0.1,0.1,-0.6,-0.6,0.8,0.1,-0.6,0.1,0.1,0.1,-0.6,-1.4,1.5,-0.6,0.1,-0.6,0.8,-0.6,-1.4,-0.6,0.1,0.1,2.3,-0.6,0.8,0.1,0.1,-0.6,1.5 },
		{ 0.53,-0.17,-0.17,-0.17,-0.17,-0.87,0.53,-0.17,-0.17,-0.17,0.53,-0.17,0.53,0.53,0.53,-0.17,0.53,-0.17,0.53,0.53,1.23,-0.17,1.23,-0.17,1.23,-0.17,-0.17,-0.17,0.53,-0.17,-0.17,3.43,-0.17,-0.17,-0.17,-0.17,-0.17,-0.87,0.53,-0.17,-0.17,-0.17,0.53,-0.87,0.53,-0.87,0.53,-0.17,1.23,-0.17,-0.17,-0.17,1.23,-0.17,0.53,-0.17,-0.87,-0.17,-0.17,-0.17,0.53,1.23,0.53,-0.87,-0.17,0.53,0.53,1.23,0.53,-0.17,-0.17,-0.87,-0.17,-3.07,-0.17,0.53,0.53,-0.17,-0.87,0.53,0.53,0.53,-0.17,0.53,0.53,-0.17,-0.17,-0.87,-0.17,0.53,2.03,-0.17,-0.17,-0.17,0.53,-0.17,0.53,-0.17,-0.17,-0.17,0.53,-0.17,-0.17,0.53,-0.17,-0.87,-1.67,-0.17,0.53,-0.17,0.53,0.53,0.53,0.53,0.53,-0.17,2.03,-0.17,1.23,0.53,-0.17,-0.17,-0.17,-0.17,-0.87,-0.87,0.53,-0.17,1.23,-0.87,-0.87,0.53,0.53,0.53,0.53,-0.17,0.53,0.53,0.53,-0.17,0.53,-0.17,-0.17,-0.87,0.53,-0.87,0.53,-0.17,-0.17,-0.87,0.53,-0.17,0.53,-0.87,1.23,0.53,2.03,-0.17,-0.87,-0.87,-0.17,-0.17,1.23,-0.17,0.53,0.53,0.53,-0.17,-0.17,-0.17,-0.17,-0.17,2.03,0.53,-0.87,0.53,-0.17,-0.87,-0.17,-0.87,-0.17,1.23,-0.17,-1.67,0.53,-0.87,-0.17,-0.17,-0.87,-0.17,-0.17,-0.17,-0.17,-0.17,-3.07,-0.87,1.23,-0.17,0.53,0.53,2.03,-0.17,1.23,-0.17,0.53,-0.17,0.53,-0.17,1.23,-0.87,-0.17,-0.17,0.53,-0.17,0.53,-0.17,0.53,2.73,-0.17,-0.17,-0.17,-0.17,0.53,-0.87,0.53,-0.87,0.53,0.53,0.53,-0.87,0.53,-0.17,-0.87,-0.17,0.53,-0.17,0.53,-0.17,0.53,0.53,-3.07,-0.17,1.23,-0.17,0.53,-0.17,-0.17,-0.17,-0.17,0.53,0.53,-0.17,-0.17,-0.17,2.03,-0.17,-0.17,-0.17,0.53,-4.57,-0.17,-2.37,0.53,-0.17,0.53,2.03,-0.17,-0.17,0.53,-0.17,0.53,0.53,-0.17,-0.17,0.53,4.23,-0.17,-2.37,0.53,2.73,-0.87,-0.17,-0.17,-0.17,-0.17,-0.87,-0.17,0.53,0.53,0.53,-0.17,-0.17,0.53,-0.17,-0.17,1.23,0.53,-0.87,1.23,0.53,0.53,0.53,-0.17,0.53,-1.67,0.53,-0.17,-0.17,0.53,0.53,1.23,0.53,-3.07,-0.17,-0.17,2.03,-0.87,0.53,0.53,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,0.53,0.53,0.53,-0.17,0.53,-0.17,0.53,-0.17,-0.17,-0.87,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.87,-1.67,-1.67,0.53,-0.17,-0.87,-0.17,-0.17,0.53,-1.67,-0.87,-0.87,-0.17,0.53,0.53,-0.17,0.53,-0.87,-0.17,-0.17,-0.17,0.53,-0.17,-0.17,-0.17,1.23,-1.67,-0.87,-1.67,-0.17,-0.87,-0.17,0.53,-0.17,1.23,-0.17,-0.87,0.53,1.23,0.53,0.53,-0.87,-0.17,-3.07,0.53,-0.17,0.53,2.73,0.53,-0.17,-0.17,0.53,0.53,0.53,-0.17,-0.17,0.53,-0.17,0.53,-0.17,-0.17,-0.17,-0.87,-0.17,1.23,-0.17,-0.17,-0.17,-0.17,-0.17,-0.87,0.53,-0.17,-0.17,1.23,-0.17,1.23,-0.17,0.53,-0.17,0.53,-0.17,0.53,0.53,0.53,-0.87,0.53,-0.87,-0.17,-0.17,-0.17,-0.17,-0.17,0.53,-0.17,-0.17,0.53,-0.87,-0.17,-0.17,-0.17,-0.17,-0.17,-1.67,-0.17,-0.87,-0.17,-0.17,0.53,-0.17,0.53,-0.17,-0.87,-0.17,1.23,-0.17,-0.17,-0.17,-0.87,-0.87,0.53,-0.17,-0.17,2.73,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,1.23,0.53,-0.17,-0.87,-0.17,0.53,-0.17,-0.17,0.53,-0.17,-0.87,-0.17,1.23,-0.17,0.53,-0.17,0.53,-0.17,-0.17,-0.17,-0.17,0.53,0.53,-0.17,-0.17,0.53,-0.17,-0.17,-0.17,-2.37,-1.67,0.53,-0.17,-0.17,-0.17,-0.17,0.53,0.53,1.23,0.53,-0.17,0.53,-0.17,0.53,-0.17,-0.17,2.73,-0.17,0.53,0.53,-0.17,-0.17,0.53,-0.17,-0.17,0.53,-0.17,0.53,1.23,0.53,-0.17,-0.17,-0.87,-0.17,-0.17,0.53,-0.17,-2.37,-0.17,-0.17,0.53,0.53,-0.17,0.53,-0.17,0.53,-0.17,-0.17,0.53,-0.17,0.53,-0.17,0.53,-0.17,0.53,0.53,-0.17,-0.17,0.53,0.53,-0.17,-0.17,-0.87,-0.17,-0.17,-0.17,-0.17,-0.17,0.53,-0.17,-0.17,0.53,-0.17,-0.17,-0.17,-0.87,0.53,0.53,0.53,-0.87,-0.17,-0.17,0.53,-0.17,-0.17,0.53,-0.17,-0.17,0.53,-0.17,-0.17,-0.17,-0.17,-0.87,-1.67,-0.17,0.53,-0.17,0.53,-0.17,0.53,-0.17,-0.87,0.53,-0.87,-0.17,0.53,-0.87,-0.17,-0.87,0.53,0.53,-0.17,-0.17,-0.17,-0.17,-0.17,-0.87,-0.17,-0.17,-0.17,0.53,-0.17,0.53,-0.17,-0.17,-0.17,0.53,-0.17,-0.17,-0.87,-0.17,-0.87,-0.17,0.53,-0.17,-0.17,-0.17,0.53,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,0.53,-0.17,0.53,-0.17,-0.17,-0.17,0.53,1.23,-0.17,-0.17,-0.17,-0.17,-0.17,-0.87,-0.17,0.53,-0.17,0.53,-0.17,0.53,0.53,-0.17,0.53,-0.17,-0.17,-0.17,-0.17,0.53,-0.87,-0.17,0.53,-0.17,0.53,-0.17,-0.17,-0.17,-0.17,-0.17,0.53,-0.17,0.53,-0.87,-0.17,-0.87,0.53,0.53,-0.17,0.53,-0.17,-0.17,0.53,-0.17,0.53,-0.17,0.53,-0.17,0.53,-0.17,0.53,-0.17,-0.17,-1.67,-0.17,-0.17,1.23,-0.17,1.23,-0.17,-1.67,0.53,-0.17,-0.17,2.73,-0.17,0.53,-0.87,-27.97,-90.07,-66.67,-72.47,-69.57,-71.07,-76.87,-76.17,-74.67,-79.77,-83.47,-90.07,-97.37,-102.47,-108.27,-111.17,-114.17,-117.07,-113.37,-112.67,-108.97,-105.37,-100.27,-95.87,-90.77,-87.07,-82.67,-78.37,-72.47,-71.07,-66.67,-64.47,-60.77,-60.07,-56.47,-54.27,-52.07,-49.17,-47.67,-45.47,-41.07,-41.77,-40.37,-38.17,-35.97,-33.77,-30.87,-26.47,-27.97,-25.77,-25.77,-25.77,-24.97,-23.57,-22.07,-21.37,-19.87,-19.17,-16.27,-14.77,-11.17,-10.37,-8.17,-6.07,-3.87,-1.67,-1.67,-2.37,-4.57,-0.17,2.73,4.23,4.23,2.73,1.23,0.53,1.23,-0.87,-0.17,-0.17,0.53,-0.17,-2.37,-0.17,-1.67,0.53,-0.17,-0.17,0.53,1.23,-0.17,-1.67,-0.17,-0.17,0.53,-0.17,0.53,-0.17,0.53,0.53,-0.17,-0.87,-0.17,-0.17,-0.17,0.53,-0.17,0.53,-0.17,-0.17,1.23,-0.17,0.53,-0.17,0.53,-0.17,0.53,-0.17,0.53,-0.17,0.53,0.53,-0.17,-0.87,0.53,-2.37,-0.17,0.53,0.53,-0.87,0.53,-0.17,0.53,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,0.53,0.53,-3.07,-0.17,-0.17,-0.87,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,0.53,0.53,0.53,-0.17,0.53,-0.17,-0.17,-0.17,0.53,-0.17,-0.17,-3.87,-0.17,-1.67,0.53,0.53,0.53,-1.67,0.53,-0.17,0.53,-0.17,0.53,-0.17,0.53,-0.17,0.53,-0.87,-0.17,-0.17,1.23,-0.17,1.23,0.53,-0.17,-0.17,0.53,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,0.53,0.53,-0.17,0.53,-0.17,1.23,-0.17,-0.17,-1.67,-0.17,-0.87,-0.17,0.53,-0.17,-0.87,-0.17,-0.17,-0.17,-0.17,0.53,2.03,0.53,-0.87,-0.87,-0.17,2.73,-0.17,-0.17,-0.17,2.03,-0.17,-0.17,-0.17,1.23,-0.17,0.53,-0.17,0.53,-0.17,0.53,-0.17,1.23,-0.17,0.53,-0.17,0.53,3.43,-0.17,-0.87,-0.87,-0.17,-0.17,-0.17,-0.17,-0.87,0.53,-0.17,-0.17,-0.17,-0.87,-0.17,-0.17,-0.17,-0.17,-0.87,-2.37,-0.17,-1.67,-0.17,0.53,-0.17,0.53,-0.17,-0.17,-0.87,0.53,-0.17,0.53,-0.87,-0.17,-0.17,-0.87,0.53,0.53,-1.67,-0.17,-0.17,1.23,-0.17,0.53,-0.17,-0.17,2.03,1.23,0.53,1.23,-0.87,-0.17,0.53,-0.17,-0.17,2.03,-0.87,1.23,-0.17,2.03,-0.17,-0.17,-0.87,0.53,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-0.17,-3.87,0.53,-1.67,-0.17,-1.67,0.53,-0.17,0.53,0.53,0.53,1.23,-0.17,-0.17,0.53,-0.17,-0.17,-0.87,1.23,-2.37,2.03,-0.87,3.43,-0.17,2.03,-0.17,-0.17,-0.17,-0.87,-0.17,0.53,-0.17,3.43,-0.87,0.53,-0.17,0.53,-0.87,0.53,-0.17,-0.87,-0.17,0.53,-0.87,0.53,-0.87,-0.17,0.53,-0.17,-1.67,0.53,-0.17,-0.17,-0.17,0.53,-0.17,1.23,-0.17,-0.17,-0.17,-1.67,-0.17,2.03,-0.17,-0.17,-0.17,0.53,-0.17,-0.17,-0.17,-0.87,-0.17,-0.17,-0.87,-0.17,-0.17,-0.87 },
		{ -0.35,0.05,-0.35,0.05,-0.65,0.05,1.25,0.05,0.45,0.05,0.45,-0.35,0.05,-0.35,0.05,-0.35,0.05,-0.35,0.05,-0.35,0.05,-0.35,0.05,0.05,0.45,-0.35,0.05,0.05,0.05,0.05,0.45,-0.35,0.05,0.05,0.45,0.05,0.45,0.05,0.45,-0.35,0.05,-0.35,0.05,0.05,0.05,-0.35,0.45,0.05,0.05,-0.35,0.05,-0.35,1.65,0.05,0.45,-0.35,0.05,-0.35,0.05,-0.65,0.05,0.05,0.05,0.05,0.45,-1.45,0.05,0.45,0.05,0.05,0.45,0.05,0.05,0.05,0.05,0.45,0.05,0.05,1.65,0.05,0.45,0.05,0.45,-0.65,0.45,0.05,0.45,-0.35,0.05,0.05,-0.65,0.05,0.45,-1.05,0.05,-0.35,0.05,0.05,0.05,-0.35,0.85,0.45,0.05,-0.35,0.05,0.05,0.05,-0.35,0.05,-0.35,0.45,-0.35,0.05,0.05,0.05,0.05,0.05,-0.35,-0.35,0.05,1.65,-0.35,0.05,-0.35,0.05,0.05,-1.05,-0.35,-0.35,0.05,0.05,1.25,0.05,-0.35,0.05,0.05,0.45,-0.35,0.05,-0.35,0.05,0.45,0.05,0.05,0.05,0.45,0.05,0.05,0.45,0.45,0.05,0.05,-0.35,0.05,0.05,-0.35,-0.65,0.05,1.25,0.05,-0.65,-0.35,-0.35,-0.35,0.45,-0.35,-0.35,0.05,0.45,0.05,0.05,1.25,0.45,0.05,0.45,-0.35,0.05,-0.35,0.05,0.05,0.45,0.45,0.45,0.45,0.05,-0.65,0.45,0.05,-0.35,0.05,0.45,-0.35,0.45,0.05,-1.05,0.05,0.45,-0.35,0.05,-0.35,0.05,0.05,0.05,0.05,0.05,-0.35,0.85,0.05,0.05,-0.65,0.45,-1.45,0.05,0.45,0.45,-0.35,0.05,-0.65,0.05,0.05,0.45,-0.35,0.05,-0.35,0.45,-0.35,0.45,-0.35,0.45,0.05,0.05,0.05,-0.65,0.05,-1.85,-0.35,0.05,-0.35,0.85,-0.35,-0.65,-0.35,-0.65,0.05,0.05,-0.35,0.45,0.45,0.45,0.05,0.05,0.05,0.05,-1.05,0.45,0.05,0.05,0.05,0.45,-0.35,0.05,-0.35,0.45,1.25,0.05,0.45,0.05,0.05,0.45,-0.35,0.45,-0.35,0.05,0.45,0.45,0.05,0.45,-0.35,0.05,0.05,-0.35,0.05,0.05,0.05,0.05,-0.35,0.05,1.65,0.05,0.05,0.05,0.05,0.05,-0.35,0.05,0.45,0.05,0.05,0.05,0.05,-0.35,-0.35,-0.35,-0.35,0.05,-0.35,0.05,-0.35,0.05,0.05,0.45,-1.85,-1.05,0.05,0.05,-0.35,0.05,0.05,0.05,0.05,0.05,0.05,-0.35,0.45,-0.35,0.05,0.45,-0.35,0.45,0.05,-0.65,-0.35,0.45,0.05,0.05,0.05,0.05,-0.35,-0.35,0.05,0.05,-0.35,0.45,-0.35,0.45,0.05,0.45,-0.35,0.05,0.05,-0.65,-1.85,0.05,0.05,0.05,0.05,0.45,-0.35,0.05,-0.35,0.05,-0.35,0.45,0.05,0.45,-1.05,-0.65,0.05,0.05,0.05,0.45,0.05,0.05,-0.35,-0.35,-0.35,0.45,0.05,0.05,-0.35,0.05,-0.35,0.05,0.05,0.45,-0.35,0.05,-0.35,0.45,-0.35,0.85,-0.65,0.05,-0.35,0.05,-0.35,0.05,0.05,0.05,-0.35,0.45,0.05,0.05,-0.35,0.05,-0.35,0.05,-0.35,-0.35,-0.35,0.45,0.05,0.45,-0.35,0.05,0.05,0.05,-0.35,0.05,-0.35,0.05,0.05,0.05,0.05,0.05,-0.35,0.05,0.45,0.05,1.95,0.05,-1.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.45,-0.35,-0.35,0.05,0.85,0.05,0.05,0.45,0.05,0.05,0.05,0.05,0.05,0.05,0.45,0.05,0.05,0.05,0.05,0.05,0.85,0.05,0.05,0.05,-0.35,0.05,0.45,-0.65,0.05,0.05,0.05,-0.35,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.45,0.05,0.45,0.05,0.05,0.05,-0.35,0.05,-0.35,0.05,0.05,0.05,0.05,0.05,0.45,-0.35,0.05,0.45,0.05,0.05,0.05,-0.35,0.05,0.05,0.05,0.45,0.05,0.05,0.05,0.85,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.45,0.05,-0.35,-0.35,0.85,0.05,0.45,0.05,-0.35,0.05,0.05,0.05,0.45,-0.35,0.05,0.05,0.45,-0.35,0.05,0.05,0.05,-0.35,0.05,0.45,0.45,-0.65,0.05,0.45,0.05,0.05,-0.35,-0.35,0.45,0.05,0.05,0.05,0.05,-0.65,0.05,0.05,0.45,-0.35,0.05,0.05,-0.35,0.05,0.45,-0.35,0.05,-0.35,0.45,0.05,0.05,0.05,0.05,0.05,0.05,-0.35,0.45,0.05,0.05,0.85,-0.35,1.25,0.45,-0.65,0.05,-0.35,0.05,0.05,-0.35,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.85,0.05,-1.05,0.05,0.05,0.05,0.05,0.05,0.45,-0.35,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,-0.35,0.05,-0.35,0.05,0.05,0.05,-0.35,0.45,0.05,0.05,0.05,0.05,0.05,0.05,0.45,0.05,0.05,0.05,-0.35,-0.35,0.05,0.05,0.05,0.45,0.05,0.05,-0.35,0.05,0.05,-0.35,0.05,0.05,0.05,0.05,0.05,0.05,-0.65,0.05,0.45,-0.35,0.05,0.45,0.05,0.05,0.05,0.05,-0.35,0.05,-0.35,0.05,0.05,0.05,-0.35,-0.35,0.05,-0.35,0.05,-0.35,0.05,0.05,-0.35,0.05,-0.35,0.05,-0.35,0.05,0.05,-0.35,0.05,-0.35,-0.35,0.05,0.05,0.05,0.05,0.05,-0.35,0.05,-0.35,0.45,0.05,0.45,0.05,0.45,0.05,0.05,-0.35,0.05,0.05,0.05,0.05,0.05,-0.35,-0.35,-0.35,0.45,-0.35,0.85,0.05,0.45,0.05,0.05,0.05,0.45,0.05,0.05,0.05,0.05,0.45,0.85,8.15,9.25,7.75,6.15,3.95,0.45,0.05,-1.05,-1.85,-1.05,-2.95,-3.35,-3.75,-3.75,-4.55,-4.55,-6.05,-4.85,-4.85,-5.25,-5.65,-5.25,-5.65,-5.25,-4.85,-4.85,-4.55,-4.15,-4.15,-2.95,-2.95,-1.45,-2.55,-2.25,-2.55,-1.45,-1.45,-1.45,-1.45,-1.05,-0.35,0.05,-0.35,0.05,0.85,0.85,1.25,1.25,0.45,1.95,1.95,2.35,2.35,2.75,2.35,2.75,2.35,1.65,1.65,1.95,1.65,1.25,1.65,1.95,1.25,1.65,1.65,2.35,1.95,2.35,1.25,1.25,0.05,0.45,0.45,0.05,0.05,0.05,0.05,0.05,-0.65,0.45,-1.05,0.45,-1.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,-0.35,0.05,-1.05,0.45,0.05,-0.65,0.05,0.05,0.05,-0.35,-0.35,0.45,0.05,0.05,0.05,0.05,-0.35,0.05,0.05,0.05,0.05,0.45,-0.35,0.05,-0.35,0.05,-0.65,0.05,-0.65,0.45,0.45,0.05,-0.35,0.45,0.05,0.05,0.05,0.05,0.05,0.05,-0.35,0.45,0.05,0.85,-0.65,-0.35,-0.35,-0.65,-0.35,0.05,0.05,-0.35,-0.35,0.05,0.45,0.05,0.05,0.05,0.05,0.45,-0.35,0.85,0.45,0.45,0.05,0.45,-0.35,0.05,0.45,-0.35,0.05,0.45,-0.35,0.05,0.05,0.05,0.05,0.05,-0.35,0.45,-0.35,0.05,0.05,0.05,-0.35,-0.35,0.05,0.85,0.05,0.45,-0.35,0.05,0.05,0.45,-0.35,-0.35,-0.35,0.45,0.05,-0.65,0.05,-0.35,-0.35,0.45,0.05,0.05,0.05,0.05,0.05,0.05,-0.35,0.05,0.05,0.05,-0.35,0.45,0.05,0.05,0.05,0.05,1.95,0.45,-0.35,0.45,0.05,1.95,0.05,0.45,-0.35,0.45,-0.35,0.45,-0.35,0.05,-0.35,-0.35,0.05,0.05,0.05,0.05,-0.65,0.45,0.05,0.05,0.05,0.05,-0.35,0.45,-0.35,0.05,-0.35,0.05,0.05,0.05,0.05,-0.35,0.05,0.45,0.05,0.05,0.05,0.85,-0.35,0.05,-0.65,0.45,-0.35,0.45,-0.35,0.05,0.05,0.05,-0.35,0.45,-0.35,0.05,0.05,0.05,-0.35,0.05,0.05,-0.35,-1.05,0.05,-0.65,0.05,0.45,0.05,0.05,0.05,0.05,0.05,-0.35,0.05,-0.35,0.05,-0.65,0.05,-0.35,0.05,-0.35,1.95,0.05,-0.35,-0.35,-0.35,-0.35,0.05,0.05,0.05,-0.35,0.05,-0.35,0.05,0.05,0.05,0.05,0.45,-0.35,0.45,0.85,0.05,-0.35,-0.35,-0.35,0.05,0.05,0.05,-0.35,0.05,0.05,0.05,0.05,0.45,-0.35,0.45,0.05,-1.05,-0.35,-1.45,1.25,-0.35,-0.35,0.45,0.05,0.05,-0.35,0.45,-0.35,0.05,-0.35,0.05,0.05,-0.35,-0.35,0.05,-0.35,0.05,-0.35,0.05,1.25,0.85,-0.65,0.05,-0.35,0.05,0.05,0.05,0.05,0.05,0.45,0.05,-0.35,0.05,-0.35,0.45,0.05,-1.85,-0.35,-1.05,0.45,0.05,0.05,0.05,-0.35,0.05,-0.35,0.05,0.05,0.05,-0.35,0.45,-0.35,0.05,0.05,0.85,0.45,0.85 }
	};

	const char* moxie_forcechannel_name[moxie_numforcechannels] = 
	{
		"FP - Force X",
		"FP - Force Y",
		"FP - Force Z",
		"FP - Moment X",
		"FP - Moment Y",
		"FP - Moment Z",
	};
}