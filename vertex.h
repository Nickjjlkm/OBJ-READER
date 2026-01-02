//
// Created by angel on 25/11/2025.
//

#ifndef VERTEX_H
#define VERTEX_H
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"

struct Vertex {
    sf::Vector3f position;
    sf::Vector2f uv;
    Vertex() = default;
    Vertex(const sf::Vector3f& pos, const sf::Vector2f& tex) : position(pos), uv(tex) {}
};

struct Triangle {
    int v0, v1, v2;
};

struct Face {
    int v0, v1, v2, v3;
};

#endif //VERTEX_H