#pragma once
#include <memory>


namespace CE
{
    //definicion de la case Lista para poder
    //asignarla como amigable antes de definirla
    template <typename T> class Lista;
    template <typename T> class Nodo{

        //Nadie puede instanciar la clase Nodo\
        //solo clases amigables
        friend class Lista<T>;

    protected:
        Nodo (T d)
        :dato{d}
        {
            m_next=nullptr;
            m_prev=nullptr;
        }

    public:
        T dato;
        std::shared_ptr<Nodo<T>> m_prev;
        std::shared_ptr<Nodo<T>> m_next;
        unsigned int size;
    };
}