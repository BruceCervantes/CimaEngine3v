#pragma once
#include "GLayer.hpp"
#include "../Primitivos/Objetos.hpp"
#include "../Componentes/IComponentes.hpp"

namespace CE
{
    class GPropiedades : public GLayer
    {
    public:
        ~GPropiedades() override{};
        void OnInit(const MotorConfig& des) override;
        void OnUpdate(float dt) override;
        void OnRender(void) override;
        //ref.get() para pasar el rawpointer
        void seleccionarObjeto(Objeto* ref);
    private:
        bool panelTransformada();
        bool panelSprite(ISprite *sprite);
        bool panelShaders(IShader *shader);
        bool panelIControl(IControl *control);
    private:
        Objeto *objeto_select{nullptr};
        int m_openglMajor{};
        int m_openglMinor{};
    };
}
