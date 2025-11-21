#pragma once
#include <Motor/App/ALayer.hpp>
#include "Figuras/Figura.hpp"
#include "Motor/Primitivos/Escena.hpp"
#include <memory>

namespace IVJ
{
    class Juego : public CE::GameLayer
    {
        public:
            explicit Juego();
            void OnInit(void);
            void OnInputs(float,std::optional<sf::Event>& eventos);
            void OnUpdate(float dt);
            void OnRender(float dt);
        private:
            CE::Escena* escenaActual;
    };
}
