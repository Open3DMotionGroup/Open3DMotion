/*--
  Open3DMotion 
  Copyright (c) 2004-2021.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "XMLWritingMachine.h"
#include "ReadWriteXML.h"
#include "Open3DMotion/Biomechanics/Trial/TimeSequence.h"
#include "Open3DMotion/Biomechanics/Trial/EventGroup.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/Biomechanics/Trial/EventArray.h"
#include <pugixml.hpp>

namespace Open3DMotion
{
	XMLWritingMachine::XMLWritingMachine(std::ostream& _os, bool _extended/*=false*/) :
		XMLReadWriteMachine(_extended), os(_os)
	{
	}

	XMLWritingMachine::~XMLWritingMachine()
	{
	}

	void XMLWritingMachine::Write(const ReadWriteXML* xml_class, const std::string& name, const TreeValue* value)
	{
		os << "<" << name;
		const char* type_attribute = xml_class->TypeAttribute();
		if (type_attribute != NULL)
		{
			os << " type=\"" << type_attribute << "\"";
		}
		os << ">";
		xml_class->WriteValue(*this, value);
		os << "</" << name << ">";
	}

	void XMLWritingMachine::WriteExtendedData(const TreeValue* value)
	{
		os << "\n<DataExt>\n";
		const TreeCompound* compound = TreeValueCast<TreeCompound>(value);
		if (compound)
		{
			char buffer[256];

			if (compound->Get(TimeSequence::StructureName) != NULL)
			{
				TimeSequence ts;
				ts.FromTree(compound);
				try
				{
					for (TSOccVector3ConstIter iter(ts); iter.HasFrame(); iter.Next())
					{
						snprintf(buffer, sizeof(buffer), "<f x=\"%.08f\" y=\"%.08f\" z=\"%.08f\" o=\"%d\"/>\n",
							iter.Value()[0], iter.Value()[1], iter.Value()[2], iter.Occluded() ? 1 : 0);
						os << buffer;
					}
				}
				catch (...)
				{
					try
					{
						for (TSScalarConstIter iter(ts); iter.HasFrame(); iter.Next())
						{
							snprintf(buffer, sizeof(buffer), "<f x=%.08f />\n", iter.Value());
							os << buffer;
						}
					}
					catch (...)
					{
					}
				}
			}
			else if (compound->Get(EventGroup::StructureName) != NULL)
			{
				try
				{
					EventGroup eg;
					eg.FromTree(compound);
					for (EventGroupIter iter(eg); iter.HasFrame(); iter.Next())
					{
						snprintf(buffer, sizeof(buffer), "<e f=\"%d\" t=\"%.08f\"/>\n",
							iter.ID(), iter.Time());
						os << buffer;
					}
				}
				catch (...)
				{
				}
			}
		}

		os << "</DataExt>\n";
	}

	void XMLWritingMachine::WriteTextNode(const std::string& value)
	{
		pugi::xml_document d;
		pugi::xml_node n = d.append_child(pugi::node_pcdata);
		n.set_value(value.c_str());
		d.save(os, "", pugi::format_no_declaration | pugi::format_raw, pugi::encoding_utf8);
	}
	
	void XMLWritingMachine::WriteValue(const std::string& name, const TreeValue* value)
	{
		for (std::vector<ReadWriteXML*>::const_iterator iter_element( element.begin() ); iter_element != element.end(); iter_element++)
		{
			if ((*iter_element)->CanWrite(value))
			{
				Write(*iter_element, name, value);
				break;
			}
		}
	}
}
