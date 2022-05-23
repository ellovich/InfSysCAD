#pragma once
#include "CAD/Core/Window.h"
#include "CAD/Core/Viewer.h"
#include "CAD/Core/Scene.h"
#include "CAD/ImGui/ImGuiLayer.h"

namespace InfSysCAD
{
    struct AppCmdLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        AppCmdLineArgs(int argc = 0, char** argv = nullptr)
            : Count(argc), Args(argv)
        {
        }

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

        Application(const char* name, Ref<AppCmdLineArgs> args);
        ~Application() = default;

        void Run();
        void Close() { m_Running = false; }

        Window& GetWindow() const { return *m_Window; }
        Viewer& GetViewer() const { return *m_Viewer; }
        Scene& GetScene() const { return *m_Scene; }
        ImGuiLayer& GetImGuiLayer() const { return *m_ImGuiLayer; }
        AppCmdLineArgs& GetArgs() const { return *m_CmdLineArgs; }

    private:
        static Application* s_Instance;

        Ref<AppCmdLineArgs> m_CmdLineArgs;

        Window* m_Window;
        Scope<Viewer> m_Viewer;
        Scope<Scene> m_Scene;
        Scope<ImGuiLayer> m_ImGuiLayer;

        bool m_Running = true;
    };
}