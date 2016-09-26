#include "MyApplication.h"

int main()
{
	
	Application* theApp = new SolarSystem();
	//std::cout << "1. For SolarSystem \n2. For RenderGeo \n";
	int input = 3;
	//std::cin >> input;


	switch (input)
	{
	case 1:
		theApp = new SolarSystem();
		break;
	case 2:
		theApp = new RenderGeo();
		break;
	case 3:
		theApp = new Textures();
		break;
	}

	if (theApp->startup() == true) {
		while (theApp->update() == true)
			theApp->draw();
		theApp->shutdown();
	}

	delete theApp;
	return 0;
}