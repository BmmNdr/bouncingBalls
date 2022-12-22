#pragma once

#include <Windows.h>

#define _WIN32_WINNT 0x0500



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI CreateMyWindow(void* funzioneDisegno = NULL);


void (*funzioneDiDisegno)(HDC, RECT);
RECT windowSize;
HBITMAP backbuffer = NULL;
HDC backbuffDC = NULL;
int savedDC = 0;
bool toRecreateBackBuffer = true;

/// <summary>
/// Permette di creare una finestra, necessita di una funzione da richiamare per disegnare la finestra
/// ATTENZIONE! SI PUO' APRIRE SOLO 1 FINESTRA!
/// </summary>
/// <param name="funzioneDisegno">la funzione occorre dichiararla    void Draw(HDC hdc, RECT bounds)   </param>
/// <returns></returns>
int WINAPI CreateMyWindow(void* funzioneDisegno)
{
    //controllo per una sola finestra ( se no la funzione di disegno si sovrascrive )
    if (funzioneDiDisegno != NULL)
        return 0;

    //imposto la funzione per disegnare
    if (funzioneDisegno == NULL)
        funzioneDiDisegno = NULL;
    else
        funzioneDiDisegno = (void (*)(HDC, RECT))funzioneDisegno;


    //creo la finestra
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    const WCHAR g_szClassName[] = L"myWindowClass";

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;           //gli assegno la procedura che gestisce i messaggi ( vedere dopo ) 
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //chiedo al sistema operativo di creare una finestra chiamata "My window" con tutti i parametri che ho impostato sopra
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        L"My Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, nullptr, NULL);

    //controllo se il sistema me l'ha creata
    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //la visualizzo ed imposto la dimensione a 800x600
    ShowWindow(hwnd, 1);
    UpdateWindow(hwnd);
    MoveWindow(hwnd, 0, 0, 1700, 600, TRUE);


    GetClientRect(hwnd, &windowSize);

    //creo un ciclo infinito che fino a quando la finestra rimane aperta controlla se ci sono "messaggi" ( mouse cliccato, mouse spostato, ecc ecc ) 
    //ed "invalida" l'area di disegno ( ovvero gli dice di ri-disegnare la schermata, lanciando la funzione WndProc ) 
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
        InvalidateRect(hwnd, &windowSize, TRUE);
        
    }
    return Msg.wParam;
}


/// <summary>
/// questa funzione gestisce i messaggi della finestra 
/// </summary>
/// <param name="hwnd"></param>
/// <param name="msg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    //in base al tipo di messaggio fa qualcosa
    switch (msg)
    {
    case WM_LBUTTONDOWN:
    {
        WCHAR szFileName[MAX_PATH];
        HINSTANCE hInstance = GetModuleHandle(NULL);

        //qua ci arriva quando viene premuto il tasto sinistro del mouse ( le coordinate dovrebbero essere salvate in modo "strano" dentro le variabili wParam e lParam )  
        
    }
    break;
    case WM_CLOSE:      //finestra chiusa
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:        //finestra "distrutta"
        PostQuitMessage(0);
        break;
    case WM_PAINT:
    {
        //quando deve disegnare!!!
        hdc = BeginPaint(hwnd, &ps);


        RECT rect;
        GetClientRect(hwnd, &rect);
        int width = rect.right;
        int height = rect.bottom;

        //uso il "doubleBuffer"
        //per evitare problemi di "flickering" ( sfarfallio ) si usa questa tecnica
        //disegno prima tutto quello che devo disegnare su una bitmap
        //e poi disegno in un colpo solo la bitmap sulla finestra

        //se usate la libreria così come ve l'ho passata, non vi accorgete neanche...



        //ricreo il buffer solo se ho ridimensionato la finestra ( booleana settata in WM_SIZE) 
        if (toRecreateBackBuffer)
        {
            if (backbuffDC != NULL && backbuffer != NULL && savedDC != 0)        //se è la prima volta, non restoro la vecchia DC
            {
                RestoreDC(backbuffDC, savedDC);
                DeleteDC(backbuffDC);                                       //e non cancello gli oggetti ( che sarebbero a NULL ) 
                DeleteObject(backbuffer);
            }
            
            backbuffDC = CreateCompatibleDC(hdc);                           //credo l'HDC per il double buffer
            backbuffer = CreateCompatibleBitmap(hdc, width, height);        //creo l'immagine di disegno
            SelectObject(backbuffDC, backbuffer);                           
            savedDC = SaveDC(backbuffDC);                                   //salvo lo stato ( che verrà ripristinato prima di cancellarlo ) 
            toRecreateBackBuffer = false;                                   
        }

        //pulisco lo schermo
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(backbuffDC, &rect, hBrush);
        DeleteObject(hBrush);



        //richiama la funzione che avete passato 
        funzioneDiDisegno(backbuffDC, rect);

        //copio quello salvato nell'immagine temporanea sullo schermo
        BitBlt(hdc, 0, 0, width, height, backbuffDC, 0, 0, SRCCOPY);


        //finisco la stampa del frame
        EndPaint(hwnd, &ps);


        return 0L;

        break;
    }
    case WM_SIZE:
        GetClientRect(hwnd, &windowSize);
        toRecreateBackBuffer = true;
    case WM_ERASEBKGND:
        return 1;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}



