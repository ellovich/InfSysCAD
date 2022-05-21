#pragma once
#include "infSys_pch.h"
#include "CAD/Core/WindowsWindow.h"

#include <CAD/Core/Application.h>

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

namespace InfSysCAD
{
	static void glfwErrorCallbackFunc(int error, const char* description)
	{
		INFSYS_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const char* title, int w, int h)
	{
		glfwSetErrorCallback(glfwErrorCallbackFunc);

		int succsess = glfwInit();
		INFSYS_ASSERT(succsess, "Failed to init GLFW!");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#if defined (__APPLE__)
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined (INFSYS_DEBUG)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
		INFSYS_INFO("Creating window {0}: ({1}, {2})", title, w, h);
		m_GLFWwindow = glfwCreateWindow(w, h, title, NULL, NULL);
		INFSYS_ASSERT(m_GLFWwindow, "Failed to create window!");

		glfwGetWindowPos(m_GLFWwindow, &m_xLeft, &m_yTop);
		m_xRight = m_xLeft + w;
		m_yBottom = m_yTop + h;

#if !defined(_WIN32) && !defined(__APPLE__)
		m_Display = new Aspect_DisplayConnection(glfwGetX11Display());
#endif

		glfwMakeContextCurrent(m_GLFWwindow);
		glfwSwapInterval(1);
	}


	void WindowsWindow::Close()
	{
		if (m_GLFWwindow != nullptr)
		{
			glfwDestroyWindow(m_GLFWwindow);
			m_GLFWwindow = nullptr;
		}
	}


	Aspect_RenderingContext WindowsWindow::NativeGlContext() const
	{
#if defined (__APPLE__)
		return (NSOpenGLContext*)glfwGetNSGLContext(m_GLFWwindow);
#elif defined (_WIN32)
		return glfwGetWGLContext(m_GLFWwindow);
#else
		return glfwGetGLXContext(m_GLFWwindow);
#endif	
	}

	// TODO get size from ImGui Texture!!!
	Graphic3d_Vec2i WindowsWindow::CursorPosition() const
	{
		Graphic3d_Vec2d aPos;
		glfwGetCursorPos(m_GLFWwindow, &aPos.x(), &aPos.y());
		return Graphic3d_Vec2i((int)aPos.x(), (int)aPos.y());
	}

	Aspect_Drawable WindowsWindow::NativeHandle() const
	{
#if defined (__APPLE__)
		return (Aspect_Drawable)glfwGetCocoaWindow(m_GLFWwindow);
#elif defined (_WIN32)
		return (Aspect_Drawable)glfwGetWin32Window(m_GLFWwindow);
#else
		return (Aspect_Drawable)glfwGetX11Window(m_GLFWwindow);
#endif	
	}

	// TODO get size from ImGui Texture!!!
	Aspect_TypeOfResize WindowsWindow::DoResize()
	{
		if (glfwGetWindowAttrib(m_GLFWwindow, GLFW_VISIBLE) == 1)
		{
			int anXPos = 0, anYPos = 0, aWidth = 0, aHeight = 0;
			glfwGetWindowPos(m_GLFWwindow, &anXPos, &anYPos);
			glfwGetWindowSize(m_GLFWwindow, &aWidth, &aHeight);
			m_xLeft = anXPos;
			m_xRight = anXPos + aWidth;
			m_yTop = anYPos;
			m_yBottom = anYPos + aHeight;
		}
		return Aspect_TOR_UNKNOWN;
	}

	bool WindowsWindow::IsMapped() const
	{
		return glfwGetWindowAttrib(m_GLFWwindow, GLFW_VISIBLE) != 0;
	}

	void WindowsWindow::Map() const
	{
		glfwShowWindow(m_GLFWwindow);
	}

	void WindowsWindow::Unmap() const
	{
		glfwHideWindow(m_GLFWwindow);
	}
}