/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatXMove.h"
#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatOptionsXMove.h"
#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatDescriptor.h"
#include <set>
#include "Open3DMotion/OpenORM/IO/XML/XMLReadingMachine.h"
#include "Open3DMotion/OpenORM/IO/XML/XMLWritingMachine.h"
#include "Open3DMotion/MotionFile/Formats/XMove/XMLReadingMachineLegacy.h"
#include "Open3DMotion/MotionFile/Formats/XMove/XMLWritingMachineLegacy.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/RichBinaryConvertFloat.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include <pugixml.hpp>
#include <memory>

namespace Open3DMotion
{	
	const char FileFormatXMove::XMLFormatDescriptorSection[] = "FileFormat";
	const char FileFormatXMove::XMLRoot[] = "xmove";

	FileFormatXMove::FileFormatXMove() :
			MotionFileFormat("XMove", "XMove", "xmove,xml")
	{
	}

	FileFormatXMove::~FileFormatXMove()
	{
	}

	bool FileFormatXMove::Probe(const MotionFileHandler& /*context*/, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException)
	{
		// check for format
		if (!ProbeTextString(is, "<xmove", 1024))
			return false;

		// reset to beginning and check whether legacy read should be used
		is.seekg(0, std::ios::beg);
		FileFormatOptionsXMove xmove_options;
		xmove_options.LegacyCompoundNames = ProbeTextString(is, "<FormatID>CODAmotion_xmove</FormatID>", 1024);

		// store options
		readoptions = xmove_options.ToTree();

		// either way, will be ok to read this
		return true;
	}

  TreeValue* FileFormatXMove::Read(const MotionFileHandler& /*context*/, std::istream& is, const BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException) 
	{
		// options
		FileFormatOptionsXMove xmove_options;
		if (readoptions)
		{
			xmove_options.FromTree(readoptions);
		}

		// load XML
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(is);
		if (!result)
		{
			throw MotionFileException(MotionFileException::formaterror, result.description());
		}

		std::unique_ptr<XMLReadingMachine> reader;
		if (xmove_options.LegacyCompoundNames)
		{
			// Support legacy version of format in which 
			// elements of time sequence and event groups structures have assumed types
			// and some naming conventions are different
			reader = std::unique_ptr<XMLReadingMachine>( new XMLReadingMachineLegacy(memfactory) );
		}
		else
		{
			// Standard XML reader
			reader = std::unique_ptr<XMLReadingMachine>( new XMLReadingMachine(memfactory) );
		}

		// find xmove node
		pugi::xml_node xmove_node = doc.child("xmove");

		// must have xmove node
		if (!xmove_node)
		{
			throw MotionFileException(MotionFileException::formaterror, "XML missing xmove section");
		}

		// parse XML tree
		TreeValue* trial_tree = reader->ReadValue(xmove_node);
		TreeCompound* trial_object = TreeValueCast<TreeCompound>( trial_tree );

		// optionally replace any float32 info with float64
		if (xmove_options.ConvertBinaryFloat32)
		{
			for (size_t calclevel = 0; calclevel < trial_object->NumElements(); calclevel++)
			{
				TreeCompound* section = TreeValueCast<TreeCompound> ( trial_object->Node(calclevel)->Value() );
				ConvertListFloat32To64(section, "Sequences", TimeSequence::StructureName, memfactory);
				ConvertListFloat32To64(section, "EventGroups", EventGroup::StructureName, memfactory);
			}
		}

		return trial_object;

	}

	void FileFormatXMove::WriteHeader(std::ostream& os) const
	{
		os << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";

		// write contents
		os << "<" << FileFormatXMove::XMLRoot << ">\n";
	}

	void FileFormatXMove::WriteFooter(std::ostream& os) const
	{
		os << "</" << FileFormatXMove::XMLRoot << ">\n";
	}

	XMLWritingMachine* FileFormatXMove::BuildXMLWritingMachine(std::ostream& os, const FileFormatOptionsXMove& xmove_options) const
	{
		// choose format based on options
		if (xmove_options.LegacyCompoundNames)
		{
			return new XMLWritingMachineLegacy(os);
		}
		else
		{
			return new XMLWritingMachine(os);
		}
	}

