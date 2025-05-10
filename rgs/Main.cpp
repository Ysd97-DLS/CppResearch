#include "Application.h"
#include <iostream>
#include <string>

int main() {
    RGS::Application app("RGS", 800, 800);
    
    std::cout << "可用的模型列表：" << std::endl;
    std::cout << "1. sphere.obj" << std::endl;
    std::cout << "2. box.obj" << std::endl;
    std::cout << "请选择要渲染的模型 (输入数字): ";
    
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
    
    std::cout << "\n可用的纹理列表：" << std::endl;
    std::cout << "1. P3.ppm" << std::endl;
    std::cout << "2. earthmap.ppm" << std::endl;
    std::cout << "请选择要使用的纹理 (输入数字): ";
    
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