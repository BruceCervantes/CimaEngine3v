#include "CEPool.hpp"
#include <algorithm>

namespace CE
{
    Pool::Pool(int size): maxsize{size}
    {
        objetos.reserve(maxsize);
    }

    void Pool::agregarPool(const std::shared_ptr<Objeto>& obj)
    {
        if (objetos.size() == maxsize)
        {
            maxsize += maxsize; // duplica la capacidad
            objetos.resize(maxsize);
        }
        objetos.emplace_back(obj);
    }

    void Pool::borrarPool()
    {
        // Lambda para borrar todos los objetos marcados como muertos
        objetos.erase(
            std::remove_if(
                objetos.begin(),
                objetos.end(),
                [](std::shared_ptr<Objeto>& o)
                {
                    return !o->estaVivo();
                }),
            objetos.end());
    }

    std::shared_ptr<Objeto>& Pool::operator[](std::size_t idx)
    {
        return objetos[idx];
    }

    const std::shared_ptr<Objeto>& Pool::operator[](std::size_t idx) const
    {
        return objetos[idx];
    }
}
