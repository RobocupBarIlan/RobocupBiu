#include "Brain/Brain.h"
#include "Communication/Communication.h"
#include "Brain/Compass/Compass.h"

void Debug()
{
	cout << "A";
	int a;
	cin >> a;
}

int main(int argc, char* argv[])
{
	Brain::GetInstance()->RunThread();
	Vision::GetInstance()->Run();
	Communication::GetInstance();
	Compass::GetInstance();

	char key;
	cout << "Do you want to free all the engines? [y/n]" << endl;
	cin >> key;
	if (key == 'y' || key == 'Y')
	{
		Motion::GetInstance()->FreeAllEngines();
	}

	cout << "Program finished" << endl;
	return 0;
}

