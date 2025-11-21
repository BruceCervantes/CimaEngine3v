#include "Sistema.hpp"
#include <algorithm>
#include "../../Motor/Componentes/IComponentes.hpp"
#include "Juego/Figuras/Figura.hpp"
#include <cmath>
#include <memory>
#include "Motor/Primitivos/GestorSonido.hpp"
#include "Juego/objetos/Entidad.hpp"
#include <SFML/Audio.hpp>

namespace IVJ
{

    void SistemaMover(const std::vector<std::shared_ptr<CE::Objeto>>& entes, float dt)
    {

        for(auto& ente : entes)
        {
            //todo ente tiene ITransform por lo que no requiere ferificación
            auto trans = ente->getTransformada();
            trans->posicion.suma(trans->velocidad.escala(dt));

        }

    }

    bool SistemaColAABB(CE::Objeto& A, CE::Objeto& B, bool resolucion) {

        if (!A.tieneComponente<CE::IBoundingBox>() || !B.tieneComponente<CE::IBoundingBox>())
            return false;
        auto bA = A.getComponente<CE::IBoundingBox>()->tam;
        auto mA = A.getComponente<CE::IBoundingBox>()->mitad;
        auto *pa = &A.getTransformada()->posicion;
        auto prevA = A.getTransformada()->pos_previa;

        auto bB = B.getComponente<CE::IBoundingBox>()->tam;
        auto mB = B.getComponente<CE::IBoundingBox>()->mitad;
        auto pb = B.getTransformada()->posicion;

        bool H = pa->y-mA.y < pb.y+bB.y - mB.y && pb.y-mB.y < pa->y+bA.y - mA.y;
        bool V = pa->x-mA.x < pb.x+bB.x - mB.x && pb.x-mB.x < pa->x+bA.x - mA.x;
        bool hay_colision = H && V;
        if (hay_colision && resolucion)
            *pa = prevA;
        return hay_colision;
    }

    void MoverJugador(const std::vector<std::shared_ptr<CE::Objeto>>& entes, float dt, float altoVentana, float anchoVentana)
   {
       for (auto& e : entes)
       {
           if (auto jugador = std::dynamic_pointer_cast<IVJ::Rectangulo>(e))
           {
               auto trans = jugador->getTransformada();

               // Movimiento controlado en ambos ejes
               trans->posicion.x += trans->velocidad.x * dt;
               trans->posicion.y += trans->velocidad.y * dt;

               // Limitar el movimiento dentro de la ventana
               if (trans->posicion.x < 0.f)
                   trans->posicion.x = 0.f;
               if (trans->posicion.x > anchoVentana)
                   trans->posicion.x = anchoVentana;

               if (trans->posicion.y < 0.f)
                   trans->posicion.y = 0.f;
               if (trans->posicion.y > altoVentana)
                   trans->posicion.y = altoVentana;
           }
       }

   }


       void SistemaMovimientoEnemigo(const std::vector<std::shared_ptr<CE::Objeto>> &entes, float dt) {
           for (auto& e : entes)
           {
               auto trans = e->getTransformada();
               if (!trans) continue;

               // Inicializa velocidad si es cero
               if (trans->velocidad.magnitud() == 0)
                   trans->velocidad = CE::Vector2D{300.f, 300.f};

               // Actualiza posición
               trans->posicion.x += trans->velocidad.x * dt;
               trans->posicion.y += trans->velocidad.y * dt;

               // Rebote en ventana
               if (trans->posicion.x < 0 || trans->posicion.x > 1920)
                   trans->velocidad.x *= -1;
               if (trans->posicion.y < 0 || trans->posicion.y >1080 )
                   trans->velocidad.y *= -1;
           }
       }



