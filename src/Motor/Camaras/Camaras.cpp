#include "Camaras.hpp"
#include <algorithm>
#include <cmath>
#include "Motor/GUI/GLogger.hpp"

namespace CE {
    int Camara::num_camaras;
    Camara::Camara(float x, float y, float w, float h):
    cam_width{w},cam_height{h},esta_activo{false}
    {
        m_transform=std::make_shared<ITransform>(Vector2D{x,y},Vector2D{0,0},0);
        m_view=std::make_shared<sf::View>(sf::FloatRect{{x,y},{w,h}});
        m_view->setCenter(sf::Vector2{cam_width/2.f,cam_height/2.f});
        Camara::num_camaras++;
        nombre="Camara Default#" + std::to_string(Camara::num_camaras);
    }
    Camara::Camara(const Vector2D& pos, const Vector2D& dim):
    cam_width{dim.x},cam_height{dim.y},esta_activo{false}
    {
        m_transform=std::make_shared<ITransform>(pos,Vector2D{0,0},0);
        m_view=std::make_shared<sf::View>(sf::FloatRect{{pos.x,pos.y},{cam_width,cam_height}});
        m_view->setCenter(sf::Vector2{cam_width/2.f,cam_height/2.f});
        Camara::num_camaras++;
        nombre="Camara Default#" + std::to_string(Camara::num_camaras);
    }
    void Camara::setViewSize(float x, float y) {
        m_view->setSize({x,y});
    }
    void Camara::lockEnObjeto(const std::shared_ptr<Objeto> &obj) {
        m_lockObj=obj;
    }
    void Camara::onUpdate(float dt) {
        m_view->setCenter({m_transform->posicion.x,m_transform->posicion.y});
    }


    CamaraCuadro::CamaraCuadro(const Vector2D& pos, const Vector2D& dim)
        : Camara{ pos, dim }, limitex{ dim.x }, limitey{ dim.y }
    {
        nombre = "Camara Cuadro #" + std::to_string(Camara::num_camaras);
    }

    void CamaraCuadro::onUpdate(float dt)
    {
        Camara::onUpdate(dt);

        if (!m_lockObj.lock())
            return;

        auto mitad = Vector2D(cam_width, cam_height).escala(0.5f);
        auto obj_trans = m_lockObj.lock()->getTransformada();
        auto opos = obj_trans->posicion;

        std::string log = "Obj: (" + std::to_string(opos.x) + ", " + std::to_string(opos.y) + ")";
        std::string log2 = nombre + ": (" +
                           std::to_string(m_transform->posicion.x) + ", " +
                           std::to_string(m_transform->posicion.y) + ")";

        GLogger::Get().agregarLog(log, GLogger::Niveles::LOG_DEBUG);
        GLogger::Get().agregarLog(log2, GLogger::Niveles::LOG_DEBUG);

        if (obj_trans->posicion.x > (m_transform->posicion.x + mitad.x))
        {
            m_transform->posicion.x += cam_width;
            limitex += limitex;
        }
        else if (opos.x < (m_transform->posicion.x - mitad.x))
        {
            // Se mueve hacia la izquierda
            m_transform->posicion.x -= cam_width;
            limitex -= cam_width;
        }
    }


    CamaraAreaBox::CamaraAreaBox(const Vector2D& pos, const Vector2D& dim)
        : Camara{ pos, dim }, dimensionx{ dim.x }, dimensiony{ dim.y }
    {
        nombre = "Camara AreaBox #" + std::to_string(Camara::num_camaras);
    }

    void CamaraAreaBox::onUpdate(float dt)
    {
        Camara::onUpdate(dt);

        auto obj = m_lockObj.lock();
        if (!obj) return;

        auto opos = obj->getTransformada()->posicion;

        float mitadX = dimensionx / 2.0f;


        float marginX = dimensionx * 0.50f;

        if (opos.x > m_transform->posicion.x + mitadX - marginX)
            m_transform->posicion.x = opos.x - (mitadX - marginX);
        else if (opos.x < m_transform->posicion.x - mitadX + marginX)
            m_transform->posicion.x = opos.x + (mitadX - marginX);


        if (m_transform->posicion.x < mitadX)
            m_transform->posicion.x = mitadX;

        if (m_transform->posicion.x > 1920 - mitadX)
            m_transform->posicion.x = 1920 - mitadX;

    }

 CamaraTracking::CamaraTracking(const Vector2D& pos, const Vector2D& dim)
: Camara(pos, dim),
  camAngle(0.f),
  smoothing(8.f),
  hasBounds(false)      // cámara mira 200px hacia adelante
{
    nombre = "Camara Tracking";
        followOffset = {0.f, -250.f};
        currentRotation = sf::degrees(0.f);
}


void CamaraTracking::setFollowOffset(const Vector2D& offset) {
    followOffset = offset;
}

void CamaraTracking::setSmoothing(float s) {
    smoothing = s;
}


    void CamaraTracking::onUpdate(float dt)
    {
        Camara::onUpdate(dt);

        auto obj = m_lockObj.lock();
        if (!obj) return;

        auto objTrans = obj->getTransformada();
        if (!objTrans) return;

        // 1) ROTACIÓN SUAVE DE CÁMARA
        float targetDeg  = objTrans->angulo;              // ángulo del coche
        float currentDeg = currentRotation.asDegrees();   // ángulo actual de la cámara

        float diff = targetDeg - currentDeg;

        // normalizar diff a [-180, 180] para el camino corto
        while (diff > 180.f)  diff -= 360.f;
        while (diff < -180.f) diff += 360.f;

        // suavizado de rotación
        currentDeg += diff * 10.0f * dt;

        currentRotation = sf::degrees(currentDeg);
        m_view->setRotation(currentRotation);

        // ============================
        // 2) POSICIÓN CON OFFSET ADELANTE

        // followOffset está en espacio local del coche:
        // X = derecha, Y- = hacia adelante
        // Por ejemplo: followOffset = {0.f, -250.f};
        float angRad = currentDeg * 3.14159265f / 180.f;
        float cosA   = std::cos(angRad);
        float sinA   = std::sin(angRad);

        // rotamos el offset local al espacio del mundo
        float offX =  followOffset.x * cosA - followOffset.y * sinA;
        float offY =  followOffset.x * sinA + followOffset.y * cosA;

        // queremos que la CÁMARA esté ADELANTE del coche:
        // centro cámara = posición coche + offset rotado
        Vector2D targetPos{
            objTrans->posicion.x + offX,
            objTrans->posicion.y + offY
        };

        // suavizamos el movimiento de la cámara hacia targetPos
        Vector2D delta = targetPos - m_transform->posicion;
        m_transform->posicion.suma( delta.escala(smoothing * dt) );


        // 3) ACTUALIZAR VIEW
        m_view->setCenter({ m_transform->posicion.x, m_transform->posicion.y });
    }


}
