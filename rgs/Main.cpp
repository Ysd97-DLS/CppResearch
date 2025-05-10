#include "Application.h"
#include <iostream>
#include <string>

int main() {
    RGS::Application app("RGS", 300, 300);
    app.GetScene().SetBackgroundColor({ 0.529f, 0.808f, 0.922f });

    auto object = std::make_shared<RGS::SceneObject>("MyObject");
    object->SetPosition({ 0, 1, 0 });
    object->SetRotation({ 0, PI / 4, 0 });
    std::cout << "Choose model type:" << std::endl;
    std::cout << "1. sphere.obj" << std::endl;
    std::cout << "2. box.obj" << std::endl;
    int modelChoice;
    std::cin >> modelChoice;
    std::string modelPath;
    switch(modelChoice) {
        case 1:
            modelPath = "\\cpp\\CppResearch\\Assets\\sphere.obj";
            break;
        case 2:
            modelPath = "\\cpp\\CppResearch\\Assets\\box.obj";
            break;
        default:
            modelPath = "\\cpp\\CppResearch\\Assets\\sphere.obj";
            break;
    }
    
    std::cout << "\nChoose texture type:" << std::endl;
    std::cout << "1. P3.ppm" << std::endl;
    std::cout << "2. earthmap.ppm" << std::endl;
    
    int textureChoice;
    std::cin >> textureChoice;
    
    std::string texturePath;
    switch(textureChoice) {
        case 1:
            texturePath = "\\cpp\\CppResearch\\Assets\\P3.ppm";
            break;
        case 2:
            texturePath = "\\cpp\\CppResearch\\Assets\\earthmap.ppm";
            break;
        default:
            texturePath = "\\cpp\\CppResearch\\Assets\\P3.ppm";
            break;
    }
    
    app.SetModelPath(modelPath);
    app.SetTexturePath(texturePath);
    
    app.Run();
    return 0;
}