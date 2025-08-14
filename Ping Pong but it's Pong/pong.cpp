#include <iostream>
#include <time.h>
#include <conio.h>
#include <Windows.h>

using namespace std;

// Directions for ball and paddles
enum eDirection{STOP, RIGHT, UPRIGHT, DOWNRIGHT, LEFT, UPLEFT, DOWNLEFT};
enum eController{PAUSE, UP, DOWN};

// Pong ball
class cBall
{
private:
	int x, y;
	int originalX, originalY;
	eDirection dir;

public:
	// Set starting position & direction of the ball
	cBall(int posX, int posY)
	{
		originalX = posX;
		originalY = posY;
		x = posX; y = posY;
		dir = STOP;
	}
	// Reset position & direction of the ball to the starting position
	void reset()
	{
		x = originalX;
		y = originalY;
		dir = STOP;
	}
	// Change direction of the ball
	void changeDirection(eDirection direction)
	{
		dir = direction;
	}
	// Set a random direction to the ball
	void randDirection()
	{
		srand(time(0));
		dir = (eDirection)((rand() % 6) + 1);
	}
	// Get current x and y coordinates of the ball
	inline int getX() { return x; }
	inline int getY() { return y; }
	// Get the current direction of the ball
	inline int getDir() { return dir; }
	// Move the ball according to the direction
	void move()
	{
		switch (dir)
		{
		case STOP:
			break;
		case RIGHT:
			x++;
			break;
		case UPRIGHT:
			x++; y--;
			break;
		case DOWNRIGHT:
			x++; y++;
			break;
		case LEFT:
			x--;
			break;
		case UPLEFT:
			x--; y--;
			break;
		case DOWNLEFT:
			x--; y++;
			break;
		default:
			break;
		}
	}
};

// Paddles
class cPaddle
{
private:
	int x, y;
	int originalX, originalY;
	eController joyStick;
public:
	// Set the position and direction of paddles to the starting position
	cPaddle(int posX, int posY)
	{
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
		joyStick = PAUSE;
	}
	// Reset the position and direction of the paddles to the starting position
	void reset()
	{
		x = originalX;
		y = originalY;
		joyStick = PAUSE;
	}
	// Get current x and y coordinates of the paddles
	inline int getX() { return x; }
	inline int getY() { return y; }
	// Get current direction of paddles
	void changeDirection(eController dir) { joyStick = dir; }
	void move()
	{
		switch (joyStick)
		{
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		case PAUSE:
			break;
		default:
			break;
		}
	}
};

