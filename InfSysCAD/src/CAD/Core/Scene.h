#pragma once
#include <AIS_InteractiveContext.hxx>

namespace InfSysCAD
{
	class Scene
	{
	public:
		Scene(Handle(AIS_InteractiveContext) viewer);
		~Scene() = default;

		void AddObeject(const TopoDS_Shape& topoDS_Shape, gp_Pnt pos);
		void HideObject();
		void RemoveObject();
		void LoadFile();
		void SaveFile();
		
		const std::string& GetCurrentFilename() const { return m_CurrentFilename; }

	private:
		Handle(AIS_InteractiveContext) m_Context;
		gp_Ax2 m_Axis;		
		std::string m_CurrentFilename;
	};
}