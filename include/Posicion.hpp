#pragma once
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
using namespace std;

class posicion
{
private:
    int x;
    int y;

public:
    posicion() : x(0), y(0) {}
    int posicionX(int X) const;
    int posicionY(int Y) const;
};

int posicion::posicionX(int X) const
{
    while (true)
    {
        if (GetAsyncKeyState('A') || ('D'))
        {
            if (GetAsyncKeyState('A'))
            {
                X--;
                if (X = 0)
                {
                    X = 0;
                    X = x;
                    // }
                }
                if (GetAsyncKeyState('D'))
                {
                    X++;
                    if (X = 20)
                    {
                        X = 20;
                    }
                    X = x;
                }
            }
        }
    }