// Game manager
class cGameManager
{
private:
	int width, height;
	int score1, score2;
	bool quit;
	// Initializing the ball, player1 & player2
	cBall* ball;
	cPaddle* player1;
	cPaddle *player2;
public:
	// Set the height, widht of the box and starting positions
	cGameManager()
	{
		quit = false;
		height = 15; width = 50;
		score1 = score2 = 0;
		ball = new cBall(width / 2, height / 2);
		player1 = new cPaddle(1, height / 2);
		player2 = new cPaddle(width - 2, height / 2);
	}
	// Delete created class objects
	~cGameManager()
	{
		delete ball, player1, player2;
	}
	// Increase score then reset to starting position
	void scoreUp(cPaddle* player)
	{
		if (player == player1)
			score1++;
		else if (player == player2)
			score2++;

		ball->reset();
		player1->reset();
		player2->reset();
	}
	// Draw the layout of the box
	void draw()
	{
		// Stop the screen from blinking
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });

		// Top wall
		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";
		cout << endl;

		// Right and left walls
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int ballX = ball->getX();
				int ballY = ball->getY();
				int player1x = player1->getX();
				int player1y = player1->getY();
				int player2x = player2->getX();
				int player2y = player2->getY();

				if (j == 0)
					cout << "\xB1";

				// Ball, right and left paddle
				if (ballX == j && ballY == i)
					cout << "o";
				else if (player1x == j && (player1y >= i && player1y < i + 4))
					cout << "\xDB";
				else if (player2x == j && (player2y >= i && player2y < i + 4))
					cout << "\xDB";
				else
					cout << " ";

				if (j == width - 1)
					cout << "\xB1";
			}
			cout << endl;
		}
		// Bottom wall
		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";
		cout << endl;

		// Score
		for (int i = 0; i < width / 2 - 3; i++)
			cout << " ";
		cout << score1 << " : " << score2 << endl;
	}
	// Control the paddles
	void input()
	{
		int player1y = player1->getY();
		int player2y = player2->getY();

		if (_kbhit())
		{
			// Press any key to start the game
			if (ball->getDir() == STOP)
				ball->randDirection();

			switch (_getch())
			{
			case 'w':
				if (player1y - 3 > 0) // if Paddle not hit top wall
					player1->changeDirection(UP);
				break;
			case 's':
				if (player1y + 1 < height) // if Paddle not hit bottom wall
					player1->changeDirection(DOWN);
				break;
			case 'i':
				if (player2y - 3 > 0) // if Paddle not hit top wall
					player2->changeDirection(UP);
				break;
			case 'j':
				if (player2y + 1 < height) // if Paddle not hit bottom wall
					player2->changeDirection(DOWN);
				break;
			case 'q':
				quit = true;
				break;
			default:
				break;
			}
		}
	}
	// Logic of the game
	void logic()
	{
		// Moving ball and paddles
		ball->move();
		player1->move();
		player2->move();

		// Getting the position of ball and paddles
		int ballX = ball->getX();
		int ballY = ball->getY();
		int player1x = player1->getX();
		int player1y = player1->getY();
		int player2x = player2->getX();
		int player2y = player2->getY();

		// Reflecting direction of ball when it hits the paddle
		if (ballX == player1x + 1)
		{
			if (ballY == player1y - 3)
				ball->changeDirection(UPRIGHT);
			else if (ballY == player1y - 2 || ballY == player1y - 1)
				ball->changeDirection((eDirection)(rand() % 3 + 1)); // Random right directions
			else if (ballY == player1y)
				ball->changeDirection(DOWNRIGHT);
		}
		else if (ballX == player2x - 1)
		{
			if (ballY == player2y - 3)
				ball->changeDirection(UPLEFT);
			else if (ballY == player2y - 2 || ballY == player2y - 1)
				ball->changeDirection((eDirection)(rand() % 3 + 4)); // Random left direction
			else if (ballY == player2y)
				ball->changeDirection(DOWNLEFT);
		}
		// Reflecting ball direction of ball if hits the top or bottom wall
		if (ballY == height - 1)
			ball->changeDirection(ball->getDir() == DOWNRIGHT ? UPRIGHT : UPLEFT);
		else if (ballY == 0)
			ball->changeDirection(ball->getDir() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
		else if (ballX == width - 1)
			scoreUp(player1);
		else if (ballX == 0)
			scoreUp(player2);

		// Stop moving paddle if any paddle hits the top or bottom wall
		if (player1y - 4 < 0)
			player1->changeDirection(PAUSE);
		else if (player1y + 2 < height)
			player1->changeDirection(PAUSE);

		if (player2y - 4 < 0)
			player2->changeDirection(PAUSE);
		else if (player2y + 2 < height)
			player2->changeDirection(PAUSE);

		// Game over if any player scored 10 points
		if (score1 == 10)
			quit = true;
		else if (score2 == 10)
			quit = true;
	}
	// Setup the game
	void game()
	{
		string p1, p2;
		cout << "Player1: ";
		cin >> p1;
		cout << "Player2: ";
		cin >> p2;

		while (!quit)
		{
			draw();
			input();
			logic();
			Sleep(50);
		}
		if (score1 != score2)
			score1 > score2 ? cout << p1 << " Won" : cout << p2 << " Won";
		else
			cout << "Tie";
	}
};

int main()
{
	cGameManager c;
	c.game();
	return 0;
}