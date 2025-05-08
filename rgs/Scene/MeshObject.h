#pragma once
#include "../Base/Maths.h"
#include "../Renderer/Renderer.h"
#include "SceneObject.h"
#include "../Shader/BlinnShader.h"
#include <vector>
#include <string>

namespace RGS {
    class MeshObject : public SceneObject {
    public:
        MeshObject(const std::string& name = "MeshObject") : SceneObject(name) {}
        
        void LoadFromFile(const char* fileName);
        const std::vector<Triangle<BlinnVertex>>& GetMesh() const { return m_Mesh; }
        
        void SetTexturePath(const std::string& path) { m_TexturePath = path; }
        const std::string& GetTexturePath() const { return m_TexturePath; }
        
    private:
        std::vector<Triangle<BlinnVertex>> m_Mesh;
        std::string m_TexturePath;
    };
}