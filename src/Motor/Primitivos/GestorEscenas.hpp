#pragma once

#include "Escena.hpp"
#include <map>
#include <memory>
#include <vector>
#include <string>

namespace CE
{
    class GestorEscenas
    {
    public:
        using HEscenas = std::map<std::string, std::shared_ptr<Escena>>;

        static GestorEscenas& Get();

        void registrarEscena(const std::string& key, const std::shared_ptr<Escena>& escena);
        void cambiarEscena(const std::string& key);
        Escena& getEscenaActual();

        HEscenas& getTodasEscenas()
        {
            return escenas;
        }

        std::vector<std::string> getKeys() const
        {
            std::vector<std::string> keys;
            for (auto it = escenas.begin(); it != escenas.end(); ++it)
                keys.push_back(it->first);
            return keys;
        }

    private:
        std::string escenaActual;
        HEscenas escenas;

        static inline GestorEscenas* instancia = nullptr;
    };
}
