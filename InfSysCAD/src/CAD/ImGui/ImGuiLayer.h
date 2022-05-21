#pragma once
#include "CAD/ImGui/ImGuiWindow.h"

namespace InfSysCAD
{
    class ImGuiLayer
    {
    public:
		ImGuiLayer();
		~ImGuiLayer();

		void AddWindow(ImGuiWindow* newWin);

		void PreRender();
		void Render();
		void PostRender();

	private:
		std::vector<ImGuiWindow*> m_imguiWindows;
	};
}
