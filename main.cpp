#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "mesh.h"

sf::Vector2<float> project(const sf::Vector3<float> p, float fov, float width, float height) {
    float z = p.z == 0 ? 0.0001f : p.z;
    float factor = fov / z;
    float x = p.x * factor + width / 2.0f;
    float y = p.y * factor + height / 2.0f;
    return {x, y};
}

struct FaceToDraw {
    Face f;
    float z;
    int faceId;
};

Mesh createCube(float size = 1.f) {
    Mesh m;
    m.position = {0, 0, 0};
    float s = size * 0.5f;

    // --------- Crear vértices (con UV por defecto) ---------
    m.vertices = {
        {{-s, -s, -s}, {0,0}}, // 0
        {{ s, -s, -s}, {1,0}}, // 1
        {{ s,  s, -s}, {1,1}}, // 2
        {{-s,  s, -s}, {0,1}}, // 3

        {{-s, -s,  s}, {0,0}}, // 4
        {{ s, -s,  s}, {1,0}}, // 5
        {{ s,  s,  s}, {1,1}}, // 6
        {{-s,  s,  s}, {0,1}}  // 7
    };

    // ------------ Agregar 6 caras (quads) ------------
    m.addQuad(0,1,2,3); // atrás
    m.addQuad(4,5,6,7); // frente
    m.addQuad(0,4,7,3); // izquierda
    m.addQuad(1,5,6,2); // derecha
    m.addQuad(3,2,6,7); // arriba
    m.addQuad(0,1,5,4); // abajo

    return m;
}

int main() {
    float fov = 200.0f;
    float width = 800.0f;
    float height = 600.0f;

    sf::RenderWindow window(sf::VideoMode(800, 600), "3D Introduction");

    Mesh cube = createCube();

    sf::Texture cubeTexture;
    cubeTexture.loadFromFile("./src/gatotextura.jpg");
     cube = Mesh().loadFromObjFile("./src/Sphere.obj");
    float speed = 100.0f;
    sf::Clock clock;
    while (window.isOpen()) {

        float dt = clock.restart().asSeconds();

        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::W) {
                    cube.position.z += speed * dt;
                    cube.rotation.x += speed * dt * 10;
                }
                if (e.key.code == sf::Keyboard::A) {
                    cube.position.x -= speed * dt;
                    cube.rotation.z -= speed * dt * 10;
                }
                if (e.key.code == sf::Keyboard::S) {
                    cube.position.z -= speed * dt;
                    cube.rotation.x -= speed * dt * 10;
                }
                if (e.key.code == sf::Keyboard::D) {
                    cube.position.x += speed * dt;
                    cube.rotation.z += speed * dt * 10;
                }
                if (e.key.code == sf::Keyboard::Space) {
                    cube.position.y -= speed * dt;
                    cube.rotation.x -= speed * dt * 10;
                }
                if (e.key.code == sf::Keyboard::LShift) {
                    cube.position.y += speed * dt;
                    cube.rotation.x += speed * dt * 10;
                }
            }
        }
        window.clear(sf::Color::White);

        std::vector<Vertex> vertices = cube.getTransformedVertex();
        std::vector<Face> faces = cube.faces;

        std::vector<FaceToDraw> sortedFaces;
        for (int i = 0; i < faces.size(); i++) {
            auto& f = faces[i];
            sf::Vector3f centro =
                vertices[f.v0].position+
                vertices[f.v1].position+
                vertices[f.v2].position+
                vertices[f.v3].position;

            centro = centro / 4.0f;

            float dist = centro.x * centro.x + centro.y * centro.y + centro.z * centro.z;

            sortedFaces.push_back(FaceToDraw{f, dist, i});
        }

        std::sort(sortedFaces.begin(), sortedFaces.end(),
            [](const FaceToDraw& a, const FaceToDraw& b) {
                return a.z > b.z;
            }
        );

        for (auto &f: sortedFaces) {
            sf::Vector3f p3D[4] = {
                vertices[f.f.v0].position,
                vertices[f.f.v1].position,
                vertices[f.f.v2].position,
                vertices[f.f.v3].position
            };

            sf::Vector2f p2D[4] = {
                project(p3D[0], fov, width, height),
                project(p3D[1], fov, width, height),
                project(p3D[2], fov, width, height),

            };

            sf::VertexArray tri(sf::Triangles, 6);

            tri[0].position = p2D[0];
            tri[1].position = p2D[1];
            tri[2].position = p2D[2];

            // tri[3].position = p2D[0];
            // tri[4].position = p2D[2];
            // tri[5].position = p2D[3];

            tri[0].texCoords = vertices[f.f.v0].uv * 1121.0f; // 1121 Texture size (Square texture)
            tri[1].texCoords = vertices[f.f.v1].uv * 1121.0f;
            tri[2].texCoords = vertices[f.f.v2].uv * 1121.0f;

            // tri[3].texCoords = vertices[f.f.v0].uv * 1121.0f;
            // tri[4].texCoords = vertices[f.f.v2].uv * 1121.0f;
            // tri[5].texCoords = vertices[f.f.v3].uv * 1121.0f;

            window.draw(tri, &cubeTexture);
        }

        for (auto f: cube.faces) {
            sf::Vector3 p1 = vertices[f.v0].position;
            sf::Vector3 p2 = vertices[f.v1].position;
            sf::Vector3 p3 = vertices[f.v2].position;

            sf::Vector2f a = project(p1, fov, width, height);
            sf::Vector2f b = project(p2, fov, width, height);
            sf::Vector2f c = project(p3, fov, width, height);

            sf::Vertex line[6] = {
                sf::Vertex(a, sf::Color::Black),
                sf::Vertex(b, sf::Color::Black),

                sf::Vertex(b, sf::Color::Black),
                sf::Vertex(c, sf::Color::Black),

                sf::Vertex(c, sf::Color::Black),
                sf::Vertex(a, sf::Color::Black)
            };
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }
}
