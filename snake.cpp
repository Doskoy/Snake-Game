/////////////////////////////////////////////////////
// 
//Author: Fernando Roldán Zafra :D
//
/////////////////////////////////////////////////////

/*
_kbhit function
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
	static const int ROWS = 20;
	static const int COLUMNS = 40;
	char field[ROWS][COLUMNS];
	Directions looking; 
	Point head_position;
	Point fruit_position;
	bool eat_fruit = false;

	Point end_tail;
	int fruits;

	bool InsideField (int x, int y){
		return 0 <= x && x < ROWS && 0 <= y && y < COLUMNS;
	}

	//change the direction 
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
		for (int i = 0; i < ROWS; i++)
			for (int j = 0; j < COLUMNS; j++)
				field[i][j] = ' ';
		
		head_position.x = ROWS/2;		
		head_position.y = COLUMNS/2;
		fruit_position.x = rand() % ROWS-2 + 2;
		fruit_position.y = rand() % COLUMNS-2 + 2;

		end_tail = head_position;

		field[head_position.x][head_position.y] = 'O';
		field[fruit_position.x][fruit_position.y] = 'F';
		fruits = 0;

		looking = STOP; 
	}

	//Draw the world
	void Draw (){
		system("clear");
		for (int j = 0 ; j < COLUMNS+2; j++)
			cout << "#";

		cout << endl;

		for (int i = 0; i < ROWS; i++){
			for(int j = -1; j <= COLUMNS; j++){
				if (j == -1){
					cout << "#";
				}
				else if (j == COLUMNS) {
					cout << "#";
				}
				else
					cout << field[i][j];

			}
			cout << endl;
		}

		for (int j = 0 ; j < COLUMNS+2; j++)
			cout << "#";
		cout << endl << endl << endl << endl;


		cout << "Fruits: " << fruits << endl;

	}

	void Move(){
		Input();

		//Move the snake
		if (looking == UP){
			field[head_position.x][head_position.y] = 'o';
			head_position.x--;
			cout << head_position.x << endl;
			field[head_position.x][head_position.y] = 'O';

			if (head_position.x == fruit_position.x && head_position.y == fruit_position.y){
				end_tail.x++;
				fruits++;
				fruit_position.x = rand() % ROWS-2 + 2;
				fruit_position.y = rand() % COLUMNS-2 + 2;
				field[fruit_position.x][fruit_position.y] = 'F';
			}
			
			field[end_tail.x][end_tail.y] = ' ';
			end_tail.x--;
			// MEter toda la funcion dentro de la funcion esta de abajo. 
			if (InsideField(head_position.x, head_position.y) == false)
				GAMEOVER = true;
		}

		if (looking == DOWN){
			field[head_position.x][head_position.y] = 'o';
			head_position.x++;
			field[head_position.x][head_position.y] = 'O';

			if (head_position.x == fruit_position.x && head_position.y == fruit_position.y){
				end_tail.x--;
				fruits++;
				fruit_position.x = rand() % ROWS-2 + 2;
				fruit_position.y = rand() % COLUMNS-2 + 2;
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

			if (head_position.x == fruit_position.x && head_position.y == fruit_position.y){
				end_tail.y++;
				fruits++;
				fruit_position.x = rand() % ROWS-2 + 2;
				fruit_position.y = rand() % COLUMNS-2 + 2;
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

			if (head_position.x == fruit_position.x && head_position.y == fruit_position.y){
				end_tail.y--;
				fruits++;
				fruit_position.x = rand() % ROWS-2 + 2;
				fruit_position.y = rand() % COLUMNS-2 + 2;
				field[fruit_position.x][fruit_position.y] = 'F';
			}

			field[end_tail.x][end_tail.y] = ' ';
			end_tail.y++;
			
			if (InsideField(head_position.x, head_position.y) == false)
				GAMEOVER = true;
		}
		
		//Eat the fruit
				
	}

};

int main(){
	srand(time(NULL));
	Snake snake;
	
	while (GAMEOVER == false){
		snake.Draw();
		snake.Move();
		usleep(200000);
	}
	
}