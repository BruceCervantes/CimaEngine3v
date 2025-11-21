#include "CamarasGestor.hpp"

namespace CE
{
    GestorCamaras& GestorCamaras::Get()
    {
        if (!instancia)
            instancia = new GestorCamaras();
        return *instancia;
    }

    void GestorCamaras::agregarCamara(const std::shared_ptr<Camara>& nueva)
    {
        m_listaCamaras.push_back(nueva);
    }

    void GestorCamaras::setCamaraActiva(int id)
    {
        if (id > static_cast<int>(m_listaCamaras.size()) - 1 || id < 0)
            return;

        if (auto actual = m_camaraActiva.lock())
            actual->esta_activo = false;

        m_camaraActiva = m_listaCamaras[id];
        m_camaraActiva.lock()->esta_activo = true;
    }

    void GestorCamaras::onUpdateCamaras(float dt)
    {
        if (auto cam = m_camaraActiva.lock())
            cam->onUpdate(dt);
    }

    void GestorCamaras::onRenderCamara(sf::RenderTexture& tex)
    {
        if (auto cam = m_camaraActiva.lock())
            tex.setView(cam->getView());
    }
}
