#pragma once

#include <vector>
#include <memory>
#include "Juego/Figuras/Figura.hpp"

namespace IVJ {

    [[maybe_unused]]void SistemaMover(const std::vector<std::shared_ptr<CE::Objeto>>& entes, float dt);
    void SistemaMovimientoEnemigo(const std::vector<std::shared_ptr<CE::Objeto>> &entes, float dt);
    void MoverJugador(const std::vector<std::shared_ptr<CE::Objeto>>& e, float dt, float altoVentana, float anchoVentana);
    void SistemaMovimientoAuto(const std::vector<std::shared_ptr<CE::Objeto>>& entes, float dt);
    void SistemaSonidoAuto(const std::vector<std::shared_ptr<CE::Objeto>>& entes, float dt);

    [[maybe_unused]] bool SistemaColAABB(CE::Objeto& A, CE::Objeto& B, bool resolucion = false);
    void SistemaCollAABBMid(CE::Objeto& A, CE::Objeto& B, bool resolucion = false);
} // namespace IVJ
