#pragma once
#include <string>
#include "../Utils/Vector2D.hpp"

namespace IVJ {
    class FSMSonido;
}
namespace sf {
    class Sound;
}

namespace CE
{
    class IComponentes
    {
        public:
            virtual ~IComponentes() = default;
    };

    class INombre : public IComponentes
    {
        public:
            INombre(const std::string& nom);
            ~INombre() override = default;
            std::string nombre;
    };

    class ITransform : public IComponentes
    {
        public:
            ITransform(const Vector2D& pos, const Vector2D& vel, float ang);
            ITransform() = default;
            ~ITransform() override = default ;
        public:
            Vector2D posicion;
            Vector2D pos_previa;
            Vector2D velocidad;
            float angulo;

        float velocidadForward = 0.f;  // velocidad del coche adelante/atrás
        float velocidadMax = 320.f;
        float velocidadReversaMax = -60.f;
        float aceleracion = 90.f;
        float frenado = 260.f;
        float desaceleracion = 50.f;
        float steerAngle = 0.f;     // ángulo de giro actual
        float rotacionVel = 120.; // grados por segundo (giro estilo GTA2)
        float velocidadPrevia = 0.f;

        float centroX = 0.f;
        float centroY = 0.f;
        float radio = 0.f;
        bool inicializado = false;
    };

    class IComponenteSonido : public IComponentes {
    public:
        // Loops del motor hacia adelante
        std::shared_ptr<sf::Sound> idleLoop;
        std::shared_ptr<sf::Sound> lowLoop;
        std::shared_ptr<sf::Sound> midLoop;
        std::shared_ptr<sf::Sound> highLoop;
        std::shared_ptr<sf::Sound> veryHighLoop;

        // Loop de frenado de motor
        std::shared_ptr<sf::Sound> brakeLoop;

        // Reversa
        std::shared_ptr<sf::Sound> reversaLoop;

        // Efectos
        std::shared_ptr<sf::Sound> skid;
        std::shared_ptr<sf::Sound> downshift;

        // Parámetros
        float skidThreshold   = 0.55f;
        float choqueThreshold = 12.0f;
        float timerChoque     = 0.f;

        // Volúmenes internos para mezcla suave
        float volIdle      = 0.f;
        float volLow       = 0.f;
        float volMid       = 0.f;
        float volHigh      = 0.f;
        float volVeryHigh  = 0.f;
        float volBrake     = 0.f;
        float volReversa   = 0.f;

        // Pitch global del motor
        float pitchActual  = 1.0f;
        float volumenMaster = 80.f;

        bool skidPlaying   = false;
        bool reversaActiva = false;

        IComponenteSonido() = default;
        ~IComponenteSonido() = default;
    };
    /*
    class IComponenteSonido : public IComponentes {
    public:
        // Loop principal del motor
        std::shared_ptr<sf::Sound> motorLoop;
        std::shared_ptr<sf::Sound> reversaLoop;

        // Efectos
        std::shared_ptr<sf::Sound> skid;
        std::shared_ptr<sf::Sound> freno;
        float skidThreshold = 0.55f;

        float choqueThreshold = 12.0f;
        float timerChoque = 0.f;

        // Estado actual del motor
        float pitchActual = 1.0f;
        float volumenActual = 50.f;

        bool skidPlaying = false;
        bool reversaActiva = false;

        IComponenteSonido() = default;
        ~IComponenteSonido() = default;
    };
    */
    /*
    class IComponenteSonido : public IComponentes{
    public:
        std::string motorKey = "motor";
        std::string skidKey  = "skid";
        std::string choqueKey = "impact";

        float skidThreshold = 0.55f;   // giro fuerte para derrapar
        float choqueThreshold = 12.0f; // desaceleración brusca
        float timerChoque = 0.0f;      // cooldown choque

        std::shared_ptr<sf::Sound> motor;
        std::shared_ptr<sf::Sound> skid;

        bool skidPlaying = false;

        std::shared_ptr<IVJ::FSMSonido> fsm;

        IComponenteSonido() = default;
        ~IComponenteSonido() override = default;
    };
    */
    class ITimer : public IComponentes
    {
        public:
            ITimer();
            ~ITimer() override = default;
            int frame_actual;
    };

    class IStats: public IComponentes
    {
        public:
        ~IStats()override{};
        std::uint8_t hp;
        std::uint8_t hp_max;
        std::uint8_t str;
        std::uint8_t def;
        std::uint8_t agi;
    };

    class ISprite : public IComponentes {
        public:
        ISprite(const sf::Texture& textura, float escala);
        ISprite(const sf::Texture& textura, int w, int h, float escala);
        ~ISprite() override{};

    public:
        sf::Sprite m_sprite;
        int width;
        int height;
        float escala;

    private:
        sf::Texture m_textura;
    };

    class IControl : public IComponentes {
    public:
        explicit IControl();
        ~IControl() override{};

        bool arr;
        bool abj;
        bool izq;
        bool der;
        bool acc;
        bool sacc;
        float analogAccel = 0.f;
        float analogBrake = 0.f;
        float analogSteer = 0.f;

    };

    class IBoundingBox : public IComponentes {

    public:
        explicit IBoundingBox(const Vector2D& dim);
        Vector2D tam;
        Vector2D mitad;
    };

    class ITexto : public IComponentes {

        public:
        ITexto(const sf::Font& font, const std::string& texto);
        ~ITexto() override{};

        sf::Text m_texto;
    };


}
