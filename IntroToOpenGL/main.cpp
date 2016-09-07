#include "MyApplication.h"

int main()
{
	
	Application* theApp = new RenderGeo();
	std::cout << "1. For SolarSystem \n2. For RenderGeo \n";
	int input;
	std::cin >> input;
	switch (input)
	{
	case 1:
		theApp = new SolarSystem();
		break;
	case 2:
		theApp = new RenderGeo();
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