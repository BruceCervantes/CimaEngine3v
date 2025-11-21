#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

namespace CE
{
    class Vector2D
    {
        public:
            Vector2D();

            Vector2D(const float x, const float y);
            Vector2D(const float v);
            Vector2D(const sf::Vector2f& cpy);

            float length() const;
            float magnitud() const;
            Vector2D& normalizacion();
            float distancia(const Vector2D& v);
            Vector2D& escala(float s);
            Vector2D& suma(const Vector2D& v);
            Vector2D& resta(const Vector2D& v);

            bool operator==(const Vector2D& v) const;
            Vector2D operator+(const Vector2D& v)const;
            Vector2D operator-(const Vector2D& v)const;
            void operator+=(const Vector2D& v);

            friend std::ostream& operator<<(std::ostream& os, const Vector2D& vect);
        public:
            float x;
            float y;
    };
}
