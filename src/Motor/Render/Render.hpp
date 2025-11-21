#pragma once
#include <SFML/Graphics.hpp>
#include <Motor/Motor.hpp>
#include <Motor/Utils/Utils.hpp>
#include <memory>

namespace CE
{
    class Render
    {
    public:
        static Render& Get(void);
        static void Terminar(void);

        void CrearVentana(const MotorConfig& motorinfo);
        void AutoResizeTextura(unsigned int w, unsigned int h);
        void AddToDraw(const sf::Drawable& obj);
        void OnClearColor(const sf::Color& clear);
        void OnDisplayTextura(void);
        void OnDisplayVentana(void);

        sf::RenderWindow& GetVentana(void);
        sf::RenderTexture& GetTextura(void);

    private:
        explicit Render() = default;

        std::unique_ptr<sf::RenderWindow> rventana;
        std::unique_ptr<sf::RenderTexture> rtextura;

        static inline Render* instancia = nullptr;
    };
}
