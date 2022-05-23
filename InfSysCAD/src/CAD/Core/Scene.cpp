#include "infSys_pch.h"
#include "Scene.h"

#include <CAD/Core/Application.h>
#include <CAD/Step/ImportExport.h>
#include <CAD/Step/DisplayScene.h>
#include "CAD/Utils/FileDialogs.h"

#include <AIS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

#include <XCAFDoc_DocumentTool.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDataStd_Name.hxx>

namespace InfSysCAD
{
	Scene::Scene(Handle(AIS_InteractiveContext) context)
		: m_InteractiveContext(context), m_ModelFilename("<unknown>")
	{
		m_TransportArray = CreateRef<std::vector<TransportArray>>();
		m_Tree = CreateRef<std::vector<std::string>>();

		//auto aBox1 = BRepPrimAPI_MakeBox(m_Axis, 80, 100, 80).Shape();
		//AddObeject(aBox1, gp_Pnt(0, 0, 0));

		if (Application::Get().GetArgs().Count > 0)
		{
			std::string filepath = Application::Get().GetArgs()[1];
			m_ModelFilename = filepath.substr(filepath.find_last_of("/\\") + 1);

			INFSYS_INFO("Loading '{0}'", filepath);

			m_CurrentDoc = ImportExport::LoadStepWithMeta(filepath.c_str());
			if (m_CurrentDoc.IsNull())
				INFSYS_ERROR("Failed to read STEP model from file '{0}'", filepath);

			DisplayScene ds(m_CurrentDoc, m_InteractiveContext);
			if (!ds.Execute())
			{
				INFSYS_ERROR("Failed to visualize CAD model with `DisplayScene` command.");
			}
			else
			{
				INFSYS_INFO("Loaded: '{0}'", filepath);
				Application::Get().GetViewer().GetView()->FitAll();
			}
		}
	}


	int Scene::LoadSTEP_File()
	{
		std::string filepath = FileDialogs::OpenFile("Step files(*.stp)\0*.stp\0Step files(*.step)\0*stp\0All files(*.*)\0*.*\0");
		if (!filepath.empty())
		{
			m_ModelFilename = filepath.substr(filepath.find_last_of("/\\") + 1);

			INFSYS_INFO("Loading '{0}'", filepath);

			m_CurrentDoc = ImportExport::LoadStepWithMeta(filepath.c_str());
			if (m_CurrentDoc.IsNull())
			{
				INFSYS_ERROR("Failed to read STEP model from file '{0}'", filepath);
				return false;
			}

			DisplayScene ds(m_CurrentDoc, m_InteractiveContext);
			if (!ds.Execute())
			{
				INFSYS_ERROR("Failed to visualize CAD model with `DisplayScene` command.");
				return false;
			}
			else
			{
				INFSYS_INFO("Loaded: '{0}'", filepath);
				Application::Get().GetViewer().GetView()->FitAll();
				m_Tree = ImportExport::GetTreeFromDoc(m_CurrentDoc);
				return true;
			}
		}

		return false;
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

		return false;
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

		return false;
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
			{
				INFSYS_ERROR("CSV-file is empty '{0}'", m_TransportArrayFilename);
				return false;
			}
			else
			{
				INFSYS_INFO("Loaded: '{0}'", m_TransportArrayFilename);
				return true;
			}

			return true;
		}

		return false;
	}


	int Scene::SetArrayMaterials()
	{
		Handle(XCAFDoc_ColorTool) CT = XCAFDoc_DocumentTool::ColorTool(m_CurrentDoc->Main());

		// Assign colors to parts.
		TDF_LabelSequence aLabels;
		XCAFDoc_DocumentTool::ShapeTool(m_CurrentDoc->Main())->GetShapes(aLabels);
		for (TDF_LabelSequence::Iterator aLabIter(aLabels); aLabIter.More(); aLabIter.Next())
		{
			const TDF_Label& aLabel = aLabIter.Value();
			TCollection_AsciiString aName;
			Handle(TDataStd_Name) aNodeName;
			if (aLabel.FindAttribute(TDataStd_Name::GetID(), aNodeName))
			{
				aName = aNodeName->Get();
			}

			// if(aName.StartsWith("Deck_"))
			if (aName == "wheel")
				CT->SetColor(aLabel, Quantity_Color(0.7, 0.6, 1, Quantity_TOC_RGB), XCAFDoc_ColorGen);
		}

		DisplayScene ds(m_CurrentDoc, m_InteractiveContext);
		if (!ds.Execute())
		{
			INFSYS_ERROR("Failed to visualize CAD model with `DisplayScene` command.");
			return false;
		}
		else 
		{
			INFSYS_INFO("Successful merging with transport array!");
			return true;

		}

		return false;
	}


	//void Scene::AddObeject(const TopoDS_Shape& topoDS_Shape, gp_Pnt pos)
	//{
	//	Handle(AIS_Shape) shape = new AIS_Shape(topoDS_Shape);
	//	m_Axis.SetLocation(pos);
	//	m_InteractiveContext->SetMaterial(shape, Graphic3d_NameOfMaterial_Silver, Standard_False);
	//	m_InteractiveContext->Display(shape, AIS_Shaded, 0, false);
	//}
	//
	//
	//TopoDS_Shape getShapeFromStep(STEPControl_Reader reader, char* shapeName)
	//{
	//	TopoDS_Shape retShape;
	//
	//	//Handle_TColStd_HSequenceOfTransient shapeList = reader.GiveList("xst-model-roots");
	//	Handle_TColStd_HSequenceOfTransient shapeList = reader.GiveList("xst-model-all");
	//	int numShapesTrans = reader.TransferList(shapeList);
	//	retShape = reader.OneShape();
	//
	//	for (int i = 1; i <= numShapesTrans; ++i)
	//	{
	//		Handle_Standard_Transient transient = shapeList->Value(i);
	//		Handle_XSControl_WorkSession& theSession = reader.WS();
	//		Handle_XSControl_TransferReader& aReader = theSession->TransferReader();
	//		Handle_Transfer_TransientProcess& tp = aReader->TransientProcess();
	//		TopoDS_Shape shape = TransferBRep::ShapeResult(tp, transient);
	//		if (!shape.IsNull())
	//		{
	//			Handle_Standard_Transient anEntity = aReader->EntityFromShapeResult(shape, 1);
	//			if (!anEntity.IsNull())
	//			{
	//				Handle_StepRepr_RepresentationItem entity = Handle_StepRepr_RepresentationItem::DownCast(anEntity);
	//				if (!entity.IsNull())
	//				{
	//					if (entity->Name()->String() == shapeName)
	//					{
	//						retShape = shape;
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//
	//	return retShape;
	//}
}