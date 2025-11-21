#include "EscenaCuadro.hpp"
#include "../Figuras/Figura.hpp"
#include "../../Motor/Camaras/CamarasGestor.hpp"
#include "../../Motor/Primitivos/GestorEscenas.hpp"
#include "../../Motor/Render/Render.hpp"
#include "../../Motor/Primitivos/GestorAssets.hpp"
#include "../Sistema/Sistema.hpp"
#include "../../Motor/Primitivos/Escena.hpp"
#include "../objetos/Entidad.hpp"
#include "../objetos/TileMap.hpp"
#include <memory>
#include <random>
#include <iostream>
#include "../Maquinas/JugadorFSM/IdleFSM.hpp"
#include "Motor/Primitivos/GestorSonido.hpp"

namespace IVJ {
    void EscenaCuadros::onInit() {
        if (!inicializar) return;

        auto& sndMan = CE::GestorSonido::Get();

        // Loops de motor hacia adelante
        sndMan.cargarBuffer("engine_idle",        ASSETS "/sonido/motor/Motor.mp3");
        sndMan.cargarBuffer("engine_low_rpm",     ASSETS "/sonido/motor/LowMotor.mp3");
        sndMan.cargarBuffer("engine_mid_rpm",     ASSETS "/sonido/motor/MidMotor.mp3");
        sndMan.cargarBuffer("engine_high_rpm",    ASSETS "/sonido/motor/HighMotor.mp3");
        sndMan.cargarBuffer("engine_very_high",   ASSETS "/sonido/motor/VeryMotor.mp3");

        // Loop de frenado de motor
        sndMan.cargarBuffer("engine_brake_loop",  ASSETS "/sonido/motor/DownMotor.mp3");

        // Downshift (no loop)
        sndMan.cargarBuffer("engine_downshift",   ASSETS "/sonido/motor/DownShiftMotor.mp3");

        // Ya existentes
        sndMan.cargarBuffer("reversa_loop", ASSETS "/sonido/Reversa.wav");
        sndMan.cargarBuffer("skid",         ASSETS "/sonido/Skid.wav");
        sndMan.cargarBuffer("freno",        ASSETS "/sonido/Freno.wav");
        sndMan.cargarBuffer("impacto",      ASSETS "/sonido/Impact.wav");


        CE::GestorAssets::Get().agregarTextura("hola_pink", ASSETS "/sprites_aliens/alienPink.png", CE::Vector2D{0,0}, CE::Vector2D{256, 512});
        CE::GestorAssets::Get().agregarTextura("hamilton", ASSETS "/sprites_aliens/F1(Hamilton)-Sheet.png",CE::Vector2D{0,0},CE::Vector2D{320,96});
        CE::GestorAssets::Get().agregarTextura("verstappen", ASSETS "/sprites_aliens/F1(Verstappen)-Sheet.png",CE::Vector2D{0,0},CE::Vector2D{64,96});
        CE::GestorAssets::Get().agregarTextura("jugador", ASSETS "/f1/Jugador.png",CE::Vector2D{0,0},CE::Vector2D{320,192});
        CE::GestorAssets::Get().agregarTextura("senna", ASSETS "/sprites_aliens/F1(Senna)-Sheet.png",CE::Vector2D{0,0},CE::Vector2D{64,96});
        CE::GestorAssets::Get().agregarTextura("mapa", ASSETS "/atlas/Mapa(AustriaCapas).png",CE::Vector2D{0,0},CE::Vector2D{5000,3000});

        //if (!bg[0].loadTileMap(ASSETS "/atlas/mapa_1_layer_1.txt"))
        //    exit(EXIT_FAILURE);
        //if (!bg[1].loadTileMap(ASSETS "/atlas/mapa_1_layer_2.txt"))
        //    exit(EXIT_FAILURE);

        // 🔹 Registro de controles
        registrarBotones(sf::Keyboard::Scan::W, "arriba");
        registrarBotones(sf::Keyboard::Scan::Up, "arriba");
        registrarBotones(sf::Keyboard::Scan::S, "abajo");
        registrarBotones(sf::Keyboard::Scan::Down, "abajo");
        registrarBotones(sf::Keyboard::Scan::A, "izquierda");
        registrarBotones(sf::Keyboard::Scan::Left, "izquierda");
        registrarBotones(sf::Keyboard::Scan::D, "derecha");
        registrarBotones(sf::Keyboard::Scan::Right, "derecha");
        registrarBotones(sf::Keyboard::Scan::Escape, "circulos");

        jugador = std::make_shared<Entidad>();
        jugador->getStats()->hp = 100;
        jugador->setPosicion(3444.f, 1908.f);
        auto trans = jugador->getTransformada();
        trans->angulo = 225.f;
        jugador->addComponente(std::make_shared<CE::ISprite>(CE::GestorAssets::Get().getTextura("jugador"), 64, 96, .5f));
        jugador->addComponente(std::make_shared<IVJ::IMaquinaEstado>());
        jugador->addComponente(std::make_shared<CE::IControl>());
        jugador->addComponente(std::make_shared<CE::IComponenteSonido>());


        auto compSonido = std::make_shared<CE::IComponenteSonido>();
        jugador->addComponente(compSonido);


        auto &fsm_init = jugador->getComponente<IMaquinaEstado>()->fsm;
        fsm_init = std::make_shared<IdleFSM>();
        fsm_init->onEntrar(*jugador);


        mapa = std::make_shared<Mapa>();
        mapa->getStats()->hp = 100;
        mapa->setPosicion(2500.f, 1500.f);
        mapa->addComponente(std::make_shared<CE::ISprite>(CE::GestorAssets::Get().getTextura("mapa"),1.f));


        // ==================== CREACIÓN DE 100 ENEMIGOS ALEATORIOS ====================
        srand(static_cast<unsigned>(time(nullptr)));

        // Lista de sprites posibles
        std::vector<std::string> sprites = {"hamilton", "verstappen", "senna"};

        objetos.agregarPool(mapa);
        objetos.agregarPool(jugador);

        // 🔹 Cámara

        CE::GestorCamaras::Get().agregarCamara(
                    std::make_shared<CE::CamaraCuadro>(
                        CE::Vector2D{2500, 1500}, CE::Vector2D{5000, 3000}));
        CE::GestorCamaras::Get().setCamaraActiva(1);

        // La cámara sigue al objeto 2
        CE::GestorCamaras::Get().getCamaraActiva().lockEnObjeto(objetos[1]);

        inicializar = false;
    }

