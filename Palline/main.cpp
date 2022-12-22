#include "Finestra.h"
#include "Utility.h"

#include <chrono>
#include <iostream>
using namespace std;

//Costanti
#define PI 3.141592653589793

//Firma delle funzioni
void DrawFPS(HDC hdc);
void Draw(HDC hdc, RECT bounds);
void Rimbalzo(double& cos, double& sin, double& x, double& y, RECT bounds);

//Variabili Globali
std::chrono::steady_clock::time_point last_tick;

void main()
{
    //inizializzazione variabiler per temporizzare lo spostamento della pallina 
    last_tick = std::chrono::steady_clock::now();

    //nasconde la consolle
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    //visualizza la finestra 
    CreateMyWindow(Draw);
}


//Prima Pallina (Rossa)
double x1 = 0, y = 0, angle1 = (20 * PI) / 180, sin1 = sin(angle1), cos1 = cos(angle1);

//Seconda Pallina (Verde)
double x2 = 0, y2 = 0, angle2 = (45 * PI) / 180, sin2 = sin(angle2), cos2 = cos(angle2);

//Terza Pallina (Blue)
double x3 = 0, y3 = 0, angle3 = (65 * PI) / 180, sin3 = sin(angle3), cos3 = cos(angle3);

//Quarta Pallina (Nera)
double x4 = 0, y4 = 0, angle4 = (80 * PI) / 180, sin4 = sin(angle4), cos4 = cos(angle4);

//Quinta Pallina (Bianca)
double x5 = 0, y5 = 0, angle5 = (125 * PI) / 180, sin5 = sin(angle5), cos5 = cos(angle5);

//Diametro Palline
int d = 40;

void Draw(HDC hdc, RECT bounds)
{
    DrawFPS(hdc);

    //Prima Pallina
    HBRUSH b = CreateSolidBrush(RGB(255, 0, 0));
    HGDIOBJ old = SelectObject(hdc, b);
    Ellipse(hdc, x1, y, x1 + d, y + d);
    SelectObject(hdc, old);
    DeleteObject(b);

    //Seconda Pallina
    b = CreateSolidBrush(RGB(0, 255, 0));
    old = SelectObject(hdc, b);
    Ellipse(hdc, x2, y2, x2 + d, y2 + d);
    SelectObject(hdc, old);
    DeleteObject(b);

    //Terza Pallina
    b = CreateSolidBrush(RGB(0, 0, 255));
    old = SelectObject(hdc, b);
    Ellipse(hdc, x3, y3, x3 + d, y3 + d);
    SelectObject(hdc, old);
    DeleteObject(b);

    //Quarta Pallina
    b = CreateSolidBrush(RGB(0, 0, 0));
    old = SelectObject(hdc, b);
    Ellipse(hdc, x4, y4, x4 + d, y4 + d);
    SelectObject(hdc, old);
    DeleteObject(b);

    //Quinta Pallina
    b = CreateSolidBrush(RGB(255, 255, 255));
    old = SelectObject(hdc, b);
    Ellipse(hdc, x5, y5, x5 + d, y5 + d);
    SelectObject(hdc, old);
    DeleteObject(b);

    //Timer per la gestione dello spostamento della pallina
    std::chrono::steady_clock::time_point ora = std::chrono::steady_clock::now();
    long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(ora - last_tick).count();
    if (elapsed > 1) 
    {
        //Prima Pallina
        x1 += sin1;
        y += cos1;

        //Seconda Pallina
        x2 += sin2;
        y2 += cos2;
        
        //Terza Pallina
        x3 += sin3;
        y3 += cos3;

        //Quarta Pallina
        x4 += sin4;
        y4 += cos4;

        //Quinta Pallina
        x5 += sin5;
        y5 += cos5;

        last_tick = ora;
    }

    //Prima Pallina
    Rimbalzo(cos1, sin1, x1, y, bounds);
 
    //Seconda Pallina
    Rimbalzo(cos2, sin2, x2, y2, bounds);

    //Terza Pallina
    Rimbalzo(cos3, sin3, x3, y3, bounds);
 
    //Quarta Pallina
    Rimbalzo(cos4, sin4, x4, y4, bounds);
        
    //Quinta Pallina
    Rimbalzo(cos5, sin5, x5, y5, bounds);
}

void Rimbalzo(double& cos, double& sin, double& x, double& y, RECT bounds)
{
    if (y + d > bounds.bottom || y < bounds.top)
    {
        cos = -cos;

        if (y + d > bounds.bottom)
        {
            y--;
            x--;
        }
        else
        {
            y++;
            x++;
        }
    }
    else if (x < bounds.left || x + d > bounds.right)
    {
        sin = -sin;

        if (x < bounds.left)
        {
            x++;
            y++;
        }
        else
        {
            x--;
            y--;
        }
    }
}

//Funzione e relative Variabili Globali per la stampa degli FPS
int frame;
int frameToPrint = 0;
std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();
void DrawFPS(HDC hdc)
{
    frame++;
    std::chrono::steady_clock::time_point ora = std::chrono::steady_clock::now();
    long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(ora - last).count();
    if (elapsed > 1000000)  //1sec
    {
        last = ora;
        frameToPrint = frame;
        frame = 0;
    }
    string a = NumberToString(frameToPrint);
    TextOutA(hdc, 0, 0, a.c_str(), a.length());     //funzione che stampa la stringa degli FPS ( coordinata 0,0 )
}