	void FileFormatXMove::WriteFileFormatDescriptor(const MotionFileHandler& context, XMLWritingMachine* writer, const FileFormatOptionsXMove& xmove_options) const
	{
		// Descriptor object
		FileFormatDescriptor descriptor;

		// fill with library info
		descriptor.ProgramName = context.ProgramName();
		descriptor.ProgramVersion = context.ProgramVersion();
		descriptor.LibraryName = context.LibraryName;
		descriptor.LibraryVersion = context.LibraryVersion;

		// describe format based on options
		if (xmove_options.LegacyCompoundNames)
		{
			descriptor.FormatID = "CODAmotion_xmove";
		}
		else
		{
			descriptor.FormatID = "CODAmotion_xmove2";
		}

		// write to file
		std::unique_ptr<TreeValue> descriptor_tree(descriptor.ToTree());
		writer->WriteValue(FileFormatXMove::XMLFormatDescriptorSection, descriptor_tree.get());
	}

	void FileFormatXMove::WriteContents(const MotionFileHandler& context, const TreeValue* contents, XMLWritingMachine* writer, const FileFormatOptionsXMove& xmove_options) const throw(MotionFileException)
	{
		// make copy of trial so we can adjust its structure according to export options
		std::unique_ptr<TreeCompound> export_contents(new TreeCompound);
		const TreeCompound* input_contents = TreeValueCast<TreeCompound>(contents);
		if (input_contents)
		{
			export_contents->CopyFrom(input_contents);
		}

		// remove any existing format info - we will write a new one
		export_contents->Remove(XMLFormatDescriptorSection);

		// optionally convert 64-bit floats to 32-bit
		std::unique_ptr<TreeCompound> contents_copy;
		if (xmove_options.ConvertBinaryFloat32)
		{
			// remap binary data
			BinMemFactoryDefault memfactory;
			for (size_t calclevel = 0; calclevel < export_contents->NumElements(); calclevel++)
			{
				TreeCompound* section = TreeValueCast<TreeCompound>(export_contents->Node(calclevel)->Value());
				ConvertListFloat64To32(section, "Sequences", TimeSequence::StructureName, memfactory);
				ConvertListFloat64To32(section, "EventGroups", EventGroup::StructureName, memfactory);
			}
		}

		// all elements of trial, optionally excluding Calc section
		for (size_t element_index = 0; element_index < export_contents->NumElements(); element_index++)
		{
			const TreeCompoundNode* node = export_contents->Node(element_index);

			// optionally exclude calc
			if (xmove_options.ExcludeCalc && node->Name().compare(MEMBER_NAME(Trial::Calc)) == 0)
				continue;

			// otherwise write the node
			writer->WriteValue(node->Name(), node->Value());
		}
	}

	void FileFormatXMove::Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException)
	{
		// parse options
		FileFormatOptionsXMove xmove_options;
		xmove_options.FromTree(writeoptions);

		// build writer based on options
		std::unique_ptr<XMLWritingMachine> writer(BuildXMLWritingMachine(os, xmove_options));

		// XML pre-amble
		WriteHeader(os);

		// format descriptor for audit trail
		WriteFileFormatDescriptor(context, writer.get(), xmove_options);

		// data itself
		WriteContents(context, contents, writer.get(), xmove_options);
	
		// XML post-amble
		WriteFooter(os);
	}
	
	void FileFormatXMove::ConvertListFloat32To64(TreeCompound* section, const char* listname, const char* structurename, const BinMemFactory& memfactory)
	{
		if (section)
		{
			// get items
			const TreeList* ts_list_input = section->GetType<TreeList>(listname);
			if (ts_list_input)
			{					
				// convert
				TreeList* ts_list_output = ts_list_input->NewBlank();
				for (size_t iseq = 0; iseq < ts_list_input->NumElements(); iseq++)
				{
					const TreeValue* ts_input = ts_list_input->ElementArray()[iseq];
					TreeValue* ts_output = RichBinaryConvertFloat32To64(ts_input, structurename, memfactory);
					if (ts_output != NULL)
					{
						ts_list_output->Add( ts_output );
					}
				}
		
				// re-attach
				section->Remove(listname);
				section->Set(listname, ts_list_output);
			}
		}
	}

	void FileFormatXMove::ConvertListFloat64To32(TreeCompound* section, const char* listname, const char* structurename, const BinMemFactory& memfactory)
	{
		if (section)
		{
			// get items
			const TreeList* ts_list_input = section->GetType<TreeList>(listname);
			if (ts_list_input)
			{
				// convert
				TreeList* ts_list_output = ts_list_input->NewBlank();
				for (size_t iseq = 0; iseq < ts_list_input->NumElements(); iseq++)
				{
					const TreeValue* ts_input = ts_list_input->ElementArray()[iseq];
					TreeValue* ts_output = RichBinaryConvertFloat64To32(ts_input, structurename, memfactory);
					if (ts_output != NULL)
					{
						ts_list_output->Add( ts_output );
					}
				}
		
				// re-attach
				section->Remove(listname);
				section->Set(listname, ts_list_output);
			}
		}
	}
}
