#include "IdleFSM.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "MoverFSM.hpp"

namespace IVJ {

    IdleFSM::IdleFSM()
        :FSM{}{

        nombre = "IdleFSM";
        std::cout<<nombre<<"\n";
    }

    FSM* IdleFSM::onInputs(const CE::IControl& control) {

        if (control.arr)
            return new MoverFSM(false);
        else if (control.abj)
            return new MoverFSM(true);
        else if (control.der)
            return new MoverFSM(false);
        else if (control.izq)
            return new MoverFSM(true);
        return nullptr;
    }

    void IdleFSM::onEntrar(const Entidad& obj) {

        auto c_sprite = obj.getComponente<CE::ISprite>();
        sprite = &c_sprite->m_sprite;
        s_w = c_sprite->width;
        s_h = c_sprite->height;

        ani_frames[0] = {0.f, 96.f};
        ani_frames[1] = {0.f, 96.f};
        ani_frames[2] = {0.f, 96.f};
        ani_frames[3] = {0.f, 96.f};
        ani_frames[4] = {0.f, 96.f};
        max_tiempo = 0.25f;
        tiempo = max_tiempo;
        id_actual = 0;
    }

    void IdleFSM::onSalir(const Entidad& obj){}
    void IdleFSM::onUpdate(const Entidad& obj, float dt) {

        tiempo = tiempo - 1 * dt;
        if (tiempo <= 0) {
            sprite->setTextureRect(
                sf::IntRect{{
                    (int)ani_frames[id_actual].x,
                    (int)ani_frames[id_actual].y
                },{
                    s_w,
                    s_h
                }}
        );
            id_actual = (id_actual + 1) % 5;
            tiempo = max_tiempo;

        }
    }

}