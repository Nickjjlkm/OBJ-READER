#include <SFML/Graphics.hpp>
#include <array>
#include <vector>



sf::Vector2<float> project(const sf::Vector3<float> p, float fov,float width,float height) {
    float z = p.z == 0? 0.0001f : p.z;
    float factor = fov / z;
    float x = p.x * factor + width / 2.0f;
    float y = p.y * factor + height / 2.0f;
    return {x,y};
};


sf::Vector3f aplicarTransformacion(
    const sf::Vector3f& p,
    const sf::Vector3f&traslacion,
    const sf::Vector3f& rotacion) {
    return p + traslacion;
}

struct FacetoDraw {
    std::array<int,4>indices;
    float z;
    int faceid;
};

int main() {

    sf::Texture gatoTextura;
    gatoTextura.loadFromFile("./src/gatotextura.jpg");

   float fov = 200;
    float width = 800;
    float height = 600;
    sf::RenderWindow window (sf::VideoMode(800,600), "3D Introduction");

    std::vector<sf::Vector3<float>> cube = {
        {-1, -1, -1}, {1, -1, -1},
        {1, 1, -1}, {-1, 1, -1},
        {-1, -1, 1}, {1, -1, 1},
        {1, 1, 1}, {-1, 1, 1},
    };

    std::vector<std::pair<int, int>> edges = {
        {0,1}, {1,2},{2,3}, {3,0},
        {4,5}, {5,6},{6,7}, {7,4},
        {0,4}, {1,5},{2,6}, {3,7},
    };

    std::vector<std::array<int,4>> faces = {
        {4,5,6,7},
        {0,1,5,4},
        {3,2,6,7},
        {0,3,7,4},
        {1,2,6,5},
        {0,1,2,3}
    } ;

    std::vector<sf::Vector3f> transformed(cube.size());

    sf::Vector3<float> translation = {0,-3,5};
    float speed = 50.0f;
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
                    translation.z += speed * dt;
                }
                if (e.key.code == sf::Keyboard::A) {
                    translation.x -= speed * dt;
                }
                if (e.key.code == sf::Keyboard::S) {
                    translation.z -= speed * dt;
                }
                if (e.key.code == sf::Keyboard::D) {
                    translation.x += speed * dt;
                }
                if (e.key.code == sf::Keyboard::Space) {
                    translation.y -= speed * dt;
                }
                if (e.key.code == sf::Keyboard::LShift) {
                    translation.y += speed * dt;
                }
            }
        }

        window.clear(sf::Color::White);
        for (size_t i = 0; i < transformed.size(); i++) {
            transformed[i] = aplicarTransformacion(cube[i],translation , {0,0,0});
        }

        std::vector<FacetoDraw> sortedFaces;
        for (int i = 0; i < faces.size(); i++) {
            auto&f = faces[i];
            sf::Vector3f centro =
                transformed [f[0]]+transformed [f[1]]+transformed [f[2]]+transformed [f[3]];



            centro = centro /4.0f;
            float dist = centro.x * centro.x + centro.y * centro.y + centro.z * centro.z;
            sortedFaces.push_back(FacetoDraw {f,dist,i});
        }

        std::sort(sortedFaces.begin(),sortedFaces.end(),[](const FacetoDraw& a, const FacetoDraw& b) {
            return a.z >  b.z;

        }
        );



        for (auto &f :sortedFaces) {
            sf::Vector3f p3D[4] = {
                transformed[f.indices[0]],
                transformed[f.indices[1]],
                transformed[f.indices[2]],
                transformed[f.indices[3]]
            };

            sf::Vector2f p2D[4]{
                project(p3D[0],fov,width,height),
                project(p3D[1],fov,width,height),
                project(p3D[2],fov,width,height),
                project(p3D[3],fov,width,height)

            };

            sf::Vector2f uv[4] = {
                {0,0},
                    {2500,0},
                {2500,1250},
                {0,1250},
            };

            sf::VertexArray tri(sf::Triangles,6);

            tri [0].position = p2D[0];
            tri [1].position = p2D[1];
            tri [2].position = p2D[2];

            tri[3].position = p2D[0];
            tri[4].position = p2D[2];
            tri[5].position = p2D[3];

            tri[0].texCoords = uv[0];
            tri[1].texCoords = uv[1];
            tri[2].texCoords = uv[2];

            tri[3].texCoords = uv[0];
            tri[4].texCoords = uv[2];
            tri[5].texCoords = uv[3];

            window.draw(tri, &gatoTextura);
        }

        /*for (auto e : edges) {
            sf::Vector3 p1 = cube[e.first];
            sf::Vector3 p2 = cube[e.second];

            p1 = p1 + translation;
            p2 = p2 + translation;

            sf::Vector2f a = project(p1,fov,width,height);
            sf::Vector2f b = project(p2,fov,width,height);

            sf::Vertex line[2] = {
                sf::Vertex(a, sf::Color::Black),
                sf::Vertex(b, sf::Color::Black)
            };
            window.draw(line, 2, sf::Lines);



        }*/

        window.display();
    }
}

