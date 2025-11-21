#pragma once
#include <string>
#include <Motor/App/ALayer.hpp>
#include <Motor/GUI/GLayer.hpp>
#include <Motor/Utils/Utils.hpp>
#include <memory>
#include <vector>

namespace CE
{
    class Motor
    {
    public:
        // Constructor recibe configuración del motor + puntero al juego
        Motor(const MotorConfig& config, std::unique_ptr<GameLayer> ref);

        // Loop principal
        int OnRun(void);

    private:
        // Métodos internos del ciclo de vida
        void OnInit(void);
        void OnEventFrame(float dt);
        void OnUpdateFrame(float dt);
        void OnRenderFrame(float dt);

    private:
        MotorConfig motorInfo;  // Configuración general del motor

        // Puntero a la capa principal del juego (escena/lógica)
        std::unique_ptr<GameLayer> miApp;

        // Capas de GUI (como GDock, GViewport, etc.)
        std::vector<std::shared_ptr<GLayer>> guiLayers;
    };
}
