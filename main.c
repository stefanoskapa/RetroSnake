#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

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
void hidecursor();
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
int X[800];
int Y[800];

int main(){

    init();
    hidecursor();
    system("cls");
    createWalls();
    showFruit();
    showScore();
    gotoxy(X[0],Y[0]);

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
        X[head] = x;
        Y[head] = y;
        score++;
        showScore();
        showFruit();
    } else {
        gotoxy(X[TAIL],Y[TAIL]);
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

    for (int i = 0; i < head; i++)
        if (X[i] == x && Y[i] == y)
            gameOver();
}

void adjust(int x, int y) {
    int index = 0;
    while (index < head) {
        X[index] = X[index+1];
        Y[index] = Y[index+1];
        index++;
    }
    X[head] = x;
    Y[head] = y;
}

void moveonestep() {
    int delayMillis = DELAY;
    switch(direction){
        case 'u':
            move(X[head],Y[head]-1);
            delayMillis += DELAY_INCREMENT;
        break;
        case 'd':
            move(X[head],Y[head]+1);
            delayMillis += DELAY_INCREMENT;
        break;
        case 'l':
            move(X[head]-1,Y[head]);
        break;
        case 'r':
            move(X[head]+1, Y[head]);
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
    direction='x';
    fruitX = 0;
    fruitY = 0;
    score = 0;
    X[0]= WIDTH /2 + X_OFFSET;
    Y[0]= HEIGHT /2 + Y_OFFSET;
    srand(time(NULL));
}

void showFruit() {
    generate:
    fruitX = (rand() % (WIDTH - 2)) + X_OFFSET + 1;
    fruitY = (rand() % (HEIGHT - 2)) + Y_OFFSET + 1;
    for (int i = 0; i < head; i++)
        if (X[i] == fruitX && Y[i] == fruitY)
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
    gotoxy(60,14);
    printf("Again? (y)");
    if (getch() == 'y')
        main();

    terminate();
}

void terminate() {
    changeColor(STD_COLOR);
    system("cls");
    exit(0);
}

void createWalls() {
    for (int i = Y_OFFSET; i < HEIGHT+Y_OFFSET; i++) {
        for (int j = X_OFFSET; j < WIDTH + X_OFFSET; j++ ){
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

void hidecursor(){
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
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


