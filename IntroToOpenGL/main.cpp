#include "MyApplication.h"

int main()
{
	
	Application* theApp = new SolarSystem();
	//std::cout << "1. For Solar System \n2. For Camera \n3. For Rendering Geometry \n4. For Textures \n5. For Procedural Generation";
	int input = 2;
	//std::cin >> input;


	switch (input)
	{
	case 1:
		theApp = new SolarSystem();
		break;
	case 2:
		theApp = new Camera();
		break;
	case 3:
		theApp = new RenderGeo();
		break;
	case 4:
		theApp = new Textures();
		break;
	case 5:
		theApp = new ProcGen();
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