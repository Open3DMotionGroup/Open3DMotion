/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "ForcePlate.h"

namespace Open3DMotion
{
	const char ForcePlate::TypeAMTI[] = "AMTI";
	const char ForcePlate::TypeKistler[] = "Kistler";

	ForcePlate::ForcePlate() :
		Outline("Corner", 4),
		Calibration("value"),
    Channels("Channel"),
		COPCorrection("value"),
		PreCompCoPChannels("Channel")
	{
		REGISTER_MEMBER(Type);
		REGISTER_MEMBER(Model);
		REGISTER_MEMBER(Outline);
		REGISTER_MEMBER(Calibration);
		REGISTER_MEMBER(SensorSeparation);
		REGISTER_MEMBER(COPCorrection);
		REGISTER_MEMBER(CentreOffset);
		REGISTER_MEMBER(Channels);
		REGISTER_MEMBER(PreCompCoPChannels);
	}

	ForcePlate fp;

}
