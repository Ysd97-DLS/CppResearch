#pragma once
#include "SceneObject.h"
#include <memory>
#include <vector>

namespace RGS {
    class Scene {
    public:
        Scene() = default;
        ~Scene() = default;

        void AddObject(std::shared_ptr<SceneObject> object) {
            m_Objects.push_back(object);
        }

        void RemoveObject(const std::string& name) {
            m_Objects.erase(
                std::remove_if(m_Objects.begin(), m_Objects.end(),
                    [&name](const auto& obj) { return obj->GetName() == name; }),
                m_Objects.end()
            );
        }

        std::shared_ptr<SceneObject> GetObject(const std::string& name) {
            auto it = std::find_if(m_Objects.begin(), m_Objects.end(),
                [&name](const auto& obj) { return obj->GetName() == name; });
            return (it != m_Objects.end()) ? *it : nullptr;
        }

        const std::vector<std::shared_ptr<SceneObject>>& GetObjects() const {
            return m_Objects;
        }

        void SetBackgroundColor(const Vec3& color) { m_BackgroundColor = color; }
        const Vec3& GetBackgroundColor() const { return m_BackgroundColor; }

    private:
        std::vector<std::shared_ptr<SceneObject>> m_Objects;
        Vec3 m_BackgroundColor{0.0f, 0.0f, 0.0f};
    };
}