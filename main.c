#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <wchar.h>

const int WIDTH = 40;
const int HEIGHT = WIDTH / 2;
const char SNAKE_CHAR = '#';
const char FRUIT_CHAR = '@';
const int SNAKE_COLOR = 10;
const int WALL_COLOR = 255;
const int FRUIT_COLOR = 12;
const int STD_COLOR = 7;
const int X_OFFSET = 4;
const int Y_OFFSET = 2;
const int DELAY = 100;
const int DELAY_INCREMENT = DELAY / 2;
const int TAIL = 0;

int decide(char a);
void init();
void showFruit();
void showScore();
void createWalls();
void toggleCursor();
void gotoxy(int x, int y);
void changeColor(int color);
void moveonestep();
void check(int x, int y);
void gameOver();
void terminate();
void adjust(int x, int y);
void delay(int millis);

int head,fruitX,fruitY,score;
char direction;

struct point {
    int x;
    int y;
};

struct point snake[800];

int main(){
    init();
    toggleCursor(FALSE);
    system("cls");
    createWalls();
    showFruit();
    showScore();
    gotoxy(snake[0].x,snake[0].y);

    for (;;){
        if(_kbhit() && decide(_getch()) == 0)
               continue;
        moveonestep();
    }
}

void move(int x,int y){
    check(x,y);
    if (x == fruitX && y == fruitY){
        head++;
        snake[head] = (struct point){x,y};
        ++score;
        Beep(500,10);
        showScore();
        showFruit();
    } else {
        gotoxy(snake[TAIL].x,snake[TAIL].y);
        changeColor(10);
        printf(" ");
        adjust(x,y);
    }

    gotoxy(x,y);
    changeColor(SNAKE_COLOR);
    printf("%c",SNAKE_CHAR);
}

void check(int x, int y) {
    if (x == WIDTH + X_OFFSET-1 || x < X_OFFSET+1 || y ==HEIGHT+Y_OFFSET-1 || y < Y_OFFSET+1)
        gameOver();

    for (int i = 0; i < head; ++i)
        if (snake[i].x == x && snake[i].y == y)
            gameOver();
}

void adjust(int x, int y) {
    int index = 0;
    while (index < head)
        snake[index++] = snake[index];
    snake[head] = (struct point) {x,y};
}

void moveonestep() {
    int delayMillis = DELAY;
    switch(direction){
        case 'u':
            move(snake[head].x,snake[head].y - 1);
            delayMillis += DELAY_INCREMENT;
        break;
        case 'd':
            move(snake[head].x,snake[head].y + 1);
            delayMillis += DELAY_INCREMENT;
        break;
        case 'l':
            move(snake[head].x - 1,snake[head].y);
        break;
        case 'r':
            move(snake[head].x + 1, snake[head].y);
        break;
    }
    delay(delayMillis);
}

int decide(char keyPress) {
    switch(keyPress) {
        case 72:    // key up
            if (direction != 'd'){
                direction = 'u';
                return 1;
            }
        break;
        case 80:    // key down
            if (direction != 'u'){
                direction = 'd';
                return 1;
            }
        break;
        case 77:    // key right
            if (direction != 'l'){
                direction = 'r';
                return 1;
            }
        break;
        case 75:    // key left
            if (direction != 'r'){
                direction = 'l';
                return 1;
            }
        break;
        case 27:
        case 'q':
            terminate();
    }
    return 0;
}

void init() {
    head = 0;
    direction = 'x';
    fruitX = 0;
    fruitY = 0;
    score = 0;
    snake[0] = (struct point) {WIDTH / 2 + X_OFFSET,HEIGHT / 2 + Y_OFFSET};
    srand(time(NULL));
}

void showFruit() {
    generate:
    fruitX = (rand() % (WIDTH - 2)) + X_OFFSET + 1;
    fruitY = (rand() % (HEIGHT - 2)) + Y_OFFSET + 1;
    for (int i = 0; i < head; ++i)
        if (snake[i].x == fruitX && snake[i].y == fruitY)
            goto generate;

    gotoxy(fruitX,fruitY);
    changeColor(FRUIT_COLOR);
    printf("%c",FRUIT_CHAR);
}

void showScore() {
    gotoxy(60,10);
    changeColor(STD_COLOR);
    printf("SCORE: %d",score);
}

void gameOver() {
    gotoxy(60,12);
    changeColor(STD_COLOR);
    printf("Game over!");
    Beep(400,200);
    Beep(200,500);
    gotoxy(60,14);
    printf("Again? (y/n)");
    if (getch() == 'y')
        main();
    terminate();
}

void terminate() {
    changeColor(STD_COLOR);
    system("cls");
    toggleCursor(TRUE);
    exit(0);
}

void createWalls() {
    for (int i = Y_OFFSET; i < HEIGHT+Y_OFFSET; ++i) {
        for (int j = X_OFFSET; j < WIDTH + X_OFFSET; ++j ){
            if (i == Y_OFFSET || i == HEIGHT+Y_OFFSET - 1 || j == X_OFFSET || j == WIDTH+X_OFFSET -1) {
                gotoxy(j,i);
                changeColor(WALL_COLOR);
                printf("%c",' ');
            }
        }
   }
}

void delay(int millis){
    clock_t start_time = clock();
    while (clock() < start_time + millis);
}

void toggleCursor(BOOL b) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = b;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoxy(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void changeColor(int color){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}




