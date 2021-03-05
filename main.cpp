#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <time.h>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>
#include "classes.hpp"
#include "functions.hpp"
#include "templates.hpp"
#include "defines.hpp"

using namespace sf;

typedef struct gameState{
        bool gameIsRunning = true;
        bool gameRestart = false;
        bool gameDeath = false;
        bool gameWin = false;
};

typedef struct powerUps{
        bool isFiring = false;
        bool isStar = false;
};

int main()
{
    RenderWindow window(sf::VideoMode(LARGURA, ALTURA), "BREAKOUT!", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    srand(time(NULL));


    //Loads

    Font font1;
    if(!font1.loadFromFile("font/PixeledFont.ttf")){
        std::cout<<"Error in font file"<<std::endl;
        system("pause");
    }

//-----------------------------------------------//
    Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
    Texture p1, p2, p3, p4, p5;

//-------------------Blocks--------------------------//
    t1.loadFromFile("images/block-orange.png");
    t2.loadFromFile("images/block-pink.png");
    t3.loadFromFile("images/block-red.png");
    t4.loadFromFile("images/block-yellow.png");

//-------------------Basic--------------------------//
    t5.loadFromFile("images/ball.png");
    t6.loadFromFile("images/paddle.png");
    t8.loadFromFile("images/coin.png");
    t9.loadFromFile("images/paddle_larger.png");
    t10.loadFromFile("images/red-heart.png");

//-----------------Power Ups------------------------//
    p1.loadFromFile("images/mario-power-ups-red-mushroom.png");
    p2.loadFromFile("images/mario-power-ups-star.png");
    p3.loadFromFile("images/mario-power-ups-flower.png");
    p4.loadFromFile("images/fire-ball.png");
    p5.loadFromFile("images/heart.png");

//-------------------Sounds------------------------//
    Music mainMusic;
    mainMusic.openFromFile("audio/MainMusic.ogg");


    SoundBuffer paddleBuffer, coinBuffer, fireBuffer;

    paddleBuffer.loadFromFile("audio/paddle_collision.wav");
    coinBuffer.loadFromFile("audio/coin_collision.wav");
    fireBuffer.loadFromFile("audio/fire.wav");

    Sound pCollisionSound, coinSound, fireSound;

    pCollisionSound.setBuffer(paddleBuffer);
    coinSound.setBuffer(coinBuffer);
    fireSound.setBuffer(fireBuffer);



    //Sprites
    Sprite sBall(t5), LargerPaddle(t9), sPaddle(t6), sLargerPaddle(t9), Aux;
    Sprite mushroom(p1), flower(p3), fire(p4), star(p2);
    Sprite heart(p5);
    Sprite block[1000];


    //Set Spawn
    sBall.setPosition(960,600);
    sPaddle.setPosition(950,900);
    mushroom.setTexture(p1);  mushroom.setScale(0.15,0.15);
    flower.setTexture(p3);  flower.setScale(0.15, 0.15);
    fire.setTexture(p4);    fire.setScale(0.15,0.15);
    star.setTexture(p2);    star.setScale(0.15,0.15);
    heart.setTexture(p5);   heart.setScale(0.10,0.10);  heart.setPosition(1750,0);


    //Vectors
    std::vector<Sprite> mushrooms;
    mushrooms.push_back(Sprite(mushroom));

    std::vector<Sprite> flowers;
    flowers.push_back(Sprite(flower));

    std::vector<Sprite> fires;
    fires.push_back(Sprite(fire));

    std::vector<Sprite> stars;
    stars.push_back(Sprite(star));



    //Text
    Text text1;
    Text text2(" S - Start/P - Pause/ESC - Close", font1, 30);
    Text text3("Points:", font1, 30);               text3.setPosition(((LARGURA/2)-20),0);
    Text pause("Game Paused", font1, 80);           pause.setPosition(700,600);
    Text text5("Press S to start", font1, 80);       text5.setPosition(700,600);
    Text text6("YOU LOSE! press S to start", font1, 80);  text6.setPosition(400,600);
    Text text7("YOU WON! CONGRATULATIONS!", font1, 80);   text7.setPosition(500,600);
    Text text8("Acho que vale um 20 :)", font1, 20);   text8.setPosition(0,1000);


    //Matriz de Blocos
    int num_block = 0;

    for(int linha = 0; linha<(LARGURA/LARGURA_BLOCO); linha++)
    {
        for(int coluna = 5; coluna < ((ALTURA/2)/ALTURA_BLOCO); coluna++)
        {
            block[num_block].setTexture(t1);
            block[num_block].setPosition(linha * LARGURA_BLOCO, coluna * ALTURA_BLOCO);

            num_block++;
        }
    }

    //COMUNICAÇÃO SERIAL
    unsigned long n;
    char *ncom="COM5";
    int baud=9600;
    char dado[2];
    int dado_int[2];

    HANDLE p_com;

    if((p_com = AbreComm(ncom, baud)) == INVALID_HANDLE_VALUE)
    return 1;

    //p_com = AbreComm(ncom,baud);


    //Clock e Variáveis
    Clock clock;

    bool flagBall = false;
    float Vx,Vy;
    float speed = 0.3;
    int num_brocked = 0;
    int block_erase = 0;
    int lifes = 3;
    int aux = 0;
    int points = 0;
    char c_points = '0';

    gameState State;
    powerUps powerUp;

    Vector2f Ball;
    Vector2f LargerPad;
    Vector2f Pad;
    Vector2f Block_P;


    while (window.isOpen())
    {

        WriteFile(p_com, "s", 1, &n, NULL);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)){
                window.close();
                WriteFile(p_com, "z", 1, &n, NULL);

            }
        }


    /**< Atualização */

    if(Keyboard::isKeyPressed(Keyboard::S)){ State.gameIsRunning = true; State.gameDeath = false; }

    if(State.gameIsRunning && !State.gameDeath)
    {
        ///Teste Start
        //if(!gameStart) gameIsRunning = false;

        ///Teste Pause
        if(Keyboard::isKeyPressed(Keyboard::P)) State.gameIsRunning = false;;

        ///Vencer o jogo
        if(num_brocked == num_block)
        {
            State.gameWin = true;
        }


        ///Tempo passado do jogo
        Time elapsed = clock.getElapsedTime();
        //td::cout << elapsed.asSeconds()<<std::endl;


        ///Velocidades
        if(flagBall == false)
        {
           Vx = 0.3 * elapsed.asMilliseconds();
           Vy = 0.3 * elapsed.asMilliseconds();
        }



       ///Movimentos
       //Paddle

        ReadFile(p_com, dado, 10, &n, NULL);

   	   	if(*dado == 'e')
        {
            sPaddle.move(-VELOCIDADE_PAD,0);
          //  sLargerPaddle.move(-12,0);
        }
        else if(*dado == 'd')
        {
             sPaddle.move(VELOCIDADE_PAD,0);
        //     sLargerPaddle.move(VELOCIDADE_PAD,0);
        }

        printf("%d\n",*dado);


         if(Keyboard::isKeyPressed(Keyboard::Right))  sPaddle.move(VELOCIDADE_PAD,0);
         if(Keyboard::isKeyPressed(Keyboard::Left))  sPaddle.move(-VELOCIDADE_PAD,0);

        //Ball
        sBall.move(Vx,Vy);


        //Cogumelo
        for(int i = 0; i<mushrooms.size(); i++){
            mushrooms[i].move(0.f,2.f);

            if(mushrooms[i].getPosition().y > ALTURA)
            mushrooms.erase(mushrooms.begin()+i);
        }

        //Flor
        for(int i = 0; i<flowers.size(); i++){
            flowers[i].move(0.f,2.f);

            if(flowers[i].getPosition().y > ALTURA)
            flowers.erase(flowers.begin()+i);
        }

        //Estrela
        for(int i = 0; i<stars.size(); i++){
            stars[i].move(0.f,2.f);

            if(stars[i].getPosition().y > ALTURA)
            stars.erase(stars.begin()+i);
        }

        //Fogo
        for(int i = 0; i<fires.size(); i++){
            fires[i].move(0.f,-5.f);
        }



        ///Ações
        if(powerUp.isFiring)
        {
            if(*dado == 'f')
            {
                aux++;
                fire.setPosition(Pad.x + (LARGURA_PAD/2), Pad.y);
                fires.push_back(Sprite(fire));
                if(aux == 6) powerUp.isFiring = false;
            }
        }



        ///Teste Colisão
        if(Collision(sPaddle,sBall))
        {
            Vy = -Vy;
            flagBall = true;
        }

        if(Collision(LargerPaddle,sBall))
        {
            Vy = -Vy;
            flagBall = true;
        }


        for(int i = 0; i < num_block; i++)
        {
            if(Collision(sBall,block[i]))
            {
                Vy = -Vy;

                num_brocked++;
                points++;
                c_points++;

                WriteFile(p_com, "q", 1, &n, NULL);

                Block_P = block[i].getPosition();

                if(t_mushroom())
                {
                    mushroom.setPosition(Block_P.x + (LARGURA_BLOCO/2) , Block_P.y);
                    mushrooms.push_back(Sprite(mushroom));
                }

                else if(t_flower())
                {
                    flower.setPosition(Block_P.x + (LARGURA_BLOCO/2) , Block_P.y);
                    flowers.push_back(Sprite(flower));
                }

                else if(t_star())
                {
                    star.setPosition(Block_P.x + (LARGURA_BLOCO/2) , Block_P.y);
                    stars.push_back(Sprite(star));
                }


                block[i].setPosition(-200,0);

            }
        }

        //Colisao com cogumelo
        for(int i = 0; i<mushrooms.size(); i++)
        {
            if(Collision(mushrooms[i],sPaddle))
               {
                   WriteFile(p_com, "m", 1, &n, NULL);
                   lifes++;
                   mushroom.getPosition();
                   mushrooms.erase(mushrooms.begin()+i);
               }
        }
        //Colisao com flor
        for(int i = 0; i<flowers.size(); i++)
        {
            if(Collision(flowers[i],sPaddle))
               {
                   aux = 0;
                   powerUp.isFiring = true;
                   flower.getPosition();
                   flowers.erase(flowers.begin()+i);
               }
        }

        //Colisao com estrela
        for(int i = 0; i<stars.size(); i++)
        {
            if(Collision(stars[i],sPaddle))
               {
                   aux = 0;
                   powerUp.isStar = true;
                   star.getPosition();
                   stars.erase(stars.begin()+i);
               }
        }

        //Colisão fogo com bloco
        for(int i=0; i<num_block; i++)
            {
                for(int j=0; j<fires.size(); j++)
                {
                    if(Collision(fires[j],block[i]))
                    {
                        block[i].setPosition(-100,-100);
                        fires.erase(fires.begin()+j);

                        block_erase++;
                    }
                }
            }





       ///Teste Fora de Tela
        Ball = sBall.getPosition();
        Pad = sPaddle.getPosition();
        LargerPad = LargerPaddle.getPosition();

        if( Ball.x > LARGURA || Ball.x < 0) { Vx = -Vx; flagBall = true;}
        if( Ball.y < 0) { Vy = -Vy; flagBall = true;}
        if( Ball.y > ALTURA)
        {
             WriteFile(p_com, "b", 1, &n, NULL);
             lifes--;
             sBall.setPosition(960,600);
             sPaddle.setPosition(950,900);
             //State.gameIsRunning = false;
             State.gameDeath = true;
        }

        if( Pad.x + LARGURA_PAD > LARGURA) sPaddle.move(-6,0);
        if( Pad.x < 0) sPaddle.move(6,0);






        ///Vidas
        if(lifes > 5) lifes = 5;
        if(lifes == 0) State.gameRestart = true;

        Text num_lifes(toString<int>( lifes ), font1 );
        num_lifes.setCharacterSize(55);
        num_lifes.setPosition(1850,0);



        ///Pontos
        Text text_points(toString<int>( points ), font1 );
        text_points.setCharacterSize(50);
        text_points.setPosition(LARGURA/2+95,-10);








        ///Restart the Game
        if(State.gameRestart)
        {
            int num_block = 0;

            for(int linha = 0; linha<(LARGURA/LARGURA_BLOCO); linha++){
                for(int coluna = 5; coluna < ((ALTURA/2)/ALTURA_BLOCO); coluna++){
                    block[num_block].setTexture(t1);
                    block[num_block].setPosition(linha * LARGURA_BLOCO, coluna * ALTURA_BLOCO);
                    num_block++;
                }
            }
            lifes = 3;
        }






       clock.restart().asMilliseconds();

        ///Display
        window.clear();

        //Bascio
        window.draw(sBall);

        //if(!powerUp.isStar)
        //{
            window.draw(sPaddle);
        //}
        //else window.draw(LargerPaddle);

        for(int i = 0; i < num_block; i++)
        {
            window.draw(block[i]);
        }

        //Textos
        window.draw(text2);
        window.draw(text3);
        window.draw(num_lifes);
        window.draw(text_points);
        if(!(State.gameIsRunning) && !(State.gameRestart)) window.draw(pause);
        if( State.gameDeath && !State.gameRestart) window.draw(text5);
        if(State.gameRestart)
        {
            window.draw(text6);
            State.gameRestart = false;
        }
        if(State.gameWin){
            window.draw(text7);
            window.draw(text8);
        }
        //if(!gameStart) window.draw(text5);

        //Powerups
        for(int i = 0; i < mushrooms.size(); i++)
        {
            window.draw(mushrooms[i]);
        }
        for(int i = 0; i < flowers.size(); i++)
        {
            window.draw(flowers[i]);
        }
        for(int i = 0; i < fires.size(); i++)
        {
            window.draw(fires[i]);
        }
        for(int i = 0; i < stars.size(); i++)
        {
            window.draw(stars[i]);
        }

        window.draw(heart);



       // window.draw(heart);

        window.display();
        }

    }

    return 0;
}
