#include "Render.hpp"
#include <iostream>

namespace CE
{
    Render& Render::Get()
    {
        if (!instancia)
            instancia = new Render();
        return *instancia;
    }

    void Render::Terminar()
    {
        delete instancia;
        instancia = nullptr;
    }

    void Render::CrearVentana(const MotorConfig& motorinfo)
    {
        sf::ContextSettings videosettings;
        videosettings.antiAliasingLevel = 3;

        rventana = std::make_unique<sf::RenderWindow>(
        sf::VideoMode({motorinfo.vW, motorinfo.vH} ,32),
     sf::String(motorinfo.titulo),
         sf::State::Windowed,
        videosettings
);


        rventana->setFramerateLimit(65);

        unsigned int tw, th;
    #if DEBUG
        tw = motorinfo.vW - motorinfo.vW * 0.25f;
        th = motorinfo.vH - motorinfo.vH * 0.25f;
    #else
        tw = motorinfo.vW;
        th = motorinfo.vH;
    #endif

        rtextura = std::make_unique<sf::RenderTexture>(
            sf::Vector2u{tw, th},
            videosettings
        );
    }

    void Render::AutoResizeTextura(unsigned int w, unsigned int h)
    {
        auto cs = rtextura->getSize();
        unsigned cw = cs.x;
        unsigned ch = cs.y;

        if ((cw == w && ch == h) || (w == 0 || h == 0))
            return;

        sf::ContextSettings video;
        video.antiAliasingLevel = 3;

        rtextura->clear(sf::Color::Black);

        if (!rtextura->resize(sf::Vector2u{w, h}, video))
            exit(EXIT_FAILURE);
    }

    void Render::AddToDraw(const sf::Drawable& obj)
    {
        rtextura->draw(obj);
    }

    void Render::OnClearColor(const sf::Color& clearcolor)
    {
        rtextura->clear(clearcolor);
        rventana->clear(clearcolor);
    }

    void Render::OnDisplayTextura(void)
    {
        rtextura->display();
    }

    void Render::OnDisplayVentana(void)
    {
        rventana->display();
    }

    sf::RenderWindow& Render::GetVentana(void)
    {
        return *rventana;
    }

    sf::RenderTexture& Render::GetTextura(void)
    {
        return *rtextura;
    }
} // namespace CE
