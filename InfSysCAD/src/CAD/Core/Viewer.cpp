#pragma once
#include "infSys_pch.h"
#include <CAD/Core/Viewer.h>

#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Graphic3d_Camera.hxx>

#include <GLFW/glfw3.h>
#include <CAD/Core/Application.h>
#include <backends/imgui_impl_opengl3_loader.h>

#if defined (__APPLE__)
#undef Handle // avoid name collisions in macOS headers
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#elif defined (_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#else
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <CAD/Core/Application.h>

#include <Aspect_RenderingContext.hxx>

namespace
{
	//! Convert GLFW mouse button into Aspect_VKeyMouse.
	static Aspect_VKeyMouse MouseButtonFromGlfw(int theButton)
	{
		switch (theButton)
		{
		case GLFW_MOUSE_BUTTON_LEFT:   return Aspect_VKeyMouse_LeftButton;
		case GLFW_MOUSE_BUTTON_RIGHT:  return Aspect_VKeyMouse_RightButton;
		case GLFW_MOUSE_BUTTON_MIDDLE: return Aspect_VKeyMouse_MiddleButton;
		}

		return Aspect_VKeyMouse_NONE;
	}

	//! Convert GLFW key modifiers into Aspect_VKeyFlags.
	static Aspect_VKeyFlags KeyFlagsFromGlfw(int theFlags)
	{
		Aspect_VKeyFlags aFlags = Aspect_VKeyFlags_NONE;

		if ((theFlags & GLFW_MOD_SHIFT) != 0)
			aFlags |= Aspect_VKeyFlags_SHIFT;
		if ((theFlags & GLFW_MOD_CONTROL) != 0)
			aFlags |= Aspect_VKeyFlags_CTRL;
		if ((theFlags & GLFW_MOD_ALT) != 0)
			aFlags |= Aspect_VKeyFlags_ALT;
		if ((theFlags & GLFW_MOD_SUPER) != 0)
			aFlags |= Aspect_VKeyFlags_META;

		return aFlags;
	}
}

namespace InfSysCAD
{
	Viewer::Viewer(Window* window)
		: m_Window(window)
	{
		InitCallbacks();

		const Handle(OpenGl_GraphicDriver) aGraphicDriver = new OpenGl_GraphicDriver(m_Window->GetDisplay(), false);
		aGraphicDriver->SetBuffersNoSwap(true); // swapping in mainloop

		// V3d Viewer
		const Handle(V3d_Viewer) aViewer = new V3d_Viewer(aGraphicDriver);
		aViewer->SetDefaultLights();
		aViewer->SetLightOn();
		aViewer->SetDefaultTypeOfView(V3d_PERSPECTIVE);
		aViewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);
		aViewer->SetDefaultViewSize(40000.); // todo change to FitAll

		// V3d_View
		m_View = aViewer->CreateView();
		m_View->SetImmediateUpdate(false);
		m_View->SetWindow(m_Window, m_Window->NativeGlContext());
		// optional
		m_View->ChangeRenderingParams().ToShowStats = true;
		m_View->ChangeRenderingParams().RenderResolutionScale = 2.0f;
		m_View->ChangeRenderingParams().StatsPosition = new Graphic3d_TransformPers(Graphic3d_TMF_2d, Aspect_TOTP_RIGHT_UPPER, Graphic3d_Vec2i(20, 20));
		m_View->SetBgGradientColors(
			Quantity_Color(0.75, 0.78, 0.95, Quantity_TOC_RGB),
			Quantity_Color(0.63, 0.6, 0.61, Quantity_TOC_RGB),
			Aspect_GFM_DIAG1
		);
		m_View->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_BLUE1, 0.12, V3d_ZBUFFER);
		m_View->SetShadingModel(V3d_PHONG);

		// AIS_InteractiveContext
		m_InteractiveContext = new AIS_InteractiveContext(aViewer);
		m_InteractiveContext->Activate(4, true); // faces
		m_InteractiveContext->Activate(2, true); // edges
		m_InteractiveContext->SetDisplayMode(AIS_Shaded, true);

		// Configure some global props.
		const Handle(Prs3d_Drawer)& contextDrawer = m_InteractiveContext->DefaultDrawer();
		if (!contextDrawer.IsNull())
		{
			const Handle(Prs3d_ShadingAspect)& SA = contextDrawer->ShadingAspect();
			const Handle(Graphic3d_AspectFillArea3d)& FA = SA->Aspect();
			contextDrawer->SetFaceBoundaryDraw(true); // Draw edges.
			FA->SetEdgeOff();

			// Fix for inifinite lines has been reduced to 1000 from its default value 500000.
			contextDrawer->SetMaximalParameterValue(1000);
		}
	}

	void Viewer::Update()
	{
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		{
			INFSYS_PROFILE_SCOPE("FlushViewEvents");
			FlushViewEvents(m_InteractiveContext, m_View, false);
		}

		{
			INFSYS_PROFILE_SCOPE("Redraw");
			m_View->Redraw();
		}

		glfwWaitEvents();
		{
			INFSYS_PROFILE_SCOPE("HandleViewEvents");
			HandleViewEvents(m_InteractiveContext, m_View);
		}
	}


	void Viewer::InitCallbacks()
	{
		INFSYS_PROFILE_FUNCTION();

		glfwSetWindowUserPointer(m_Window->GetGLFWwindow(), this);

		glfwSetWindowCloseCallback(m_Window->GetGLFWwindow(), [](GLFWwindow* window)
			{
				auto app = &InfSysCAD::Application::Get();
				app->Close();
			});
		glfwSetWindowSizeCallback(m_Window->GetGLFWwindow(), [](GLFWwindow* window, int w, int h)
			{
				auto view = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
				view->onResize(w, h);
			});
		glfwSetFramebufferSizeCallback(m_Window->GetGLFWwindow(), [](GLFWwindow* window, int w, int h)
			{
				auto view = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
				view->onResize(w, h);
			});
		glfwSetScrollCallback(m_Window->GetGLFWwindow(), [](GLFWwindow* window, double dx, double dy)
			{
				auto view = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
				view->onMouseScroll(dx, dy);
			});
		glfwSetMouseButtonCallback(m_Window->GetGLFWwindow(), [](GLFWwindow* window, int button, int action, int mods)
			{
				auto view = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
				view->onMouseButton(button, action, mods);
			});
		glfwSetCursorPosCallback(m_Window->GetGLFWwindow(), [](GLFWwindow* window, double x, double y)
			{
				auto view = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
				view->onMouseMove((int)x, (int)y);
			});
		glfwSetKeyCallback(m_Window->GetGLFWwindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto view = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
				view->onKey(key, scancode, action, mods);
			});
	}


