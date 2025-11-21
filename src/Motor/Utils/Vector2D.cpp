#include "Vector2D.hpp"
#include <cmath>
namespace CE
{
    Vector2D::Vector2D()
        :x{0},y{0}{}
    Vector2D::Vector2D(const float x, const float y)
        :x{x},y{y}{}
    Vector2D::Vector2D(const float v)
        :x{v},y{v}{}
    Vector2D::Vector2D(const sf::Vector2f& cpy)
        :x{cpy.x},y{cpy.y}{}

    float Vector2D::length() const
    {
        return sqrt(x*x + y*y);
    }

    float Vector2D::magnitud() const
    {
        return this->length();
    }

    Vector2D& Vector2D::normalizacion()
    {
        float mag = this->length();
        x /= mag;
        y /= mag;
        return *this;
    }

    Vector2D& Vector2D::escala(float s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    float Vector2D::distancia(const Vector2D& v)
    {
        return sqrt(pow(v.x-x, 2) + pow(v.y-y, 2));
    }

    Vector2D& Vector2D::suma(const Vector2D& v)
    {
        x+=v.x;
        y+=v.y;
        return *this;
    }

    Vector2D& Vector2D::resta(const Vector2D& v)
    {
        x-=v.x;
        y-=v.y;
        return *this;
    }

    bool Vector2D::operator==(const Vector2D& v) const
    {
        return  (x==v.x && y==v.y);
    }

     Vector2D Vector2D::operator+(const Vector2D& v) const
    {
        return Vector2D{x+v.x,y+v.y};
    }

    Vector2D Vector2D::operator-(const Vector2D& v) const
    {
        return Vector2D{x-v.x,y-v.y};
    }

    void Vector2D::operator+=(const Vector2D& v)
    {
        x+=v.x;
        y=+v.y;
    }

    std::ostream& operator<<(std::ostream& os, const Vector2D& vect)
    {
        os<< "(" <<vect.x << "," << vect.y << ")";
        return  os;
    }



}