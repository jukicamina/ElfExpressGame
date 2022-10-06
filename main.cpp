#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include <iostream>
#include <vector>
#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>
#include <cstdlib>
#include<time.h>
#define PRITISNUTO(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

const int TIMER_ID = 1;
const int BACKGROUND_TIMER_ID = 2;
const int START_TIMER_ID = 3;
const int END_TIMER_ID = 4;
const int GAME_TIMER_ID = 5;
const int LOLLIPOP_TIMER_ID = 6;
const int BALLOON_TIMER_ID = 7;
const int BIRD_TIMER_ID = 8;
const int TIME_BONUS_TIMER_ID = 9;
const int SECONDS10_BONUS_TIMER_ID = 10;
const int POINTS10_TIMER_ID = 11;
const int POINTS15_TIMER_ID = 12;
const int POINTSmin10_TIMER_ID = 13;

class Object
{
public:
    int x, y, height, width, n;
    int dx = 0;
    int dy = 0;
};

Object Elf;
static int ElfCounterX = 0, ElfCounterY = 0, BalloonCounterX = 0, BalloonCounterY = 0, BirdCounterX = 0, BirdCounterY = 0;
Object background, backgroundBeginWindow, backgroundEndWindow;
static int startPoints, gameTime;
Object ScoreTable, Points10, PointsMin10, Points15, Seconds;
Object PlayButton, TryAgainButton, ExitButton;
std::vector<Object> presents, lollipops, balloons, houses, birds, clocks;

class Chimney
{
public:
    int chimneyLeft, chimneyRight, height;
};

Chimney chimney;

BITMAP bitmap;
HBITMAP hbmBackground, hbmBackgroundBeginWindow, hbmBackgroundEndWindow;
HBITMAP hbmElf, hbmElfMask;
HBITMAP hbmHouse1, hbmHouse1Mask, hbmHouse2, hbmHouse2Mask, hbmHouse3, hbmHouse3Mask, hbmHouse4, hbmHouse4Mask, hbmHouse5, hbmHouse5Mask;
HBITMAP hbmPresent, hbmPresentMask;
HBITMAP hbmLollipop, hbmLollipopMask;
HBITMAP hbmBalloon, hbmBalloonMask;
HBITMAP hbmBird, hbmBirdMask;
HBITMAP hbmSnowman, hbmSnowmanMask;
HBITMAP hbmPurpleTree, hbmPurpleTreeMask, hbmGreenTree, hbmGreenTreeMask, hbmPinkTree, hbmPinkTreeMask;
HBITMAP hbmClock, hbmClockMask;
HBITMAP hbmScoreTable, hbmScoreTableMask;
HBITMAP hbmPoints10, hbmPoints10Mask;
HBITMAP hbmPointsMin10, hbmPointsMin10Mask;
HBITMAP hbmPoints15, hbmPoints15Mask;
HBITMAP hbmSeconds, hbmSecondsMask;
HBITMAP hbmPlay, hbmPlayMask;
HBITMAP hbmTryAgain, hbmTryAgainMask;
HBITMAP hbmExit, hbmExitMask;

void GameInitialize();
void CheckInput(HWND);
void Update();
void DrawGame(HDC, RECT*, HWND);
void makeHouse(int);
Chimney chimneyCoordinates(Object);
void finishTheGame(HWND);
void DrawWindow(HDC, RECT*);
void DrawBackground(HDC, RECT*);

static int TenPointsPic = 0, FifteenPointsPic = 0, TenSecondsPic = 0, MinusTenPointsPic = 0;
void TenPointsPicFunction(HDC, HDC, int, int);
void FifteenPointsPicFunction(HDC, HDC);
void MinusTenPointsPicFunction(HDC, HDC, int, int);
void TenSecondsPicFunction(HDC, HDC, int, int);

void UpdateHouse();
void UpdateClock();
void UpdateScore(int);
void UpdateBalloon();
void UpdateLollipop();
void UpdatePresent();
void UpdateBird();
void UpdateElf();

void presentSound();
void pointsSound();
void hitSound();
void timeLeakSound();

