#include <iostream>
#include "Application.h"
using namespace std;

int main() {
	RGS::Application app("RGS", 300, 300);
    app.GetScene().SetBackgroundColor({0.529f, 0.808f, 0.922f});
    
	auto object = std::make_shared<RGS::SceneObject>("MyObject");
	object->SetPosition({ 0, 1, 0 });
	object->SetRotation({ 0, PI / 4, 0 });
	object->SetScale({ 2, 2, 2 });
	app.GetScene().AddObject(object);
	if (auto obj = app.GetScene().GetObject("MyObject")) {
		obj->SetPosition({ 1, 1, 1 });
	}
	app.Run();
	return 0;
}