    void SistemaMovimientoAuto(const std::vector<std::shared_ptr<CE::Objeto>>& entes, float dt) {
        for (auto& e : entes) {
            auto jugador = std::dynamic_pointer_cast<IVJ::Entidad>(e);


            if (!jugador) continue;

            const float maxSteerLock = 180.f;
            const float steerSpeed = 360.f;
            const float steerReturn = 540.f;

            auto trans = jugador->getTransformada();
            auto ctrl  = jugador->getComponente<CE::IControl>();

            if (ctrl->izq) {
                trans->steerAngle -= steerSpeed * dt;
            } else if (ctrl->der) {
                trans->steerAngle += steerSpeed * dt;
            } else {
                if (trans->steerAngle > 0.f) {
                    trans->steerAngle -= steerReturn * dt;
                    if (trans->steerAngle < 0.f) trans->steerAngle = 0.f;
                } else if (trans->steerAngle < 0.f) {
                    trans->steerAngle += steerReturn * dt;
                    if (trans->steerAngle > 0.f) trans->steerAngle = 0.f;
                }
            }

            if (trans->steerAngle > maxSteerLock) trans->steerAngle = maxSteerLock;
            if (trans->steerAngle < -maxSteerLock) trans->steerAngle = -maxSteerLock;

            float speedNorm = std::clamp(std::abs(trans->velocidadForward) / trans->velocidadMax, 0.f, 1.f);

            float baseTurnRate = 120.f;
            float turnRate = baseTurnRate * (1.0f - 0.7f * speedNorm);

            float steerNorm = trans->steerAngle / maxSteerLock;
            trans->angulo += turnRate * steerNorm * dt;

            if (trans->angulo >= 360.f) trans->angulo -= 360.f;
            if (trans->angulo < 0.f) trans->angulo += 360.f;

            if (ctrl->arr) {
                if (trans->velocidadForward < 0) {
                    trans->velocidadForward += trans->frenado * dt;
                    if (trans->velocidadForward > 0) trans->velocidadForward = 0;
                } else {
                    trans->velocidadForward += trans->aceleracion * dt;
                    if (trans->velocidadForward > trans->velocidadMax)
                        trans->velocidadForward = trans->velocidadMax;
                }
            }


            else if (ctrl->abj) {

                float speedNorm = std::clamp(std::abs(trans->velocidadForward) / trans->velocidadMax, 0.f, 1.f);
                float brakeForce = trans->frenado * (0.4 + 0.6 * speedNorm);

                if (trans->velocidadForward > 0) {
                    trans->velocidadForward -= brakeForce * dt;
                    if (trans->velocidadForward < 0) trans->velocidadForward = 0;
                } else {
                    trans->velocidadForward -= trans->aceleracion * dt;
                    if (trans->velocidadForward < trans->velocidadReversaMax)
                        trans->velocidadForward = trans->velocidadReversaMax;
                }
            }

            else {
                if (trans->velocidadForward > 0) {
                    trans->velocidadForward -= trans->desaceleracion * dt;
                    if (trans->velocidadForward < 0) trans->velocidadForward = 0;
                } else if (trans->velocidadForward < 0) {
                    trans->velocidadForward += trans->desaceleracion * dt;
                    if (trans->velocidadForward > 0) trans->velocidadForward = 0;
                }
            }

            float angRad = trans->angulo * (3.14159265f / 180.f);
            float dx = std::sin(angRad);
            float dy = -std::cos(angRad);

            trans ->posicion.x += dx * trans->velocidadForward * dt;
            trans->posicion.y += dy * trans->velocidadForward * dt;


        }
    }

