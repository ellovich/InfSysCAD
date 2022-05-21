#pragma once
#include <TDocStd_Document.hxx>

#include <TDF_Label.hxx>
#include <TCollection_AsciiString.hxx>
#include <TopLoc_Location.hxx>

namespace InfSysCAD
{
	class ImportExport
	{
	public:
		static Handle(TDocStd_Document) ReadStepWithMeta(const char* filename);
		static int TraverseDocument(const Handle(TDocStd_Document)& theDoc);
	private:
		static int TraverseLabel(const TDF_Label& theLabel, 
			const TCollection_AsciiString& theNamePrefix, 
			const TopLoc_Location& theLoc);
	};
}