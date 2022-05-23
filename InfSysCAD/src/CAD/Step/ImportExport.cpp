#include "infSys_pch.h"
#include "ImportExport.h"

#include <BinXCAFDrivers.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <TDocStd_Application.hxx>
#include <TDocStd_Document.hxx>

#include <XCAFDoc_DocumentTool.hxx>
#include <TDataStd_Name.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_LabelSequence.hxx>

#include <STEPCAFControl_Writer.hxx>
#include <Interface_Static.hxx>

#include "csv.h"

namespace
{
    //! Handle single label.
    int TraverseLabel(std::vector<std::string>& tree, 
        const TDF_Label& theLabel, 
        const TCollection_AsciiString& theNamePrefix, 
        const TopLoc_Location& theLoc)
    {
        TCollection_AsciiString aName;
        {
            Handle(TDataStd_Name) aNodeName;
            if (theLabel.FindAttribute(TDataStd_Name::GetID(), aNodeName))
            {
                aName = aNodeName->Get(); // instance name
            }
            if (aName.IsEmpty())
            {
                TDF_Label aRefLabel;
                if (XCAFDoc_ShapeTool::GetReferredShape(theLabel, aRefLabel)
                    && aRefLabel.FindAttribute(TDataStd_Name::GetID(), aNodeName))
                {
                    aName = aNodeName->Get(); // product name
                }
            }
        }
        aName = theNamePrefix + aName;

        TDF_Label aRefLabel = theLabel;
        XCAFDoc_ShapeTool::GetReferredShape(theLabel, aRefLabel);
        if (XCAFDoc_ShapeTool::IsAssembly(aRefLabel))
        {
            aName += "/";
            const TopLoc_Location aLoc = theLoc * XCAFDoc_ShapeTool::GetLocation(theLabel);
            for (TDF_ChildIterator aChildIter(aRefLabel); aChildIter.More(); aChildIter.Next())
            {
                if (TraverseLabel(tree, aChildIter.Value(), aName, aLoc) == 1)
                {
                    return 1;
                }
            }
            return 0;
        }

        tree.push_back(aName.ToCString());
      //  std::cout << aName << " " << std::endl;
        return 0;
    }
}

namespace InfSysCAD
{
    Handle(TDocStd_Document) ImportExport::LoadStepWithMeta(const char* filename)
    {
        STEPCAFControl_Reader Reader;

        // Create XDE document.
        Handle(TDocStd_Application) app = new TDocStd_Application;
        BinXCAFDrivers::DefineFormat(app);
        Handle(TDocStd_Document) doc;
        app->NewDocument("BinXCAF", doc);

        // Read CAD and associated data from file
        try
        {
            IFSelect_ReturnStatus outcome = Reader.ReadFile(filename);

            if (outcome != IFSelect_RetDone)
            {
                app->Close(doc);
                return nullptr;
            }

            if (!Reader.Transfer(doc))
            {
                app->Close(doc);
                return nullptr;
            }
        }
        catch(...)
        {
            app->Close(doc);
            return nullptr;
        }

        return doc;
    }

    int ImportExport::SaveStepWithMeta(const Handle(TDocStd_Document)& doc, const char* filename)
    {
        STEPCAFControl_Writer Writer;

        // To make subshape names work, we have to turn on the following static variable of OpenCascade.
        Interface_Static::SetIVal("write.stepcaf.subshapes.name", 1);

        // Write XDE document to file.
        try
        {
            if (!Writer.Transfer(doc, STEPControl_AsIs))
            {
                return false;
            }

            const IFSelect_ReturnStatus ret = Writer.Write(filename);

            if (ret != IFSelect_RetDone)
            {
                return false;
            }
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    int ImportExport::SaveXDF(const Handle(TDocStd_Document)& doc, const char* filename)
    {
        Handle(TDocStd_Application) app = new TDocStd_Application;
        BinXCAFDrivers::DefineFormat(app);
        PCDM_StoreStatus sstatus = app->SaveAs(doc, filename);

        return sstatus;
    }

    Ref<std::vector<TransportArray>> ImportExport::LoadTransportArray(const char* filename)
    {
        Ref<std::vector<TransportArray>> arr = CreateRef<std::vector<TransportArray>>();
    
        io::CSVReader<7> in(filename);
        in.read_header(io::ignore_extra_column, "Name", "Material", "Grade", "Thickness", "Section name", "Profile type", "Length");
        std::string Name, Material, Grade, Thickness, Section_name, Profile_type, Length;

        while (in.read_row(Name, Material, Grade, Thickness, Section_name, Profile_type, Length)) 
        {
            arr->push_back(TransportArray(Name, Material, Grade, Thickness, Section_name, Profile_type, Length));
        }

        return arr;
    }
   

    int ImportExport::TraverseDocument(std::vector<std::string>& tree, const Handle(TDocStd_Document)& theDoc)
    {
        TDF_LabelSequence aLabels;
        XCAFDoc_DocumentTool::ShapeTool(theDoc->Main())->GetFreeShapes(aLabels);
        for (TDF_LabelSequence::Iterator aLabIter(aLabels); aLabIter.More(); aLabIter.Next())
        {
            const TDF_Label& aLabel = aLabIter.Value();
            if (TraverseLabel(tree, aLabel, "", TopLoc_Location()) == 1)
            {
                return 1;
            }
        }
        return 0;
    }

    Ref<std::vector<std::string>> ImportExport::GetTreeFromDoc(const Handle(TDocStd_Document)& theDoc)
    {
        Ref<std::vector<std::string>> tree = CreateRef<std::vector<std::string>>();
        TraverseDocument(*tree, theDoc);
        return tree;
    }
}
