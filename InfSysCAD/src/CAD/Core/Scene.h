#pragma once
#include <AIS_InteractiveContext.hxx>
#include <TopoDS_Shape.hxx>
#include <TDocStd_Document.hxx>
#include <CAD/Step/ImportExport.h>

namespace InfSysCAD
{
	class Scene
	{
	public:
		Scene(Handle(AIS_InteractiveContext) viewer);
		~Scene() = default;

		int LoadSTEP_File();
		int SaveSTEP_File();
		int SaveOCAF_File();
		int LoadTransportArray();

		int SetArrayMaterials();

		//void AddObeject(const TopoDS_Shape& topoDS_Shape, gp_Pnt pos);

		const std::string& GetModelFilename() const { return m_ModelFilename; }
		const Handle(TDocStd_Document) GetCurrentDoc() const { return m_CurrentDoc; }

		const std::string& GetTransportArrayFilename() const { return m_TransportArrayFilename; }
		const std::vector<TransportArray>& GetTransportArray() const { return *m_TransportArray; }

		const std::vector<std::string>& GetTree() const { return *m_Tree; }

	private:
		Handle(AIS_InteractiveContext) m_InteractiveContext;
		gp_Ax2 m_Axis;

		std::string m_ModelFilename;
		Handle(TDocStd_Document) m_CurrentDoc;

		std::string m_TransportArrayFilename;
		Ref<std::vector<TransportArray>> m_TransportArray;

		Ref<std::vector<std::string>> m_Tree;
	};
}