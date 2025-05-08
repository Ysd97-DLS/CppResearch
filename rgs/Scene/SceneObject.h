#pragma once
#include "../Base/Maths.h"
#include <string>
#include <vector>

namespace RGS {
    class SceneObject {
    public:
        SceneObject(const std::string& name = "Object") : m_Name(name) {}
        virtual ~SceneObject() = default;

        void SetPosition(const Vec3& position) { m_Position = position; }
        void SetRotation(const Vec3& rotation) { m_Rotation = rotation; }
        void SetScale(const Vec3& scale) { m_Scale = scale; }

        const Vec3& GetPosition() const { return m_Position; }
        const Vec3& GetRotation() const { return m_Rotation; }
        const Vec3& GetScale() const { return m_Scale; }
        const std::string& GetName() const { return m_Name; }

        Mat GetTransform() const {
            Mat transform = Identity();
            transform *= Scale(m_Scale.x, m_Scale.y, m_Scale.z);
            transform *= RotateX(m_Rotation.x);
            transform *= RotateY(m_Rotation.y);
            transform *= RotateZ(m_Rotation.z);
            transform *= Translate(m_Position.x, m_Position.y, m_Position.z);
            return transform;
        }

    protected:
        std::string m_Name;
        Vec3 m_Position{0.0f, 0.0f, 0.0f};
        Vec3 m_Rotation{0.0f, 0.0f, 0.0f};
        Vec3 m_Scale{1.0f, 1.0f, 1.0f};
    };
}