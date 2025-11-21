#pragma once
#include "../../Motor/Primitivos/Escena.hpp"

namespace IVJ {

    class EscenaMuerte : public CE::Escena {
    public:
        EscenaMuerte() : CE::Escena() { }
        void onInit();
        void onFinal();
        void onUpdate(float dt);
        void onInputs(const CE::Botones& accion);
        void onRender();

    private:
        bool inicializar { true };
    };

}
