#include "infSys_pch.h"
#include "Scene.h"

#include <AIS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

#include <CAD/Step/ImportExport.h>
#include <CAD/Step/DisplayScene.h>

#include "CAD/Utils/FileDialogs.h"

namespace InfSysCAD
{
	Scene::Scene(Handle(AIS_InteractiveContext) context)
		: m_Context(context), m_CurrentFilename("unknown")
	{
		//auto aBox1 = BRepPrimAPI_MakeBox(m_Axis, 80, 100, 80).Shape();
		//AddObeject(aBox1, gp_Pnt(0, 0, 0));

		auto doc = ImportExport::ReadStepWithMeta("resources/models/chassis.step");
		DisplayScene ds(doc, context);
		ds.Execute();
	}

	void Scene::AddObeject(const TopoDS_Shape& topoDS_Shape, gp_Pnt pos)
	{
		Handle(AIS_Shape) shape = new AIS_Shape(topoDS_Shape);
		m_Axis.SetLocation(pos);
		m_Context->SetMaterial(shape, Graphic3d_NameOfMaterial_Silver, Standard_False);
		m_Context->Display(shape, AIS_Shaded, 0, false);
	}

	void Scene::HideObject()
	{

	}

	void Scene::RemoveObject()
	{

	}

	void Scene::LoadFile()
	{
		std::string filepath = FileDialogs::OpenFile("Step files(*.step)\0*.step\0Stp files(*.stp)\0*stp\0All files(*.*)\0*.*\0");
		if (!filepath.empty())
		{
			m_CurrentFilename = filepath.substr(filepath.find_last_of("/\\") + 1);
			//std::thread t1([=]()
			//	{
					auto doc = ImportExport::ReadStepWithMeta(filepath.c_str());
					DisplayScene ds(doc, m_Context);
					ds.Execute();
			//	});
		}
	}
	void Scene::SaveFile()
	{
		std::string filepath = FileDialogs::SaveFile("Step files(*.step)\0*.step\0Stp files(*.stp)\0*stp\0All files(*.*)\0*.*\0");
		if (!filepath.empty())
		{
			////std::thread t1([=]()
			////	{
			//auto doc = ImportExport::ReadStepWithMeta(filepath.c_str());
			//DisplayScene ds(doc, m_Context);
			//ds.Execute();
			////	});
		}
	}
}