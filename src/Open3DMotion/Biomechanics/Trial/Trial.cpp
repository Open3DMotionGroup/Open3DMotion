/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/Trial.h"

namespace Open3DMotion
{
	const char TrialSection::timesequences_list_name[] = "Sequences";
	const char TrialSection::timesequences_element_name[] = "Sequence";
	const char TrialSection::eventgroups_list_name[] = "EventGroups";
	const char TrialSection::eventgroups_element_name[] = "EventGroup";
	const char TrialSectionAcq::TSGroupMarker[] = "Marker";
	const char TrialSectionAcq::TSGroupAnalog[] = "Analog";

	TrialSection::TrialSection() :
		TimeSequences(timesequences_element_name),
		EventGroups(eventgroups_element_name)
	{
		Register(timesequences_list_name, &TimeSequences);
		Register(eventgroups_list_name, &EventGroups);
	}

	const TimeSequence* TrialSection::GetTS(const char* groupname, const char* channelname) const
	{
		size_t numts = TimeSequences.NumElements();
		for (size_t i = 0; i < numts; i++)
		{
			const TimeSequence& ts = TimeSequences[i];
			if (ts.Group.Value().compare(groupname) == 0 && ts.Channel.Value().compare(channelname) == 0)
				return &ts;
		}
		return NULL;
	}

	const EventGroup* TrialSection::GetEventGroup(const char* name) const
	{
		size_t numgroups = EventGroups.NumElements();
		for (size_t i = 0; i < numgroups; i++)
		{
			const EventGroup& eg = EventGroups[i];
			if (eg.Name.Value().compare(name) == 0)
				return &eg;
		}
		return NULL;
	}

	void TrialSection::GetTSGroup(std::vector<const TimeSequence*>& tsgroup, const char* groupname) const
	{
		size_t numts = TimeSequences.NumElements();
		for (size_t i = 0; i < numts; i++)
		{
			const TimeSequence& ts = TimeSequences[i];
			if (ts.Group.Value().compare(groupname) == 0)
				tsgroup.push_back(&ts);
		}
	}

	bool TrialSection::GetTSIndexByHardwareID(size_t& index, const char* groupname, Int32 hardwareID) const
	{
		size_t numts = TimeSequences.NumElements();
		for (size_t i = 0; i < numts; i++)
		{
			const TimeSequence& ts = TimeSequences[i];
			if (ts.HardwareID.IsSet() && (ts.Group.Value().compare(groupname) == 0) && (ts.HardwareID.Value() == hardwareID))
			{
				index = i;
				return true;
			}				  
		}

		index = 0;
		return false;
	}

}