    void SistemaSonidoAuto(const std::vector<std::shared_ptr<CE::Objeto>>& entes, float dt)
{
    auto& sndMan = CE::GestorSonido::Get();

    for (auto& e : entes)
    {
        auto jugador = std::dynamic_pointer_cast<IVJ::Entidad>(e);
        if (!jugador) continue;

        auto trans = jugador->getTransformada();
        auto snd   = jugador->getComponente<CE::IComponenteSonido>();
        if (!snd) continue;

        snd->timerChoque -= dt;
        if (snd->timerChoque < 0) snd->timerChoque = 0;

        float velNorm   = std::clamp(std::abs(trans->velocidadForward) / trans->velocidadMax, 0.f, 1.f);
        float steerNorm = std::abs(trans->steerAngle) / 180.f;
        bool  enReversa = (trans->velocidadForward < -1.f);

        //-------------------------------------------------------------
        // 1) Inicializar TODOS los loops de motor (una sola vez)
        //-------------------------------------------------------------
        auto initLoop = [&](std::shared_ptr<sf::Sound>& ptr, const std::string& key, float volInicial = 0.f)
        {
            if (!ptr)
            {
                ptr = std::make_shared<sf::Sound>(sndMan.getBuffer(key));
                ptr->setLooping(true);
                ptr->setVolume(volInicial);
                ptr->play();
            }
        };

        initLoop(snd->idleLoop,      "engine_idle");
        initLoop(snd->lowLoop,       "engine_low_rpm");
        initLoop(snd->midLoop,       "engine_mid_rpm");
        initLoop(snd->highLoop,      "engine_high_rpm");
        initLoop(snd->veryHighLoop,  "engine_very_high");
        initLoop(snd->brakeLoop,     "engine_brake_loop", 0.f);
        initLoop(snd->reversaLoop,   "reversa_loop",      0.f);

        //-------------------------------------------------------------
        // 2) REVERSA: si va para atrás, se prioriza reversa
        //-------------------------------------------------------------
        const float mixSpeed = 6.f;

        if (enReversa)
        {
            // Objetivos
            float targetRev  = 40.f + velNorm * 60.f; // 40–100
            float targetFwd  = 5.f;                   // bajamos mucho motor hacia adelante

            snd->volReversa += (targetRev - snd->volReversa) * dt * mixSpeed;

            snd->volIdle      += (0.f - snd->volIdle)     * dt * mixSpeed;
            snd->volLow       += (0.f - snd->volLow)      * dt * mixSpeed;
            snd->volMid       += (0.f - snd->volMid)      * dt * mixSpeed;
            snd->volHigh      += (0.f - snd->volHigh)     * dt * mixSpeed;
            snd->volVeryHigh  += (0.f - snd->volVeryHigh) * dt * mixSpeed;
            snd->volBrake     += (0.f - snd->volBrake)    * dt * mixSpeed;

            // Aplicar volúmenes
            snd->reversaLoop->setVolume(snd->volReversa);

            snd->idleLoop->setVolume(snd->volIdle);
            snd->lowLoop->setVolume(snd->volLow);
            snd->midLoop->setVolume(snd->volMid);
            snd->highLoop->setVolume(snd->volHigh);
            snd->veryHighLoop->setVolume(snd->volVeryHigh);
            snd->brakeLoop->setVolume(snd->volBrake);

            // No seguimos con la lógica de motor hacia adelante
            trans->velocidadPrevia = trans->velocidadForward;
            continue;
        }

        // Si no está en reversa, apagamos reversa poco a poco
        snd->volReversa += (0.f - snd->volReversa) * dt * mixSpeed;
        snd->reversaLoop->setVolume(snd->volReversa);

        //-------------------------------------------------------------
        // 3) Motor hacia adelante: elegir capa según velNorm
        //-------------------------------------------------------------
        int capaObjetivo = 0; // 0 idle, 1 low, 2 mid, 3 high, 4 veryHigh

        static int ultimaCapa = 0;

        if      (velNorm < 0.12f) capaObjetivo = 0;
        else if (velNorm < 0.30f) capaObjetivo = 1;
        else if (velNorm < 0.60f) capaObjetivo = 2;
        else if (velNorm < 0.88f) capaObjetivo = 3;
        else if (velNorm > 0.92f) capaObjetivo = 4;
        else                      capaObjetivo = ultimaCapa; // evita parpadeos

        ultimaCapa = capaObjetivo;


        // Volumen base del motor
        float targetMasterVol = 30.f + velNorm * 70.f; // 30–100
        snd->volumenMaster += (targetMasterVol - snd->volumenMaster) * dt * 3.f;

        auto setTargetForCapa = [&](int capa, float& vol, float factor)
        {
            float target = (capa == capaObjetivo) ? snd->volumenMaster * factor : 0.f;
            vol += (target - vol) * dt * mixSpeed;
        };

        setTargetForCapa(0, snd->volIdle,     0.9f);
        setTargetForCapa(1, snd->volLow,      1.0f);
        setTargetForCapa(2, snd->volMid,      1.0f);
        if (capaObjetivo == 4) {
            // mezcla suave en máxima velocidad
            float base = snd->volumenMaster;

            snd->volHigh      += ((base * 0.35f) - snd->volHigh)       * dt * mixSpeed;
            snd->volVeryHigh  += ((base * 0.50f) - snd->volVeryHigh)   * dt * mixSpeed;
            snd->volMid       += ((base * 0.15f) - snd->volMid)        * dt * mixSpeed;
        }
        else {
            // capas normales
            setTargetForCapa(3, snd->volHigh,     1.0f);
            setTargetForCapa(4, snd->volVeryHigh, 0.0f);
        }


        // Pitch global del motor según velocidad
        float targetPitch = 0.85f + velNorm * 0.75f; // 0.9–2.3
        snd->pitchActual += (targetPitch - snd->pitchActual) * dt * 3.f;

        snd->idleLoop->setVolume(snd->volIdle);
        snd->lowLoop->setVolume(snd->volLow);
        snd->midLoop->setVolume(snd->volMid);
        snd->highLoop->setVolume(snd->volHigh);
        snd->veryHighLoop->setVolume(snd->volVeryHigh);

        snd->idleLoop->setPitch(snd->pitchActual);
        snd->lowLoop->setPitch(snd->pitchActual);
        snd->midLoop->setPitch(snd->pitchActual);
        snd->highLoop->setPitch(snd->pitchActual);
        snd->veryHighLoop->setPitch(snd->pitchActual);

        //-------------------------------------------------------------
        // 4) Loop de frenado de motor (retención)
        //-------------------------------------------------------------
        float desac = std::abs(trans->velocidadForward - trans->velocidadPrevia) / dt;

        bool estaFrenandoMotor =
            (trans->velocidadForward > 0.5f &&        // solo si iba hacia adelante
             trans->velocidadForward < trans->velocidadPrevia && // está bajando
             desac > 10.f);                           // umbral de retención

        float targetBrakeVol = (estaFrenandoMotor ? (20.f + velNorm * 60.f) : 0.f); // 20–80
        snd->volBrake += (targetBrakeVol - snd->volBrake) * dt * mixSpeed;
        snd->brakeLoop->setVolume(snd->volBrake);
        snd->brakeLoop->setPitch(0.9f + velNorm * 0.4f);

        //-------------------------------------------------------------
        // 5) SKID (derrape) - lo mantenemos como ya lo tenías
        //-------------------------------------------------------------
        if (steerNorm > snd->skidThreshold && velNorm > 0.35f)
        {
            if (!snd->skidPlaying)
            {
                snd->skid = std::make_shared<sf::Sound>(sndMan.getBuffer("skid"));
                snd->skid->setLooping(true);
                snd->skid->setVolume(40);
                snd->skid->play();
                snd->skidPlaying = true;
            }

            snd->skid->setPitch(1.0f + steerNorm);
        }
        else
        {
            if (snd->skidPlaying)
            {
                snd->skid->stop();
                snd->skidPlaying = false;
            }
        }

        //-------------------------------------------------------------
        // 6) DOWNSHIFT / Freno e impacto brusco
        //-------------------------------------------------------------
        // downshift: caída fuerte pero no tan bestia como impacto
        if (desac > snd->choqueThreshold && trans->velocidadForward > 20.f)
        {
            // reproducir un pequeño "blip" de downshift
            auto s = std::make_shared<sf::Sound>(sndMan.getBuffer("engine_downshift"));
            s->setVolume(70.f);
            s->play();
        }

        // freno fuerte como ya lo tenías
        if (desac > snd->choqueThreshold * 1.3f)
        {
            auto fr = std::make_shared<sf::Sound>(sndMan.getBuffer("freno"));
            fr->setVolume(70);
            fr->play();
        }

        // impacto muy fuerte
        if (desac > snd->choqueThreshold * 2.5f && snd->timerChoque == 0)
        {
            sndMan.play("impacto", 100, false);
            snd->timerChoque = 0.6f;
        }

        trans->velocidadPrevia = trans->velocidadForward;
    }
}



