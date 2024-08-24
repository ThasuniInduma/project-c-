#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <fstream>

using namespace std;

//hight and width
const int width = 100;
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
int highScore = 0;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CHAR_INFO buffer[width*height];

void ShowConsoleCursor(bool show){
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = show;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void SetCurSorPosition(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

void ClearScreen(){
    for(int i=0; i<width*height; i++){
        buffer[i].Char.AsciiChar=' ';
        buffer[i].Attributes=0x07;
    }
}

void DrawBorder(){
    //top border
    for(int i=0; i<width+2; i++){
        SetCurSorPosition(i, 0);
        cout<<"-";
        SetCurSorPosition(i, height+1);
        cout<<"-";
    }
    //vertical borders
    for(int i=1; i<=height;i++){
        SetCurSorPosition(0, i);
        cout<<"|";
        SetCurSorPosition(width+1, i);
        cout<<"|";
    }
}

void DrawBuffer(int x, int y, char c){
    if(x>0 && x<width && y>=0 && y<height){
        int index = x+y*width;
        buffer[index].Char.AsciiChar = c;
        buffer[index].Attributes = 0x07;
    }
}

void RenderBuffer(){
    COORD bufferSize = {width, height};
    COORD bufferCoord = {0,0};
    SMALL_RECT writeRegion = {1,1,width,height};
    WriteConsoleOutputA(hConsole, buffer,bufferSize, bufferCoord, &writeRegion);
}

void ClearTail(int prevX, int prevY){
    SetCurSorPosition(prevX+1, prevY+1);
    cout<<" ";
}

//initialize game variables
void Game(){
	isGameOver = false;
	sDir = STOP;
	x = width/2;
	y=height/2;
	fruitX = rand()%width;
	fruitY = rand()%height;
	playerScore = 0;
	snakeTailLength=0;
}

//create the game board
void GameRender(string playerName){
	//clear console
	ClearScreen();
	//draw the tail
	for (int i=0; i<snakeTailLength; i++){
        DrawBuffer(snakeTailX[i], snakeTailY[i], 'o');
    }
    //draw the fruit
    DrawBuffer(fruitX, fruitY, '#');
    //draw the head of the snake
	DrawBuffer(x,y,'O');
    //display buffer
    RenderBuffer();

		//display score
		SetCurSorPosition(0, height+2);
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

	ClearTail(prevX, prevY);

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
        //save new high score to file
        ofstream highScoreFile("highscore.txt");
        highScoreFile<<highScore;
        highScoreFile.close();
    }else{
        cout<<"High Score : "<<highScore<<endl;
    }

}
void LoadHighScore(){
    ifstream highScoreFile("highScore.txt");
    if(highScoreFile.is_open()){
        highScoreFile >> highScore;
        highScoreFile.close();
    }else{
        highScore = 0;
    }
}

void StartGame(string playerName){
    ShowConsoleCursor(false);
	int dif = SetDifficulty();

	Game();
	DrawBorder();
	while(!isGameOver){
		GameRender(playerName);
		UserInput();
		UpdateGame();
		Sleep(dif);
	}
	GameOver(playerName);
	ShowConsoleCursor(true);
}

int main(){
	char choose;
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
	cout<<"Enter your name : ";
	cin>>playerName;
	LoadHighScore();
	do{
        system("cls");
        StartGame(playerName);
        cout<<"Would you like to play again? (y/n) : ";
        cin>>choose;
	}while(choose == 'y' || choose == 'Y');
    cout<<"Press any key to return to main menu...";
    _getch();
	return 0;
}

