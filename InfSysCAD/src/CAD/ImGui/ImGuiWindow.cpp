#include "infSys_pch.h"
#include "ImGuiWindow.h"
#include "CAD/Core/Application.h"
//#include <ImGui_notify.h>

namespace 
{
	static void PushStyleCompact()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.60f)));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.60f)));
	}

	static void PopStyleCompact()
	{
		ImGui::PopStyleVar(2);
	}
}

namespace InfSysCAD
{
	void MenuBarWindow::Render()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
			/*	if (ImGui::MenuItem("Load", "CTRL + L"))
				{
					InfSysCAD::Application::Get().GetScene().LoadSTEP_File();
					ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Loaded!" });
				}*/

				if (ImGui::BeginMenu("Save"))
				{
					if (ImGui::MenuItem("Save to STEP", "CTRL + S")) { InfSysCAD::Application::Get().GetScene().SaveSTEP_File(); }
					if (ImGui::MenuItem("Save to OCAF")) { InfSysCAD::Application::Get().GetScene().SaveOCAF_File(); }
					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", "ALT + F4")) { InfSysCAD::Application::Get().Close(); }

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void PropertyWindow::Render()
	{
		ImGui::Begin("Properties");

	/*	if (ImGui::Button(ICON_FA_FILE"  Load STEP..."))
		{
			m_Loaded = InfSysCAD::Application::Get().GetScene().LoadSTEP_File();
			if (m_Loaded) ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Loaded!" });
		}*/
		ImGui::SameLine(0, 5.0f);
		ImGui::Text(InfSysCAD::Application::Get().GetScene().GetModelFilename().c_str());

		if (m_Loaded)
		{
			std::vector<std::string> tree = Application::Get().GetScene().GetTree();

			// By default, if we don't enable ScrollX the sizing policy for each columns is "Stretch"
			// Each columns maintain a sizing weight, and they will occupy all available width.
			static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

			if (ImGui::BeginTable("table1", 1, flags))
			{
				for (int row = 0; row < tree.size(); row++)
				{
					ImGui::TableNextRow();
					for (int column = 0; column < 1; column++)
					{
						ImGui::TableSetColumnIndex(column);
						ImGui::Text("%s", tree[column].c_str());
					}
				}
				ImGui::EndTable();
			}

			//{
			//	static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
			//
			//	if (ImGui::BeginTable("3ways", 3, flags))
			//	{
			//		const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
			//		// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
			//		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
			//		ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
			//		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
			//		ImGui::TableHeadersRow();
			//
			//		// Simple storage to output a dummy file-system.
			//		struct MyTreeNode
			//		{
			//			const char* Name;
			//			const char* Type;
			//			int             Size;
			//			int             ChildIdx;
			//			int             ChildCount;
			//			static void DisplayNode(const MyTreeNode* node, const MyTreeNode* all_nodes)
			//			{
			//				ImGui::TableNextRow();
			//				ImGui::TableNextColumn();
			//				const bool is_folder = (node->ChildCount > 0);
			//				if (is_folder)
			//				{
			//					bool open = ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_SpanFullWidth);
			//					ImGui::TableNextColumn();
			//					ImGui::TextDisabled("--");
			//					ImGui::TableNextColumn();
			//					ImGui::TextUnformatted(node->Type);
			//					if (open)
			//					{
			//						for (int child_n = 0; child_n < node->ChildCount; child_n++)
			//							DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
			//						ImGui::TreePop();
			//					}
			//				}
			//				else
			//				{
			//					ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
			//					ImGui::TableNextColumn();
			//					ImGui::Text("%d", node->Size);
			//					ImGui::TableNextColumn();
			//					ImGui::TextUnformatted(node->Type);
			//				}
			//			}
			//		};
			//
			//		static const MyTreeNode nodes[] =
			//		{
			//			{ "Root",                         "Folder",       -1,       1, 3    }, // 0
			//			{ "Music",                        "Folder",       -1,       4, 2    }, // 1
			//			{ "Textures",                     "Folder",       -1,       6, 3    }, // 2
			//			{ "desktop.ini",                  "System file",  1024,    -1,-1    }, // 3
			//			{ "File1_a.wav",                  "Audio file",   123000,  -1,-1    }, // 4
			//			{ "File1_b.wav",                  "Audio file",   456000,  -1,-1    }, // 5
			//			{ "Image001.png",                 "Image file",   203128,  -1,-1    }, // 6
			//			{ "Copy of Image001.png",         "Image file",   203256,  -1,-1    }, // 7
			//			{ "Copy of Image001 (Final2).png","Image file",   203512,  -1,-1    }, // 8
			//		};
			//
			//		MyTreeNode::DisplayNode(&nodes[0], nodes);
			//
			//		ImGui::EndTable();
			//	}
			//}
		}

		//if (ImGui::CollapsingHeader("Material") && mesh)
		//{
		//	ImGui::ColorPicker3("Color", (float*)&mesh->mColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
		//	ImGui::SliderFloat("Roughness", &mesh->mRoughness, 0.0f, 1.0f);
		//	ImGui::SliderFloat("Metallic", &mesh->mMetallic, 0.0f, 1.0f);
		//}

		//if (ImGui::CollapsingHeader("Light"))
		//{

		//	ImGui::Separator();
		//	ImGui::Text("Position");
		//	ImGui::Separator();
		//	nimgui::draw_vec3_widget("Position", scene_view->get_light()->mPosition, 80.0f);
		//}

		ImGui::End();
	}

	void TransportArrWindow::Render()
	{
		ImGui::Begin("Transport Array");
		{
			//ImGui::Text(ICON_FA_ARROWS_ALT);
			//ImGui::Text(ICON_FA_HAND_PAPER);
			//ImGui::Text(ICON_FA_LAYER_GROUP);
			//ImGui::Text(ICON_FA_EYE);
			//ImGui::Text(ICON_FA_EYE_SLASH);
			//ImGui::Text(ICON_FA_FILE);
			//ImGui::Text(ICON_FA_FILE_IMPORT);
			//ImGui::Text(ICON_FA_FILL_DRIP);
			//ImGui::Text(ICON_FA_LIST);
			//ImGui::Text(ICON_FA_LOCK);
			//ImGui::Text(ICON_FA_LOCK_OPEN);
			//ImGui::Text(ICON_FA_MOUSE_POINTER);
			//ImGui::Text(ICON_FA_PAINT_BRUSH);
			//ImGui::Text(ICON_FA_PAPERCLIP);
			//ImGui::Text(ICON_FA_PLUS);
			//ImGui::Text(ICON_FA_TRASH);
			//ImGui::Text(ICON_FA_TRASH_ALT);

			/*if (ImGui::Button(ICON_FA_FILE "  Load transport array..."))
			{
				m_Loaded = InfSysCAD::Application::Get().GetScene().LoadTransportArray();
				ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Loaded!" });
			}*/
			ImGui::SameLine(0, 5.0f);
			ImGui::Text(InfSysCAD::Application::Get().GetScene().GetTransportArrayFilename().c_str());

			ImGui::Spacing();

			std::vector<TransportArray> csv = Application::Get().GetScene().GetTransportArray();

			if (m_Loaded)
			{
				{
					static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
					const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

					// When using ScrollX or ScrollY we need to specify a size for our table container!
					// Otherwise by default the table will fit all available space, like a BeginChild() call.
					ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 18);
					if (ImGui::BeginTable("Transport array", 7, flags, outer_size))
					{
						ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
						ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None);
						ImGui::TableSetupColumn("Material", ImGuiTableColumnFlags_None);
						ImGui::TableSetupColumn("Grade", ImGuiTableColumnFlags_None);
						ImGui::TableSetupColumn("Thickness", ImGuiTableColumnFlags_None);
						ImGui::TableSetupColumn("Section name", ImGuiTableColumnFlags_None);
						ImGui::TableSetupColumn("Profile type", ImGuiTableColumnFlags_None);
						ImGui::TableSetupColumn("Length", ImGuiTableColumnFlags_None);

						ImGui::TableHeadersRow();

						ImGuiListClipper clipper; // using clipper for large vertical lists
						clipper.Begin(csv.size());
						while (clipper.Step())
						{
							for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
							{
								ImGui::TableNextRow();

								ImGui::TableSetColumnIndex(0); ImGui::Text("%s", csv[row].Name.c_str());
								ImGui::TableSetColumnIndex(1); ImGui::Text("%s", csv[row].Material.c_str());
								ImGui::TableSetColumnIndex(2); ImGui::Text("%s", csv[row].Grade.c_str());
								ImGui::TableSetColumnIndex(3); ImGui::Text("%s", csv[row].Thickness.c_str());
								ImGui::TableSetColumnIndex(4); ImGui::Text("%s", csv[row].Section_name.c_str());
								ImGui::TableSetColumnIndex(5); ImGui::Text("%s", csv[row].Profile_type.c_str());
								ImGui::TableSetColumnIndex(6); ImGui::Text("%s", csv[row].Length.c_str());
							}
						}
						ImGui::EndTable();
					}
				}

				//if (ImGui::Button(ICON_FA_LINK "  Merge with part"))
				//{
				//	if (Application::Get().GetScene().SetArrayMaterials())
				//	{
				//		//		m_Loaded = InfSysCAD::Application::Get().GetScene().LoadTransportArray();
				//		ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Merged!" });
				//	}
				//	else
				//	{
				//		ImGui::InsertNotification({ ImGuiToastType_Error, 3000, "Merging failed!" });
				//	}
				//}
			}
		}

		ImGui::End();
	}

	//void LogWindow::Render()
	//{
	//	//ImGui::Begin("Example: Log", &m_show);
	//
	//	//if (ImGui::SmallButton("[Debug] Add 5 entries"))
	//	//{
	//	//	static int counter = 0;
	//	//	const char* categories[3] = { "info", "warn", "error" };
	//	//	const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
	//	//	for (int n = 0; n < 5; n++)
	//	//	{
	//	//		const char* category = categories[counter % IM_ARRAYSIZE(categories)];
	//	//		const char* word = words[counter % IM_ARRAYSIZE(words)];
	//	//		log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
	//	//			ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
	//	//		counter++;
	//	//	}
	//	//}
	//	//ImGui::End();
	//}
}