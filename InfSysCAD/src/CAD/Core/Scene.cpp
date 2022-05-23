#include "infSys_pch.h"
#include "Scene.h"

#include <CAD/Core/Application.h>
#include <CAD/Step/ImportExport.h>
#include <CAD/Step/DisplayScene.h>
#include "CAD/Utils/FileDialogs.h"

#include <AIS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <BRepPrimAPI_MakeBox.hxx>


namespace InfSysCAD
{
	Scene::Scene(Handle(AIS_InteractiveContext) context)
		: m_Context(context), m_CurrentFilename("<unknown>")
	{
		m_TransportArray = CreateRef<std::vector<TransportArray>>();
		m_Tree = CreateRef<std::vector<std::string>>();

		//auto aBox1 = BRepPrimAPI_MakeBox(m_Axis, 80, 100, 80).Shape();
		//AddObeject(aBox1, gp_Pnt(0, 0, 0));

		if (Application::Get().GetArgs().Count > 0)
		{
			std::string filepath = Application::Get().GetArgs()[1];
			m_CurrentFilename = filepath.substr(filepath.find_last_of("/\\") + 1);

			INFSYS_INFO("Loading '{0}'", filepath);

			m_CurrentDoc = ImportExport::LoadStepWithMeta(filepath.c_str());
			if (m_CurrentDoc.IsNull())
				INFSYS_ERROR("Failed to read STEP model from file '{0}'", filepath);

			DisplayScene ds(m_CurrentDoc, m_Context);
			if (!ds.Execute())
			{
				INFSYS_ERROR("Failed to visualize CAD model with `DisplayScene` command.");
			}
			else
			{
				INFSYS_INFO("Loaded: '{0}'", filepath);
			}
		}
	}

	void Scene::AddObeject(const TopoDS_Shape& topoDS_Shape, gp_Pnt pos)
	{
		Handle(AIS_Shape) shape = new AIS_Shape(topoDS_Shape);
		m_Axis.SetLocation(pos);
		m_Context->SetMaterial(shape, Graphic3d_NameOfMaterial_Silver, Standard_False);
		m_Context->Display(shape, AIS_Shaded, 0, false);
	}


	int Scene::LoadSTEP_File()
	{
		std::string filepath = FileDialogs::OpenFile("Step files(*.step)\0*.step\0Stp files(*.stp)\0*stp\0All files(*.*)\0*.*\0");
		if (!filepath.empty())
		{
			m_CurrentFilename = filepath.substr(filepath.find_last_of("/\\") + 1);

			INFSYS_INFO("Loading '{0}'", filepath);

			m_CurrentDoc = ImportExport::LoadStepWithMeta(filepath.c_str());
			if (m_CurrentDoc.IsNull())
			{
				INFSYS_ERROR("Failed to read STEP model from file '{0}'", filepath);
				return false;
			}

			DisplayScene ds(m_CurrentDoc, m_Context);
			if (!ds.Execute())
			{
				INFSYS_ERROR("Failed to visualize CAD model with `DisplayScene` command.");
				return false;
			}
			else
			{
				INFSYS_INFO("Loaded: '{0}'", filepath);
				m_Tree = ImportExport::GetTreeFromDoc(m_CurrentDoc);
				return true;
			}
		}
	}

	int Scene::SaveSTEP_File()
	{
		std::string filepath = FileDialogs::SaveFile("Step files(*.step)\0*.step\0Stp files(*.stp)\0*stp\0All files(*.*)\0*.*\0");
		if (!filepath.empty())
		{
			INFSYS_INFO("Saving '{0}'", filepath);

			if (!ImportExport::SaveStepWithMeta(m_CurrentDoc, filepath.c_str()))
			{
				INFSYS_ERROR("Failed to save STEP model into file '{0}'", filepath);
				return false;
			}
			else
			{
				INFSYS_INFO("Saved: '{0}'", filepath);
				return true;
			}
		}
	}

	int Scene::SaveOCAF_File()
	{
		std::string filepath = FileDialogs::SaveFile("OCAF files(*.xdf)\0*.xdf\0All files(*.*)\0*.*\0");
		if (!filepath.empty())
		{
			INFSYS_INFO("Saving '{0}'", filepath);

			if (!ImportExport::SaveXDF(m_CurrentDoc, filepath.c_str()))
			{
				INFSYS_ERROR("Failed to save OCAF document into file '{0}'", filepath);
				return false;
			}
			else
			{
				INFSYS_INFO("Saved: '{0}'", filepath);
				return true;
			}
		}
	}

	int Scene::LoadTransportArray()
	{
		std::string filepath = FileDialogs::OpenFile("CSV files(*.csv)\0*.csv\0All files(*.*)\0*.*\0");
		if (!filepath.empty())
		{
			m_TransportArrayFilename = filepath.substr(filepath.find_last_of("/\\") + 1);

			INFSYS_INFO("Loading '{0}'", filepath);

			m_TransportArray = ImportExport::LoadTransportArray(filepath.c_str());

			if (m_TransportArray->empty())
				INFSYS_ERROR("CSV-file is empty '{0}'", m_TransportArrayFilename);
			else
				INFSYS_INFO("Loaded: '{0}'", m_TransportArrayFilename);

			return true;
		}
	}
}