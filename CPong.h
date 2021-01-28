#pragma once

#include "CBase4618.h"

class CPong : public CBase4618 {

public:

	int xpos, ypos;
	float speed_time;
	bool right = false;
	int difficulty;
	int match_pos;
	int score1 = 0;
	int score2 = 0;
	bool start_game = true;
	int ypos_2nd_board, xpos_2nd_board;
	Point p1, p2;
	int width, height;
	bool count;
	int elapsed_time;
	int FPS;
	float center_x, center_y;
	int ball_dir = 0;
	bool player2, player1, paddle2;
	Rect r,r1;
	Point center;
	Scalar color;
	CPong(int w, int h, int comport);
	~CPong();
	void draw();
	void update();
	void run();
};

