#include <iostream>
#include <unistd.h>
#include <curses.h>

using namespace std;

#define WIDTH 80
#define HEIGHT 24

int player1Y;
int player2Y;

float ballX, ballY, dX, dY;

void init()
{
    player1Y = player2Y = HEIGHT / 2;

    ballY = HEIGHT / 2;
    ballX = WIDTH / 2;

    dX = -1;
    dY = 0;
}

void input()
{
    int input = getch();

    if ((input == 'w' || input == KEY_UP) && player1Y > 2)
    {
        player1Y--;
    }
    else if ((input == 's' || input == KEY_DOWN) && player1Y < (HEIGHT - 1) - 2)
    {
        player1Y++;
    }
    else if (input == 'q')
    {
        endwin();
        exit(0);
    }
}

void draw()
{
    clear();
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH - 1; j++)
        {
            if (j == 0 && player1Y - 2 <= i && i <= player1Y + 2)
            {
                printw("*"); // player 1
            }
            else if (j == WIDTH - 2 && player2Y - 2 <= i && i <= player2Y + 2)
            {
                printw("*"); // player 2
            }
            else if (j == int(ballX) && i == int(ballY))
            {
                printw("*"); // ball
            }
            else
            {
                printw(" "); // empty spaces
            }
        }
        printw("\n");
    }
    refresh();
}

void update()
{
    // move ball
    ballX += dX;
    ballY += dY;

    // move enemy towards ball
    if ((player2Y > ballY + dY && ballY + dY - player2Y < -2) && player2Y > 2)
    {
        player2Y--;
    }
    else if ((player2Y < ballY + dY && ballY + dY - player2Y > 2) && player2Y < (HEIGHT - 1) - 2)
    {
        player2Y++;
    }

    // paddle speed look up table
    double speeds[][2] = {{0.5, -0.5}, {0.75, -0.25}, {1, 0.1}, {0.75, 0.25}, {0.5, 0.5}};

    // paddle collisions
    if (ballX == 0 && player1Y - 2 <= ballY && ballY <= player1Y + 2)
    {
        dX = speeds[int(ballY) - player1Y + 2][0];
        dY = speeds[int(ballY) - player1Y + 2][1];
    }
    if (ballX == WIDTH - 2 && player2Y - 2 <= ballY && ballY <= player2Y + 2)
    {
        dX = speeds[int(ballY) - player2Y + 2][0] * -1;
        dY = speeds[int(ballY) - player2Y + 2][1];
    }

    // wall collisions
    if (ballX <= -2 || ballX >= WIDTH + 1)
    {
        sleep(2);
        init();
    }
    if (ballY <= 0 || ballY >= HEIGHT)
    {
        dY *= -1;
    }
}

void setup()
{
    initscr(); // initialize curses
    cbreak();
    noecho();
    curs_set(0);           // hide cursor
    nodelay(stdscr, true); // set getch() to non-blocking mode
    keypad(stdscr, TRUE);  // enable function keys such as arrow keys
}

int main()
{
    setup();
    init();

    while (true)
    {
        input();
        draw();
        update();
        usleep(50000);
    }

    endwin();
    return 0;
}
