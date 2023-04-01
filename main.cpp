#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include<stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <cMath>
#include<vector>
#include <fstream>
#include <sstream>

#define FILE_MENU_BLACK 0
#define FILE_MENU_RED 1
#define FILE_MENU_BLUE 2
#define FILE_MENU_Light_Blue 3
#define FILE_MENU_White 4
#define FILE_MENU_Yellow 5
#define FILE_MENU_Purple 6
#define FILE_MENU_Dark_Green 7
#define FILE_MENU_Brown 8
#define FILE_MENU_PINK 9



#define Line_chartesian 10
#define Line_polar 11
#define Line_DDA 12
#define Line_Midpoint 13

#define Circle_chartesian 14
#define Circle_polar 15
#define Circle_Midpoint 16

#define EllipseDirect 17
#define EllipsePolar 18

//-----------------------------------------------------------------------------------------#define LineC 19
#define LineC 19
#define CircleC 20
#define EllipseC 21

#define Circle_Filling 22

#define FILE_MENU_CLEAR 23
#define FILE_MENU_LOAD 24
#define FILE_MENU_SAVE 25



using namespace std;
typedef struct point
{
    int x;
    int y;
};
point winMin = {100, 100};
point winMax = {800, 400};
point p1 = {200, 200};
point p2 = {900, 300};
void shapes_menue(HWND);
HMENU hshape;
HMENU hmenu;
static vector<int> AllPoints;
static int xc, yc, x, y,Algo=0,r,choice= -1;
ifstream in;
string line;
vector<int> vect;
static COLORREF color;
HDC hdc;

void save(int x,int y)
{
    AllPoints.push_back(x);
    AllPoints.push_back(y);
}
void Extract()
{
    ofstream out;
    out.open("text.txt");
    for(int i = 0; i<AllPoints.size(); i+=2)
        out << AllPoints[i] << "," << AllPoints[i+1] << ",";

    out.close();
}

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");


int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_CROSS);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
int Round(double x)
{
    return (int)(x + 0.5);
}
void Draw_8_points(HDC hdc, int xc, int yc,int x,int y,COLORREF color = RGB(255, 0, 0))
{
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc - x, yc - y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc + y, yc + x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc - y, yc - x, color);
    SetPixel(hdc, xc + y, yc - x, color);

    save(xc + x, yc + y);
    save(xc - x, yc + y);
    save(xc - x, yc - y);
    save(xc + x, yc - y);
    save(xc + y, yc + x);
    save(xc - y, yc + x);
    save(xc - y, yc - x);
    save(xc + y, yc - x);
}
void Drawline_DDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color = RGB(0, 0, 255))
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dy) <= abs(dx))
    {
        double m = (double)dy / dx;
        if (x1 > x2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        double y = y1;
        int x = x1;
        SetPixel(hdc, x1, y1, color);
        save(x1,y1);
        while (x < x2)
        {
            x++;
            y += m;
            SetPixel(hdc, x,Round(y), color);
            save(x,Round(y));
        }
    }
    else
    {
        double m = (double)dx / dy;

        if (y1 > y2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        int y = y1;
        double x = x1;
        SetPixel(hdc, x1, y1, color);
        save(x1,y1);
        while (y < y2)
        {
            y++;
            x += m;
            SetPixel(hdc,Round(x),y, color);
            save(Round(x),y);
        }
    }

}
void Drawfilling(HDC hdc,int xc,int yc,int x,int y,int choice,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
            save(xc+x,yc+y);

            save(xc-x,yc+y);

    if(choice==4)
    {
        Drawline_DDA( hdc, xc,yc,xc+x,yc+y,RGB(0,0,0));
        Drawline_DDA( hdc, xc, yc,xc+y,yc+x,RGB(0,0,0));
    }
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc-x,yc-y,color);
                save(xc+x,yc-y);
            save(xc-x,yc-y);

    if(choice==2)
    {
        Drawline_DDA( hdc, xc, yc,xc-x,yc-y,RGB(0,0,0));
        Drawline_DDA( hdc, xc, yc,xc-y,yc-x,RGB(0,0,0));
    }
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
                save(xc+y,yc+x);
            save(xc-y,yc-x);

    if(choice==3)
    {
        Drawline_DDA( hdc, xc, yc,xc-y,yc+x,RGB(0,0,0));
        Drawline_DDA( hdc, xc, yc,xc-x,yc+y,RGB(0,0,0 ));
    }
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc+y,yc-x,color);
                save(xc-y,yc+x);
            save(xc+y,yc-x);

    if(choice==1)
    {
        Drawline_DDA( hdc, xc, yc,xc+y,yc-x,RGB(0,0,0 ));
        Drawline_DDA( hdc, xc, yc,xc+x,yc-y,RGB(0,0,0));
    }
}
void Drawcircle_cfill(HDC hdc,int xc,int yc, int R,int choise,COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
    int d1=3, d2=5-2*R;
    Drawfilling(hdc,xc,yc,x,y,choise,color);
    while(x<y)
    {
        if(d<0)
        {
            d+=d1;
            d2+=2;
        }
        else
        {
            d+=d2;
            d2+=4;
            y--;
        }
        d1+=2;
        x++;
        Drawfilling(hdc,xc,yc,x,y,choise,color);
    }
}
void Drawelipse(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);

    save(xc+x,yc+y);
    save(xc+x,yc-y);
    save(xc-x,yc+y);
    save(xc-x,yc-y);
}

