/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "XMLReadingMachineLegacy.h"
#include <pugixml.hpp>

namespace Open3DMotion
{
	XMLReadingMachineLegacy::XMLReadingMachineLegacy() :
		legacy(legacy_none)
	{
	}

	TreeValue* XMLReadingMachineLegacy::ReadValue(const pugi::xml_node& xml_element) throw(XMLReadException)
	{
		// element to remap
		pugi::xml_node remapped_element = xml_element;

		// if already within compound item, remap accordingly
		if (legacy == legacy_time_sequence || legacy == legacy_event_group)
		{
			if (xml_element.type() == pugi::node_element)
			{
				// get type string
				pugi::xml_attribute type_node = xml_element.attribute("type");
				std::string type_string( type_node.as_string() );

				// override element name for Dimensions -> Dimension
				std::string name( xml_element.name() );
				if (name == "Dimensions")
				{
					name = "Dimension";
					remapped_element.set_name(name.c_str());
				}

				// override type string for known element types
				if (type_string.size() == 0)
				{		
					// known element types
					if (name.compare("Data") == 0)
						type_string = "base64";
					else if (name.compare("Start") == 0)
						type_string = "double";
					else if (name.compare("Rate") == 0)
						type_string = "double";
					else if (name.compare("Frames") == 0)
						type_string = "long";
					else if (name.compare("Dimension") == 0)
						type_string = "long";
					else if (name.compare("Bytes") == 0)
						type_string = "long";

					// set new type
					if (type_string.size())
					{
						remapped_element.append_attribute("type") = type_string.c_str();
					}
				}
			}
		}

		// store previous compound element legacy status
		LegacyCompoundType previous_legacy = legacy;

		// look for legacy tags for compound elements and set status
		if (xml_element.type() == pugi::node_element)
		{
			if (std::string("timesequence").compare(xml_element.attribute("type").as_string()) == 0)
			{
				legacy = legacy_time_sequence;
				remapped_element.remove_attribute("type");
			}
			else if (std::string("eventgroup").compare(xml_element.attribute("type").as_string()) == 0)
			{
				legacy = legacy_event_group;
				remapped_element.remove_attribute("type");
			}
		}

		// read data
		TreeValue* result = XMLReadingMachine::ReadValue(remapped_element);

		// flag end legacy compound item
		legacy = previous_legacy;

		return result;
	}
}
