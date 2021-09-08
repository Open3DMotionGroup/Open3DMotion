/*--
  Open3DMotion 
  Copyright (c) 2004-2021.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "ReadWriteXMLCompound.h"
#include "XMLReadingMachine.h"
#include "XMLWritingMachine.h"
#include "Open3DMotion/Biomechanics/Trial/TimeSequence.h"
#include "Open3DMotion/Biomechanics/Trial/EventGroup.h"
#include <set>
#include <pugixml.hpp>
#include <memory>

namespace Open3DMotion
{
	ReadWriteXMLCompound::ReadWriteXMLCompound(bool _extended) :
		extended(_extended)
	{
	}

	void ReadWriteXMLCompound::WriteValue(XMLWritingMachine& writer, const TreeValue* value) const
	{
		const TreeCompound* compoundobject = static_cast<const TreeCompound*>(value);

		bool use_data_extended =
			extended &&
			(compoundobject->Get("Data") != NULL) &&
			((compoundobject->Get(TimeSequence::StructureName) != NULL) ||
  		 (compoundobject->Get(EventGroup::StructureName) != NULL));

		for (size_t index = 0; index < compoundobject->NumElements(); index++)
		{
			const TreeCompoundNode* node = compoundobject->Node(index);
			
			if (use_data_extended)
			{
				if (node->Name() == "Data")
				{
					// Replace Data with DataExt and extract direct values
					writer.WriteExtendedData(value);
				}
				else if ((node->Name() == TimeSequence::StructureName) ||
								 (node->Name() == EventGroup::StructureName))
				{
					// Not written when extended
				}
				else
				{
					writer.WriteValue(node->Name(), node->Value());
				}
			}
			else
			{
				writer.WriteValue(node->Name(), node->Value());
			}

		}
	}
	
	TreeValue* ReadWriteXMLCompound::ReadValue(XMLReadingMachine& reader, const pugi::xml_node& element) const
	{
		// verify all nodes have different names
		pugi::xml_node child;
		std::set<std::string> name_set;
		for (child = element.first_child(); child; child = child.next_sibling())
		{
			if (child.type() == pugi::node_element)
			{
				if (name_set.find(child.name()) != name_set.end())
					return NULL;
				name_set.insert(child.name());
			}
		}

		// must have at least one element
		if (name_set.size() == 0)
			return NULL;

		// build compound item
		std::unique_ptr<TreeCompound> newobject( new TreeCompound() );
		for (child = element.first_child(); child; child = child.next_sibling())
		{
			if (child.type() == pugi::node_element)
			{
				TreeValue* newvalue = reader.ReadValue(child);
				if (newvalue != NULL)
				{
					newobject->Set(child.name(), newvalue);
				}
				else
				{
					// silently ignore if element type is unrecognised
				}
			}
		}

		return newobject.release();
	}
}
