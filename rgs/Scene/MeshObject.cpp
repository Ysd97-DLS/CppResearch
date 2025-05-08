#include "MeshObject.h"
#include "../Base/Base.h"
#include <fstream>

namespace RGS {
    void MeshObject::LoadFromFile(const char* fileName) {
        std::ifstream file(fileName, std::ios::in);
        ASSERT(file);
        std::vector<Vec3> positions;
        std::vector<Vec2> texCoords;
        std::vector<Vec3> normals;
        std::vector<int> posIndices;
        std::vector<int> texIndices;
        std::vector<int> normalIndices;
        std::string line;
        
        while (!file.eof()) {
            std::getline(file, line);
            int items = -1;
            if (line.find("v ") == 0) {
                Vec3 position;
                items = sscanf(line.c_str(), "v %f %f %f", &position.x, &position.y, &position.z);
                ASSERT(items == 3);
                positions.push_back(position);
            }
            else if (line.find("vt ") == 0) {
                Vec2 texCoord;
                items = sscanf(line.c_str(), "vt %f %f", &texCoord.x, &texCoord.y);
                ASSERT(items == 2);
                texCoords.push_back(texCoord);
            }
            else if (line.find("vn ") == 0) {
                Vec3 normal;
                items = sscanf(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                ASSERT(items == 3);
                normals.push_back(normal);
            }
            else if (line.find("f ") == 0) {
                int pIndices[3], uvIndices[3], nIndices[3];
                items = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                    &pIndices[0], &uvIndices[0], &nIndices[0],
                    &pIndices[1], &uvIndices[1], &nIndices[1],
                    &pIndices[2], &uvIndices[2], &nIndices[2]);
                ASSERT(items == 9);
                for (int i = 0; i < 3; i++) {
                    posIndices.push_back(pIndices[i] - 1);
                    texIndices.push_back(uvIndices[i] - 1);
                    normalIndices.push_back(nIndices[i] - 1);
                }
            }
        }
        file.close();

        m_Mesh.clear();
        int triNum = posIndices.size() / 3;
        for (int i = 0; i < triNum; i++) {
            Triangle<BlinnVertex> triangle;
            for (int j = 0; j < 3; j++) {
                int index = 3 * i + j;
                int posIndex = posIndices[index];
                int texIndex = texIndices[index];
                int nIndex = normalIndices[index];
                triangle[j].ModelPos = { positions[posIndex],1.0f };
                triangle[j].TexCoord = texCoords[texIndex];
                triangle[j].ModelNormal = normals[nIndex];
            }
            m_Mesh.emplace_back(triangle);
        }
    }
}