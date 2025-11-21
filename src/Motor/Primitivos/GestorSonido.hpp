#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace CE {

    class GestorSonido {
    public:
        static GestorSonido& Get();

        // Cargar sonidos
        bool cargarBuffer(const std::string& key, const std::string& filepath);

        // Reproducir sonido
        void play(const std::string& key, float volume = 100.f, bool loop = false);

        // Reproducir música
        void playMusic(const std::string& filepath, float volume = 100.f, bool loop = true);

        // Control música
        void stopMusic();
        void pauseMusic();
        void resumeMusic();

    public:
        const sf::SoundBuffer& getBuffer(const std::string& key) const {
            auto it = buffers.find(key);
            if (it != buffers.end())
                return *(it->second);
            throw std::runtime_error("Buffer no encontrado: " + key);
        }


    private:
        GestorSonido() = default;

        std::map<std::string, std::shared_ptr<sf::SoundBuffer>> buffers;
        std::vector<std::shared_ptr<sf::Sound>> sonidosActivos;

        std::unique_ptr<sf::Music> musicaActual;

        static inline GestorSonido* instancia = nullptr;
    };

} // namespace CE
