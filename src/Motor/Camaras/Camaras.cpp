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





    /*
    CamaraTracking::CamaraTracking(const Vector2D& pos, const Vector2D& dim)
      : Camara(pos, dim), smoothing(8.f), hasBounds(false) {
        nombre = "Camara Tracking #" + std::to_string(Camara::num_camaras);
        followOffset = {0.f, 0.f};
        velocity = {0.f, 0.f};
    }

    void CamaraTracking::setFollowOffset(const Vector2D& offs) { followOffset = offs; }

    void CamaraTracking::setSmoothing(float s) { smoothing = s; }

    void CamaraTracking::setBounds(float minX, float minY, float maxX, float maxY) {
        hasBounds = true;
        boundMin = {minX, minY};
        boundMax = {maxX, maxY};
    }

    void CamaraTracking::onUpdate(float dt) {
    auto obj = m_lockObj.lock();
    if (!obj) return;

    auto objTrans = obj->getTransformada();

    // =========================
    // 1. ROTACIÓN SUAVE DE LA CÁMARA
    // =========================
    // Queremos que la cámara gire siguiendo el ángulo del coche,
    // pero suavemente, no de golpe.

    float targetAngle = objTrans->angulo;                // ángulo del coche (en grados)
    float currentAngle = m_view->getRotation();    // ángulo actual de la cámara

    // Tomar siempre el camino más corto (evita giros locos al cruzar 360/0)
    float diff = targetAngle - currentAngle;
    while (diff > 180.f)  diff -= 360.f;
    while (diff < -180.f) diff += 360.f;

    float rotationSmooth = 6.f; // entre más grande, más rápida la cámara
    currentAngle += diff * smoothing * dt;

    currentRotation = sf::degrees(currentAngle);
    m_view->setRotation(currentAngle);

    // =========================
    // 2. POSICIÓN DE LA CÁMARA: AUTO MÁS ABAJO EN LA PANTALLA
    // =========================
    // Queremos que el coche aparezca un poco POR DEBAJO del centro,
    // para ver más pista hacia adelante. Eso es un offset en "pantalla"
    // hacia arriba; cuando rotamos la cámara, hay que convertir ese offset
    // a coordenadas del mundo.

    // Offset en pantalla: (x=0, y=-offset) => centro un poco arriba del coche.
    float screenOffset = 10.f; // píxeles "hacia adelante" (juega con este valor)

    // Calcular vector "arriba de la pantalla" en coordenadas del mundo:
    float angRad = currentAngle * 3.14159265f / 180.f;

    // upScreen = (0, -1)
    // upWorld = rotación(upScreen, ang) => ( sin(ang), -cos(ang) )
    CE::Vector2D upWorld{ std::sin(angRad), -std::cos(angRad) };

    // Offset de la cámara en el mundo
    CE::Vector2D worldOffset = upWorld.escala(screenOffset);

    // La posición objetivo de la cámara es: posición del coche + offset
    CE::Vector2D objetivo = objTrans->posicion + worldOffset;

    // =========================
    // 3. SEGUIMIENTO SUAVIZADO DE POSICIÓN
    // =========================
    CE::Vector2D diferencia = objetivo - m_transform->posicion;
    m_transform->posicion.suma(diferencia.escala(smoothing * dt));

    // =========================
    // 4. RESPETAR LÍMITES (SI LOS HAY)
    // =========================
    if (hasBounds) {
        float halfW = cam_width / 2.f;
        float halfH = cam_height / 2.f;

        m_transform->posicion.x =
            std::clamp(m_transform->posicion.x, boundMin.x + halfW, boundMax.x - halfW);
        m_transform->posicion.y =
            std::clamp(m_transform->posicion.y, boundMin.y + halfH, boundMax.y - halfH);
    }

    // =========================
    // 5. APLICAR CENTRO EN LA VIEW
    // =========================
    m_view->setCenter({ m_transform->posicion.x, m_transform->posicion.y });
}
*/

}
