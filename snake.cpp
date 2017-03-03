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
#include <vector>
using namespace std;

static bool GAMEOVER = false;
enum Directions{STOP, UP, DOWN, LEFT, RIGHT};

struct Point{
	int x;
	int y;
};
/*
Mostly this class is a vector where you store the positions of the tails
*/
class Tail{
private:
	static const int SIZE = 100;
	Point private_vector[SIZE];
	int total_used;

public:
	Tail():total_used(0){}

	int GetTotal_used(){
		return total_used;
	}

	int SetTotal_used(int new_total_used){
		total_used = new_total_used;
	}

	int Capacity(){
		return SIZE;
	}

	void DeleteTail(){
		total_used = 0;
	}

	void Add(Point new_point){
		if(total_used < SIZE){
			private_vector[total_used] = new_point;
			total_used++;
		}
	}

	void ChangeValue(int position, Point new_value){
		if(position >= 0 && position < total_used)
			private_vector[position] = new_value;
	}

	Point Element(int index){
		return private_vector[index];
	}

	void Insert(int position, Point new_value){
      if (total_used < SIZE  &&  position >= 0    
         &&  position <= total_used){

         for (int i = total_used ; i > position ; i--)
            private_vector[i] = private_vector[i-1];

         private_vector[position] = new_value;
         total_used++;
      }
   }

};

class Snake{
private:
	//ROWS and COLUMNS are the size of the field
	static const int ROWS = 30;
	static const int COLUMNS = 15;
	char field[COLUMNS][ROWS];
	Directions looking; 
	Point head_position;
	Point fruit_position;
	bool eat_fruit = false;
	Tail tail; 

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
		for (int i = 0; i < 3; ++i)
		{
			tail.Add(head_position);
		}
		

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

		cout << "Fruit position x-> " << fruit_position.x << " y-> " << fruit_position.y << endl;

		cout << "Fruits: " << fruits << endl;
		cout << "Use the numeric keys for move: 8 -> UP | 4 -> LEFT | 6 -> RIGHT | 2 -> DOWN \n";

	}

	void MoveHead(){
		Input();

		if(looking == UP){
			if (InsideField(head_position.x - 1 , head_position.y) == false){
				GAMEOVER = true;
			}else{
				if(field[head_position.x - 1][head_position.y] == 'o')
				GAMEOVER = true;
				head_position.x--;
			}
		}

		if(looking == DOWN){
			if (InsideField(head_position.x + 1, head_position.y) == false){
				GAMEOVER = true;
			}else{
				if(field[head_position.x+1][head_position.y] == 'o')
				GAMEOVER = true;
				head_position.x++;
			}
		}

		if(looking == LEFT){
			if (InsideField(head_position.x, head_position.y - 1) == false){
				GAMEOVER = true;
			}else{
				if(field[head_position.x][head_position.y-1] == 'o')
				GAMEOVER = true;
				head_position.y--;
			}
		}

		if(looking == RIGHT){
			if (InsideField(head_position.x, head_position.y + 1) == false){
				GAMEOVER = true;
			}else
			if(field[head_position.x][head_position.y+1] == 'o')
				GAMEOVER = true;
			head_position.y++;
		}
	}
 	//Move the tail positions through the vector
	void MoveTail(){
		tail.Insert(0, head_position);
		tail.SetTotal_used(tail.GetTotal_used() - 1);
		for(int i = 0; i < tail.GetTotal_used() -1 ; i++){
			field[tail.Element(i).x][tail.Element(i).y] = 'o';
		}
		field[tail.Element(tail.GetTotal_used() -1 ).x][tail.Element(tail.GetTotal_used() -1 ).y] = ' ';
		field[head_position.x][head_position.y] = 'O';
	}


	//Check if there is a fruit in a given position
	bool IsThereAFruit(Point position){
		return position.x == fruit_position.x && position.y == fruit_position.y;
	}

	void CreateFruit(){
	fruits++;
	fruit_position.x = rand() % (COLUMNS-2) + 2;
	fruit_position.y = rand() % (ROWS-2) + 2;
	field[fruit_position.x][fruit_position.y] = 'F';

	}

	void Move(){
		Input();
		MoveHead();
		if (IsThereAFruit(head_position)){
			tail.SetTotal_used(tail.GetTotal_used() + 1);
			CreateFruit();
		}
		MoveTail();
	}

};


int main(){
	srand(time(NULL));
	Snake snake;
	
	while (GAMEOVER == false){
		snake.Draw();
		snake.Move();
		//Speed of the snake
		usleep(100000);
	}

	system("clear");
	for(int i = 0; i <= 10; i++)
		cout << endl;
	cout << "\tGAMEOVER!";

	for(int i = 0; i <= 10; i++)
		cout << endl;
	
}