#ifndef SFML_CLASSES_HPP
#define SFML_CLASSES_HPP

#include "functions.hpp"

using namespace sf;

class Animation{

public:
    float Frame, speed;
    Sprite sprite;  // Objeto que se move
    std::vector<IntRect> frames;  // Cria um retângulo que vai ser o que será exibido

    Animation(){}   // Construtor padrão

    Animation(Texture &t, int x, int y, int w, int h, int count, float Speed, float scale_x, float scale_y, float pos_x, float pos_y){
        Frame = 0;
        speed = Speed;

        for(int i=0; i<count; i++)
            frames.push_back(IntRect(x+i*w, y, w, h));

        sprite.setTexture(t);
        sprite.setOrigin(w/2,h/2);
        sprite.setTextureRect(frames[0]);
        sprite.setScale(scale_x, scale_y);
        sprite.setPosition(pos_x, pos_y);
    }

    void update(){
        Frame+=speed;
        int n = frames.size();
        if(Frame >= n) Frame -= n;
        if(n>0) sprite.setTextureRect(frames[int(Frame)]);
    }

    void Delete()
    {
        sprite.setPosition(-100,0);
    }
};

class PowerUp{
public:

    Sprite sprite;


    PowerUp(){}

    PowerUp(Texture &t, float scale_x, float scale_y, float pos_x, float pos_y)
    {
        sprite.setTexture(t);
        sprite.setScale(scale_x,scale_y);
        sprite.setPosition(pos_x,pos_y);
    }

    PowerUp(std::vector<PowerUp> power_up, Texture &t){
        sprite.setTexture(t);
    }

    void Delete()
    {
        sprite.setPosition(-100,0);
    }
};

class Fire{
private:
    Sprite fire;

public:
    Fire(Texture &t){
        Sprite fire(t);
        //fire.setScale(size);
    }

    void setPos(Vector2f newPos){
        fire.setPosition(newPos);
    }

    void shoot( int speed ) { fire.move(0,speed); }

    void draw( RenderWindow &window){
    window.draw(fire);
    }
};

class Paddle{
private:
    Sprite paddle;

public:
    Paddle(Vector2f size){
        paddle.setScale(size);
    }

    void setPos(Vector2f newPos){
        paddle.setPosition(newPos);
    }

    void mov(int speedX, int speedY){
        paddle.move(speedX,speedY);
    }

    void draw(RenderWindow &window){
        window.draw(paddle);
    }

    int getX(){ return paddle.getPosition().x; }
    int getY(){ return paddle.getPosition().y; }
};

class Heart{
public:
    Sprite heart;



    Heart(Texture &t, int x, int y){
        heart.setTexture(t);
        heart.setPosition(x,y);
    }

};
#endif // SFML_FILENAME_HPP
