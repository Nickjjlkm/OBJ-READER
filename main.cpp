#include <SFML/Graphics.hpp>


sf::Vector2<float> project(const sf::Vector3<float> p, float fov,float width,float height) {
    float z = p.z == 0? 0.0001f : p.z;
    float factor = fov / z;
    float x = p.x * factor + width / 2.0f;
    float y = p.y * factor + height / 2.0f;
    return {x,y};
};


int main() {

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

        for (auto e : edges) {
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



        }

        window.display();
    }
}