    /*
    void SistemaSonidoAuto(const std::vector<std::shared_ptr<CE::Objeto>>& entes, float dt) {

    for (auto& e : entes) {
        auto jugador = std::dynamic_pointer_cast<IVJ::Entidad>(e);
        if (!jugador) continue;

        auto trans = jugador->getTransformada();
        auto snd = jugador->getComponente<CE::IComponenteSonido>();
        if (!snd) continue;

        snd->timerChoque -= dt;
        if (snd->timerChoque < 0) snd->timerChoque = 0;

        //-------------------------------------------------------
        // 🎵 1. Motor
        //-------------------------------------------------------
        if (!snd->motor) {
            CE::GestorSonido::Get().cargarBuffer("motor", ASSETS "/sonido/Idle.wav");
            snd->motor = std::make_shared<sf::Sound>(CE::GestorSonido::Get().getBuffer("motor"));
            snd->motor->setLooping(true);
            snd->motor->setVolume(100);
            snd->motor->play();
        }

        float velNorm = std::clamp(std::abs(trans->velocidadForward) / trans->velocidadMax, 0.f, 1.f);

        float rpm   = 0.6f + 1.4f * velNorm;
        float pitch = rpm;

        snd->motor->setPitch(pitch);
        snd->motor->setVolume(30 + velNorm * 70);

        //-------------------------------------------------------
        // 🎵 2. Skid / Derrape
        //-------------------------------------------------------

        float steerNorm = std::abs(trans->steerAngle) / 180.f;

        if (steerNorm > snd->skidThreshold && velNorm > 0.35f) {
            // Activar skid si no está ya activo
            if (!snd->skidPlaying) {
                if (!snd->skid) {
                    CE::GestorSonido::Get().cargarBuffer("skid", ASSETS "/sonido/RPM.wav");
                    snd->skid = std::make_shared<sf::Sound>(CE::GestorSonido::Get().getBuffer("skid"));
                    snd->skid->setLooping(true);
                }
                snd->skid->setVolume(40);
                snd->skid->play();
                snd->skidPlaying = true;
            }

            snd->skid->setPitch(1.0f + steerNorm);
        }
        else {
            // Detener skid si ya no aplica
            if (snd->skidPlaying) {
                snd->skid->stop();
                snd->skidPlaying = false;
            }
        }

        //-------------------------------------------------------
        // 💥 3. Golpe brusco
        //-------------------------------------------------------

        float desac = std::abs(trans->velocidadForward - trans->velocidadPrevia) / dt;

        if (desac > snd->choqueThreshold && snd->timerChoque == 0) {
            CE::GestorSonido::Get().play("impact", 100, false);
            snd->timerChoque = 0.6f; // cooldown
        }

        trans->velocidadPrevia = trans->velocidadForward;
    }
}
    */

}