HWND hwndWindow;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK GameWindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("Elf Express");

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = "Parent";
    wincl.lpfnWndProc = GameWindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = (HCURSOR)LoadImage(NULL, "cursor.cur", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;
    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                       /* Extended possibilites for variation */
               "Parent",                /* Classname */
               _T("Elf Express"),       /* Title Text */
               WS_SYSMENU | WS_CAPTION, /* default window */
               60,                      /* Windows decides the position */
               60,                      /* where the window ends up on the screen */
               1030,                    /* The programs width */
               600,                     /* and height in pixels */
               HWND_DESKTOP,            /* The window is a child-window to desktop */
               NULL,                    /* No menu */
               hThisInstance,           /* Program Instance handler */
               NULL                     /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* The Window structure */
    wincl.lpszClassName = "ChoiceWindow";
    wincl.lpfnWndProc = WindowProcedure;
    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwndWindow = CreateWindowEx (
                     0,                   /* Extended possibilites for variation */
                     "ChoiceWindow",       /* Classname */
                     _T("Elf Express"),   /* Title Text */
                     WS_CAPTION,          /* default window */
                     60,                  /* Windows decides the position */
                     60,                  /* where the window ends up on the screen */
                     1030,                /* The programs width */
                     600,                 /* and height in pixels */
                     hwnd,                /* The window is a child-window to desktop */
                     NULL,                /* No menu */
                     (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE),/* Program Instance handler */
                     NULL                 /* No Window Creation data */
                 );

    /* Make the window visible on the screen */
    ShowWindow (hwndWindow, nCmdShow);
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

int START = 0;
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    BITMAP bitmap;
    int playButtonEndX, playButtonEndY, TryAgainButtonW, TryAgainButtonH, ExitButtonW, ExitButtonH;
    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    if(START == 0)
    {
        switch (message)                  /* handle the messages */
        {
        case WM_CREATE:
        {
            SetTimer(hwnd, START_TIMER_ID, 1, NULL);

            mciSendString("close gameSound", NULL, 0, NULL);
            mciSendString("open sounds/gameSound.wav type waveaudio alias gameSound", NULL, 0, NULL);
            mciSendString("play gameSound notify", NULL, 0, hwnd);

            hbmBackgroundBeginWindow = (HBITMAP)LoadImage(NULL, "backgrounds/firstWindow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            GetObject(hbmBackgroundBeginWindow, sizeof(bitmap), &bitmap);
            backgroundBeginWindow.width = bitmap.bmWidth;
            backgroundBeginWindow.height = bitmap.bmHeight;

            hbmPlay = (HBITMAP)LoadImage(NULL, "buttons/playWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            hbmPlayMask = (HBITMAP)LoadImage(NULL, "buttons/playBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            GetObject(hbmPlay, sizeof(bitmap), &bitmap);
            PlayButton.width = bitmap.bmWidth;
            PlayButton.height = bitmap.bmHeight;
            break;
        }
        case WM_TIMER:
        {
            switch(wParam)
            {
            case START_TIMER_ID:
            {
                HDC hdc = GetDC(hwnd);
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                DrawWindow(hdc, &clientRect);
                ReleaseDC(hwnd, hdc);
                break;
            }
            }
            break;
        }
        case WM_KEYDOWN:
        {
            if(PRITISNUTO(0x50))//P as PLAY
            {
                HWND parent = GetWindow(hwnd, GW_OWNER);
                DestroyWindow(hwndWindow);
                SendMessage(parent, WM_SETREDRAW, 0, 0);
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            HWND parent = GetWindow(hwnd, GW_OWNER);
            playButtonEndX = (PlayButton.x + PlayButton.width);
            playButtonEndY = (PlayButton.y + PlayButton.height);
            if(x > PlayButton.x && x < playButtonEndX && y > PlayButton.y && y < playButtonEndY)
            {
                DestroyWindow(hwndWindow);
                SendMessage(parent, WM_SETREDRAW, 0, 0);
            }
            break;
        }
        case WM_DESTROY:
            KillTimer(hwnd, START_TIMER_ID);
            mciSendString("close gameSound", NULL, 0, NULL);
            break;
        default:                          /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
        }
    }
    if(START >= 1)
    {
        switch (message)                  /* handle the messages */
        {
        case WM_CREATE:
        {
            SetTimer(hwnd, END_TIMER_ID, 40, NULL);

            mciSendString("close gameSound", NULL, 0, NULL);
            mciSendString("open sounds/gameSound.wav type waveaudio alias gameSound", NULL, 0, NULL);
            mciSendString("play gameSound notify", NULL, 0, hwnd);

            hbmBackgroundEndWindow = (HBITMAP)LoadImage(NULL, "backgrounds/endWindow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            GetObject(hbmBackgroundEndWindow, sizeof(bitmap), &bitmap);
            backgroundEndWindow.height = bitmap.bmHeight;
            backgroundEndWindow.width = bitmap.bmWidth;

            hbmExit = (HBITMAP)LoadImage(NULL, "buttons/exitWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            hbmExitMask = (HBITMAP)LoadImage(NULL, "buttons/exitBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            GetObject(hbmExit, sizeof(bitmap), &bitmap);
            ExitButton.height = bitmap.bmHeight;
            ExitButton.width = bitmap.bmWidth;

            hbmTryAgain = (HBITMAP)LoadImage(NULL, "buttons/tryAgainWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            hbmTryAgainMask = (HBITMAP)LoadImage(NULL, "buttons/tryAgainBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            GetObject(hbmTryAgain, sizeof(bitmap), &bitmap);
            TryAgainButton.height = bitmap.bmHeight;
            TryAgainButton.width = bitmap.bmWidth;
            break;
        }
        case WM_TIMER:
        {
            switch(wParam)
            {
            case END_TIMER_ID:
            {
                HDC hdc = GetDC(hwnd);
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                DrawWindow(hdc, &clientRect);
                ReleaseDC(hwnd, hdc);
                break;
            }
            }
            break;
        }
        case WM_KEYDOWN:
        {
            HWND parent = GetWindow(hwnd, GW_OWNER);
            if(PRITISNUTO(0x45))//E as EXIT
            {
                SendMessage(parent, WM_DESTROY, 0, 0);
            }
            if(PRITISNUTO(0x41))//A as AGAIN
            {
                DestroyWindow(hwndWindow);
                SendMessage(parent, WM_SETREDRAW, 0, 0);
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            HWND parent = GetWindow(hwnd, GW_OWNER);
            TryAgainButtonW = TryAgainButton.x + TryAgainButton.width;
            TryAgainButtonH = TryAgainButton.y + TryAgainButton.height;
            ExitButtonW = ExitButton.x + ExitButton.width;
            ExitButtonH = ExitButton.y + ExitButton.height;
            if(x > TryAgainButton.x && x < TryAgainButtonW && y > TryAgainButton.y && y < TryAgainButtonH)
            {
                DestroyWindow(hwndWindow);
                SendMessage(parent, WM_SETREDRAW, 0, 0);
            }
            if(x > ExitButton.x && x < ExitButtonW && y > ExitButton.y && y < ExitButtonH)
            {
                SendMessage(parent, WM_DESTROY, 0, 0);
            }
            break;
        }
        case WM_DESTROY:
            KillTimer(hwnd, END_TIMER_ID);
            mciSendString("close gameSound", NULL, 0, NULL);
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
        }
    }
    return 0;
}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK GameWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
    {
        SetTimer(hwnd, BACKGROUND_TIMER_ID, 100, NULL);
        break;
    }
    case WM_SETREDRAW:
    {
        startPoints = 0;
        gameTime = 59;
        Elf.x = 0;
        Elf.y = 150;
        KillTimer(hwnd, BACKGROUND_TIMER_ID);
        SetTimer(hwnd, TIMER_ID, 100, NULL);
        SetTimer(hwnd, GAME_TIMER_ID, 1000, NULL);
        SetTimer(hwnd, TIME_BONUS_TIMER_ID, 9000, NULL);
        SetTimer(hwnd, BALLOON_TIMER_ID, 13000, NULL);
        SetTimer(hwnd, LOLLIPOP_TIMER_ID, 7000, NULL);
        SetTimer(hwnd, BIRD_TIMER_ID, 11000, NULL);
        GameInitialize();
        break;
    }
    case WM_KEYDOWN:
    {
        CheckInput(hwnd);
        break;
    }
    case WM_TIMER:
    {
        BITMAP bitmap;
        HDC hdc = GetDC(hwnd);
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        switch(wParam)
        {
        case TIMER_ID:
        {
            Update();
            DrawGame(hdc, &clientRect, hwnd);
            ReleaseDC(hwnd, hdc);
            break;
        }
        case GAME_TIMER_ID:
        {
            gameTime-=1;;
            break;
        }
        case TIME_BONUS_TIMER_ID:
        {
            Object clock;
            GetObject(hbmClock, sizeof(bitmap), &bitmap);
            clock.height = bitmap.bmHeight;
            clock.width = bitmap.bmWidth;
            clock.dx = 10;
            clock.x = 1024;
            srand(time(0));
            clock.y = rand() % (285 - clock.height) + 1;
            clocks.push_back(clock);
            break;
        }
        case LOLLIPOP_TIMER_ID:
        {
            Object lollipop;
            GetObject(hbmLollipop, sizeof(bitmap), &bitmap);
            lollipop.height = bitmap.bmHeight;
            lollipop.width = bitmap.bmWidth;
            lollipop.dx = 10;
            lollipop.x = 1024;
            srand(time(0));
            lollipop.y = rand()%285;
            lollipops.push_back(lollipop);
            break;
        }
        case BALLOON_TIMER_ID:
        {
            Object balloon;
            GetObject(hbmBalloon, sizeof(bitmap), &bitmap);
            balloon.height = bitmap.bmHeight / 2;
            balloon.width = bitmap.bmWidth / 5;
            balloon.dx = 20;
            balloon.x = 1024;
            srand(time(0));
            balloon.y = rand()%(285 - balloon.height);
            balloons.push_back(balloon);
            break;
        }
        case BIRD_TIMER_ID:
        {
            Object bird;
            GetObject(hbmBird, sizeof(bitmap), &bitmap);
            bird.height = bitmap.bmHeight / 2;
            bird.width = bitmap.bmWidth / 8;
            bird.dx = 20;
            bird.x = 1024;
            srand(time(0));
            bird.y = rand()%285;
            birds.push_back(bird);
            break;
        }

        case POINTS15_TIMER_ID:
        {
            FifteenPointsPic = 0;
            break;
        }
        case POINTS10_TIMER_ID:
        {
            TenPointsPic = 0;
            break;
        }
        case SECONDS10_BONUS_TIMER_ID:
        {
            TenSecondsPic = 0;
            break;
        }
        case POINTSmin10_TIMER_ID:
        {
            MinusTenPointsPic = 0;
            break;
        }
        case BACKGROUND_TIMER_ID:
        {
            DrawBackground(hdc, &clientRect);
            ReleaseDC(hwnd, hdc);
            break;
        }
        }
        break;
    }
    case WM_DESTROY:
        KillTimer(hwnd, TIMER_ID);
        KillTimer(hwnd, GAME_TIMER_ID);
        KillTimer(hwnd, TIME_BONUS_TIMER_ID);
        KillTimer(hwnd, BALLOON_TIMER_ID);
        KillTimer(hwnd, LOLLIPOP_TIMER_ID);
        KillTimer(hwnd, BIRD_TIMER_ID);
        KillTimer(hwnd, POINTS15_TIMER_ID);
        KillTimer(hwnd, POINTS10_TIMER_ID);
        KillTimer(hwnd, POINTSmin10_TIMER_ID);
        KillTimer(hwnd, SECONDS10_BONUS_TIMER_ID);
        PostQuitMessage (0);                     /* send a WM_QUIT to the message queue */

        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void DrawWindow(HDC hdc, RECT *rect)
{
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect->right, rect->bottom);
    HBITMAP hbmBufferOld = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

    HDC hdcMem = CreateCompatibleDC(hdc);
    if(START == 0)
    {
        HBITMAP hbmMemOld = (HBITMAP)SelectObject(hdcMem, hbmBackgroundBeginWindow);

        BitBlt(hdcBuffer, backgroundBeginWindow.x, backgroundBeginWindow.y, backgroundBeginWindow.width, backgroundBeginWindow.height, hdcMem, 0, 0, SRCCOPY);
        PlayButton.x = rect->right/2 - PlayButton.width/2;
        PlayButton.y = 273;
        SelectObject(hdcMem, hbmPlay);
        BitBlt(hdcBuffer, PlayButton.x, PlayButton.y, PlayButton.width, PlayButton.height, hdcMem, 0, 0,SRCAND);
        SelectObject(hdcMem, hbmPlayMask);
        BitBlt(hdcBuffer, PlayButton.x, PlayButton.y,PlayButton.width, PlayButton.height, hdcMem, 0, 0,SRCPAINT);

        SelectObject(hdcMem, hbmMemOld);
        DeleteDC(hdcMem);

        BitBlt(hdc, 0, 0, rect->right, rect->bottom, hdcBuffer, 0, 0, SRCCOPY);

        SelectObject(hdcBuffer, hbmBufferOld);
        DeleteDC(hdcBuffer);
        DeleteObject(hbmBuffer);
    }

    else if(START >= 1)
    {
        HBITMAP hBmMemOld = (HBITMAP)SelectObject(hdcMem, hbmBackgroundEndWindow);

        BitBlt(hdcBuffer, backgroundEndWindow.x, backgroundEndWindow.y, backgroundEndWindow.width, backgroundEndWindow.height, hdcMem, 0, 0, SRCCOPY);

        TryAgainButton.x = 110;
        TryAgainButton.y = 360;
        SelectObject(hdcMem, hbmTryAgain);
        BitBlt(hdcBuffer, TryAgainButton.x, TryAgainButton.y, TryAgainButton.width, TryAgainButton.height, hdcMem, 0, 0,SRCAND);
        SelectObject(hdcMem, hbmTryAgainMask);
        BitBlt(hdcBuffer, TryAgainButton.x, TryAgainButton.y, TryAgainButton.width, TryAgainButton.height, hdcMem, 0, 0,SRCPAINT);

        ExitButton.x = 600;
        ExitButton.y = 360;
        SelectObject(hdcMem, hbmExit);
        BitBlt(hdcBuffer, ExitButton.x, ExitButton.y, ExitButton.width, ExitButton.height, hdcMem, 0, 0,SRCAND);
        SelectObject(hdcMem, hbmExitMask);
        BitBlt(hdcBuffer, ExitButton.x, ExitButton.y, ExitButton.width, ExitButton.height, hdcMem, 0, 0,SRCPAINT);

        SelectObject(hdcMem, hBmMemOld);
        DeleteDC(hdcMem);

        BitBlt(hdc, 0, 0, rect->right, rect->bottom, hdcBuffer, 0, 0, SRCCOPY);

        SelectObject(hdcBuffer, hbmBufferOld);
        DeleteDC(hdcBuffer);
        DeleteObject(hbmBuffer);

        SetTextColor(hdc, RGB(102,0,102));
        SetBkMode(hdc, TRANSPARENT);
        HFONT hFont = CreateFont (50, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
        SelectObject(hdc,(HFONT)(hFont));
        if(startPoints < 10)
        {
            TextOut(hdc, rect->right/2 - 10, rect->bottom/2 + 15, std::to_string(startPoints).c_str(), std::to_string(startPoints).length());
        }
        if(startPoints > 9 && startPoints < 100)
        {
            TextOut(hdc, rect->right/2 - 25, rect->bottom/2 + 15, std::to_string(startPoints).c_str(), std::to_string(startPoints).length());
        }
        if(startPoints > 99 && startPoints < 1000)
        {
            TextOut(hdc, rect->right/2 - 35, rect->bottom/2 + 15, std::to_string(startPoints).c_str(), std::to_string(startPoints).length());
        }
    }
}
void GameInitialize()
{
    BITMAP bitmap;
    hbmBackground = (HBITMAP)LoadImage(NULL, "backgrounds/background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBackground, sizeof(bitmap), &bitmap);
    background.height = bitmap.bmHeight;
    background.width = bitmap.bmWidth;

    hbmHouse1 = (HBITMAP)LoadImage(NULL, "houses/house1White.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmHouse1Mask = (HBITMAP)LoadImage(NULL, "houses/house1Black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmHouse2 = (HBITMAP)LoadImage(NULL, "houses/house2White.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmHouse2Mask = (HBITMAP)LoadImage(NULL, "houses/house2Black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmHouse3 = (HBITMAP)LoadImage(NULL, "houses/house3White.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmHouse3Mask = (HBITMAP)LoadImage(NULL, "houses/house3Black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmHouse4 = (HBITMAP)LoadImage(NULL, "houses/house4White.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
    hbmHouse4Mask = (HBITMAP)LoadImage(NULL, "houses/house4Black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmHouse5 = (HBITMAP)LoadImage(NULL, "houses/house5White.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmHouse5Mask = (HBITMAP)LoadImage(NULL, "houses/house5Black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmPurpleTree = (HBITMAP)LoadImage(NULL, "trees/purpleTreeWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmPurpleTreeMask = (HBITMAP)LoadImage(NULL, "trees/purpleTreeBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmGreenTree = (HBITMAP)LoadImage(NULL, "trees/greenTreeWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmGreenTreeMask = (HBITMAP)LoadImage(NULL, "trees/greenTreeBlack.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);

    hbmPinkTree = (HBITMAP)LoadImage(NULL, "trees/pinkTreeWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmPinkTreeMask = (HBITMAP)LoadImage(NULL, "trees/pinkTreeBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmSnowman = (HBITMAP)LoadImage(NULL, "trees/snowmanWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmSnowmanMask = (HBITMAP)LoadImage(NULL, "trees/snowmanBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmPresent = (HBITMAP)LoadImage(NULL, "other/presentWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmPresentMask = (HBITMAP)LoadImage(NULL, "other/presentBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmLollipop = (HBITMAP)LoadImage(NULL, "other/lollipopWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmLollipopMask = (HBITMAP)LoadImage(NULL, "other/lollipopBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmBird = (HBITMAP)LoadImage(NULL, "other/birdWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBirdMask = (HBITMAP)LoadImage(NULL, "other/birdBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmBalloon = (HBITMAP)LoadImage(NULL, "other/balloonWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBalloonMask = (HBITMAP)LoadImage(NULL, "other/balloonBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmClock = (HBITMAP)LoadImage(NULL, "other/clock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmClockMask = (HBITMAP)LoadImage(NULL, "other/clockBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmElf = (HBITMAP)LoadImage(NULL, "other/elfWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmElfMask = (HBITMAP)LoadImage(NULL, "other/elfBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmElf, sizeof(bitmap), &bitmap);
    Elf.height = bitmap.bmHeight/2;
    Elf.width = bitmap.bmWidth/5;
    Elf.x = 0;
    Elf.y = 150;
    Elf.dx = 15;
    Elf.dy = 15;

    hbmScoreTable = (HBITMAP)LoadImage(NULL, "points/scoreTableWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmScoreTableMask = (HBITMAP)LoadImage(NULL, "points/scoreTableBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmScoreTable, sizeof(bitmap), &bitmap);
    ScoreTable.height = bitmap.bmHeight;
    ScoreTable.width = bitmap.bmWidth;
    ScoreTable.x = 512 - (ScoreTable.width/2);
    ScoreTable.y = 5;

    hbmPoints10 = (HBITMAP)LoadImage(NULL, "points/plus10.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmPoints10Mask = (HBITMAP)LoadImage(NULL, "points/plus10Black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmPoints10Mask, sizeof(bitmap), &bitmap);
    Points10.height = bitmap.bmHeight;
    Points10.width = bitmap.bmWidth;

    hbmPointsMin10 = (HBITMAP)LoadImage(NULL, "points/min10White.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmPointsMin10Mask = (HBITMAP)LoadImage(NULL, "points/min10Black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmPointsMin10Mask, sizeof(bitmap), &bitmap);
    PointsMin10.height = bitmap.bmHeight;
    PointsMin10.width = bitmap.bmWidth;

    hbmPoints15 = (HBITMAP)LoadImage(NULL, "points/plus15.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmPoints15Mask = (HBITMAP)LoadImage(NULL, "points/plus15Black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmPoints15Mask, sizeof(bitmap), &bitmap);
    Points15.height = bitmap.bmHeight;
    Points15.width = bitmap.bmWidth;

    hbmSeconds = (HBITMAP)LoadImage(NULL, "points/plusTimeWhite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmSecondsMask = (HBITMAP)LoadImage(NULL, "points/plusTimeBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmSeconds, sizeof(bitmap), &bitmap);
    Seconds.height = bitmap.bmHeight;
    Seconds.width = bitmap.bmWidth;

    srand(time(0));
    makeHouse(rand() % 9 + 1);
}
void DrawBackground(HDC hdc, RECT* rect)
{
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hBmBuffer = CreateCompatibleBitmap(hdc, rect->right, rect->bottom);
    HBITMAP hBmBufferOld = (HBITMAP)SelectObject(hdcBuffer, hBmBuffer);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBmMemOld = (HBITMAP)SelectObject(hdcMem, hbmBackground);

    BitBlt(hdcBuffer, background.x, background.y, background.width, background.height, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hBmMemOld);
    DeleteDC(hdcMem);

    BitBlt(hdc, 0, 0, rect->right, rect->bottom, hdcBuffer, 0, 0, SRCCOPY);

    SelectObject(hdcBuffer, hBmBufferOld);
    DeleteDC(hdcBuffer);
    DeleteObject(hBmBuffer);
}
void DrawGame(HDC hdc, RECT *rect, HWND hwnd)
{
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect->right, rect->bottom);
    HBITMAP hbmBufferOld = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmMemOld = (HBITMAP)SelectObject(hdcMem, hbmBackground);

    BitBlt(hdcBuffer, background.x, background.y, background.width, background.height, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hbmElf);
    BitBlt(hdcBuffer, Elf.x, Elf.y, Elf.width, Elf.height, hdcMem, ElfCounterX*Elf.width, ElfCounterY*Elf.height, SRCAND);
    SelectObject(hdcMem, hbmElfMask);
    BitBlt(hdcBuffer, Elf.x, Elf.y, Elf.width, Elf.height, hdcMem, ElfCounterX*Elf.width, ElfCounterY*Elf.height, SRCPAINT);

    for(int i = 0; i<(int)(houses.size()); i++)
    {
        if(houses[i].n == 1)
        {
            SelectObject(hdcMem, hbmHouse1);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, hbmHouse1Mask);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCPAINT);
        }
        else if(houses[i].n == 2)
        {
            SelectObject(hdcMem, hbmHouse2);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, hbmHouse2Mask);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCPAINT);
        }
        else if(houses[i].n == 3)
        {
            SelectObject(hdcMem, hbmGreenTree);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, hbmGreenTreeMask);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCPAINT);
        }
        else if(houses[i].n == 4)
        {
            SelectObject(hdcMem, hbmHouse4);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, hbmHouse4Mask);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCPAINT);
        }
        else if(houses[i].n == 5)
        {
            SelectObject(hdcMem, hbmSnowman);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, hbmSnowmanMask);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCPAINT);
        }
        else if(houses[i].n == 6)
        {
            SelectObject(hdcMem, hbmHouse3);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, hbmHouse3Mask);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCPAINT);
        }
        else if(houses[i].n == 7)
        {
            SelectObject(hdcMem, hbmPinkTree);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, hbmPinkTreeMask);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCPAINT);
        }
        else if(houses[i].n == 8)
        {
            SelectObject(hdcMem, hbmHouse5);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, hbmHouse5Mask);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCPAINT);
        }
        else if(houses[i].n == 9)
        {
            SelectObject(hdcMem, hbmPurpleTree);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, hbmPurpleTreeMask);
            BitBlt(hdcBuffer, houses[i].x, houses[i].y, houses[i].width, houses[i].height, hdcMem, 0, 0, SRCPAINT);
        }

        for(int j = 0; j<(int)(presents.size()); j++)
        {
            Object pr = presents[j];
            int prX = presents[j].x;
            int prY = presents[j].y;
            SelectObject(hdcMem, hbmPresent);
            BitBlt(hdcBuffer, prX, prY, pr.width, pr.height, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, hbmPresentMask);
            BitBlt(hdcBuffer, prX, prY, pr.width, pr.height, hdcMem, 0, 0, SRCPAINT);

            if(houses[i].n > 0 && houses[i].n < 9)
            {
                chimney = chimneyCoordinates(houses[i]);
                if((chimney.height - 10 <= prY) && (chimney.height + 10 >= prY) && (chimney.chimneyLeft <= prX) && (chimney.chimneyRight >= prX))
                {
                    UpdateScore(15);
                    Points15.x = prX;
                    Points15.y = prY;
                    FifteenPointsPic = 1;
                    presents.erase(presents.begin() + j);
                    SetTimer(hwnd, POINTS15_TIMER_ID, 1000, NULL);
                    presentSound();
                }
            }
            else if(prY > rect->bottom)
            {
                FifteenPointsPic = 0;
                presents.erase(presents.begin() + j);
            }
        }
    }

    if((houses[0].x + houses[0].width) < rect->left)
    {
        houses.erase(houses.begin());
    }

    if((houses[houses.size()-1].x + houses[houses.size()-1].width) < rect->right)
    {
        srand(time(0));
        makeHouse(rand() % 9 + 1);
    }

    if(FifteenPointsPic != 0)
    {
        FifteenPointsPicFunction(hdcMem, hdcBuffer);
    }

    for(int k = 0; k <(int)(lollipops.size()); k++)
    {
        Object lp = lollipops[k];
        int lpX = lollipops[k].x;
        int lpY = lollipops[k].y;
        SelectObject(hdcMem, hbmLollipop);
        BitBlt(hdcBuffer, lpX, lpY, lp.width, lp.height, hdcMem, 0, 0, SRCAND);
        SelectObject(hdcMem, hbmLollipopMask);
        BitBlt(hdcBuffer, lpX, lpY, lp.width, lp.height, hdcMem, 0, 0, SRCPAINT);
        if(lpX < Elf.x)
        {
            TenPointsPic = 0;
        }
        else if((lpX <= Elf.x + Elf.width) && (lpY <= Elf.y + Elf.height) && (lpY + lp.height >= Elf.y))
        {
            Points10.x = lpX;
            Points10.y = lpY;
            UpdateScore(10);
            TenPointsPic = 1;
            lollipops.erase(lollipops.begin() + k);
            SetTimer(hwnd, POINTS10_TIMER_ID, 1000, NULL);
            pointsSound();
        }
    }
    if(TenPointsPic!=0)
    {
        TenPointsPicFunction(hdcMem, hdcBuffer, Points10.x, Points10.y);
    }

    if(!lollipops.empty() && (lollipops[0].x + lollipops[0].width < rect->left))
    {
        lollipops.erase(lollipops.begin());
    }

    for(int l = 0; l<(int)(birds.size()); l++)
    {
        Object b = birds[l];
        int bX = birds[l].x;
        int bY = birds[l].y;
        SelectObject(hdcMem, hbmBird);
        BitBlt(hdcBuffer, bX, bY, b.width, b.height, hdcMem, BirdCounterX*b.width, BirdCounterY*b.height, SRCAND);
        SelectObject(hdcMem, hbmBirdMask);
        BitBlt(hdcBuffer, bX, bY, b.width, b.height, hdcMem, BirdCounterX*b.width, BirdCounterY*b.height, SRCPAINT);

        if(bX + b.width < rect->left || bY + b.height > rect->bottom)
        {
            birds.erase(birds.begin()+l);
        }
        if(bX < Elf.x)
        {
            MinusTenPointsPic = 0;
        }
        else if((bX <= Elf.x + Elf.width) && (bY <= Elf.y + Elf.height) && (bY + b.height >= Elf.y))
        {
            PointsMin10.x = bX;
            PointsMin10.y = bY;
            UpdateScore(-10);
            MinusTenPointsPic = 1;
            birds.erase(birds.begin() + l);
            SetTimer(hwnd, POINTSmin10_TIMER_ID, 1000, NULL);
            hitSound();
        }
    }
    if(MinusTenPointsPic!=0)
    {
        MinusTenPointsPicFunction(hdcMem, hdcBuffer, PointsMin10.x, PointsMin10.y);
    }

    if(!birds.empty() && (birds[0].x + birds[0].width < rect->left))
    {
        birds.erase(birds.begin());
    }
    for(int m = 0; m<(int)(balloons.size()); m++)
    {
        Object bl = balloons[m];
        int blX = balloons[m].x;
        int blY = balloons[m].y;
        SelectObject(hdcMem, hbmBalloon);
        BitBlt(hdcBuffer, blX, blY, bl.width, bl.height, hdcMem, BalloonCounterX*bl.width, BalloonCounterY*bl.height,SRCAND);
        SelectObject(hdcMem, hbmBalloonMask);
        BitBlt(hdcBuffer, blX, blY, bl.width, bl.height, hdcMem, BalloonCounterX*bl.width, BalloonCounterY*bl.height,SRCPAINT);
        if(blX + bl.width < rect->left || blY + bl.height > rect->bottom)
        {
            balloons.erase(balloons.begin() + m);
        }
        if(blX < Elf.x)
        {
            TenPointsPic = 0;
        }
        else if((blX <= Elf.x + Elf.width) && (blY <= Elf.y + Elf.height) && (blY + bl.height >= Elf.y))
        {
            Points10.x = blX;
            Points10.y = blY;
            UpdateScore(10);
            TenPointsPic = 1;
            SetTimer(hwnd, POINTS10_TIMER_ID, 1000, NULL);
            pointsSound();
            balloons.erase(balloons.begin() + m);
        }
    }

    if(TenPointsPic!=0)
    {
        TenPointsPicFunction(hdcMem, hdcBuffer, Points10.x, Points10.y);
    }
    if(!balloons.empty() && (balloons[0].x + balloons[0].width < rect->left))
    {
        balloons.erase(balloons.begin());

    }

    for(int n = 0; n<(int)(clocks.size()); n++)
    {
        Object cl = clocks[n];
        int clX = clocks[n].x;
        int clY = clocks[n].y;
        SelectObject(hdcMem, hbmClock);
        BitBlt(hdcBuffer, clX, clY, cl.width, cl.height, hdcMem, 0, 0, SRCAND);
        SelectObject(hdcMem, hbmClockMask);
        BitBlt(hdcBuffer, clX, clY, cl.width, cl.height, hdcMem, 0, 0, SRCPAINT);
        if(clX < Elf.x)
        {
            TenSecondsPic = 0;
        }
        else if((clX <= Elf.x + Elf.width) && (clY <= Elf.y + Elf.height) && (clY + cl.height >= Elf.y))
        {
            gameTime += 5;
            Seconds.x = clX;
            Seconds.y = clY;
            TenSecondsPic = 1;
            clocks.erase(clocks.begin() + n);
            SetTimer(hwnd, SECONDS10_BONUS_TIMER_ID, 1000, NULL);
            pointsSound();
        }
    }

    if(TenSecondsPic!=0)
    {
        TenSecondsPicFunction(hdcMem, hdcBuffer, Seconds.x, Seconds.y);
    }

    if(!clocks.empty() && (clocks[0].x + clocks[0].width < rect->left))
    {
        clocks.erase(clocks.begin());
    }

    SelectObject(hdcMem, hbmScoreTable);
    BitBlt(hdcBuffer, ScoreTable.x, ScoreTable.y,ScoreTable.width, ScoreTable.height, hdcMem, 0, 0, SRCAND);
    SelectObject(hdcMem, hbmScoreTableMask);
    BitBlt(hdcBuffer, ScoreTable.x, ScoreTable.y, ScoreTable.width, ScoreTable.height, hdcMem, 0, 0, SRCPAINT);

    SelectObject(hdcMem, hbmMemOld);
    DeleteDC(hdcMem);

    BitBlt(hdc, 0, 0, rect->right, rect->bottom, hdcBuffer, 0, 0, SRCCOPY);

    SelectObject(hdcBuffer, hbmBufferOld);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
    if(startPoints == 0)
    {
        std::string level = "LEVEL 1";
        SetTextColor(hdc, RGB(102,0,102));
        SetBkMode(hdc, TRANSPARENT);
        HFONT hFont = CreateFont (50, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Black Cooper"));
        SelectObject(hdc,(HFONT)(hFont));
        TextOut(hdc, 435, 80, level.c_str(), level.length());
    }
    if(startPoints >= 150 && startPoints < 160)
    {
        std::string level = "LEVEL 2";
        SetTextColor(hdc, RGB(102,0,102));
        SetBkMode(hdc, TRANSPARENT);
        HFONT hFont = CreateFont (50, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Black Cooper"));
        SelectObject(hdc,(HFONT)(hFont));
        TextOut(hdc, 435, 80, level.c_str(), level.length());
    }
    if(startPoints >= 350 && startPoints < 360)
    {
        std::string level = "LEVEL 3";
        SetTextColor(hdc, RGB(102,0,102));
        SetBkMode(hdc, TRANSPARENT);
        HFONT hFont = CreateFont (50, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Black Cooper"));
        SelectObject(hdc,(HFONT)(hFont));
        TextOut(hdc, 435, 80, level.c_str(), level.length());

    }
    if(startPoints >= 750 && startPoints < 760)
    {
        std::string level = "LEVEL 4";
        SetTextColor(hdc, RGB(102,0,102));
        SetBkMode(hdc, TRANSPARENT);
        HFONT hFont = CreateFont (50, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Black Cooper"));
        SelectObject(hdc,(HFONT)(hFont));
        TextOut(hdc, 435, 80, level.c_str(), level.length());
    }

    SetTextColor(hdc, RGB(255,255,255));
    SetBkMode(hdc,TRANSPARENT);
    HFONT hFont = CreateFont (20, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
    SelectObject(hdc, (HFONT)(hFont));
    TextOut(hdc, ScoreTable.x + 100, ScoreTable.y + 2, std::to_string(startPoints).c_str(), std::to_string(startPoints).length());

    if(gameTime == 5)
    {
        timeLeakSound();
    }
    if(gameTime >= 0)
    {
        SetTextColor(hdc, RGB(255,255,255));
        SetBkMode(hdc,TRANSPARENT);
        HFONT hFont = CreateFont (20, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
        SelectObject(hdc, (HFONT)(hFont));
        TextOut(hdc, ScoreTable.x + 82, ScoreTable.y + 19, ("00:" + std::to_string(gameTime)).c_str(), ("00:" + std::to_string(gameTime)).length());
    }
    else
    {
        finishTheGame(hwnd);
    }
}
void CheckInput(HWND hwnd)
{
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    if(PRITISNUTO(VK_UP))
    {
        if(Elf.y > clientRect.top)
        {
            Elf.y -= 15;
        }
    }
    else if(PRITISNUTO(VK_DOWN))
    {
        if((Elf.y + Elf.height) < clientRect.bottom)
        {
            Elf.y += 15;
        }
    }
    else if(PRITISNUTO(VK_LEFT))
    {
        if((Elf.x + Elf.width) > clientRect.left + Elf.width)
        {
            Elf.x -= 15;
        }
    }
    else if(PRITISNUTO(VK_RIGHT))
    {
        if((Elf.x + Elf.width) < clientRect.right)
        {
            Elf.x += 15;
        }
    }
    else if(PRITISNUTO(VK_SPACE))
    {
        Object present;
        BITMAP bitmap;
        GetObject(hbmPresent, sizeof(bitmap), &bitmap);
        present.height = bitmap.bmHeight;
        present.width = bitmap.bmWidth;
        present.x = Elf.x + present.width;
        present.y = Elf.y + present.height;
        present.dx = 0;
        present.dy = 15;
        presents.push_back(present);
    }
}
void TenPointsPicFunction(HDC hdcMem, HDC hdcBuffer, int X, int Y)
{
    SelectObject(hdcMem, hbmPoints10);
    BitBlt(hdcBuffer, X, Y, Points10.width, Points10.height, hdcMem, 0, 0, SRCAND);
    SelectObject(hdcMem, hbmPoints10Mask);
    BitBlt(hdcBuffer, X, Y, Points10.width, Points10.height, hdcMem, 0, 0, SRCPAINT);
}
void FifteenPointsPicFunction(HDC hdcMem, HDC hdcBuffer)
{
    SelectObject(hdcMem, hbmPoints15);
    BitBlt(hdcBuffer, Points15.x, Points15.y, Points15.width, Points15.height, hdcMem, 0, 0, SRCAND);
    SelectObject(hdcMem, hbmPoints15Mask);
    BitBlt(hdcBuffer, Points15.x, Points15.y, Points15.width, Points15.height, hdcMem, 0, 0, SRCPAINT);
}
void MinusTenPointsPicFunction(HDC hdcMem, HDC hdcBuffer, int X, int Y)
{
    SelectObject(hdcMem, hbmPointsMin10);
    BitBlt(hdcBuffer, X, Y, PointsMin10.width, PointsMin10.height, hdcMem, 0, 0, SRCAND);
    SelectObject(hdcMem, hbmPointsMin10Mask);
    BitBlt(hdcBuffer, X, Y, PointsMin10.width, PointsMin10.height, hdcMem, 0, 0, SRCPAINT);
}
void TenSecondsPicFunction(HDC hdcMem, HDC hdcBuffer, int X, int Y)
{
    SelectObject(hdcMem, hbmSeconds);
    BitBlt(hdcBuffer, X, Y, Seconds.width, Seconds.height, hdcMem, 0, 0, SRCAND);
    SelectObject(hdcMem, hbmSecondsMask);
    BitBlt(hdcBuffer, X, Y, Seconds.width, Seconds.height, hdcMem, 0, 0, SRCPAINT);
}

void Update()
{
    UpdateHouse();
    UpdateClock();
    UpdateBalloon();
    UpdateLollipop();
    UpdatePresent();
    UpdateBird();
    UpdateElf();
}

void UpdateHouse()
{
    for(int i=0; i<(int)(houses.size()); i++)
    {
        if(startPoints < 150)
        {
            houses[i].x -= 15;
        }
        else if(startPoints >= 150 && startPoints < 350)
        {
            houses[i].x -= 25;
        }
        else if(startPoints >=350 && startPoints < 750)
        {
            houses[i].x -= 35;
        }
        else
        {
            houses[i].x -= 40;
        }
    }
}
void UpdateClock()
{
    for(int i=0; i<(int)(clocks.size()); i++)
    {
        if(startPoints < 150)
        {
            clocks[i].x -= 10;
        }
        else if(startPoints >= 150 && startPoints < 350)
        {
            clocks[i].x -= 12;
        }
        else if(startPoints >=350 && startPoints < 750)
        {
            clocks[i].x -= 14;
        }
        else
        {
            clocks[i].x -= 16;
        }
    }
}
void UpdateBalloon()
{
    for(int i=0; i<(int)(balloons.size()); i++)
    {
        if(startPoints < 150)
        {
            balloons[i].x -= 20;
        }
        else if(startPoints >= 150 && startPoints < 350)
        {
            balloons[i].x -= 24;
        }
        else if(startPoints >=350 && startPoints < 750)
        {
            balloons[i].x -= 28;
        }
        else
        {
            balloons[i].x -= 32;
        }
    }
    BalloonCounterX++;
    if(BalloonCounterX == 5)
    {
        BalloonCounterY++;
        if(BalloonCounterY == 2)
        {
            BalloonCounterY = 0;
        }
        BalloonCounterX = 0;
    }
}
void UpdateLollipop()
{
    for(int i=0; i<(int)(lollipops.size()); i++)
    {
        if(startPoints < 150)
        {
            lollipops[i].x -= 10;
        }
        else if(startPoints >= 150 && startPoints < 350)
        {
            lollipops[i].x -= 12;
        }
        else if(startPoints >=350 && startPoints < 750)
        {
            lollipops[i].x -= 14;
        }
        else
        {
            lollipops[i].x -= 16;
        }

    }
}
void UpdatePresent()
{
    for(int i=0; i<(int)(presents.size()); i++)
    {
        presents[i].y += 15;
    }
}

void UpdateBird()
{
    for(int i=0; i<(int)(birds.size()); i++)
    {
        if(startPoints < 150)
        {
            birds[i].x -= 20;
        }
        else if(startPoints >= 150 && startPoints < 350)
        {
            birds[i].x -= 24;
        }
        else if(startPoints >=350 && startPoints < 750)
        {
            birds[i].x -= 28;
        }
        else
        {
            birds[i].x -= 32;
        }
    }
    BirdCounterX++;
    if(BirdCounterX == 8)
    {
        BirdCounterY++;
        if(BirdCounterY == 2)
        {
            BirdCounterY = 0;
        }
        BirdCounterX = 0;
    }
}
void UpdateElf()
{
    ElfCounterX++;
    if(ElfCounterX == 5)
    {
        ElfCounterY++;
        if(ElfCounterY == 2)
        {
            ElfCounterY = 0;
        }
        ElfCounterX = 0;
    }
}
void UpdateScore(int newPoints)
{
    if((startPoints + newPoints) > 0)
    {
        startPoints += newPoints;
    }
    else if(startPoints + newPoints < 0)
    {
        startPoints = 0;
    }
}

void makeHouse(int nHouse)
{
    Object house;
    BITMAP bitmap;
    house.n = nHouse;
    house.x = 1000;
    house.dx = 15;
    house.dy = 0;
    if(nHouse == 1)
    {
        GetObject(hbmHouse1, sizeof(bitmap), &bitmap);
        house.width = bitmap.bmWidth;
        house.height = bitmap.bmHeight;
        house.y = 245;
    }
    else if(nHouse == 2)
    {
        GetObject(hbmHouse2, sizeof(bitmap), &bitmap);
        house.width = bitmap.bmWidth;
        house.height = bitmap.bmHeight;
        house.y = 300;
    }
    else if(nHouse == 6)
    {
        GetObject(hbmHouse3, sizeof(bitmap), &bitmap);
        house.width = bitmap.bmWidth;
        house.height = bitmap.bmHeight;
        house.y = 195;
    }
    else if(nHouse == 4)
    {
        GetObject(hbmHouse4, sizeof(bitmap), &bitmap);
        house.width = bitmap.bmWidth;
        house.height = bitmap.bmHeight;
        house.y = 300;
    }
    else if(nHouse == 5)
    {
        GetObject(hbmSnowman, sizeof(bitmap), &bitmap);
        house.width = bitmap.bmWidth;
        house.height = bitmap.bmHeight;
        house.y = 427;
    }
    else if(nHouse == 3)
    {
        GetObject(hbmGreenTree, sizeof(bitmap), &bitmap);
        house.width = bitmap.bmWidth;
        house.height = bitmap.bmHeight;
        house.y = 370;
    }
    else if(nHouse == 7)
    {
        GetObject(hbmPinkTree, sizeof(bitmap), &bitmap);
        house.width = bitmap.bmWidth;
        house.height = bitmap.bmHeight;
        house.y = 370;
    }
    else if(nHouse == 8)
    {
        GetObject(hbmHouse5, sizeof(bitmap), &bitmap);
        house.width = bitmap.bmWidth;
        house.height = bitmap.bmHeight;
        house.y = 310;
    }
    else if(nHouse == 9)
    {
        GetObject(hbmPurpleTree, sizeof(bitmap), &bitmap);
        house.width = bitmap.bmWidth;
        house.height = bitmap.bmHeight;
        house.y = 370;
    }
    houses.push_back(house);
}

Chimney chimneyCoordinates(Object house)
{
    if(house.n == 1)
    {
        chimney.height = house.y + 7;
        chimney.chimneyLeft = house.x + 80;
        chimney.chimneyRight = house.x + 105;
    }
    else if(house.n == 2)
    {
        chimney.height = house.y + 21;
        chimney.chimneyLeft = house.x + 60;
        chimney.chimneyRight = house.x + 91;
    }
    else if(house.n == 6)
    {
        chimney.height = house.y + 24;
        chimney.chimneyLeft = house.x + 69;
        chimney.chimneyRight = house.x + 102;
    }
    else if(house.n == 4)
    {
        chimney.height = house.y + 16;
        chimney.chimneyLeft = house.x + 60;
        chimney.chimneyRight = house.x + 93;
    }
    else if(house.n == 8)
    {
        chimney.height = house.y;
        chimney.chimneyLeft = house.x + 184;
        chimney.chimneyRight = house.x + 240;
    }
    return chimney;
}

void timeLeakSound()
{
    mciSendString("close clock", NULL, 0, NULL);
    mciSendString("open sounds/clock.wav type waveaudio alias clock", NULL, 0, NULL);
    mciSendString("play clock", NULL, 0, NULL);
}

void hitSound()
{
    mciSendString("close hit", NULL, 0, NULL);
    mciSendString("open sounds/birdBump.wav type waveaudio alias hit", NULL, 0, NULL);
    mciSendString("play hit", NULL, 0, NULL);
}
void presentSound()
{
    mciSendString("close present", NULL, 0, NULL);
    mciSendString("open sounds/presentSound.wav type waveaudio alias present", NULL, 0, NULL);
    mciSendString("play present", NULL, 0, NULL);
}
void pointsSound()
{
    mciSendString("close points", NULL, 0, NULL);
    mciSendString("open sounds/pointsSound.wav type waveaudio alias points", NULL, 0, NULL);
    mciSendString("play points", NULL, 0, NULL);
}

void finishTheGame(HWND hwnd)
{
    ++START;
    hwndWindow = CreateWindowEx (
                     0,                   /* Extended possibilites for variation */
                     "ChoiceWindow",      /* Classname */
                     _T("Elf Express"),   /* Title Text */
                     WS_CAPTION,          /* default window */
                     60,                  /* Windows decides the position */
                     60,                  /* where the window ends up on the screen */
                     1030,                /* The programs width */
                     600,                 /* and height in pixels */
                     hwnd,                /* The window is a child-window to desktop */
                     NULL,                /* No menu */
                     (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE),/* Program Instance handler */
                     NULL                 /* No Window Creation data */
                 );
    if(!houses.empty() || !presents.empty() || !lollipops.empty() || !balloons.empty() || !birds.empty() || !clocks.empty())
    {
        presents.clear();
        lollipops.clear();
        balloons.clear();
        birds.clear();
        houses.clear();
        clocks.clear();
    }
    KillTimer(hwnd, TIMER_ID);
    KillTimer(hwnd, LOLLIPOP_TIMER_ID);
    KillTimer(hwnd, BALLOON_TIMER_ID);
    KillTimer(hwnd, BIRD_TIMER_ID);
    KillTimer(hwnd, TIME_BONUS_TIMER_ID);
    KillTimer(hwnd, POINTS15_TIMER_ID);
    KillTimer(hwnd, POINTS10_TIMER_ID);
    KillTimer(hwnd, POINTSmin10_TIMER_ID);
    KillTimer(hwnd, SECONDS10_BONUS_TIMER_ID);
    SetTimer(hwnd, BACKGROUND_TIMER_ID, 100, NULL);
    ShowWindow(hwndWindow, SW_SHOW);
}
