#include "infSys_pch.h"
#include "Scene.h"

#include <AIS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

#include <CAD/Step/ImportExport.h>

#include <CAD/Step/DisplayScene.h>

namespace InfSysCAD
{
	Scene::Scene(Handle(AIS_InteractiveContext) context)
		: m_Context(context)
	{
		//auto aBox1 = BRepPrimAPI_MakeBox(m_Axis, 80, 100, 80).Shape();
		//AddObeject(aBox1, gp_Pnt(0, 0, 0));

		auto doc = ImportExport::ReadStepWithMeta("resources/models/chassis.step");
		DisplayScene ds(doc, context);
		ds.Execute();

		std::cout << ImportExport::TraverseDocument(doc) << std::endl;
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
}