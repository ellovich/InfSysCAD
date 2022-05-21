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

namespace InfSysCAD
{
    Handle(TDocStd_Document) ImportExport::ReadStepWithMeta(const char* filename)
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




    //! Handle single label.
    int ImportExport::TraverseLabel(const TDF_Label& theLabel, const TCollection_AsciiString& theNamePrefix, const TopLoc_Location& theLoc)
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
                if (TraverseLabel(aChildIter.Value(), aName, aLoc) == 1)
                {
                    return 1;
                }
            }
            return 0;
        }
        std::cout << aName << " ";
        return 0;
    }

    //! Handle document root shapes.
    int ImportExport::TraverseDocument(const Handle(TDocStd_Document)& theDoc)
    {
        TDF_LabelSequence aLabels;
        XCAFDoc_DocumentTool::ShapeTool(theDoc->Main())->GetFreeShapes(aLabels);
        for (TDF_LabelSequence::Iterator aLabIter(aLabels); aLabIter.More(); aLabIter.Next())
        {
            const TDF_Label& aLabel = aLabIter.Value();
            if (TraverseLabel(aLabel, "", TopLoc_Location()) == 1)
            {
                return 1;
            }
        }
        return 0;
    }
}