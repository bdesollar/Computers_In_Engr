// Programmer: Ben DeSollar
// Date: 4/28/21
// Filename: main.cpp
// Description: The program will play the asteroid game in which the player can move a ship around trying to dodge and shoot asteroids.
//      The player will also try to avoid and shit a UFO that will appear throughout the game.
//      Added Features: I added multiple features to the game. These include lives, which give the player only three lives to get as high of a score as they can.
//      I also added a score to the game which adds a point if the player destroys an asteroid and subtracts a point if the player is hit.
//      There is also a gameover image that is displayed when the player is out of lives. There is now sounds of explosions as well when an asteroid
//      or UFO is hit by the player or a bullet. I also reformatted the code to get rid of the
//      continue statements and have proper indentation and curly brackets. Hopefully this is enough for the extra credit :)



#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <cmath>
#include <time.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdio>
using namespace std;


using namespace sf;

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;

class Animation {
public:
    float Frame, speed;
    Sprite sprite;
    std::vector<IntRect> frames;

    Animation() {}

    Animation(Texture &t, int x, int y, int w, int h, int count, float Speed) {
        Frame = 0;
        speed = Speed;

        for (int i = 0; i < count; i++) {
            frames.push_back(IntRect(x + i * w, y, w, h));
        }
        sprite.setTexture(t);
        sprite.setOrigin(w / 2, h / 2);
        sprite.setTextureRect(frames[0]);
    }


    void update() {
        Frame += speed;
        int n = frames.size();
        if (Frame >= n) {
            Frame -= n;
        }
        if (n > 0) {
            sprite.setTextureRect(frames[int(Frame)]);
        }
    }

    bool isEnd() {
        return Frame + speed >= frames.size();
    }

};


class Entity {
public:
    float x, y; // x, y location of the entity
    float dx, dy; // speed in the x and y directions of the entity
    float R; // collision radius of the entity
    float angle; // rotation of entity
    bool life; // if false, remove from the entity list
    std::string name; // the name of the derived class
    Animation anim; // object for displaying the animation

    Entity() {
        life = true; // set the entity to be drawn on the screen
    }

    void settings(Animation &a, int X, int Y, float Angle = 0, int radius = 1) {
        anim = a;
        x = X;
        y = Y;
        angle = Angle;
        R = radius;
    }

    virtual void update() {};

    void draw(RenderWindow &app) {
        anim.sprite.setPosition(x, y);
        anim.sprite.setRotation(angle + 90);
        app.draw(anim.sprite);

        CircleShape circle(R);
        circle.setFillColor(Color(255, 0, 0, 170));
        circle.setPosition(x, y);
        circle.setOrigin(R, R);
    }

    virtual ~Entity() {};
};


class asteroid : public Entity {
public:
    asteroid() {
        dx = rand() % 8 - 4;
        dy = rand() % 8 - 4;
        name = "asteroid";
        count++;
    }

    // concrete class, i.e., not virtual
    void update() {
        x += dx; // update x position of the asteroid
        y += dy;

        // Draw the asteroid on the opposite side of the screen if it goes off the edge
        if (x > W) {
            x = 0;
        }
        if (x < 0) {
            x = W;
        }
        if (y > H) {
            y = 0;
        }
        if (y < 0) {
            y = H;
        }
    }

    static unsigned int getCount(){
        // counts the number of asteroid objects that are added
        return count;
    }

    ~asteroid(){
        // Decrements the count when an asteroid is destroyed
        --count;
    }

private:
    static unsigned int count;
};

unsigned int asteroid::count{0};

class bullet : public Entity {
public:
    bullet() {
        name = "bullet";
    }

    // set the direction and speed of bullet
    void update() {
        dx = cos(angle * DEGTORAD) * 6;
        dy = sin(angle * DEGTORAD) * 6;
        // angle+=rand()%6-3;
        x += dx;
        y += dy;

        // When the bullet hits the edge of the screen, remove it from the display list
        if (x > W || x < 0 || y > H || y < 0) {
            life = false;
        }
    }

};


class player : public Entity {
public:
    bool thrust;

    player() {
        name = "player";
    }

    // When the up arrow if pressed, then thrust is true
    // Speed up the space ship
    void update() {
        if (thrust) {
            dx += cos(angle * DEGTORAD) * 0.2; // increase the speed by 20% in the angle direction
            dy += sin(angle * DEGTORAD) * 0.2;
        } else {
            dx *= 0.99; // slow down by 1%
            dy *= 0.99;
        }

        // Limit the speed of the player to 15
        int maxSpeed = 15;
        float speed = sqrt(dx * dx + dy * dy);
        if (speed > maxSpeed) {
            dx *= maxSpeed / speed;
            dy *= maxSpeed / speed;
        }

        // Move the player
        x += dx;
        y += dy;

        // Enforcing a periodicity of motion
        if (x > W) {
            x = 0;
        }
        if (x < 0) {
            x = W;
        }
        if (y > H) {
            y = 0;
        }
        if (y < 0) {
            y = H;
        }
    }

};

