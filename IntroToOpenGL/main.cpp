#include "MyApplication.h"

int main()
{
	Application* theApp = new Application();
	if (theApp->startup() == true) {
		while (theApp->update() == true)
			theApp->draw();
		theApp->shutdown();
	}

	delete theApp;
	return 0;
}