#include "infsys_pch.h"
#include "CAD/Core/Application.h"
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_opengl3_loader.h>

namespace InfSysCAD
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const char* name, AppCmdLineArgs args)
		: m_CmdLineArgs(args)
	{
		INFSYS_PROFILE_FUNCTION();

		INFSYS_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = CreateScope<WindowsWindow>(name, 800, 600);
		m_Viewer = CreateScope<Viewer>(m_Window.get());       
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
	//		m_ImGuiLayer->Render();
		}
	}
}