#pragma region EVENTS

	void Viewer::onResize(int w, int h)
	{
		m_View->Window()->DoResize();
		m_View->MustBeResized();
		m_View->Invalidate();
		m_View->Redraw();
	}

	void Viewer::onMouseScroll(double dx, double dy)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent((float)dx, (float)dy);

		if (!io.WantCaptureMouse) // ONLY forward mouse data to your underlying app/game.
		{
			UpdateZoom(Aspect_ScrollDelta(m_Window->CursorPosition(), int(dy * 8.0)));
		}
	}

	void Viewer::onMouseButton(int button, int action, int mods)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(button, action);

		if (!io.WantCaptureMouse) // ONLY forward mouse data to your underlying app/game.
		{
			const Graphic3d_Vec2i aPos = m_Window->CursorPosition();
			if (action == GLFW_PRESS)
				PressMouseButton(aPos, ::MouseButtonFromGlfw(button), ::KeyFlagsFromGlfw(mods), false);
			else
				ReleaseMouseButton(aPos, ::MouseButtonFromGlfw(button), ::KeyFlagsFromGlfw(mods), false);
		}
	}

	void Viewer::onMouseMove(int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent((float)x, (float)y);

		if (!io.WantCaptureMouse) // ONLY forward mouse data to your underlying app/game.
		{
			const Graphic3d_Vec2i aNewPos(x, y);
			UpdateMousePosition(aNewPos, PressedMouseButtons(), LastMouseFlags(), false);
		}
	}

	void Viewer::onKey(int key, int scancode, int action, int mods)
	{
		ImGuiIO& io = ImGui::GetIO();
		//	io.AddKeyEvent(key, action);

		if (!io.WantCaptureKeyboard)
		{
			switch (key)
			{
			case GLFW_KEY_F:
			{
				if (m_InteractiveContext->NbSelected() > 0)
					m_InteractiveContext->FitSelected(m_View);
				else
					m_View->FitAll();
				break;
			}

			case GLFW_KEY_S:
			case GLFW_KEY_W:
			{
				const int dm = (key == GLFW_KEY_S) ? AIS_Shaded : AIS_WireFrame;

				if (m_InteractiveContext->NbSelected() == 0)
				{
					m_InteractiveContext->SetDisplayMode(dm, false);
					m_InteractiveContext->UpdateCurrentViewer();
				}
				else
				{
					for (m_InteractiveContext->InitSelected(); m_InteractiveContext->MoreSelected(); m_InteractiveContext->NextSelected())
					{
						m_InteractiveContext->SetDisplayMode(m_InteractiveContext->SelectedInteractive(), dm, false);
					}
					m_InteractiveContext->UpdateCurrentViewer();
				}
				break;
			}

			// axonometry-top-bottom-left-right views
			case GLFW_KEY_BACKSPACE:
			{
				m_View->SetProj(V3d_XposYnegZpos);	// Axonometry
				m_View->Redraw();
				break;
			}
			case GLFW_KEY_T:
			{
				m_View->SetProj(V3d_TypeOfOrientation_Zup_Top);
				m_View->Redraw();
				break;
			}
			case GLFW_KEY_B:
			{
				m_View->SetProj(V3d_TypeOfOrientation_Zup_Bottom);
				m_View->Redraw();
				break;
			}
			case GLFW_KEY_L:
			{
				m_View->SetProj(V3d_TypeOfOrientation_Zup_Left);
				m_View->Redraw();
				break;
			}
			case GLFW_KEY_R:
			{
				m_View->SetProj(V3d_TypeOfOrientation_Zup_Right);
				m_View->Redraw();
				break;
			}

			// perspective-orthographic
			case GLFW_KEY_P:
			{
				m_View->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Perspective);
				m_View->Redraw();
				break;
			}
			case GLFW_KEY_O:
			{
				m_View->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Orthographic);
				m_View->Redraw();
				break;
			}

			default: break;
			}
		}
	}

#pragma endregion	
}