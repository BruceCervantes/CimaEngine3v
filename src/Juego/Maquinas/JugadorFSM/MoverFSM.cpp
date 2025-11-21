#include "MoverFSM.hpp"
#include "IdleFSM.hpp"

namespace IVJ {

    MoverFSM::MoverFSM(bool flip_sprite)
        :FSM{}, flip{flip_sprite} {

        nombre = "MoverFSM";
        std::cout<<nombre<<"\n";
    }

    FSM* MoverFSM::onInputs(const CE::IControl& control) {

        if (!control.arr && !control.abj && !control.der && !control.izq)
            return new IdleFSM();
        if (control.der)
            flip = false;
        else if (control.izq)
            flip = true;
        return nullptr;
    }

    void MoverFSM::onEntrar(const Entidad& obj) {

        auto c_sprite = obj.getComponente<CE::ISprite>();
        sprite = &c_sprite->m_sprite;
        s_w = c_sprite->width;
        s_h = c_sprite->height;

        ani_frames[0] = {0.f, 0.f};
        ani_frames[1] = {64.f, 0.f};
        ani_frames[2] = {128.f, 0.f};
        ani_frames[3] = {192.f, 0.f};
        ani_frames[4] = {256.f, 0.f};
        ani_frames[5] = {0.f, 96.f};
        ani_frames[6] = {64.f, 96.f};
        ani_frames[7] = {128.f, 96.f};
        ani_frames[8] = {192.f, 96.f};
        ani_frames[9] = {256.f, 96.f};
        max_tiempo = 0.25f;
        tiempo = max_tiempo;
        id_actual = 0;
        flipSprite(obj);
    }

    void MoverFSM::flipSprite(const Entidad& obj) {

        auto c_sprite = obj.getComponente<CE::ISprite>();
        if (flip)
            sprite->setScale({-c_sprite->escala, c_sprite->escala});
        else
            sprite->setScale({c_sprite->escala, c_sprite->escala});

    }

    void MoverFSM::onSalir(const Entidad& obj) {}
    void MoverFSM::onUpdate(const Entidad& obj, float dt) {

        tiempo -= dt;
        flipSprite(obj);


        auto trans = obj.getTransformada();
        bool usarHumo = true;
        std::cout << "velForward: " << trans->angulo
                  << " usarHumo: " << usarHumo << "\n";

        if (trans->velocidadForward > 100.f) {
            usarHumo = false;
        }

        // Detectar cambio de animación
        if (usarHumo != ultimoHumo) {
            id_actual = 0;   // reiniciar animación al cambiar
            tiempo = max_tiempo;
        }

        ultimoHumo = usarHumo;

        if (tiempo <= 0) {
            int frame;
            if (usarHumo) {
                frame = id_actual % 5;   // frames 0–4
            }
            else {
                frame = (id_actual % 5) + 5; // frames 5–9

            }
            sprite->setTextureRect(
                                sf::IntRect{
                                    {(int)ani_frames[frame].x,
                                        (int)ani_frames[frame].y},
                                    {s_w, s_h}
                                }
                            );
            id_actual++;
            tiempo = max_tiempo;
        }

    }
}