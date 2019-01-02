/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FILE_FORMAT_XMOVE_H_
#define _OPEN3DMOTION_FILE_FORMAT_XMOVE_H_

#include "Open3DMotion/MotionFile/MotionFileFormat.h"
#include "Open3DMotion/OpenORM/Mappings/MapBool.h"
#include <string>

namespace Open3DMotion
{
	// forward decl
	class XMLWritingMachine;
	class FileFormatOptionsXMove;

	// XMove-compliant XML read/write
	class FileFormatXMove : public MotionFileFormat
  {
	public:

		FileFormatXMove();

		virtual ~FileFormatXMove();

		static const char XMLFormatDescriptorSection[] /*="FileFormat"*/;
		static const char XMLRoot[] /*="xmove"*/;

  public:

		// Check id and number type
    virtual bool Probe(const MotionFileHandler& context, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException);

    // Read XMove
    virtual TreeValue* Read(const MotionFileHandler& context, std::istream& is, const BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException) ;

    // Write XMove
    virtual void Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException);
		
		// Write header only
		void WriteHeader(std::ostream& os) const;

		// Write footer only
		void WriteFooter(std::ostream& os) const;

		// Build internal writing object
		Open3DMotion::XMLWritingMachine* BuildXMLWritingMachine(std::ostream& os, const FileFormatOptionsXMove& xmove_options) const;

		// Write format descriptor only
		void WriteFileFormatDescriptor(const MotionFileHandler& context, XMLWritingMachine* writer, const FileFormatOptionsXMove& xmove_options) const;

		// Write data contents
		void WriteContents(const MotionFileHandler& context, const TreeValue* contents, XMLWritingMachine* writer, const FileFormatOptionsXMove& xmove_options) const throw(MotionFileException);

	protected:
		static void ConvertListFloat32To64(TreeCompound* section, const char* listname, const char* structurename, const BinMemFactory& memfactory);
		static void ConvertListFloat64To32(TreeCompound* section, const char* listname, const char* structurename, const BinMemFactory& memfactory);

	};
}


#endif
