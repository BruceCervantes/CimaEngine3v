#pragma once
#include <SFML/Graphics.hpp>
#include <optional> // necesario para std::optional

namespace CE
{
    class GameLayer
    {
    public:
        virtual ~GameLayer() { }

        // Inicialización de la capa del juego
        virtual void OnInit(void) { }

        // Controla si el juego sigue corriendo
        virtual bool EstaCorriendo(void) { return !termino; }

        // Manejo de inputs/eventos
        virtual void OnInputs(float dt, std::optional<sf::Event>& eventos) { }

        // Actualización de lógica
        virtual void OnUpdate(float dt) { }

        // Renderizado
        virtual void OnRender(float dt) { }

    protected:
        bool termino = false; // por defecto el juego corre
    };
}
