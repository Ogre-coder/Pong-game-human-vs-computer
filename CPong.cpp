#include "stdafx.h"
#include "CPong.h"
#include "time.h"
#include "conio.h"
#include "CControl.h"

CPong::CPong(int w, int h, int comport)
{
    width = w;
    height = h;
    Control.init_com(comport);
    Size size(w, h);
    _canvas = Mat::zeros(size, CV_8UC3);
    //waitKey(1);
}

CPong::~CPong()
{

}

void CPong::draw()
{
    auto calc_start = steady_clock::now();

    rectangle(_canvas, r, color, FILLED);
    rectangle(_canvas, r1, color, FILLED);
    circle(_canvas, center, 8, color, FILLED);
    line(_canvas, p1, p2, color, 1, 8, 0 );

    sleep_for(std::chrono::milliseconds(33));

    string text2 = "Player1: " + to_string(score1);
    string text3 = "Player2: " + to_string(score2);
    putText(_canvas, text2, Point(305, 25), FONT_HERSHEY_DUPLEX, 1, color, 1);
    putText(_canvas, text3, Point(_canvas.size().width - 470, 25), FONT_HERSHEY_DUPLEX, 1, color, 1);

    auto calc_end = steady_clock::now();
    auto calc_elapsed = duration_cast<milliseconds>(calc_end - calc_start);

    elapsed_time = calc_elapsed.count();
    if (elapsed_time < 33 )
    {
        Sleep(33 - elapsed_time);
    }

    FPS = 1 / (elapsed_time * 0.001);

    string text1 = "FPS: " + to_string(FPS);
    putText(_canvas, text1, Point(_canvas.size().width - 80, 20), FONT_HERSHEY_DUPLEX, 0.5, color, 1);

    imshow("blank", _canvas);

}

