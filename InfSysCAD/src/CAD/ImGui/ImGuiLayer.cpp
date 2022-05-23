#include "infSys_pch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "ImGuiStyles.h" // custom style

#include "CAD/Core/Application.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_opengl3_loader.h>
#include <ImGui_notify.h>
//#include "ImGuizmo.h"

namespace InfSysCAD
{
	ImGuiLayer::ImGuiLayer()
	{
		INFSYS_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui::SetupImGuiStyle();

		GLFWwindow* window = Application::Get().GetWindow().GetGLFWwindow();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		AddWindow(new MenuBarWindow());
		AddWindow(new PropertyWindow());
		AddWindow(new TransportArrWindow());
	}

	ImGuiLayer::~ImGuiLayer()
	{
		INFSYS_PROFILE_FUNCTION();

		for (int i = 0; i < m_imguiWindows.size(); i++)
			delete m_imguiWindows[i];

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Render()
	{
		INFSYS_PROFILE_FUNCTION();

		PreRender();

		ImGui::ShowDemoWindow();
		for (auto i = 0; i < m_imguiWindows.size(); i++)
			m_imguiWindows[i]->Render();

		RenderNotifications();

		PostRender();
	}

	void ImGuiLayer::AddWindow(ImGuiWindow* newWin)
	{
		INFSYS_PROFILE_FUNCTION();

		m_imguiWindows.push_back(newWin);
	}

	void ImGuiLayer::PreRender()
	{
		INFSYS_PROFILE_FUNCTION();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//TODO ImGuizmo::BeginFrame();

		// Create the docking environment
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoBackground;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
		ImGui::PopStyleVar(3);
		ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
		ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
		ImGui::End();
	}

	void ImGuiLayer::PostRender()
	{
		INFSYS_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	void ImGuiLayer::RenderNotifications()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.f); // Round borders
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.78f, 0.87f, 0.98f, 0.70f));
		ImGui::RenderNotifications();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);

	}
}