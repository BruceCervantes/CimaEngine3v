#pragma once

#include "../Maquinas/FSM.hpp"
#include "../../Motor/Componentes/IComponentes.hpp"

namespace IVJ {

    class FSM;
    class IMaquinaEstado : public CE::IComponentes{
    public:
        explicit IMaquinaEstado();
        ~IMaquinaEstado() override{};

    public:
        std::shared_ptr<FSM> fsm;
    };
}