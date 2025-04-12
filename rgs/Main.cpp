#include <iostream>
#include "Application.h"
using namespace std;

int main() {
	RGS::Application app("RGS", 500, 200);
	app.Run();
	return 0;
}