    void EscenaCuadros::onFinal() { }

    void EscenaCuadros::onUpdate(float dt) {
        MoverJugador(objetos.getPool(),dt, 3000, 5000);
        SistemaMovimientoEnemigo(enemigos.getPool(),dt);
        SistemaMovimientoAuto(objetos.getPool(),dt);
        SistemaSonidoAuto(objetos.getPool(), dt);
        //SistemaMovimientoEntes(objetos.getPool(), dt);
        jugador->inputFSM();


        for (auto& f : objetos.getPool())
            f->onUpdate(dt);

        for (auto& e : enemigos.getPool())
            e->onUpdate(dt);

        objetos.borrarPool();
        enemigos.borrarPool();
    }

    void EscenaCuadros::onInputs(const CE::Botones& accion) {
        auto ctrl = jugador->getComponente<CE::IControl>();

        if (accion.getTipo() == CE::Botones::TipoAccion::OnPress) {
            if (accion.getNombre()=="arriba")    ctrl->arr = true;
            if (accion.getNombre()=="abajo")     ctrl->abj = true;
            if (accion.getNombre()=="derecha")   ctrl->der = true;
            if (accion.getNombre()=="izquierda") ctrl->izq = true;
        }
        else if (accion.getTipo() == CE::Botones::TipoAccion::OnRelease) {
            if (accion.getNombre()=="arriba")    ctrl->arr = false;
            if (accion.getNombre()=="abajo")     ctrl->abj = false;
            if (accion.getNombre()=="derecha")   ctrl->der = false;
            if (accion.getNombre()=="izquierda") ctrl->izq = false;
        }
    }
void EscenaCuadros::onRender() {

    for (auto& b: bg)
        CE::Render::Get().AddToDraw(b);

    for (auto& f : objetos.getPool())
        CE::Render::Get().AddToDraw(*f);

    for (auto& e : enemigos.getPool())
        CE::Render::Get().AddToDraw(*e);

}

}
