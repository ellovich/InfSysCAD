/***************************************************************************
 *   Copyright (c) OPEN CASCADE SAS                                        *
 *                                                                         *
 *   This file is part of Open CASCADE Technology software library.        *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 ***************************************************************************/

#ifndef _DFBrowser_DFNode_h
#define _DFBrowser_DFNode_h

#include <asiUI.h>

#include <Standard_Transient.hxx>

#include <Standard_CString.hxx>
#include <TCollection_AsciiString.hxx>
#include <DFBrowser_Colors.hxx>
#include <DFBrowser_Picture.hxx>
#include <DFBrowser_NodeType.hxx>

class DFBrowser_DFTree;

class DFBrowser_DFNode : public Standard_Transient
{
  DEFINE_STANDARD_RTTI_INLINE(DFBrowser_DFNode, Standard_Transient)

public:

  asiUI_EXPORT DFBrowser_DFNode();

  asiUI_EXPORT virtual DFBrowser_NodeType GetType() const = 0;

  asiUI_EXPORT virtual void Update() = 0;

  asiUI_EXPORT void Next(const Handle(DFBrowser_DFNode)& theNext);

  inline const Handle(DFBrowser_DFNode) & Next() const
  {
    return myNext;
  }

  asiUI_EXPORT void Parent(const Handle(DFBrowser_DFNode)& theParent);

  inline const Handle(DFBrowser_DFNode) & Parent() const
  {
    return myParent;
  }

  asiUI_EXPORT virtual void AddSub(Handle(DFBrowser_DFNode)& theNode) = 0;

  asiUI_EXPORT virtual Handle(DFBrowser_DFNode) Sub() const = 0;

  asiUI_EXPORT virtual const TCollection_AsciiString & Name() = 0;

  const Handle(DFBrowser_DFTree)& Tree() const;

  void Tree(const Handle(DFBrowser_DFTree)& theTree);

  inline Standard_Boolean Opened() const
  {
    return myIsOpened;
  }

  asiUI_EXPORT void Opened(const Standard_Boolean theOpened);

  inline Standard_Boolean CanOpen() const
  {
    return myCanOpen;
  }

  asiUI_EXPORT void CanOpen(const Standard_Boolean theCanOpen);

  inline Standard_Boolean Selected() const
  {
    return myIsSelected;
  }

  inline void Selected(const Standard_Boolean theIsSelected)
  {
    myIsSelected = theIsSelected;
  }

  inline Standard_Boolean Changed() const
  {
    return myIsChanged;
  }

  inline void Changed(const Standard_Boolean theIsChanged)
  {
    myIsChanged = theIsChanged;
  }

  inline Standard_Boolean Visible() const
  {
    return myIsVisible;
  }

  asiUI_EXPORT void Visible(const Standard_Boolean theIsVisible);

  inline DFBrowser_Colors Color() const
  {
    return myColor;
  }

  inline DFBrowser_Picture Pixmap() const
  {
    return myPixmap;
  }

  asiUI_EXPORT virtual void Del() = 0;

 protected:

  TCollection_AsciiString myName;
  DFBrowser_Colors myColor;
  DFBrowser_Picture myPixmap;

 private:

  Handle(DFBrowser_DFTree) myTree;
  Handle(DFBrowser_DFNode) myNext;
  Handle(DFBrowser_DFNode) myParent;
  Standard_Boolean myIsOpened;
  Standard_Boolean myCanOpen;
  Standard_Boolean myIsSelected;
  Standard_Boolean myIsChanged;
  Standard_Boolean myIsVisible;
};

#endif
