#include "infsys_pch.h"
#include "CAD/Core/Application.h"

#include <GLFW/glfw3.h>

namespace InfSysCAD
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const char* name, AppCmdLineArgs args)
		: m_CmdLineArgs(args)
	{
		INFSYS_PROFILE_FUNCTION();

		INFSYS_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = new Window(name, 1280, 720);
		m_Viewer = CreateScope<Viewer>(m_Window);
		m_Scene = CreateScope<Scene>(m_Viewer->GetContext());
		m_ImGuiLayer = CreateScope<ImGuiLayer>();
	}

	void Application::Run()
	{
		INFSYS_PROFILE_FUNCTION();

		while (m_Running)
		{
			INFSYS_PROFILE_SCOPE("RunLoop");

			m_Viewer->Update();
			m_ImGuiLayer->Render();
			glfwSwapBuffers(m_Window->GetGLFWwindow());
		}
		std::cout << "";
	}
}
