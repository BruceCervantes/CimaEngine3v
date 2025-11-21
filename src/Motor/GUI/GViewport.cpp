#include "GViewport.hpp"
#include <Motor/Render/Render.hpp>
#include <imgui_internal.h>
#include <imgui-SFML.h>
#include <iostream>

namespace CE
{
    void GViewport::OnInit(const MotorConfig& des)
    {
        vw = 0;
        vh = 0;
        motorInfo = des;
    }

    void GViewport::OnUpdate(float dt)
    {
        // Actualización de lógica del viewport (si hace falta)
    }

    void GViewport::OnRender(void)
    {
#if DEBUG
        ImGui::Begin("GViewPort", nullptr, ImGuiWindowFlags_NoCollapse);
#else
        ImGui::Begin("GViewPort", nullptr,
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoMove);
#endif

        // Obtener dimensiones disponibles
        vw = static_cast<unsigned int>(ImGui::GetContentRegionAvail().x);
        vh = static_cast<unsigned int>(ImGui::GetContentRegionAvail().y);

#if DEBUG
        // En debug, si el área disponible es 0, asignamos un tamaño mínimo
        if (vw == 0 || vh == 0)
        {
            vw = motorInfo.vW - static_cast<unsigned int>(motorInfo.vW * 0.25f);
            vh = motorInfo.vH - static_cast<unsigned int>(motorInfo.vH * 0.25f);
        }
#endif

        // Renderizar la textura en ImGui
        ImGui::Image(
            Render::Get().GetTextura(),                // textura
            sf::Vector2f(vw, vh),                      // tamaño
            sf::Color::White,                          // color mask
            sf::Color::Transparent                     // borde
        );

        // Ajustar automáticamente el tamaño de la textura
        Render::Get().AutoResizeTextura(vw, vh);

        ImGui::End();
    }
}
