#include "GestorSonido.hpp"
#include <iostream>
#include <algorithm>

namespace CE {

    GestorSonido& GestorSonido::Get() {
        if (!instancia) instancia = new GestorSonido();
        return *instancia;
    }

    bool GestorSonido::cargarBuffer(const std::string& key, const std::string& filepath) {
        auto buffer = std::make_shared<sf::SoundBuffer>();

        try {
            *buffer = sf::SoundBuffer(filepath);
        }
        catch (...) {
            std::cerr << "ERROR: No se pudo cargar el sonido: " << filepath << "\n";
            return false;
        }

        buffers[key] = buffer;
        return true;
    }

    void GestorSonido::play(const std::string& key, float volume, bool loop) {
        auto it = buffers.find(key);
        if (it == buffers.end()) {
            std::cerr << "ERROR: El buffer '" << key << "' no existe\n";
            return;
        }

        // Crear sonido con el buffer (no existe constructor vacío)
        auto snd = std::make_shared<sf::Sound>(*it->second);

        snd->setVolume(volume);
        snd->setLooping(loop);
        snd->play();

        sonidosActivos.push_back(snd);

        // Limpiar sonidos que ya terminaron
        sonidosActivos.erase(
            std::remove_if(
                sonidosActivos.begin(),
                sonidosActivos.end(),
                [](const std::shared_ptr<sf::Sound>& s) {
                    return s->getStatus() == sf::Sound::Status::Stopped;
                }
            ),
            sonidosActivos.end()
        );
    }

    void GestorSonido::playMusic(const std::string& filepath, float volume, bool loop) {
        musicaActual = std::make_unique<sf::Music>(filepath);

        musicaActual->setLooping(loop);
        musicaActual->setVolume(volume);
        musicaActual->play();
    }

    void GestorSonido::stopMusic() {
        if (musicaActual) musicaActual->stop();
    }

    void GestorSonido::pauseMusic() {
        if (musicaActual) musicaActual->pause();
    }

    void GestorSonido::resumeMusic() {
        if (musicaActual) musicaActual->play();
    }

} // namespace CE
