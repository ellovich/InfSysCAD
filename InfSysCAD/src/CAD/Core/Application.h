#pragma once
#include "CAD/Core/Base.h"
#include "CAD/Core/WindowsWindow.h"
#include "CAD/Core/Viewer.h"
#include "CAD/Core/Scene.h"
#include "CAD/ImGui/ImGuiLayer.h"

namespace InfSysCAD
{
    struct AppCmdLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
            INFSYS_ASSERT(index < Count);
            return Args[index];
        }
    };

    class Application
    {
    public:
        static Application& Get() { return *s_Instance; }

        Application(const char* name = "InfSysCAD", AppCmdLineArgs args = AppCmdLineArgs());
        ~Application() = default;

        void Run();
        void Close() { m_Running = false; }

        WindowsWindow& GetWindow() { return *m_Window; }
        Viewer& GetViewer() { return *m_Viewer; }
        ImGuiLayer& GetImGuiLayer() { return *m_ImGuiLayer; }
        Scene& GetScene() { return *m_Scene; }

    private:
        static Application* s_Instance;

        AppCmdLineArgs m_CmdLineArgs;

        Scope<WindowsWindow> m_Window;
        Scope<Viewer> m_Viewer;
        Scope<Scene> m_Scene;
        Scope<ImGuiLayer> m_ImGuiLayer;

        bool m_Running = true;
    };
}