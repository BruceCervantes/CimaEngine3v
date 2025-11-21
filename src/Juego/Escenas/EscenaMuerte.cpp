#include "EscenaMuerte.hpp"
#include "../Figuras/Figura.hpp"
#include "../../Motor/Camaras/CamarasGestor.hpp"
#include "../../Motor/Primitivos/GestorEscenas.hpp"
#include "../../Motor/Render/Render.hpp"
#include "Juego/Sistema/Sistema.hpp"

#include "../Sistema/Sistema.hpp"
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

namespace IVJ {

void EscenaMuerte::onInit() {
    if (!inicializar) return;
    srand(time(0)); // Seed the random number generator
    // 🔹 Registro de controles
    registrarBotones(sf::Keyboard::Scan::W, "arriba");
    registrarBotones(sf::Keyboard::Scan::Up, "arriba");
    registrarBotones(sf::Keyboard::Scan::S, "abajo");
    registrarBotones(sf::Keyboard::Scan::Down, "abajo");
    registrarBotones(sf::Keyboard::Scan::A, "izquierda");
    registrarBotones(sf::Keyboard::Scan::Left, "izquierda");
    registrarBotones(sf::Keyboard::Scan::D, "derecha");
    registrarBotones(sf::Keyboard::Scan::Right, "derecha");
    registrarBotones(sf::Keyboard::Scan::Tab, "circulos");



    for(int i=0; i<50; i++) {
        int xr = 50+rand() % 1700;
        int yr = 50+rand() % 600;
        auto fig1 = std::make_shared<Rectangulo>(
        100, 100 ,sf::Color::Green, sf::Color::Black);
        fig1->setPosicion(xr, yr);
        fig1->getStats()->hp = 100;
        objetos.agregarPool(fig1);
    }


    auto camTrack = std::make_shared<CE::CamaraTracking>(
            CE::Vector2D{960, 540},
            CE::Vector2D{800, 500}
            );

    CE::GestorCamaras::Get().agregarCamara(camTrack);
    CE::GestorCamaras::Get().setCamaraActiva( (int)CE::GestorCamaras::Get().getListaCamaras().size() - 1);
    CE::GestorCamaras::Get().getCamaraActiva().lockEnObjeto(objetos[1]);

    camTrack->setFollowOffset({0.f, -170.f});
    camTrack->setSmoothing(5.0f);


    inicializar = false;
}

void EscenaMuerte::onFinal() { }

void EscenaMuerte::onUpdate(float dt) {
    MoverJugador(objetos.getPool(), dt, 1920, 1080);
    //SistemaMovimientoEntes(objetos.getPool(), dt);

    for (auto& f : objetos.getPool()) {
        if(f->estaVivo()){
        f->onUpdate(dt);
        f->getStats()->hp -=rand() % 20;
    }
    }
    objetos.borrarPool();


}

void EscenaMuerte::onInputs(const CE::Botones& accion) {
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
        else if (accion.getNombre() == "circulos")
            CE::GestorEscenas::Get().cambiarEscena("Circulos");
    }
    else if (accion.getTipo() == CE::Botones::TipoAccion::OnRelease) {
        if (accion.getNombre() == "arriba" || accion.getNombre() == "abajo")
            p->velocidad.y = 0;
        else if (accion.getNombre() == "derecha" || accion.getNombre() == "izquierda")
            p->velocidad.x = 0;
    }
}

void EscenaMuerte::onRender() {
    for (auto& f : objetos.getPool())
        CE::Render::Get().AddToDraw(*f);
}

}