// Class: UFO Class that is used to control the UFO
// Inputs: None
// Outputs: None

class UFO : public Entity {

public:
    UFO(){
        name = "UFO";
        dx = 1;
        dy = 0;
    }

    void update() {
        x += dx;
        y += dy;

        // Makes the UFO continue from one edge of the screen to the other and reappear when it goes off one side to the other
        if (x > W) {
            x = 0;
        }
        if (x < 0) {
            x = 0;
        }
        if (y > H) {
            y = 0;
        }
        if (y < 0) {
            y = 0;
        }
    }
};


bool isCollide(Entity *a, Entity *b) {
    return (b->x - a->x) * (b->x - a->x) +
           (b->y - a->y) * (b->y - a->y) <
           (a->R + b->R) * (a->R + b->R);
}


int main() {
    srand(time(0));

    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);
    Texture t1, t2, t3, t4, t5, t6, t7, t8, t9;
    if(!t1.loadFromFile("images/spaceship.png")){
        return EXIT_FAILURE;
    }
    if(!t2.loadFromFile("images/background.jpg")){
        return EXIT_FAILURE;
    }
    if(!t3.loadFromFile("images/explosions/type_C.png")){
        return EXIT_FAILURE;
    }
    if(!t4.loadFromFile("images/rock.png")){
        return EXIT_FAILURE;
    }
    if(!t5.loadFromFile("images/fire_blue.png")){
        return EXIT_FAILURE;
    }
    if(!t6.loadFromFile("images/rock_small.png")){
        return EXIT_FAILURE;
    }
    if(!t7.loadFromFile("images/explosions/type_B.png")){
        return EXIT_FAILURE;
    }
    // Icons made by "https://www.freepik.com" Freepik from "https://www.flaticon.com/" title="Flaticon">www.flaticon.com
    if(!t8.loadFromFile("images/ufo.png")){
        return EXIT_FAILURE;
    }
    if(!t9.loadFromFile("images/gameover.png")){
        return EXIT_FAILURE;
    }
    t1.setSmooth(true);
    t2.setSmooth(true);

    // Adds a sprite that will display the game over image
    Sprite background(t2), sGameover(t9);
    sGameover.setPosition(350, 275);

    Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
    Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
    Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
    Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
    Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
    Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
    Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);
    Animation sUFO(t8, 1, 0, 55, 55, 1, 0.8);

    SoundBuffer buffer;

    // Alarm, alert, siren loop, good for science fiction by Alexander Gastrell, License: Standard License
    if(!buffer.loadFromFile("sounds/siren.ogg")){
        return EXIT_FAILURE;
    }

    Sound sound;
    sound.setBuffer(buffer);

    SoundBuffer buffer1;

    // Bank safe explosion by Blastwave FX, License: Standard License
    if(!buffer1.loadFromFile("sounds/blast.ogg")){
        return EXIT_FAILURE;
    }

    Sound sound1;
    sound1.setBuffer(buffer1);

    Clock clock;

    std::list<Entity *> entities; // list of entity pointers

    for (int i = 0; i < 15; i++) {
        asteroid *a = new asteroid();
        a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
        entities.push_back(a);
    }

    player *p = new player();
    p->settings(sPlayer, 200, 200, 0, 20);
    entities.push_back(p);


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
    text.setCharacterSize(32); // in pixels, not points!

    // set the color
    text.setFillColor(Color::White);

    // set the text style
    text.setStyle(Text::Bold);
    text.setPosition(1000, 50);

    Text text1;

    // select the font
    text1.setFont(font); // font is a sf::Font

    float lives = 3;
    char scoreString1[100];
    sprintf(scoreString1, "Lives = %.1g", lives);
    text1.setString(scoreString1);

    // set the character size
    text1.setCharacterSize(32); // in pixels, not points!

    // set the color
    text1.setFillColor(Color::Red);

    // set the text style
    text1.setStyle(Text::Bold);
    text1.setPosition(1000, 100);
    float time;
    // Used to add the time
    float timer = 0;
    // Checks if there is a UFO in the game
    bool ufoAlive = false;
    // Checks if the game is still going on, i.e. if the player still has lives left
    bool game = true;

    /////main loop/////
    while (app.isOpen()) {
        time = clock.getElapsedTime().asSeconds();
        timer += time;
        clock.restart();
        Event event;
        while (app.pollEvent(event)) {
            if (event.type == Event::Closed) {
                app.close();
            }

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Space) {
                    bullet *b = new bullet();
                    b->settings(sBullet, p->x, p->y, p->angle, 10);
                    entities.push_back(b);
                }
            }
        }
        if(game) {

            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                p->angle += 3;
            }
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                p->angle -= 3;
            }
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                p->thrust = true;
            } else {
                p->thrust = false;
            }


            for (auto a:entities) {
                for (auto b:entities) {
                    if (a->name == "asteroid" && b->name == "bullet") {
                        if (isCollide(a, b)) {
                            a->life = false;
                            b->life = false;

                            Entity *e = new Entity();
                            e->settings(sExplosion, a->x, a->y);
                            e->name = "explosion";
                            entities.push_back(e);
                            // Plays an explosion sound
                            sound1.play();
                            score += 1;

                            for (int i = 0; i < 2; i++) {
                                if (a->R != 15) { ;
                                    Entity *e = new asteroid();
                                    e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                                    entities.push_back(e);
                                }
                            }

                        }
                    }

                    if (a->name == "player" && b->name == "asteroid") {
                        if (isCollide(a, b)) {
                            b->life = false;

                            Entity *e = new Entity();
                            e->settings(sExplosion_ship, a->x, a->y);
                            e->name = "explosion";
                            entities.push_back(e);
                            sound1.play();
                            p->settings(sPlayer, W / 2, H / 2, 0, 20);
                            p->dx = 0;
                            p->dy = 0;
                            score -= 1;
                            lives -= 1;
                        }
                    }
                    if (a->name == "player" && b->name == "UFO") {
                        if (isCollide(a, b)) {
                            sound.stop();
                            b->life = false;

                            Entity *e = new Entity();
                            e->settings(sExplosion_ship, a->x, a->y);
                            e->name = "explosion";
                            entities.push_back(e);

                            p->settings(sPlayer, W / 2, H / 2, 0, 20);
                            p->dx = 0;
                            p->dy = 0;
                            ufoAlive = false;
                            // Resets the timer, so that the count can start again for when another UFO comes
                            timer = 0;
                            sound.stop();
                            sound1.play();
                            score -= 1;
                            lives -= 1;
                        }
                    }
                    if (a->name == "UFO" && b->name == "bullet") {
                        if (isCollide(a, b)) {
                            a->life = false;
                            b->life = false;

                            Entity *e = new Entity();
                            e->settings(sExplosion, a->x, a->y);
                            e->name = "explosion";
                            entities.push_back(e);
                            ufoAlive = false;
                            timer = 0;
                            sound.stop();
                            sound1.play();
                        }
                    }
                }

            }
            if (p->thrust) {
                p->anim = sPlayer_go;
            } else {
                p->anim = sPlayer;
            }

            for (auto e:entities) {
                if (e->name == "explosion") {
                    if (e->anim.isEnd()) {
                        e->life = 0;
                    }
                }
            }
            // If there is no asteroids left, then 15 more are created
            if (asteroid::getCount() == 0) {
                for (int i = 0; i < 15; i++) {
                    asteroid *a1 = new asteroid();
                    a1->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
                    entities.push_back(a1);
                }
            }

            // Checks if there has been at least 10 seconds since the last UFO was destroyed or the game was started
            if (timer > 10) {
                // Gets the mod remainder to check if there has been been at least 10 seconds passed to call the UFO again.
                float timeGap = fmod(timer, 10);
                if (timeGap <= 0.05) {
                    // If there is no UFO's in the game, and at least 10 seconds have passed than a new UFO is created.
                    if (!ufoAlive) {
                        sound.play();
                        UFO *f1 = new UFO();
                        f1->settings(sUFO, 0, 100);
                        f1->name = "UFO";
                        entities.push_back(f1);
                        ufoAlive = true;
                        timer = 0;
                    }
                }
            }

            // entities.begin() returns the first iterator of the entities list
            // entities.end() returns the iterator of the theoretical element after the last element in the list
            // i is an iterator, an iterator is an object that is like a pointer
            for (auto i = entities.begin(); i != entities.end();) {
                Entity *e = *i; // Use the * on the iterator i to get the element of the list, i.e., an Entity pointer

                e->update(); // polymorphism, call the derived class update
                e->anim.update(); // update the animation

                if (e->life == false) {
                    i = entities.erase(i);
                    delete e;
                } else {
                    i++;
                }
            }



            //////draw//////
            app.draw(background);
            sprintf(scoreString, "Score = %g", score);
            // Displays the score
            text.setString(scoreString);
            app.draw(text);
            sprintf(scoreString1, "Lives = %g", lives);
            // Displays the lives
            text1.setString(scoreString1);
            app.draw(text1);

            for (auto i:entities) {
                i->draw(app);
            }

            // If the player has no lives left than the game is over and the game over image is presented to the screen
            if (lives == 0) {
                app.draw(sGameover);
                game = false;
                sound.stop();
                sound1.stop();
            }
            app.display();
        }
    }

    return 0;
}
