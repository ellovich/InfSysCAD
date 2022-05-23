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

#include <DFBrowser_DFNode.hxx>

#include <DFBrowser_DFTree.hxx>

DFBrowser_DFNode::DFBrowser_DFNode ()
: myColor(DFBrowser_BLACK),
  myPixmap(DFBrowser_NONE),
  myIsOpened(Standard_False),
  myCanOpen(Standard_False),
  myIsSelected(Standard_False),
  myIsChanged(Standard_True),
  myIsVisible(Standard_True)
{
}

void DFBrowser_DFNode::Next (const Handle(DFBrowser_DFNode)& theNext)
{
  if (myNext != theNext) {
    Changed(Standard_True);
    if (!theNext.IsNull()) theNext->Changed(Standard_True);
  }
  myNext = theNext;
}

void DFBrowser_DFNode::Parent (const Handle(DFBrowser_DFNode)& theParent)
{
  if (myParent != theParent) {
    Changed(Standard_True);
    if (!theParent.IsNull()) theParent->Changed(Standard_True);
  }
  myParent = theParent;
}

void DFBrowser_DFNode::Opened (const Standard_Boolean theOpened)
{
  if (myIsOpened != theOpened) Changed(Standard_True);
  myIsOpened = theOpened;
}

void DFBrowser_DFNode::CanOpen (const Standard_Boolean theCanOpen)
{
  if (myCanOpen != theCanOpen) Changed(Standard_True);
  myCanOpen = theCanOpen;
}

void DFBrowser_DFNode::Visible (const Standard_Boolean theIsVisible)
{
  if (myIsVisible != theIsVisible) Changed(Standard_True);
  myIsVisible = theIsVisible;
}

void DFBrowser_DFNode::Del()
{
  myTree.Nullify();
  myNext.Nullify();
  myParent.Nullify();
}

const Handle(DFBrowser_DFTree)& DFBrowser_DFNode::Tree() const
{
  return myTree;
}

void DFBrowser_DFNode::Tree(const Handle(DFBrowser_DFTree)& theTree)
{
  myTree = theTree;
}
