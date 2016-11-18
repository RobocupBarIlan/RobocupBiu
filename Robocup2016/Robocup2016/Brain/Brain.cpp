#include "Brain.h"
#include "MenuOption.h"
#include "../Common/PlayerInfo.h"
#include "BrainData.h"

// Declartion of the thread method.
void* BrainActionAsync(void*);
MenuOption ShowMenuAndGetOptionFromUser();

Brain* Brain::m_instance = NULL;
Brain* Brain::GetInstance()
{
    if(m_instance == NULL)
    {
    	m_instance = new Brain();
        return m_instance;
    }
    else
    {
        return m_instance;
    }
}

Brain::Brain()
{
	isDebug = true;
}

Brain::~Brain()
{

}

void Brain::RunThread()
 {
	pthread_t brainThread;

	if (1 == pthread_create(&brainThread, NULL, BrainActionAsync, NULL)) {
		fprintf(stderr, "Couldn't create Brain thread\n");
		exit(1);
	}
}

void Brain::StartPlay()
{
	bool flag = true;
	while (flag)
	{
		try
		{
			Log::GetInstance()->Info("Starting to play\n", "Brain");
			PlayerInfo info;
			StateMachine* fsm;
			if (info.isGoalkeeper)
			{
				cout << "Goalkeeper statem machine selected"<<endl;
				fsm = new GoalKeeperStateMachine();
			}
			else
			{
				fsm = new StateMachine();
			}
			fsm->Run();

			if(fsm != NULL)
			{
				delete fsm;
			}

			flag = false; //finished successfully
		}
		catch (Exception ex)
		{
			cout << "Caught exception!!!. Msg: " << ex.msg << endl;
		}
	}
}

void* BrainActionAsync(void*)
{
	// Sleep until the Vision thread will print its header.
	//usleep(1000000);

	bool terminate = false;
	Motion* motion = Motion::GetInstance();
	motion->StartEngines();

	if (!Brain::GetInstance()->isDebug)
	{
		Brain::GetInstance()->StartPlay();
	}
	else
	{
		while (!terminate)
		{
			MenuOption menuOption = ShowMenuAndGetOptionFromUser();
			switch (menuOption)
			{
				case Play:
				{
					Log::GetInstance()->Info("Starting to play", "Brain");
					PlayerInfo info;
					StateMachine* fsm;
					cout << "before select statemachine type"<<endl;
					if (info.isGoalkeeper){
						cout << "entered to goalkeeper statem machine"<<endl;
						fsm = new GoalKeeperStateMachine();
					}
					else {
						fsm = new StateMachine();
					}
					fsm->Run();
					terminate = true;
					if(fsm != NULL){
					delete fsm;
					}
					break;
				}
				case Stand:
				{
					motion->RunAction(ActionPage::Init);
					break;
				}
				case Kick:
				{
					motion->RunAction(ActionPage::RightKick);
					break;
				}
				case Exit:
				{
					terminate = true;
					break;
				}
				case GetTilt:
				{
					HeadTilt currentTilt = motion->GetHeadTilt();
					cout << "Pan: " << currentTilt.Pan << ", Tilt:" << currentTilt.Tilt << endl;
					break;
				}
				case SetTilt:
				{
					float pan,tilt;
					cout << "Enter pan to set:" << endl;
					cin >> pan;
					cout << "Enter tilt to set:" << endl;
					cin >> tilt;
					motion->SetHeadTilt(HeadTilt(tilt, pan));
					break;
				}

				//TODO: Finish options
				case Walk:
				{

					motion->TurnByAngle(90);
					//motion->StartWalking(0,40,5);
					//usleep(4000*1000);
					//motion->StopWalking();
					//motion->StartWalking(5,0,0);

					/*while (MotionStatus::FALLEN == STANDUP)
					{
						//printf( "Robot is walking!\n");
						//keep walking

					}*/
					break;
				}

				case TurnLeft:
				{
					motion->RunAction(ActionPage::BendToBall);
					break;
				}

				case TurnRight:
				{
					//motion->StartWalking();
					//usleep(3000*1000);
					//motion->StopWalking();
					motion->TurnByAngle(90);
					break;
				}

				case StopWalking:
				{
					motion->StopWalking();
					break;
				}

				case Run:
				{
					motion->StartWalking();
					for (int i=0; i<30; i++)
					{
						usleep(250*1000);
						Walking::GetInstance()->X_MOVE_AMPLITUDE = i;
					}
					for (int i=30; i>0; i--)
					{
						usleep(250*1000);
						Walking::GetInstance()->X_MOVE_AMPLITUDE = i;
					}
					break;
				}

				case Reset:
				{
					break;
				}
			}
		}
	}
	return NULL;
}

MenuOption ShowMenuAndGetOptionFromUser()
{
	MenuOption menuOption;
	cout << "Please choose an option:" << endl;
	for (int option = MenuOption::Play ; option != MenuOption::Exit ; option++)
	{
		menuOption = static_cast<MenuOption>(option);
		cout << menuOption << " - " << MenuOptionString[menuOption] << endl;
	}

	int userOption;
	cin >> userOption;
	menuOption = static_cast<MenuOption>(userOption);
	return menuOption;
}



