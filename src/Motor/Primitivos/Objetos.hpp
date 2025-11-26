#pragma once

#include <SFML/Graphics.hpp>
#include "../Componentes/IComponentes.hpp"
#include "Motor/Utils/Vector2D.hpp"

namespace CE
{
    class Objeto: public sf::Drawable, public sf::Transformable
    {
        public:
            explicit Objeto();
            virtual ~Objeto() = default ;
            void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
            Objeto& addComponente(const std::shared_ptr<IComponentes>& componentes);

            template<typename T> T* getComponente() const
            {
                static_assert(std::is_base_of<IComponentes, T>::value,"Solo derivados de IComponente");
                for (auto& c: componentes)
                {
                    T*comp =dynamic_cast<T*>(c.get());
                    if (comp != nullptr)
                        return comp;
                }
                return nullptr;
            }

            template<typename T> bool tieneComponente() const
            {
                static_assert(std::is_base_of<IComponentes, T>::value,"Solo derivado de IComponente");
                for (auto& c: componentes)
                {
                    T*comp =dynamic_cast<T*>(c.get());
                    if (comp != nullptr)
                        return true;
                }
                return false;
            }

            static int getNumObjetos()
            {
                return num_objetos;
            }

            std::shared_ptr<INombre> getNombre()
            {
                return nombre;
            }

            std::shared_ptr<ITransform> getTransformada()
            {
                return transform;
            }

            std::shared_ptr<ITransform> getTransformada() const
            {
                return transform;
            }

            void setPosicion(float x, float y);
            virtual void onUpdate(float dt){};
        virtual std::string toString()
        {
            auto t = getTransformada();

            return "ID: "+ std::to_string(id)+"|"+nombre->nombre + " | " +
                "pos x: " + std::to_string(t->posicion.x) +
                " pos y: " + std::to_string(t->posicion.y) +
                    " velocidad: (" + std::to_string(t->velocidadForward) + ") |"
                " anguloVolante: (" + std::to_string(t->angulo) + ")" ;      }

            int getId(){return id;}

        std::shared_ptr<IStats>& getStats()
        {
            return stats;
        }
        const bool estaVivo()const {
            return stats->hp>0;
        }

        private:
            static int num_objetos;
        protected:
            int id;
            std::vector<std::shared_ptr<IComponentes>> componentes;
            std::shared_ptr<INombre> nombre;
            std::shared_ptr<ITransform> transform;
            std::shared_ptr<IStats> stats;
    };
}