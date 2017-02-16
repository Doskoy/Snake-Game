/////////////////////////////////////////////////////
// 
//Author: Fernando Roldán Zafra :D
//
/////////////////////////////////////////////////////

/*
_kbhit function from http://www.flipcode.com/archives/_kbhit_for_Linux.shtml
*/
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <sys/ioctl.h>

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}


#include <iostream>
#include <unistd.h> //usleep()
#include <stdlib.h> //rand()
using namespace std;

static bool GAMEOVER = false;
enum Directions{STOP, UP, DOWN, LEFT, RIGHT};

struct Point{
	int x;
	int y;
};

class Snake{
private:
	static const int ROWS = 40;
	static const int COLUMNS = 20;
	char field[COLUMNS][ROWS];
	Directions looking; 
	Point head_position;
	Point fruit_position;
	bool eat_fruit = false;

	Point end_tail;
	int fruits;

	bool InsideField (int positionX, int positionY){
		return 0 <= positionX && positionX < COLUMNS && 0 <= positionY && positionY < ROWS;
	}

	//change the direction, works with the ASCII value of numbers 
	void Input(){
		
		int input = 9;
		if(_kbhit()){
			input = cin.get();
			switch(input){
				case 56:
				looking = UP;
				break;

				case 52:
				looking = LEFT;
				break;
			
				case 54:
				looking = RIGHT;
				break;

				case 50:
				looking = DOWN;
				break;

				case 48: 
				GAMEOVER = true;
				break;
			}
		}
	}

public: 
	//Constructor
	Snake(){
		for (int i = 0; i < COLUMNS; i++)
			for (int j = 0; j < ROWS; j++)
				field[i][j] = ' ';
		
		head_position.x = COLUMNS/2;		
		head_position.y = ROWS/2;
		fruit_position.x = rand() % COLUMNS-2 + 2;
		fruit_position.y = rand() % ROWS-2 + 2;

		end_tail = head_position;

		field[head_position.x][head_position.y] = 'O';
		field[fruit_position.x][fruit_position.y] = 'F';
		fruits = 0;

		looking = STOP; 
	}

	//Draw the world
	void Draw (){
		system("clear");
		//Draw corners
		for (int j = 0 ; j < ROWS+2; j++)
			cout << "#";

		cout << endl;

		for (int i = 0; i < COLUMNS; i++){
			for(int j = -1; j <= ROWS; j++){
				if (j == -1){
					cout << "#";
				}
				else if (j == ROWS) {
					cout << "#";
				}
				else
					cout << field[i][j];

			}
			cout << endl;
		}
		//Drow corners
		for (int j = 0 ; j < ROWS+2; j++)
			cout << "#";
		cout << endl << endl << endl << endl;


		cout << "Fruits: " << fruits << endl;

	}
	void MoveHead(){
		Input();

		if(looking == UP){
			if (InsideField(head_position.x - 1 , head_position.y) == false){
				GAMEOVER = true;
			}else{
				field[head_position.x][head_position.y] = ' ';
				head_position.x--;
				field[head_position.x][head_position.y] = 'O';
			}
		}

		if(looking == DOWN){
			if (InsideField(head_position.x + 1, head_position.y) == false){
				GAMEOVER = true;
			}else{
				field[head_position.x][head_position.y] = ' ';
				head_position.x++;
				field[head_position.x][head_position.y] = 'O';
			}
		}

		if(looking == LEFT){
			if (InsideField(head_position.x, head_position.y - 1) == false){
				GAMEOVER = true;
			}else{
				field[head_position.x][head_position.y] = ' ';
				head_position.y--;
				field[head_position.x][head_position.y] = 'O';
			}
		}

		if(looking == RIGHT){
			if (InsideField(head_position.x, head_position.y + 1) == false){
				GAMEOVER = true;
			}else
			field[head_position.x][head_position.y] = ' ';
			head_position.y++;
			field[head_position.x][head_position.y] = 'O';
		}
	}
	//Check if there is a fruit in a given position
	bool IsThereAFruit(Point position){
		return position.x == fruit_position.x && position.y == fruit_position.y;
	}

	void CreateFruit(){
	fruits++;
	fruit_position.x = rand() % COLUMNS-2 + 2;
	fruit_position.y = rand() % ROWS-2 + 2;
	field[fruit_position.x][fruit_position.y] = 'F';

	}

	void Move(){
		Input();
		MoveHead();
		if (IsThereAFruit(head_position)){
			CreateFruit();
		}

		//Move the snake
		/*
		if (looking == UP){
			field[head_position.x][head_position.y] = 'o';
			head_position.x--;
			field[head_position.x][head_position.y] = 'O';

			if (IsThereAFruit(head_position)){
				fruits++;
				fruit_position.x = rand() % COLUMNS-2 + 2;
				fruit_position.y = rand() % ROWS-2 + 2;
				field[fruit_position.x][fruit_position.y] = 'F';
			}
			
			field[end_tail.x][end_tail.y] = ' ';
			end_tail.x--;
			// Meter toda la funcion dentro de la funcion InsideField 
			if (InsideField(head_position.x, head_position.y) == false)
				GAMEOVER = true;
		}

		if (looking == DOWN){
			field[head_position.x][head_position.y] = 'o';
			head_position.x++;
			field[head_position.x][head_position.y] = 'O';

			if (IsThereAFruit(head_position)){
				end_tail.x--;
				fruits++;
				fruit_position.x = rand() % COLUMNS-2 + 2;
				fruit_position.y = rand() % ROWS-2 + 2;
				field[fruit_position.x][fruit_position.y] = 'F';
			}

			field[end_tail.x][end_tail.y] = ' ';
			end_tail.x++;
			
			if (InsideField(head_position.x, head_position.y) == false)
				GAMEOVER = true;
		}

		if (looking == LEFT){
			field[head_position.x][head_position.y] = 'o';
			head_position.y--;
			field[head_position.x][head_position.y] = 'O';

			if (IsThereAFruit(head_position)){
				end_tail.y++;
				fruits++;
				fruit_position.x = rand() % COLUMNS-2 + 2;
				fruit_position.y = rand() % ROWS-2 + 2;
				field[fruit_position.x][fruit_position.y] = 'F';
			}

			field[end_tail.x][end_tail.y] = ' ';
			end_tail.y--;
			
			if (InsideField(head_position.x, head_position.y) == false)
				GAMEOVER = true;
		}

		if (looking == RIGHT){
			field[head_position.x][head_position.y] = 'o';
			head_position.y++;
			field[head_position.x][head_position.y] = 'O';

			if (IsThereAFruit(head_position)){
				end_tail.y--;
				fruits++;
				fruit_position.x = rand() % COLUMNS-2 + 2;
				fruit_position.y = rand() % ROWS-2 + 2;
				field[fruit_position.x][fruit_position.y] = 'F';
			}

			field[end_tail.x][end_tail.y] = ' ';
			end_tail.y++;
			
			if (InsideField(head_position.x, head_position.y) == false)
				GAMEOVER = true;
		}
		
		*/		
	}

};

int main(){
	srand(time(NULL));
	Snake snake;
	
	while (GAMEOVER == false){
		snake.Draw();
		snake.Move();
		usleep(100000);
	}

	system("clear");
	for(int i = 0; i <= 10; i++)
		cout << endl;
	cout << "\tGAMEOVER!";

	for(int i = 0; i <= 10; i++)
		cout << endl;
	
}