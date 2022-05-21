//-----------------------------------------------------------------------------
// Created on: 18 July 2021
//-----------------------------------------------------------------------------
// Copyright (c) 2021, Sergey Slyadnev (sergey.slyadnev@gmail.com)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder(s) nor the
//      names of all contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#pragma once

// OpenCascade includes
#include <NCollection_DataMap.hxx>
#include <NCollection_List.hxx>

class TDF_Label;
class TDF_LabelMapHasher;
class TopLoc_Location;
class XCAFPrs_Style;
class TCollection_AsciiString;
class AIS_InteractiveObject;
class AIS_InteractiveContext;
class TDocStd_Document;

namespace InfSysCAD
{    //! Type alias for label-presentation map.
    typedef NCollection_DataMap<TDF_Label,
        NCollection_List<Handle(AIS_InteractiveObject)>,
        TDF_LabelMapHasher> LabelPrsMap;

    //! Redisplays all objects in the viewer.
    class DisplayScene : public Standard_Transient
    {
    public:
        DEFINE_STANDARD_RTTI_INLINE(DisplayScene, Standard_Transient)

    public:

        //! Ctor accepting the interactive context to use.
        //! \param[in] doc the XDE document to visualize.
        //! \param[in] ctx the interactive context instance.
        DisplayScene(const Handle(TDocStd_Document)& doc,
            const Handle(AIS_InteractiveContext)& ctx)
            : Standard_Transient(),
            m_doc(doc),
            m_ctx(ctx)
        {}

    public:

        //! Executes this visualization command.
        //! \return execution status (true for success, false for failure).
        virtual bool Execute();

    protected:

        //! Display items conatined in the XDE document.
        //! \param[in] label          the OCAF label with assembly or shape to display.
        //! \param[in] parentTrsf     the transformation of the  parent assembly.
        //! \param[in] parentStyle    the style of the parent.
        //! \param[in] parentId       the entry of the parent label.
        //! \param[in] mapOfOriginals the map of the created AIS objects. New parts are
        //!                           connected to already created objects contained in the map.
        //! \param[in] processed      the map of processed items.
        void displayItem(const TDF_Label& label,
            const TopLoc_Location& parentTrsf,
            const XCAFPrs_Style& parentStyle,
            const TCollection_AsciiString& parentId,
            LabelPrsMap& mapOfOriginals);

    protected:

        //! XDE document to visualize.
        Handle(TDocStd_Document) m_doc;

        //! Interactive context facade to work with AIS.
        Handle(AIS_InteractiveContext) m_ctx;

    };
}