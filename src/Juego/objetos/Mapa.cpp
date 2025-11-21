#include "Mapa.hpp"
#include "../../Motor/Componentes/IComponentes.hpp"

namespace IVJ {

    void Mapa::onUpdate(float dt) {

        if (tieneComponente<CE::ISprite>()) {
            auto sprite = getComponente<CE::ISprite>();
            auto pos = getTransformada()->posicion;
            sprite->m_sprite.setPosition({pos.x, pos.y});
        }
    }

    void Mapa::draw(sf::RenderTarget &target, sf::RenderStates state) const {

        state.transform *= getTransform();

        if (tieneComponente<CE::ISprite>()) {

            auto sprite = getComponente<CE::ISprite>();
            target.draw(sprite->m_sprite);
        }
    }
}