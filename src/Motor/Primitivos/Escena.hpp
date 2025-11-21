#pragma once
#include "../Inputs/Botones.hpp"
#include "CEPool.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

namespace CE
{
    class Escena
    {
    public:
        using HashBotones = std::map<sf::Keyboard::Scancode, std::string>;

        virtual ~Escena() = default;

        virtual void onInit() = 0;
        virtual void onFinal() = 0;
        virtual void onUpdate(float dt) = 0;
        virtual void onInputs(const CE::Botones& accion) = 0;
        virtual void onRender() = 0;

        void registrarBotones(sf::Keyboard::Scancode boton, const std::string& accion)
        {
            botones[boton] = accion;
        }

        HashBotones& getBotones()
        {
            return botones;
        }

        Pool& getPool()
        {
            return objetos;
        }

    public:
        HashBotones botones;
        std::string nombre;

    protected:
        Pool objetos{100};
        Pool enemigos{100};
    };
}
