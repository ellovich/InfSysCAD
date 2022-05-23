#pragma once
#include <TDocStd_Document.hxx>

#include <TDF_Label.hxx>
#include <TCollection_AsciiString.hxx>
#include <TopLoc_Location.hxx>

namespace InfSysCAD
{
	struct TransportArray;

	class ImportExport
	{
	public:
		static Handle(TDocStd_Document) LoadStepWithMeta(const char* filename);
		static bool SaveStepWithMeta(const Handle(TDocStd_Document)& doc, const char* filename);

		static bool SaveXDF(const Handle(TDocStd_Document)& doc, const char* filename);
		
		static Ref<std::vector<TransportArray>> ImportExport::LoadTransportArray(const char* filename);

		static Ref<std::vector<std::string>> ImportExport::GetTreeFromDoc(const Handle(TDocStd_Document)& theDoc);

	private:
		static int ImportExport::TraverseDocument(std::vector<std::string>& tree, const Handle(TDocStd_Document)& theDoc);
	};

	struct TransportArray
	{
		std::string Name, Material, Grade, Thickness, Section_name, Profile_type, Length;

		TransportArray(std::string name, std::string material, std::string grade, 
			std::string thickness, std::string section_name, std::string profile_type, std::string length)
			: Name(name), Material(material), Grade(grade), 
			Thickness(thickness), Section_name(section_name), Profile_type(profile_type), Length(length)
		{

		}

		TransportArray()
		{
				
		}
	};
}