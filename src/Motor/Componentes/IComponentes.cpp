#include "IComponentes.hpp"

namespace CE
{
    INombre::INombre(const std::string& nom)
        :nombre(std::move(nom   )){}

    ITransform::ITransform(const Vector2D& pos, const Vector2D& vel, float ang)
    :posicion{pos},pos_previa(pos),velocidad(vel),angulo{ang},
    centroX(0.f),
    centroY(0.f),
    radio(0.f),
    inicializado(false){}

    ITimer::ITimer()
        :frame_actual(0){}

    ISprite::ISprite(const sf::Texture &textura, float escala)
        :IComponentes{},m_sprite{textura}, escala{escala}{

        m_sprite.setScale({escala, escala});
        auto dim = textura.getSize();
        width = dim.x;
        height = dim.y;
        m_sprite.setOrigin({dim.x/2.f, dim.y/2.f});
    }

    ISprite::ISprite(const sf::Texture& textura, int w, int h, float escala)
        :IComponentes{}, m_sprite{m_textura}, m_textura{textura}, width{w}, height{h} ,escala{escala} {

        m_textura.setSmooth(true);
        m_sprite.setTexture(m_textura);
        m_sprite.setTextureRect(sf::IntRect({0, 0}, {w, h}));
        m_sprite.setScale({escala, escala});

        m_sprite.setOrigin({w/2.f, h/2.f});
    }

    IControl::IControl() {
        arr = false;
        abj = false;
        izq = false;
        der = false;
        acc = false;
        sacc = false;

    }

    IBoundingBox::IBoundingBox(const Vector2D& dim)
        :tam{dim.x, dim.y}, mitad {dim.x/2, dim.y/2}{}


    ITexto::ITexto(const sf::Font& font, const std::string& texto)
        :IComponentes{},m_texto{sf::Text(font, texto)}
    {}


}