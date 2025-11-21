#pragma once
#include "../../Motor/Primitivos/Escena.hpp"
#include "Juego/objetos/TileMap.hpp"

namespace IVJ {

    class EscenaCirculos : public CE::Escena {
    public:
        EscenaCirculos() : CE::Escena() { }

        void onInit();
        void onFinal();
        void onUpdate(float dt);
        void onInputs(const CE::Botones& accion);
        void onRender();

    private:
        bool inicializar { true };
        TileMap bg[2];

    };

}
