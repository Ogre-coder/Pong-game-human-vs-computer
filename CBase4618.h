#pragma once

#include "Serial.h" // Must include Windows.h after Winsock2.h, so Serial must include after Client/Server
#include "CControl.h"
#include <chrono>
#include <thread>

// OpenCV Include
#include "opencv.hpp"
#include <opencv2/core/hal/interface.h>

// OpenCV Library
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")

//#include "CControl.h"
#ifndef CBASE4618_H
#define CBASE4618_H

using namespace std;
using namespace cv;
using namespace std::chrono;
using namespace std::this_thread;

class CBase4618
{

protected:

	CControl Control;
	Mat _canvas;


public:


	CBase4618();
	~CBase4618();
	virtual void update();
	virtual void draw();
};

#endif






