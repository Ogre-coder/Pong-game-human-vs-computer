#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>
#include <cstddef>

#include "Client.h"
#include "Server.h"
#include "Serial.h" // Must include Windows.h after Winsock2.h, so Serial must include after Client/Server
#include "CControl.h"
#include <Windows.h>
#include <chrono>
#include <conio.h>

using namespace std;
using namespace std::chrono;

CControl::CControl()
{

}

CControl::~CControl()
{

}

void CControl::init_com(int comport)
{
    string Port = "COM" + to_string(comport);
    _com.open(Port);
}

bool CControl::get_data(int type, int channel, int& result)
{
    string tx_str = "G " + to_string(type) + " " + to_string(channel) + '\n';
    string rx_str;

    // temporary storage
    char buff[2];

    // Send TX string
    _com.write(tx_str.c_str(), tx_str.length());
    Sleep(10); // wait for ADC conversion, etc. May not be needed?

    rx_str = "";
    // start timeout count
    double start_time = cv::getTickCount();

    buff[0] = 0;
    // Read 1 byte and if an End Of Line then exit loop

// Timeout after 1 second, if debugging step by step this will cause you to exit the loop
    while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
    {
        if (_com.read(buff, 1) > 0)
        {
            rx_str = rx_str + buff[0];
        }
    }

    size_t found = rx_str.find_last_of(" ");
    string r = rx_str.substr(found + 1);
    result = stoi(r);
    cv::waitKey(1);

    return true;
}

bool CControl::set_data(int type, int channel, int val)
{
    string result;
    string tx_str = "S " + to_string(type) + " " + to_string(channel) + " " + to_string(val) + '\n';
    string rx_str;
    char buff[2];

    _com.write(tx_str.c_str(), tx_str.length());

    Sleep(10); // wait for ADC conversion, etc. May not be needed?

    rx_str = "";
    // start timeout count
    double start_time = cv::getTickCount();

    buff[0] = 0;

    while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
    {
        if (_com.read(buff, 1) > 0)
        {
            rx_str = rx_str + buff[0];
        }
    }
    size_t found = rx_str.find_last_of(" ");
    string r = rx_str.substr(found + 1);
    result = stoi(r);
    cv::waitKey(1);
}

bool CControl::get_analog()
{

    get_data(1, 26, num1);
    pos1 = (num1 * 100) / 1022;

    get_data(1, 26, num2);
    pos2 = (num2 * 100) / 1020;

    cout << "ANALOG TEST:CH9 = " << num1 << " (" << pos1 << "%) " << "CH15 = " << num2 << " (" << pos2 << "%) " << endl;

    return true;
}

bool CControl::get_digital()
{

    auto start = high_resolution_clock::now();
    get_data(0, 32, num3);
    auto stop = high_resolution_clock::now();

    if (duration_cast<milliseconds>(stop - start).count() - time > debounceDelay)
    {
        cout << "DIGITAL TEST:CH32 = " << num3 << endl;
        if (num3 != 1)
        {
            set_data(0, 38, num3);
            time = duration_cast<seconds>(stop - start).count();
        }
        else
        {
            set_data(0, 38, num3);
            time = duration_cast<seconds>(stop - start).count();
        }
    }

    return true;
}

bool CControl::get_button(int channel)
{

    auto start = high_resolution_clock::now();
    get_data(0, channel, num4);
    auto stop = high_resolution_clock::now();

    if (duration_cast<milliseconds>(stop - start).count() - time2 > debounceDelay1)
    {
        if (num4 != 1)
        {
            // button_count++;
            button_count = true;
            while (num4 == 0)
            {
                get_data(0, channel, num4);
                if (num4 == 1)
                {
                    break;
                }
            }
            time2 = duration_cast<seconds>(stop - start).count();
            // cout << "BUTTON TEST: " << button_count_32 << endl;

            cout << "BUTTON TEST: " << button_count << endl;
        }
        else
        {
            time2 = duration_cast<seconds>(stop - start).count();
        }
    }

    return true;
}

bool CControl::get_servo() {

    /*
    // MIN = 7;
    //MAX = 543;
     int pos3;
     cout << "Specifiy position: ";
     cin >> pos3;
     set_data(2, 0, pos3);
     */

    for (int pos = 7; pos <= 120; pos = pos + 2)
    {
        set_data(2, 0, pos);
        cout << "SERVO TEST: CH0 POS " << pos << endl;
        Sleep(50);
    }

    for (int pos2 = 120; pos2 >= 7; pos2 = pos2 - 2)
    {
        set_data(2, 0, pos2);
        cout << "SERVO TEST: CH0 POS " << pos2 << endl;
        Sleep(50);
    }

    return true;
}




