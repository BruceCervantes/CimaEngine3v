#include "GestorEscenas.hpp"

namespace CE
{
    GestorEscenas& GestorEscenas::Get()
    {
        if (!instancia)
            instancia = new GestorEscenas();
        return *instancia;
    }

    void GestorEscenas::registrarEscena(const std::string& key, const std::shared_ptr<Escena>& escena)
    {
        if (escenas.empty())
            escenaActual= key;

        escenas[key] = escena;
        escena->nombre = key;
    }


    void GestorEscenas::cambiarEscena(const std::string& key)
    {
        // Si no existe esa escena o no hay escenas registradas, no hacer nada
        if (escenas.find(key) == escenas.end() || escenas.empty())
            return;

        // Salir de la escena anterior
        escenas[escenaActual]->onFinal();

        // Cambiar y entrar a la nueva
        escenaActual = key;
        escenas[escenaActual]->onInit();
    }

    Escena& GestorEscenas::getEscenaActual()
    {
        return *escenas[escenaActual];
    }

}
