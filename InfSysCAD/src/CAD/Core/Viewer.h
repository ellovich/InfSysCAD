#pragma once
#include "CAD/Core/Window.h"

#include <AIS_ViewController.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>

namespace InfSysCAD
{
	class Viewer : protected AIS_ViewController
	{
	public:
		Viewer(Window* window);
		~Viewer() { m_View->Remove(); }
		
		void Update();

		Handle(AIS_InteractiveContext) GetContext() const { return m_InteractiveContext; }
		Handle(V3d_View) GetView() const { return m_View; }

	private:
		void InitCallbacks();

	private:
		Handle(Window) m_Window;
		Handle(V3d_View) m_View;
		Handle(AIS_InteractiveContext) m_InteractiveContext;

	private:
		// Events
		void onResize(int w, int h);
		void onMouseScroll(double dx, double dy);
		void onMouseButton(int button, int action, int mods);
		void onMouseMove(int x, int y);
		void onKey(int key, int scancode, int action, int mods);
	};
}