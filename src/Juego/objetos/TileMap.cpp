#include "TileMap.hpp"
#include "SFML/Graphics/PrimitiveType.hpp"
#include <fstream>
#include <iostream>

namespace IVJ {

    bool TileMap::loadTileMap(const std::string &atlas_path) {

        std::ifstream archivo {atlas_path};
        std::string info;
        archivo >> info;
        archivo >> atlas_p >> atlas_w >> atlas_h>>
        tile_w >> tile_h >> map_r >> map_c >> info;
        tiles_vertex.setPrimitiveType(sf::PrimitiveType::Triangles);
        tiles_vertex.resize(map_r*map_c*6);
        if (!atlas_texture.loadFromFile(ASSETS+atlas_p)) {
            std::cerr<< "[TileMap] No se pudo cargar la imagen" << atlas_p<< "\n";
            return false;
        }
        int id = 0;
        int i = 0;
        int j = 0;
        int max_col = atlas_w/tile_w;
        while (archivo>>id) {
            if (id != -1) {
                sf::Vertex *trian = &tiles_vertex[(i+j*map_c)*6];
                int tU = id%max_col;
                int tV = id/max_col;

                //vertices del triángulo
                trian[0].position = sf::Vector2f(i*tile_w,j*tile_h);
                trian[1].position = sf::Vector2f((i+1)*tile_w,j*tile_h);
                trian[2].position = sf::Vector2f(i*tile_w,(j+1)*tile_h);
                trian[3].position = sf::Vector2f(i*tile_w,(j+1)*tile_h);
                trian[4].position = sf::Vector2f((i+1)*tile_w,j*tile_h);
                trian[5].position = sf::Vector2f((i+1)*tile_w,(j+1)*tile_h);
                //textura del triángulo
                trian[0].texCoords = sf::Vector2f(tU*tile_w,tV*tile_h);
                trian[1].texCoords = sf::Vector2f((tU+1)*tile_w,tV*tile_h);
                trian[2].texCoords = sf::Vector2f(tU*tile_w,(tV+1)*tile_h);
                trian[3].texCoords = sf::Vector2f(tU*tile_w,(tV+1)*tile_h);
                trian[4].texCoords = sf::Vector2f((tU+1)*tile_w,tV*tile_h);
                trian[5].texCoords = sf::Vector2f((tU+1)*tile_w,(tV+1)*tile_h);
            }
            if ((i+1)%map_c == 0) ++j;
            ++i%=map_c;
        }
        return true;
    }

    void TileMap::draw(sf::RenderTarget& target, sf::RenderStates state) const {
        state.transform *= getTransform();
        state.texture = &atlas_texture;
        target.draw(tiles_vertex, state);
    }

}