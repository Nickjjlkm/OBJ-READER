

#ifndef MESH_H
#define MESH_H
#include <cmath>
#include <vector>
#include "vertex.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
    std::vector<Face> faces;

    sf::Vector3f origin = {0,0,0};
    sf::Vector3f position = {0,0,0};
    sf::Vector3f rotation = {0,0,0};
    sf::Vector3f scale = {1,1,1};


    Mesh() = default;

    Vertex transformVertex(const Vertex & p) const {
        Vertex v = p;
        sf::Vector3f r = v.position;

        r -= origin;

        sf::Vector3f rc = rotation;

        rc.x *= M_PI / 180.0f;
        rc.y *= M_PI / 180.0f;
        rc.z *= M_PI / 180.0f;

        const auto cx = static_cast<float>(cos(rc.x));
        const auto sx = static_cast<float>(sin(rc.x));

        const auto cy = static_cast<float>(cos(rc.y));
        const auto sy = static_cast<float>(sin(rc.y));

        const auto cz = static_cast<float>(cos(rc.z));
        const auto sz = static_cast<float>(sin(rc.z));

        // Rotacion X
        const float y1 = r.y * cx - r.z * sx;
        const float z1 = r.y * sx + r.z * cx;

        r.y = y1;
        r.z = z1;

        // Rotacion Y
        const float x2 = r.x * cy - r.z * sy;
        const float z2 = -r.x * sy + r.z * cy;

        r.x = x2;
        r.z = z2;

        // Rotacion Z
        const float x3 = r.x * cz - r.y * sz;
        const float y3 = r.x * sz + r.y * cz;

        r.x = x3;
        r.y = y3;

        r += origin;
        r += position;

        v.position = r;
        return v;
    };

    std::vector<Vertex> getTransformedVertex() {
        std::vector<Vertex> v (vertices.size());
        std::ranges::transform(vertices, v.begin(), [this](const Vertex & v) {return transformVertex(v);});
        return v;
    };

    void addQuad(int a, int b, int c, int d) {
        faces.push_back({a, b, c, d});

        // 2 triángulos por quad
        triangles.push_back({a, b, c});
        triangles.push_back({a, c, d});
    }

    void addTriangle(int a, int b, int c) {
        faces.push_back({a, b, c, -1});
        triangles.push_back({a, b, c});
    }


    Mesh loadFromObjFile(sf::String fileName) {
        Mesh mesh;
        std::vector<sf::Vector3f> positions;
        std::vector<sf::Vector2f> uvs;


        std::map<std::string, int> vertexCache;

        std::ifstream file (fileName);

        if (!file.is_open()) {
            std::cout << "No abrio" << std::endl;
           return Mesh();
        }

        std::string line;
        while (std::getline(file, line)) {

            if (line.empty() || line[0] == '#') continue;
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "v") {
                sf::Vector3f pos;
                iss >> pos.x;
                iss >> pos.y;
                iss >> pos.z;

                positions.push_back(pos);
            }

            else if (type == "vt") {
                sf::Vector2f v;
                iss >> v.x;
                iss >> v.y;
                uvs.push_back(v);

            }

            else if (type == "f") {
                std::vector<int> faceIndex;
                std::string vertexData;

                while (iss >> vertexData) {
                    std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
                    std::istringstream vertexStream(vertexData);

                    int posIndex = -1,uvindex = -1, normalIndex = -1;
                    vertexStream >> posIndex;
                    if (vertexStream.peek() == ' ' ) {
                        vertexStream.ignore();
                        if (vertexStream.peek() != ' ' && vertexStream.peek() != EOF) {
                            vertexStream >> uvindex;
                        }
                    }

                    posIndex--;
                    if (uvindex != -1)uvindex--;

                    std::string key = std::to_string(posIndex) + "/" + std::to_string(uvindex);

                    int finalIndex;
                    auto it = vertexCache.find(key);

                    if (it != vertexCache.end()) {
                        //Encontramos el elemento
                        finalIndex = it->second;
                    }
                    else {
                        Vertex v;
                        v.position = positions[posIndex];
                        if (uvindex >= 0 && uvindex< uvs.size() ) {
                            v.uv = uvs[uvindex];
                        }
                        else {
                            v.uv = sf::Vector2f(0,0);
                        }
                        finalIndex = mesh.vertices.size();
                        mesh.vertices.push_back(v);
                        vertexCache[key] = finalIndex;
                    }
                    faceIndex.push_back(finalIndex);


                }
                if (faceIndex.size() == 3) {
                    mesh.addTriangle (faceIndex[0], faceIndex[1], faceIndex[2]);
                }else if (faceIndex.size() == 4) {
                    mesh.addQuad(faceIndex[0], faceIndex[1], faceIndex[2], faceIndex[3]);

                }else if (faceIndex.size() > 4) {
                    for (int i = 1; i < faceIndex.size() -1; i++) {
                        mesh.addTriangle(faceIndex[0], faceIndex[i], faceIndex[i+1]);
                    }
                }
            }

            std::cout <<  line << std::endl;
        }

        file.close();
        return mesh;
    }
};

#endif //MESH_H