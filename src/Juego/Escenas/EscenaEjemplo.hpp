#pragma once
#include "../../Motor/Primitivos/Escena.hpp"
#include "../objetos/Entidad.hpp"
#include "../objetos/TileMap.hpp"
#include <memory>

namespace IVJ
{
    class EscenaEjemplos :public CE::Escena
    {
    public:
        EscenaEjemplos():CE::Escena{}{};
        void onInit();
        void onFinal();
        void onUpdate(float dt);
        void onInputs(const CE::Botones& accion);
        void onRender();
    private:
        bool inicializar{true};
        std::shared_ptr<Entidad> jugador;
        TileMap bg[2];
    };
}
