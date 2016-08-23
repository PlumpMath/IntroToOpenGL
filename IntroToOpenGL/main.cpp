#include "MyApplication.h"

int main()
{
	Application* theApp = new SolarSystem();
	if (theApp->startup() == true) {
		while (theApp->update() == true)
			theApp->draw();
		theApp->shutdown();
	}

	delete theApp;
	return 0;
}