#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>

using namespace std;

//hight and width
const int width = 50;
const int height = 20;

//snake head
int x,y;
//food
int fruitX,fruitY;
//store score of the player
int playerScore;
//snake tail
int snakeTailX[100],snakeTailY[100];
//store length of the snake tail
int snakeTailLength;
//snake move
enum snakeDir{STOP = 0, LEFT, RIGHT, UP, DOWN};
snakeDir sDir;
//checking game over or not
bool isGameOver;
int highScore;


//initialize game variables
void Game(){
	isGameOver = false;
	sDir = STOP;
	x = width/2;
	y=height/2;
	fruitX = rand()%width;
	fruitY = rand()%height;
	playerScore = 0;
}

//create the game board
void GameRender(string playerName){
	//clear console
	system("cls");
	//create top walls
	for(int i=0; i<width+2; i++)
		cout<<"-";
		cout<<endl;

		for(int i=0; i<height; i++){
			for(int j=0; j<=width; j++){
				if(j==0 || j==width)
					cout<<"|";
				if(i==y && j==x)
					cout<<"O";
				else if(i==fruitY && j==fruitX)
					cout<<"#";
				else{
					bool prTail = false;
					for(int k=0; k<snakeTailLength; k++){
						if(snakeTailX[k] == j && snakeTailY[k]==i){
							cout<<"o";
							prTail = true;
						}
					}
					if(!prTail)
						cout<<" ";
				}
			}
			cout<<endl;
		}
		//create bottom walls
		for(int i=0; i<width+2; i++)
			cout<<"-";
			cout<<endl;
		//display score
		cout<<playerName<<" Score : "<<playerScore<<endl;
}

//update game state
void UpdateGame(){
	int prevX = snakeTailX[0];
	int prevY = snakeTailY[0];
	int prev2X,prev2Y;
	snakeTailX[0] = x;
	snakeTailY[0]= y;

	for(int i=1; i<snakeTailLength; i++){
		prev2X = snakeTailX[i];
		prev2Y = snakeTailY[i];
		snakeTailX[i] = prevX;
		snakeTailY[i] = prevY;
		prevX=prev2X;
		prevY=prev2Y;
	}
	switch(sDir){
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
	}
	if(x>=width || x<0 || y>=height || y<0)
		isGameOver = true;

	for(int i=0; i<snakeTailLength; i++){
		if(snakeTailX[i] == x && snakeTailY[i] == y)
			isGameOver = true;
	}
	if(x == fruitX && y ==fruitY){
		playerScore += 10;
		fruitX = rand()%width;
		fruitY = rand()%height;
		snakeTailLength++;
	}
}

//select difficulty
int SetDifficulty(){
	int dif, choice;
	cout<<"\nSET DIFFICULTY\n1: Easy\n2: Medium\n3: Hard "
		  "\nNOTE:if not choosen or pressed any other "
		  "key, the difficulty will be automatically set "
		  "to medium\nChoose difficulty level: ";
	cin>>choice;
	switch(choice){
		case '1':
			dif=50;
			break;
		case '2':
			dif=100;
			break;
		case '3':
			dif=150;
			break;
		default:
			dif=100;
	}
	return dif;
}

//handle user inputs
void UserInput(){
	if(_kbhit()){
		switch(_getch()){
			case 'a':
				sDir = LEFT;
				break;
			case 'd':
				sDir = RIGHT;
				break;
			case 'w':
				sDir = UP;
				break;
			case 's':
				sDir = DOWN;
				break;
			case 'x':
				isGameOver = true;
				break;
		}
	}
}
void GameOver(string playerName){
    system("cls");
    cout<< "  _____                         ____                 \n";
    cout<< " / ____|                       / __ \\                \n";
    cout<< "| |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ \n";
    cout<< "| | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|\n";
    cout<< "| |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   \n";
    cout<< " \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   \n";
    cout<< "\n\n";
    cout<<playerName<<" Score : "<<playerScore<<endl;
    if(playerScore>highScore){
        highScore = playerScore;
        cout<<"New High score : "<<highScore<<endl;
    }else{
        cout<<"High Score : "<<highScore<<endl;
    }
    cout<<"Press any key to return to main menu...";
    _getch();
}
void StartGame(){
    string playerName;
	cout<<"...................................................................\n";
    cout<<"...................................................................\n";
	cout<<"..                                                __      __     .. \n";
	cout<<"..    * * *  **    *      *      *     *  * * * * \\ \\    / /     ..\n";
	cout<<"..   *       * *   *     * *     *   *    *        \\ \\  / /      .. \n";
	cout<<"..    * *    *  *  *    * * *    * *      * * *     \\ \\/ /       ..\n";
	cout<<"..        *  *   * *   *     *   *   *    *         / /\\ \\       ..\n";
	cout<<"..   * * *   *    **  *       *  *     *  * * * *  / /  \\ \\      ..\n";
	cout<<"..                                                /_/    \\_\\     ..\n";
	cout<<"...................................................................\n";
	cout<<"...................................................................\n";
	cout<<endl;
	cout<<endl;
	cout<<"         Enter your name : ";
	cin>>playerName;
	int dif = SetDifficulty();

	Game();
	while(!isGameOver){
		GameRender(playerName);
		UserInput();
		UpdateGame();
		Sleep(dif);
	}
	GameOver(playerName);
}

int main(){
	char choose;
	do{
            system("cls");
        StartGame();
        cout<<"Would you like to play again? (y/n) : ";
        cin>>choose;
	}while(choose == 'y' || choose == 'Y');
	cout<<"Press any key to return to exit...";
    _getch();
	return 0;
}

