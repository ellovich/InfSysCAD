#include "infSys_pch.h"
#include "ImGuiWindow.h"
#include "CAD/Core/Application.h"

namespace InfSysCAD
{
	void LogWindow::Render()
	{
		ImGui::Begin("Example: Log", &m_show);

		if (ImGui::SmallButton("[Debug] Add 5 entries"))
		{
			static int counter = 0;
			const char* categories[3] = { "info", "warn", "error" };
			const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
			for (int n = 0; n < 5; n++)
			{
				const char* category = categories[counter % IM_ARRAYSIZE(categories)];
				const char* word = words[counter % IM_ARRAYSIZE(words)];
				log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
					ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
				counter++;
			}
		}

		// log.Draw(
		ImGui::End();
	}

	void MenuBarWindow::Render()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load")) {}
				if (ImGui::MenuItem("Save")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}






	//PropertyWindow::PropertyWindow()
	//{
	//	m_CurrentFile = "< ... >";

	//	m_FileDialog.SetTitle("Open mesh");
	////	m_FileDialog.SetFileFilters({ ".fbx", ".obj" });
	//}

	//void PropertyWindow::Render()
	//{
	//	ImGui::Begin("Properties");
	//	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	//	{
	//		if (ImGui::Button("Open..."))
	//		{
	//			m_FileDialog.Open();
	//		}
	//		ImGui::SameLine(0, 5.0f);
	//		ImGui::Text(m_CurrentFile.c_str());
	//	}

	//	//if (ImGui::CollapsingHeader("Material") && mesh)
	//	//{
	//	//	ImGui::ColorPicker3("Color", (float*)&mesh->mColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
	//	//	ImGui::SliderFloat("Roughness", &mesh->mRoughness, 0.0f, 1.0f);
	//	//	ImGui::SliderFloat("Metallic", &mesh->mMetallic, 0.0f, 1.0f);
	//	//}

	//	//if (ImGui::CollapsingHeader("Light"))
	//	//{

	//	//	ImGui::Separator();
	//	//	ImGui::Text("Position");
	//	//	ImGui::Separator();
	//	//	nimgui::draw_vec3_widget("Position", scene_view->get_light()->mPosition, 80.0f);
	//	//}

	//	ImGui::End();

	//	m_FileDialog.Display();
	//	if (m_FileDialog.HasSelected())
	//	{
	//		auto file_path = m_FileDialog.GetSelected().string();
	//		m_CurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);

	//	//	m_MeshLoadCallback(file_path);

	//		m_FileDialog.ClearSelected();
	//	}
	//}
}