#include "Entidad.hpp"
#include "../../Motor/Componentes/IComponentes.hpp"

namespace IVJ {
    void Entidad::onUpdate(float dt) {

        if (tieneComponente<CE::ISprite>()) {
            auto sprite = getComponente<CE::ISprite>();
            auto pos = getTransformada()->posicion;
            sprite->m_sprite.setPosition({pos.x, pos.y});
            if (sprite) {
                sprite->m_sprite.setRotation(sf::degrees(getTransformada()->angulo));
            }
        }




        if (tieneComponente<IVJ::IMaquinaEstado>()) {
            auto mq = getComponente<IVJ::IMaquinaEstado>();
            if (mq->fsm)
                mq->fsm->onUpdate(*this, dt);
        }
    }

    void Entidad::draw(sf::RenderTarget &target, sf::RenderStates state) const {

        state.transform *= getTransform();

        if (tieneComponente<CE::ISprite>()) {

            auto sprite = getComponente<CE::ISprite>();
            target.draw(sprite->m_sprite);
        }
    }

    void Entidad::inputFSM() {
        if (!getComponente<IMaquinaEstado>() || !getComponente<CE::IControl>() ||
            !getComponente<IMaquinaEstado>()->fsm)
            return;

        auto mq = getComponente<IMaquinaEstado>();
        auto control = *getComponente<CE::IControl>();
        auto n_estado = mq->fsm->onInputs(control);
        std::shared_ptr<FSM> estado(n_estado);
        if (estado)
            setFSM(estado);
    }

    void Entidad::setFSM(const std::shared_ptr<FSM> &mq) {

        auto &estado_actual = getComponente<IMaquinaEstado>()->fsm;
        estado_actual->onSalir(*this);
        estado_actual = mq;
        estado_actual->onEntrar(*this);
    }
}