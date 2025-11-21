#include "Motor.hpp"
#include "GUI/GLogger.hpp"
#include <utility>
#include <chrono>
#include <Motor/Render/Render.hpp>
#include <Motor/GUI/GDock.hpp>
#include <Motor/GUI/GViewport.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>
#include "Camaras/CamarasGestor.hpp"
#include "Motor/GUI/GListaObjetos.hpp"

namespace CE
{
    Motor::Motor(const MotorConfig& config, std::unique_ptr<GameLayer> ref)
        : motorInfo(std::move(config)), miApp(std::move(ref))
    {}

    int Motor::OnRun(void)
    {
        OnInit();

        auto frameAnt = std::chrono::high_resolution_clock::now();
        float ms = 0.01666f; // ~60 FPS
        sf::Clock clock;

        while (miApp->EstaCorriendo())
        {
            OnEventFrame(ms);

            ImGui::SFML::Update(Render::Get().GetVentana(), clock.restart());

            OnUpdateFrame(ms);
            OnRenderFrame(ms);

            // calcular dt
            auto frameAct = std::chrono::high_resolution_clock::now();
            ms = std::chrono::duration_cast<std::chrono::duration<float>>(frameAct - frameAnt).count();
            frameAnt = frameAct;
        }

        Render::Terminar();
        ImGui::SFML::Shutdown();

        return 0;
    }


    void Motor::OnInit(void)
    {
        // Crear ventana principal del motor
        Render::Get().CrearVentana(motorInfo);

        // Inicializar ImGui-SFML
        if (!ImGui::SFML::Init(Render::Get().GetVentana()))
            exit(-1);

        // Configuración de ImGui
        auto& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigDockingWithShift = true;

        // Capas GUI del motor
        guiLayers.push_back(std::make_shared<GDock>());
        guiLayers.push_back(std::make_shared<GViewport>());
        guiLayers.push_back(std::make_shared<GListaObjetos>());

        // Crear cámara base y activarla
        GestorCamaras::Get().agregarCamara(
            std::make_shared<Camara>(
                Vector2D{960, 540},
                Vector2D{1920, 1080}
            )
        );
        GestorCamaras::Get().setCamaraActiva(0);

        // Inicializar aplicación del usuario
        miApp->OnInit();

        // Inicializar capas GUI
        for (auto& gui : guiLayers)
            gui->OnInit(motorInfo);

        // Inicializar logger gráfico
        GLogger::Get().OnInit(motorInfo);
    }
    
    void Motor::OnUpdateFrame(float dt)
    {
        GestorCamaras::Get().onUpdateCamaras(dt);

        miApp->OnUpdate(dt);

        for (auto& gui : guiLayers)
            gui->OnUpdate(dt);

        GLogger::Get().OnUpdate(dt);
    }


    void Motor::OnEventFrame(float dt)
    {
        while (auto eventos = Render::Get().GetVentana().pollEvent())
        {
            ImGui::SFML::ProcessEvent(Render::Get().GetVentana(), *eventos);
            miApp->OnInputs(dt, eventos);
        }
    }
    void Motor::OnRenderFrame(float dt)
    {

        Render::Get().OnClearColor(sf::Color::White);
        GestorCamaras::Get().onRenderCamara(
       Render::Get().GetTextura()
        );

        miApp->OnRender(dt);

        for (auto& gui : guiLayers)
            gui->OnRender();
        GLogger::Get().OnRender();
        Render::Get().OnDisplayTextura();

        // ventana de prueba
        ImGui::Begin("TEST", nullptr, 0);
        ImGui::Text("Ventana de prueba del motor");
        ImGui::End();

        Render::Get().OnDisplayTextura();

        // ImGui windows
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        ImGui::SFML::Render(Render::Get().GetVentana());
        Render::Get().OnDisplayVentana();
    }
} // namespace CE
