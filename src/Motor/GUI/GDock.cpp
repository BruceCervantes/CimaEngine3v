#include "GDock.hpp"
#include <imgui.h>
#include <imgui_internal.h>

namespace CE
{
    void GDock::OnInit(const MotorConfig& des)
    {
        this->des = des;

    #if DEBUG
        flags = ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize   |
                ImGuiWindowFlags_NoMove     |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoNavFocus |
                ImGuiWindowFlags_NoDocking  |
                ImGuiWindowFlags_MenuBar;
    #else
        flags = ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize   |
                ImGuiWindowFlags_NoMove     |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoNavFocus |
                ImGuiWindowFlags_NoDocking;
    #endif
    }

    void GDock::OnUpdate(float dt)
    {
        // lógica de actualización si hace falta
    }

    void GDock::OnRender(void)
    {
        // https://github.com/ocornut/imgui/issues/2109
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        // Inicio de ventana principal dock
        ImGui::Begin("GDock", nullptr, flags);

    #if DEBUG
        ImGuiDockNodeFlags dock_flags = 0;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::Button("Archivos")) { /* acción */ }
            if (ImGui::Button("Edit")) { /* acción */ }
            ImGui::EndMenuBar();
        }
    #else
        ImGuiDockNodeFlags dock_flags =
            ImGuiDockNodeFlags_PassthruCentralNode |
            ImGuiDockNodeFlags_AutoHideTabBar;
    #endif

        auto id = ImGui::GetID("GDock");
        dx = static_cast<unsigned int>(ImGui::GetContentRegionAvail().x);
        dy = static_cast<unsigned int>(ImGui::GetContentRegionAvail().y);

        ImGui::DockSpace(id, ImVec2(dx, dy), dock_flags);

        ImGui::DockBuilderDockWindow("GViewPort", id);
        ImGui::DockBuilderFinish(id);

        ImGui::End(); // Fin de GDock
    }
}
