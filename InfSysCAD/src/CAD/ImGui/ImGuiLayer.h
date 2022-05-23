#pragma once
#include "CAD/ImGui/ImGuiWindow.h"

namespace InfSysCAD
{
    class ImGuiLayer
    {
    public:
		ImGuiLayer();
		~ImGuiLayer();

		void Render();

		void AddWindow(ImGuiWindow* newWin);

	private:
		void PreRender();
		void PostRender();
		void RenderNotifications();

	private:
		std::vector<ImGuiWindow*> m_imguiWindows;
	};
}
