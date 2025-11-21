#include "EscenaCirculo.hpp"
#include "../Figuras/Figura.hpp"
#include "../../Motor/Camaras/CamarasGestor.hpp"
#include "../../Motor/Primitivos/GestorEscenas.hpp"
#include "../../Motor/Render/Render.hpp"
#include "../Sistema/Sistema.hpp"
#include "../../Motor/Utils/Lerp.hpp"

namespace IVJ {

void EscenaCirculos::onInit() {
    if (!inicializar) return;

    if (!bg[0].loadTileMap(ASSETS "/atlas/mapa_1_layer_1.txt"))
        exit(EXIT_FAILURE);
    if (!bg[1].loadTileMap(ASSETS "/atlas/mapa_1_layer_2.txt"))
        exit(EXIT_FAILURE);

    // 🔹 Registrar controles
    registrarBotones(sf::Keyboard::Scan::W, "arriba");
    registrarBotones(sf::Keyboard::Scan::Up, "arriba");
    registrarBotones(sf::Keyboard::Scan::S, "abajo");
    registrarBotones(sf::Keyboard::Scan::Down, "abajo");
    registrarBotones(sf::Keyboard::Scan::A, "izquierda");
    registrarBotones(sf::Keyboard::Scan::Left, "izquierda");
    registrarBotones(sf::Keyboard::Scan::D, "derecha");
    registrarBotones(sf::Keyboard::Scan::Right, "derecha");
    registrarBotones(sf::Keyboard::Scan::Escape, "cuadros");

    // 🔹 Crear tres círculos
    auto fig1 = std::make_shared<Rectangulo>(
        100, 100 ,sf::Color(255, 0, 0, 255), sf::Color(0, 0, 0, 255));
    fig1->setPosicion(100, 400);
    fig1->getStats()->hp = 100;

    auto fig2 = std::make_shared<Rectangulo>(
        200, 100, sf::Color(169, 169, 0, 255), sf::Color(0, 0, 0, 255));
    fig2->setPosicion(300, 100);
    fig2->getStats()->hp = 100;

    auto fig3 = std::make_shared<Rectangulo>(
        100, 200 ,sf::Color(0, 0, 255, 255), sf::Color(0, 0, 0, 255));
    fig3->setPosicion(600, 400);
    fig3->getStats()->hp = 100;

    auto fig4 = std::make_shared<Circulo>(
        10,sf::Color(255, 255, 255, 255), sf::Color(0, 0, 0, 255));
    fig4->setPosicion(100, 400);
    fig4->getStats()->hp = 100;

    // 🔹 Añadir los objetos
    objetos.agregarPool(fig1);
    objetos.agregarPool(fig2);
    objetos.agregarPool(fig3);
    objetos.agregarPool(fig4);

    // 🔹 La cámara sigue al segundo círculo
    CE::GestorCamaras::Get().setCamaraActiva(0);
    CE::GestorCamaras::Get().getCamaraActiva().lockEnObjeto(objetos[1]);

    inicializar = false;
}

void EscenaCirculos::onFinal() { }

void EscenaCirculos::onUpdate(float dt) {
    //MoverJugador(objetos.getPool(),dt, 1920, 1080);
    //SistemaMovimientoEntes(objetos.getPool(), dt);
    SistemaMover(objetos.getPool(), dt);
    const float max_frames = 120.f;
    static int tiempo_frame = 0;
    static unsigned char reversa = 0;
    const CE::Vector2D inicio = objetos[0]->getTransformada()->posicion;
    const CE::Vector2D control = objetos[1]->getTransformada()->posicion;
    const CE::Vector2D destino = objetos[2]->getTransformada()->posicion;
    auto npos = CE::lerp2(inicio, control, destino, tiempo_frame/max_frames);
    objetos[3]->setPosicion(npos.x, npos.y);

    if (tiempo_frame > max_frames)
        reversa = 1;
    if (tiempo_frame < 0)
        reversa = 0;
    if (reversa == 1)
        tiempo_frame--;


    for (auto& f : objetos.getPool()) {
        f->onUpdate(dt);

        objetos.borrarPool();
    }
}

void EscenaCirculos::onInputs(const CE::Botones& accion) {
    auto p = objetos[1]->getTransformada();

    if (accion.getTipo() == CE::Botones::TipoAccion::OnPress) {
        if (accion.getNombre() == "arriba")
            p->velocidad.y = -800;
        else if (accion.getNombre() == "derecha")
            p->velocidad.x = 800;
        else if (accion.getNombre() == "abajo")
            p->velocidad.y = 800;
        else if (accion.getNombre() == "izquierda")
            p->velocidad.x = -800;
        else if (accion.getNombre() == "cuadros")
            CE::GestorEscenas::Get().cambiarEscena("Cuadros");
    }
    else if (accion.getTipo() == CE::Botones::TipoAccion::OnRelease) {
        if (accion.getNombre() == "arriba" || accion.getNombre() == "abajo")
            p->velocidad.y = 0;
        else if (accion.getNombre() == "derecha" || accion.getNombre() == "izquierda")
            p->velocidad.x = 0;
    }
}

void EscenaCirculos::onRender() {
    for (auto& b: bg)
        CE::Render::Get().AddToDraw(b);

    for (auto& f : objetos.getPool())
        CE::Render::Get().AddToDraw(*f);
}

}
