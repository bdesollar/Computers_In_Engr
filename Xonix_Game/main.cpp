////////////////////////////////////////////////////////////////////////
//
// Name: Ben DeSollar
// Date: 03/14/2021
// Filename: main.cpp
// Description: Creates and runs a usable Xonix game for the user
//
////////////////////////////////////////////////////////////////////////


#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdio>
using namespace std;

using namespace sf;

const int rowNum = 25;
const int colNum = 40;

// A value of zero means a blank square
int grid[rowNum][colNum] = {0};
int titleSize = 18;

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class: Creates and sets the coordinates for the "Enemies" position on the window
// Inputs: None
// Outputs: None
//
//////////////////////////////////////////////////////////////////////////////////////////////////

class Enemy {
public:

    Enemy() {
        // Setting location of spinner
        x = y = 300;
        // sets the enemies velocity to a number between 4 and -3
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }


    //////////////////////////////////////
    //
    // Function: returns the x value
    // Inputs: None
    // Outputs: x - int - x coordinate
    //
    ///////////////////////////////////////

    int getX() const{
        return x;
    }

    /////////////////////////////////////////////////////////////////////////
    //
    // Function: Sets the x value
    // Inputs: x1 - int - the value that the x coordinate will be set to
    // Outputs: None
    //
    ////////////////////////////////////////////////////////////////////////////

    void setX(int x1){
        if(x1 >= 0){
            x = x1;
        }
        else{
            x = 0;
        }
    }

    int getY() const{
        return y;
    }

    void setY(int y1){
        if(y1 >= 0){
            y = y1;
        }
        else{
            y = 0;
        }
    }

    int getDy() const{
        return dy;
    }

    void setDy(int dy1){
        if(dy1 > 1 || dy1 < -1){
            dy = 0;
        }
        else{
            dy = dy1;
        }
    }

    int getDx() const{
        return dx;
    }

    void setDx(int dx1){
        if(dx1 > 1 || dx1 < -1){
            dx = 0;
        }
        else{
            dx = dx1;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////
    //
    // Function: Moves teh enemies location to different coordinates on the window
    // Inputs: None
    // Outputs: None
    //
    ///////////////////////////////////////////////////////////////////////////////////

    void move(Sound & sound1) {
        // Reflects the x coordinate
        x += dx;
        // Reflects the motion if it hits a blue tile
        if (grid[y / titleSize][x / titleSize] == 1) {
            sound1.play();
            dx = -dx;
            x += dx;
        }
        y += dy;
        if (grid[y / titleSize][x / titleSize] == 1) {
            sound1.play();
            dy = -dy;
            y += dy;
        }
    }

private:
    // Location of th enemy
    int x, y;
    // Speed of the spinner
    int dx, dy;
};

///////////////////////////////////////////////////////////////////////////////////
//
// Function: Figure out if the spinner is enclosed in a rectangular region
//      Used to decide whether or not to fill in the enclosed square
//      Recursive function
// Inputs: y - int - y coordinate, x - int - x coordinate
// Outputs: None
//
//////////////////////////////////////////////////////////////////////////////////

void drop(int y, int x) {
    // Base case
    if (grid[y][x] == 0) {
        // If the grid was empty fill with -1 which is no color
        grid[y][x] = -1;
    }
    // Breaks into a simpler problem and calls drop to solve these problems.
    if (grid[y - 1][x] == 0) {
        // Checks if north neighbor is 0
        drop(y - 1, x);
    }
    // Call itself to continue enclosing the rectangle
    if (grid[y + 1][x] == 0) {
        // Checks if the south neighbor is zero, if so set to -1
        drop(y + 1, x);
    }
    if (grid[y][x - 1] == 0) {
        // Checks if the west neighbor is zero, if so set to -1
        drop(y, x - 1);
    }
    if (grid[y][x + 1] == 0) {
        // Checks if the east neighbor is zero, if so set to -1
        drop(y, x + 1);
    }
}

int main() {
    srand(time(0));

    RenderWindow window(VideoMode(colNum * titleSize, rowNum * titleSize), "Xonix Game!");
    window.setFramerateLimit(60); // Max frame rate is 60 frame/sec

    Texture t1, t2, t3;
    // Checks if the file is opened
    if(!t1.loadFromFile("images/tiles.png")){
        return EXIT_FAILURE;
    }
    if(!t2.loadFromFile("images/gameover.png")){
        return EXIT_FAILURE;
    }
    if(!t3.loadFromFile("images/enemy.png")){
        return EXIT_FAILURE;
    }

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    SoundBuffer buffer;

    // Arcade by rozi, https://www.1001freefonts.com/, License: Free for personal use.
    if(!buffer.loadFromFile("sounds/sound.ogg")){
        return EXIT_FAILURE;
    }

    Sound sound;
    sound.setBuffer(buffer);

    // Photon Gun Shot by free-sound-effects-science-fiction , https://www.audiomicro.com/, License: Free for personal use.
    Font font;
    if (!font.loadFromFile("fonts/Arcade.ttf"))
    {
        return EXIT_FAILURE;
    }

    Text text;

// select the font
    text.setFont(font); // font is a sf::Font

// set the string to display
    float score = 0;
    char scoreString[100];
    sprintf(scoreString, "Score = %.1g", score);
    text.setString(scoreString);

// set the character size
    text.setCharacterSize(24); // in pixels, not points!

// set the color
    text.setFillColor(Color::Red);

// set the text style
    text.setStyle(Text::Bold);

    text.setPosition(15,425);

// inside the main loop, between window.clear() and window.display()

    int enemyCount = 4;
    Enemy a[10];

    bool Game = true;

    int x = 0, y = 0;
    // Delta x and y for the player motion
    int dx = 0, dy = 0;
    float timer = 0, delay = 0.07;
    Clock clock; // Used to control the game time

    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            if (i == 0 || j == 0 || i == rowNum - 1 || j == colNum - 1) {
                // Fill in the border
                grid[i][j] = 1;
            }
        }
    }

