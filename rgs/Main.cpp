#include "Application.h"
#include <iostream>
#include <string>

int main() {
    RGS::Application app("RGS", 300, 300);

    float r = 0.0f, g = 0.0f, b = 0.0f;
    std::cout << "\nChoose background color(r,g,b) [0-1]" << std::endl;
    do {
        std::cout << "r: ";
        std::cin >> r;
    } while (r < 0.0f || r > 1.0f);
    
    do {
        std::cout << "g: ";
        std::cin >> g;
    } while (g < 0.0f || g > 1.0f);
    
    do {
        std::cout << "b: ";
        std::cin >> b;
    } while (b < 0.0f || b > 1.0f);
    
    app.GetScene().SetBackgroundColor({ r, g, b });
    
    app.Run();
    return 0;
}