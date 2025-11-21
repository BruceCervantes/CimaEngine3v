#pragma once
#include "../../Motor/Primitivos/Objetos.hpp"
#include "../Maquinas/FSM.hpp"
#include <memory>

namespace IVJ {
    class FSM;
    class Entidad : public CE::Objeto {
        public:

        void onUpdate(float dt) override;
        void draw(sf::RenderTarget &target, sf::RenderStates state) const override;

        void setFSM(const std::shared_ptr<FSM> &mq);
        void inputFSM();
    };
}