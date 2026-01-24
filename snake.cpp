#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
using namespace std;

void drawStaticGrid(int screenW, int screenH, int cell) {
    setcolor(DARKGRAY);
    for (int gx = 0; gx <= screenW; gx += cell)
        line(gx, 60, gx, screenH - 40); 
    for (int gy = 60; gy <= screenH - 40; gy += cell)
        line(0, gy, screenW, gy);
}

void spawnFood(int &foodx, int &foody, int cell, int screenW, int screenH) {
    int maxCols = (screenW - 20) / cell;  
    int maxRows = (screenH - 100) / cell; 

    int col = rand() % maxCols;
    int row = rand() % maxRows;

    foodx = 10 + col * cell;   
    foody = 60 + row * cell;   
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    setbkcolor(LIGHTGRAY);
    cleardevice();

    int screenW = getmaxx();
    int screenH = getmaxy();
    int cell = 10;

    int x[200], y[200];
    int length = 4;
    int dir = 3;
    int foodx = 0, foody = 0, score = 0;
    bool gameover = false;
    int speed = 100;
    int level = 1;
    int prevLevel = 1;
    int lifeline = 4;

    for (int i = 0; i < length; i++) {
        x[i] = 100 - i * cell;
        y[i] = 100;
    }

    spawnFood(foodx, foody, cell, screenW, screenH);

    setcolor(BLACK);
    rectangle(10, 10, 630, 470);
    settextstyle(DEFAULT_FONT, 0, 2);
    outtextxy(230, 5, "SNAKE GAME");

    while (!gameover) {
        drawStaticGrid(screenW, screenH, cell);

        if (score < 20) level = 1;
        else if (score < 40) level = 2;
        else if (score < 50) level = 3;
        else level = 4;

        if (level != prevLevel) {
            string levelName;
            int nameColor;
            switch (level) {
                case 1: levelName = "Baby Snake"; nameColor = LIGHTBLUE; break;
                case 2: levelName = "King Cobra"; nameColor = GREEN; break;
                case 3: levelName = "Python"; nameColor = BLUE; break;
                case 4: levelName = "Anaconda"; nameColor = MAGENTA; break;
            }

            cleardevice();
            setbkcolor(LIGHTGRAY);
            setcolor(nameColor);
            rectangle(10, 10, 630, 470);

            settextstyle(DEFAULT_FONT, 0, 3);
            char msg[100];
            sprintf(msg, "Level %d - %s", level, levelName.c_str());

            int textWidth = textwidth(msg);
            int textHeight = textheight(msg);
            int xPos = (getmaxx() - textWidth) / 2;
            int yPos = (getmaxy() - textHeight) / 2;
            outtextxy(xPos, yPos, msg);

            delay(2000);

            cleardevice();
            rectangle(10, 10, 630, 470);
            outtextxy(230, 5, "SNAKE GAME");

            prevLevel = level;
        }

        rectangle(10, 10, 630, 470);

        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        bar(foodx, foody, foodx + cell, foody + cell);

        int snakeColor;
        switch (level) {
            case 1: snakeColor = LIGHTBLUE; break;
            case 2: snakeColor = GREEN; break;
            case 3: snakeColor = BLUE; break;
            case 4: snakeColor = MAGENTA; break;
        }

        setcolor(snakeColor);
        setfillstyle(SOLID_FILL, snakeColor);
        for (int i = 0; i < length; i++)
            bar(x[i], y[i], x[i] + cell, y[i] + cell);

        delay(speed);

        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(x[length - 1], y[length - 1], x[length - 1] + cell, y[length - 1] + cell);
        setcolor(DARKGRAY);
        rectangle(x[length - 1], y[length - 1], x[length - 1] + cell, y[length - 1] + cell);

        for (int i = length - 1; i > 0; i--) {
            x[i] = x[i - 1];
            y[i] = y[i - 1];
        }

        if (kbhit()) {
            switch (getch()) {
                case 72: if (dir != 2) dir = 1; break;
                case 80: if (dir != 1) dir = 2; break;
                case 75: if (dir != 3) dir = 4; break;
                case 77: if (dir != 4) dir = 3; break;
            }
        }

        switch (dir) {
            case 1: y[0] -= cell; break;
            case 2: y[0] += cell; break;
            case 3: x[0] += cell; break;
            case 4: x[0] -= cell; break;
        }

        bool wallHit = false;
        if (x[0] < 10) { x[0] = 630 - cell; wallHit = true; }
        else if (x[0] >= 620 - cell) { x[0] = 10; wallHit = true; }
        else if (y[0] <= 60) { y[0] = 450 - cell; wallHit = true; }
        else if (y[0] >= 450 - cell) { y[0] = 60; wallHit = true; }

        if (wallHit) {
            lifeline--;
            if (lifeline <= 0) {
                gameover = true;
            }
        }

        for (int i = 1; i < length; i++)
            if (x[0] == x[i] && y[0] == y[i])
                gameover = true;

        if (abs(x[0] - foodx) < cell && abs(y[0] - foody) < cell) {
            score += 10;
            length++;
            spawnFood(foodx, foody, cell, screenW, screenH); 
            if (speed > 40) speed -= 5; 
        }

        setcolor(BLACK);
        settextstyle(DEFAULT_FONT, 0, 2);
        char s[50], l[30], lifeText[20];
        sprintf(s, "Score: %d", score);
        sprintf(l, "Level: %d", level);
        sprintf(lifeText, "Life: %d", lifeline);
        outtextxy(30, 5, l);
        outtextxy(500, 5, s);
        outtextxy(30, 20, lifeText);
    }

    setcolor(BLACK);
    settextstyle(DEFAULT_FONT, 0, 3);
    outtextxy(230, 200, "GAME OVER!");

    settextstyle(DEFAULT_FONT, 0, 2);
    outtextxy(40, 240, "Press 'R' to Restart or any key to Exit...");

    char ch = getch();
    if (ch == 'r' || ch == 'R') {
        closegraph();
        main();
    } else {
        closegraph();
        return 0;
    }
}