    // The player will have three states to choose from and the program will then use the state machines to decide what to do

    enum playerAction {intro_game, start_game};
    playerAction playerResponse;
    playerResponse = intro_game;
    Text welcomeText;

    welcomeText.setFont(font);

    welcomeText.setString("Welcome to Xonix!");

    welcomeText.setCharacterSize(32);

    welcomeText.setFillColor(Color::Red);

    welcomeText.setStyle(Text::Bold);

    welcomeText.setPosition(200,100);

    Text startGameText;

    startGameText.setFont(font); // font is a sf::Font

    startGameText.setString("To play the game you will use the arrow\n"
                            "keys to direct the red square on the screen\n"
                            "Once you have left the blue squares your\n"
                            "red square will turn green and create a path\n"
                            "behind you that will enclose with blue squares\n"
                            "once your lead square reaches the blue squares\n"
                            "again. If you hit one of the enemy objects during\n"
                            "this time however, you will lose the game. The\n"
                            "umber of seconds you last while enclosing\n"
                            "the area and not getting hit by the enemies \n"
                            "is your score. Good luck! Press Enter to Begin.\n");

    startGameText.setCharacterSize(24);

    startGameText.setFillColor(Color::Red);

    startGameText.setStyle(Text::Bold);

    startGameText.setPosition(75,150);


    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        score += time;
        clock.restart();
        timer += time;
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape) {
                    for (int i = 1; i < rowNum - 1; i++) {
                        for (int j = 1; j < colNum - 1; j++) {
                            grid[i][j] = 0;
                        }
                    }
                    // Moves red block to x position of 10
                    x = 10;
                    y = 0;
                    Game = true;
                }
            }
        }
        // Used for control of the arrow keys and actions
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            dx = -1;
            dy = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            dx = 1;
            dy = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            dx = 0;
            dy = -1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            dx = 0;
            dy = 1;
        }

        if (Game) {
            window.clear();
            // Check if the player has started the game yet
            if(playerResponse == intro_game){
                // If the player has yet to hit enter than the initial phase of the game is started
                window.draw(welcomeText);
                window.draw(startGameText);
                // Displays the given text to the window
                window.display();
            }
            while(playerResponse == intro_game) {
                // If the user presses enter than the loop exits and the game starts
                if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                    playerResponse = start_game;
                }

            }
            // If the timer is greater than the delay then the player can move
            if (timer > delay) {
                x += dx;
                y += dy;

                // Keeps the player inside the grid on the left
                if (x < 0) {
                    x = 0;
                }
                // Keeps the player inside the grid on the right
                if (x > colNum - 1) {
                    x = colNum - 1;
                }
                // Keeps the player inside the grid on the top
                if (y < 0) {
                    y = 0;
                }
                // Keeps the player inside the grid on the bottom
                if (y > rowNum - 1) {
                    y = rowNum - 1;
                }
                // If you go back on yourself the game is lost
                if (grid[y][x] == 2) {
                    Game = false;
                }
                if (grid[y][x] == 0) {
                    grid[y][x] = 2;
                }
                timer = 0;
            }

            for (int i = 0; i < enemyCount; i++) {
                // Adds the enemies in
                a[i].move(sound);
            }

            if (grid[y][x] == 1) {
                dx = dy = 0;

                // Fill in grid squares that contain enemies
                for (int i = 0; i < enemyCount; i++) {
                    // Call drop at the a,y coordinate of each enemy
                    drop(a[i].getY() / titleSize, a[i].getX() / titleSize);
                }

                //
                for (int i = 0; i < rowNum; i++) {
                    for (int j = 0; j < colNum; j++) {
                        // If grid is -1 then it sets the spot back to zero
                        if (grid[i][j] == -1) {
                            grid[i][j] = 0;
                        } else {
                            // If grid was zero, set grid location to blue
                            grid[i][j] = 1;
                        }
                    }
                }
            }

            for (int i = 0; i < enemyCount; i++) {
                // If the enemy hits a green tile the game is over
                if (grid[a[i].getY() / titleSize][a[i].getX() / titleSize] == 2) {
                    Game = false;
                }
            }

            for (int i = 0; i < rowNum; i++) {
                for (int j = 0; j < colNum; j++) {
                    if (grid[i][j] != 0) {
                        if (grid[i][j] == 1) {
                            // Grabs the blue tile
                            sTile.setTextureRect(IntRect(0, 0, titleSize, titleSize));
                        }
                        if (grid[i][j] == 2) {
                            // Grabs the green tile
                            sTile.setTextureRect(IntRect(54, 0, titleSize, titleSize));
                        }
                        sTile.setPosition(j * titleSize, i * titleSize);
                        window.draw(sTile);
                    }
                }
            }

            // Grabs the red tile
            sTile.setTextureRect(IntRect(36, 0, titleSize, titleSize));
            // Sets the position of the red tile
            sTile.setPosition(x * titleSize, y * titleSize);
            // Draws the red tile
            window.draw(sTile);
            sprintf(scoreString, "Score = %g", score);
            text.setString(scoreString);
            window.draw(text);

            sEnemy.rotate(10);
            for (int i = 0; i < enemyCount; i++) {
                sEnemy.setPosition(a[i].getX(), a[i].getY());
                window.draw(sEnemy);
            }

            if (!Game) {
                window.draw(sGameover);
                score = 0;
            }

            window.display();
        }
    }
    return 0;
}