void PointClipping(HDC hdc,int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF color)
{
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)
    {
        SetPixel(hdc,  x,  y, color);
        save(x,y);



    }
}
union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};
OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out;
    out.All=0;
    if(x<xleft)out.left=1;
    else if(x>xright)out.right=1;
    if(y<ytop)out.top=1;
    else if(y>ybottom)out.bottom=1;
    return out;
}
void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}
void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    //trival case
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {
        /*MoveToEx(hdc,Round(x1),Round(y1),NULL);
        LineTo(hdc,Round(x2),Round(y2));*/
        Drawline_DDA(hdc,x1,y1,x2,y2,color);


    }
}
void Drawline_Polar(HDC hdc,int x1,int y1,int x2,int y2, COLORREF color )
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    double dt = 1.0 / max(dx, dy);
    for (double t = 0; t <= 1; t += dt)
    {
        int x = Round(x1 + t * dx);
        int y = Round(y1 + t * dy);
        SetPixel(hdc, x, y, color);
        save(x,y);
    }
}
void Drawline_Cartesian(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color )
{
    int dy = y2 - y1;
    int dx = x2 - x1;
    if (abs(dy) <= abs(dx))
    {
        double m =(double) dy / dx;
        double c = y1 - m * x1;
        if (x1 > x2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        for (int x = x1; x <= x2; x++)
        {
            int y = Round(m * x + c);
            SetPixel(hdc, x, y, color);
            save(x,y);
        }
    }
    else
    {
        double m = (double)dx / dy;
        double c = x1 - m * y1;
        if (y1 > y2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        for (int y = y1; y <= y2; y++)
        {
            int x =Round( m * y + c);
            SetPixel(hdc, x, y, color);
            save(x,y);
        }
    }

}
void Drawline_Midpoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color )
{
    int dy = y2 - y1;
    int dx = x2 - x1;
    if (dy < dx)
    {
        if (x2 < x1)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        int x = x1;
        int y = y1;
        SetPixel(hdc,x1,y1,color);
        save(x1,y1);
        int d = dx - 2*dy;
        int d1 = -2 * dy;
        int d2 = 2 * dx - 2 * dy;
        while (x < x2)
        {
            if (d > 0)
            {
                x++;
                d += d1;
            }
            else
            {
                x++;
                y++;
                d += d2;
            }
            SetPixel(hdc, x, y, color);
            save(x,y);
        }
    }
    else
    {

        if (y2 < y1)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        int x = x1;
        int y = y1;
        SetPixel(hdc, x1, y1, color);
        save(x1,y1);
        int d = 2 * dx - dy;
        int d1 = 2 * dx;
        int d2 = 2 * dx - 2 * dy;
        while (y < y2)
        {
            if (d < 0)
            {
                y++;
                d += d1;
            }
            else
            {
                x++;
                y++;
                d += d2;
            }
            SetPixel(hdc, x, y, color);
            save(x,y);
        }
    }
}

void DrawCircle_Polar(HDC hdc, int xc, int yc, int R, COLORREF color )
{
    double dtheta = 1.0 / R;
    double c = cos(dtheta);
    double s = sin(dtheta);
    double x = R, y = 0;
    Draw_8_points(hdc, xc, yc, R, 0, color);


    while (x > y)
    {
        double x1 = (double)x * c - y * s;
        y = x * s + y * c;
        x = x1;
        Draw_8_points(hdc, xc, yc, Round(x), Round(y), color);


    }
}

void DrawCircle_Cartesian(HDC hdc, int xc, int yc, int R, COLORREF color )
{
    double slope = (double)-xc / yc;
    if (slope <= 1)
    {
        int x = 0, y = R;
        Draw_8_points(hdc, xc, yc, 0, R, color);
        while (x < y)
        {
            x++;
            y = sqrt(R * R - x * x);
            Draw_8_points(hdc, xc, yc, x, y, color);
        }
    }
    else
    {
        int y = 0, x = R;
        Draw_8_points(hdc, xc, yc, 0, R, color);
        while (x < y)
        {
            y++;
            x = sqrt(R * R - y * y);
            Draw_8_points(hdc, xc, yc, x, y, color);

        }
    }
}

void DrawCircle_Midpoint(HDC hdc, int xc, int yc, int R, COLORREF color )
{
    int x = 0, y = R;
    int d = 1 - R;
    int d1 = 3, d2 = 5 - 2 * R;
    Draw_8_points(hdc, xc, yc, x, y, color);
    while (x < y)
    {
        if (d <= 0)
        {
            d += d1;
            d2 += 2;

        }
        else
        {
            d += d2;
            d2 += 4;
            y--;
        }
        x++;
        d1 += 2;
        Draw_8_points(hdc, xc, yc, x, y, color);
    }
}

void Ellipse_Direct(HDC hdc, int xc, int yc, int A,int B, COLORREF color )
{
    int x = 0, y = B;
    Drawelipse(hdc,xc, yc, x, y, color);
    while (x*(B*B) < ((A*A)*y))
    {
        x++;
        y =round( sqrt(((B*B)*((A*A)-(x*x)))/(A*A)));
        Drawelipse(hdc,xc, yc, x, y, color);
    }
    y = 0, x = A;
    Drawelipse(hdc,xc, yc, x, y, color);
    while (x*(B*B) >((A*A)*y))
    {
        y++;
        x = round (sqrt(((A*A)*((B*B) - (y * y)))/(B*B)));
        Drawelipse(hdc,xc, yc, x, y, color);
    }
}
void Ellipse_Polar (HDC hdc,int xc,int yc,int A,int B,COLORREF color)
{
    for (double theta = 0.0; theta < 6.28; theta += 0.01)
    {
        int x = Round(xc + A* cos(theta));
        int y = Round(yc + B* sin(theta));

        SetPixel(hdc, x, y, color);
        save(x,y);
    }
}
void Line_clipping(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color )
{
    int dx = x2 - x1 ;
    int dy = y2 - y1 ;
    if (abs(dy) <= abs(dx))
    {
        double m = (double)dy / dx;
        if (x1 > x2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        double y = y1;
        int x = x1;
        while (x < x2)
        {
            x++;
            y += m;
            CohenSuth( hdc, x1,y1, x, y,winMin.x, winMin.y, winMax.x,winMax.y);

        }
    }
    else
    {
        double m = (double)dx / dy;

        if (y1 > y2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        int y = y1;
        double x = x1;
        while (y < y2)
        {
            y++;
            x += m;
            CohenSuth( hdc, x1,y1, x, y,winMin.x, winMin.y, winMax.x,winMax.y);

        }
    }
}

void Circle_clipping(HDC hdc,int xc,int yc,int xc1,int yc1,int r,COLORREF color) // using polar
{
    double dtheta=1.0/r;
    for( double theta=0; theta< 6.28; theta+=dtheta)
    {
        int x=round(xc+r*cos(theta));
        int y=round(yc+r*sin(theta));
        int R=round(sqrt(pow(xc1-x,2)+pow(yc1-y,2)));
        PointClipping( hdc,x, y, winMin.x,winMin.y,winMax.x,winMax.y,color);

    }
}

void Ellipse_clipping (HDC hdc,int xc,int yc,int A,int B,COLORREF color)
{
    for (double theta = 0.0; theta < 6.28; theta += 0.01)
    {
        int x = Round(xc + A* cos(theta));
        int y = Round(yc + B* sin(theta));
        PointClipping( hdc,x, y, winMin.x,winMin.y,winMax.x,winMax.y,color);
    }
}
void Draw_Rectangle(HDC hdc,point winMin, point winMax)
{
    int x, y;
    x = winMin.x;
    for(y = winMin.y; y < winMax.y; y++)
    {
        SetPixel(hdc,x,y, RGB(0,0,0));
                save(x,y);

    }
    y = winMin.y;
    for(x = winMin.x; x < winMax.x; x++)
    {
        SetPixel(hdc,x,y,  RGB(0,0,0));
                save(x,y);

    }
    x = winMax.x;
    for(y = winMin.y; y < winMax.y; y++)
    {
        SetPixel(hdc,x,y,  RGB(0,0,0));
                save(x,y);

    }
    y = winMax.y;
    for(x = winMin.x; x < winMax.x; x++)
    {
        SetPixel(hdc,x,y,  RGB(0,0,0));
                save(x,y);

    }
}
void load (vector<int> &arr,string Nums)
{
    stringstream ss(Nums);

    for (int i; ss >> i;)
    {
        arr.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }
}
void prev(HDC hdc,vector<int> numbers,COLORREF c)
{
    for(int i = 0; i<numbers.size(); i+=2)
    {
        SetPixel(hdc,numbers[i],numbers[i+1],c);
    }
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    hdc = GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
    case WM_LBUTTONDOWN:
        xc = LOWORD(lParam);
        yc = HIWORD(lParam);
        break;
    case WM_LBUTTONUP:
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        switch(Algo)
        {
        case 10:
            Drawline_Cartesian(hdc,xc,yc,x,y,color);
            break;
        case 11:
            Drawline_Polar(hdc,xc,yc,x,y,color);
            break;
        case 12:
            Drawline_DDA(hdc,xc,yc,x,y,color);
            break;
        case 13:
            Drawline_Midpoint(hdc,xc,yc,x,y,color);
            break;
        case 14:
            r = round(sqrt(pow(x-xc,2.0)+pow(y-yc,2.0)));
            DrawCircle_Cartesian(hdc,xc,yc,r,color);
            break;
        case 15:
            r = round(sqrt(pow(x-xc,2.0)+pow(y-yc,2.0)));
            DrawCircle_Polar(hdc,xc,yc,r,color);
            break;
        case 16:
            r = round(sqrt(pow(x-xc,2.0)+pow(y-yc,2.0)));
            DrawCircle_Midpoint(hdc,xc,yc,r,color);
            break;
        case 17:
            Ellipse_Direct(hdc,xc,yc,150,70,color);
            break;
        case 18:
            Ellipse_Polar(hdc,xc,yc,200,150,color);
            break;
        case 19:
            Line_clipping(hdc, xc, yc, x, y, color );
            break;

        case 20:
            r = round(sqrt(pow(x-xc,2.0)+pow(y-yc,2.0)));
            Circle_clipping(hdc,xc,yc,x,y,r,color);
            break;
        case 21:
            Ellipse_clipping (hdc,xc,yc,300,150,color);
            break;
        case 22:
            r = round(sqrt(pow(x-xc,2.0)+pow(y-yc,2.0)));
            Drawcircle_cfill(hdc,xc,yc,r,choice,color);
        }
        break;
    case WM_RBUTTONDBLCLK:
        Draw_Rectangle(hdc,winMin,winMax);


    case WM_CREATE:
        shapes_menue(hwnd);
        break;
    case WM_COMMAND:
        switch(wParam)
        {
        case 0:
            color = RGB(0,0,0);
            break;
        case 1:
            color = RGB(255,0,0);
            break;
        case 2:
            color = RGB(0,0,255);
            break;
        case 3:
            color = RGB(0,255,255);
            break;
        case 4:
            color = RGB(255,255,255);
            break;
        case 5:
            color = RGB(255,255,0);
            break;
        case 6:
            color = RGB(100,0,100);
            break;
        case 7:
            color = RGB(0,100,100);
            break;
        case 8:
            color = RGB(123,100,100);
            break;
        case 9:
            color = RGB(255,0,255);
            break;
        case 10:
            Algo = 10;
            break;
        case 11:
            Algo = 11;
            break;
        case 12:
            Algo = 12;
            break;
        case 13:
            Algo = 13;
            break;
        case 14:
            Algo = 14;
            break;
        case 15:
            Algo = 15;
            break;
        case 16:
            Algo = 16;
            break;
        case 17:
            Algo = 17;
            break;
        case 18:
            Algo = 18;
            break;
        case 19:
            Algo = 19;
            break;
        case 20:
            Algo = 20;
            break;
        case 21:
            Algo = 21;
            break;
        case 22:
            cout<<"Enter your Circle filling choice";
            cin>>choice;
            Algo = 22;
            break;
        case 23:
           ShowWindow(hwnd, SW_HIDE);
            MessageBeep(MB_OK);
            ShowWindow(hwnd, SW_SHOW);
            break;
        case 24:
           in.open("text.txt");
            getline(in, line);
            load(vect, line);
            hdc = GetDC(hwnd);
            AllPoints = vect;
            prev(hdc,vect,color);
            in.close();
            break;
        case 25:
           Extract();
            break;
        }

    break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
void shapes_menue(HWND hwnd)
{
    hmenu = CreateMenu();
    HMENU hfileMenu =CreateMenu();
    HMENU hfileCOLOR=CreateMenu();
    hshape = CreateMenu();

    HMENU shapes_list= CreateMenu();
    HMENU line_algos= CreateMenu();
    HMENU circle_algos= CreateMenu();
    HMENU ellipse_algos= CreateMenu();

    HMENU clipping_list= CreateMenu();
    HMENU Lineclipping= CreateMenu();
    HMENU Circleclipping= CreateMenu();
    HMENU Ellipseclipping= CreateMenu();

    AppendMenu(hshape,MF_POPUP,(UINT_PTR)hfileMenu,"MENU");
    AppendMenu(hfileMenu,MF_STRING,FILE_MENU_CLEAR,"clear");
    AppendMenu(hfileMenu,MF_STRING,FILE_MENU_LOAD,"load");
    AppendMenu(hfileMenu,MF_STRING,FILE_MENU_SAVE,"save");

    AppendMenu(hshape,MF_POPUP,(UINT_PTR)shapes_list,"Shapes" );



    AppendMenu(shapes_list,MF_POPUP,(UINT_PTR)line_algos,"Line" );
    AppendMenu(line_algos,MF_STRING,Line_polar,"Chartesian Method" );
    AppendMenu(line_algos,MF_STRING,Line_chartesian,"Polar Method" );
    AppendMenu(line_algos,MF_STRING,Line_DDA,"Simple DDA Method" );
    AppendMenu(line_algos,MF_STRING,Line_Midpoint,"Midpoint Method" );

    AppendMenu(shapes_list,MF_POPUP,(UINT_PTR)circle_algos,"Circle" );
    AppendMenu(circle_algos,MF_STRING,Circle_chartesian,"Chartesian Method" );
    AppendMenu(circle_algos,MF_STRING,Circle_polar,"Polar Method" );
    AppendMenu(circle_algos,MF_STRING,Circle_Midpoint,"Midpoint Method" );
    AppendMenu(circle_algos,MF_STRING,Circle_Filling,"Filling" );

    AppendMenu(shapes_list,MF_POPUP,(UINT_PTR)ellipse_algos,"Ellipse" );
    AppendMenu(ellipse_algos,MF_STRING,EllipseDirect,"Direct Method" );
    AppendMenu(ellipse_algos,MF_STRING,EllipsePolar,"Polar Method" );

//-----------------------------------------------------------------------------------------

    AppendMenu(hshape,MF_POPUP,(UINT_PTR)clipping_list,"Clipping" );
    AppendMenu(clipping_list,MF_STRING,LineC,"Line Clipping" );
    AppendMenu(clipping_list,MF_STRING,CircleC,"Circle Clipping" );
    AppendMenu(clipping_list,MF_STRING,EllipseC,"Ellipse Clipping" );


//-----------------------------------------------------------------------------------------
    AppendMenu(hshape,MF_POPUP,(UINT_PTR)hfileCOLOR,"COLOR");
    AppendMenu(hfileCOLOR,MF_STRING,FILE_MENU_BLACK,"Black");
    AppendMenu(hfileCOLOR,MF_STRING,FILE_MENU_RED,"Red");
    AppendMenu(hfileCOLOR,MF_STRING,FILE_MENU_BLUE,"Blue");
    AppendMenu(hfileCOLOR,MF_STRING,FILE_MENU_Light_Blue,"Light Blue");
    AppendMenu(hfileCOLOR,MF_STRING,FILE_MENU_White,"White");
    AppendMenu(hfileCOLOR,MF_STRING,FILE_MENU_Yellow,"Yellow");
    AppendMenu(hfileCOLOR,MF_STRING,FILE_MENU_Purple,"Purple");
    AppendMenu(hfileCOLOR,MF_STRING,FILE_MENU_Dark_Green,"Dark Green");
    AppendMenu(hfileCOLOR,MF_STRING,FILE_MENU_Brown,"Brown");
    AppendMenu(hfileCOLOR,MF_STRING,FILE_MENU_PINK,"PINK");


    SetMenu(hwnd,hshape);

}