void CPong::update()
{

    Control.get_analog();

    //paddle 1
    ypos = ((100 - Control.pos1 - 5) * _canvas.size().height) / 100;
    xpos = 2;

    //speed * time
    if (difficulty == 1)
    {
        speed_time = elapsed_time * 200 * 0.001;
    }
    else if (difficulty == 2)
    {
        speed_time = 200 * (elapsed_time * 4) * 0.001;
    }
    else if (difficulty == 3)
    {
        speed_time = 200 * (elapsed_time * 4) * 0.001;
        if (score1 >= 2 || score2 >= 2)
        {
            speed_time = 200 * (elapsed_time * 6) * 0.001;
        }
    }

    //paddle 2
    if (paddle2 == true)
    {
        if (center_x >= _canvas.size().width / 2 && center_x < 550)
        {
            ypos_2nd_board = center_y;
            if (difficulty == 1)
            {
                match_pos = 10 * ((rand() % 8) + 6);
            }
            else if (difficulty == 2)
            {
                match_pos = 10 * ((rand() % 10) + 1);
            }
            else if (difficulty == 3)
            {
                match_pos = 10 * ((rand() % 9) + 1);
            }
        }
        else if (center_x >= 550)
        {
            ypos_2nd_board = center_y - match_pos;
        }
    }
    else
    {
        ypos_2nd_board = center_y;
    }


    //edge detection of paddle 1 & 2
    if (ypos > (_canvas.size().height - 94))
    {
        ypos = _canvas.size().height - 94;
    }
    else if (ypos < 3)
    {
        ypos = 3.5;
    }
    else if (ypos_2nd_board > (_canvas.size().height - 94))
    {
        ypos_2nd_board = _canvas.size().height - 94;
    }
    else if (ypos_2nd_board < 3)
    {
        ypos_2nd_board = 3.5;
    }

    //shape of paddle
    r = Rect(xpos, ypos, 13, 90);
    r1 = Rect(xpos_2nd_board, ypos_2nd_board, 13, 90);

    //trajectory of ball

    if (ball_dir == 0)
    {
        center_x = center.x - speed_time;
        center_y = center.y;
    }
    else if (ball_dir == 1)
    {
        center_x = center.x + speed_time;
        center_y = center.y + speed_time;
    }
    else if (ball_dir == 2)
    {
        center_x = center.x - speed_time;
        center_y = center.y + speed_time;
    }
    else if (ball_dir == 3)
    {
        center_x = center.x + speed_time;
        center_y = center.y - speed_time;
    }
    else if (ball_dir == 4)
    {
        center_x = center.x - speed_time;
        center_y = center.y - speed_time;
    }
    if (ball_dir == 5)
    {
        center_x = center.x + speed_time;
        center_y = center.y;
    }
    if (ball_dir == 6)
    {
        center_x = _canvas.size().width / 2;
        center_y = _canvas.size().width / 2;
    }

    if (start_game == true)
    {
        sleep_for(seconds(2));
        start_game = false;

        if (ball_dir == 0)
        {
            ball_dir = 0;
        }
        if (ball_dir == 5)
        {
            ball_dir = 5;
        }
    }

    if (score1 == 5 || score2 == 5)
    {
        exit(0);
    }

    if (ball_dir == 6)
    {
        sleep_for(seconds(2));
        if (right == false)
        {
            ball_dir = 0;
            start_game = true;
        }
        else if (right == true)
        {
            ball_dir = 5;
            start_game = true;
        }
    }

    //start game direction of ball

    //collision detection of player 1
    if ( center_x <= xpos + 13 && (center_y <= ypos + 30 && center_y >= ypos) )
    {
        ball_dir = 3;
        player1 = true;
        player2 = false;
    }
    else if ( center_x <= xpos + 13 && (center_y > ypos + 30 && center_y < ypos + 60) )
    {
        ball_dir = 5;
        player1 = true;
        player2 = false;
    }
    else if ( center_x <= xpos + 13 && (center_y >= ypos + 60 && center_y <= ypos + 93) )
    {
        ball_dir = 1;
        player1 = true;
        player2 = false;
    }
    else if ( center_x <= xpos + 13 && (center_y < ypos  || center_y > ypos + 90) )
    {
        score2++;
        right = false;
        ball_dir = 6;
    }
  
    //collision detection of player 2
    if ( center_x >= xpos_2nd_board && (center_y <= ypos_2nd_board + 30 && center_y >= ypos_2nd_board ) )
    {
        ball_dir = 4;
        player2 = true;
        player1 = false;
    }
    else if ( center_x >= xpos_2nd_board && (center_y < ypos_2nd_board + 60 && center_y > ypos_2nd_board + 30) )
    {
        ball_dir = 0;
        player2 = true;
        player1 = false;
    }
    else if ( center_x >= xpos_2nd_board && (center_y >=  ypos_2nd_board + 60 && center_y <= ypos_2nd_board + 90) )
    {
        ball_dir = 2;
        player2 = true;
        player1 = false;
    }
    else if (center_x >= xpos_2nd_board  && (center_y < ypos_2nd_board || center_y > ypos_2nd_board + 90) )
    {
        score1++;
        right = true;
        ball_dir = 6;
    }

    //edge detection of ball when hit by player 2 & 1
    if (player1 == true && player2 == false)
    {
        if (center_y <= 4)
        {
            ball_dir = 1;
            paddle2 = true;
        }
        else if (center_y >= _canvas.size().height - 10)
        {
            ball_dir = 3;
            paddle2 = true;
        }
        else
        {
            paddle2 = true;
        }
    }
    else if (player2 == true && player1 == false)
    {
        if ( center_y <= 4)
        {
            ball_dir = 2;
            paddle2 = false;
         
        }
        else if ( center_y >= _canvas.size().height - 10)
        {
            ball_dir = 4;
            paddle2 = false;
        }
        else
        {
            paddle2 = false;
        }
    }

    //center of ball
    center = Point(center_x, center_y);


}

void CPong::run()
{

    Control.get_analog();
    ypos = (50 * _canvas.size().height) / 100;
    xpos = 2;

    ypos_2nd_board = (50 * _canvas.size().height) / 100;
    xpos_2nd_board = _canvas.size().width - 13;

    color = Scalar(255, 255, 255);

    r = Rect(xpos, ypos - 25, 13, 90);
    r1 = Rect(xpos_2nd_board, ypos_2nd_board - 20, 13, 90);
    center = Point(_canvas.size().width / 2, _canvas.size().height / 2);
    p1 = Point(_canvas.size().width / 2, 0);
    p2 = Point(_canvas.size().width / 2, _canvas.size().height);
   
    cout << " 1 - EASY, 2 - INTERMEDIATE, 3 - HARD " << endl;
    cout << "Choose Difficulty: " << endl;
    cin >> difficulty;

    while (!(kbhit() == 'q'))
    {
        draw();
        update();
        _canvas = Mat::zeros(Size(width, height), CV_8UC3);
    }

}