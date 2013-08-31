#include <windows.h>
#include <stdio.h>
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
TCHAR szClassName[ ] = TEXT("CodeBlocksWindowsApp");
int WINAPI WinMain(HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    if (!RegisterClassEx (&wincl))
        return 0;
    hwnd = CreateWindowEx (
           0,
           szClassName,
           TEXT("Code::Blocks Template Windows App"),
           WS_OVERLAPPEDWINDOW,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           544,
           375,
           HWND_DESKTOP,
           NULL,
           hThisInstance,
           NULL
           );
    ShowWindow (hwnd, SW_HIDE);
    joyReleaseCapture( 0 );
    joySetThreshold( 0, 25 );
    joySetCapture( hwnd, 0, 100, TRUE );
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    joyReleaseCapture( 0 );
    return messages.wParam;
}
void process_buttons(int* buttons, WPARAM wParam, int res)
{
    int i;
    for(i = 0; i < 5; i++) 
        if(wParam == (1 << i) + (256 << i)) buttons[i] = res;
    for(i = 5; i < 13; i++)
        if(wParam & (1 << i) && !(wParam & 0x1f)) buttons[i] = res;
}
void display_buttons(int* axes, int* buttons, int size)
{
    int i;
    printf("Axes:");
    for(i = 0; i < 6; i++)
        printf("  %d: %6d", i, axes[i]);
    printf(" Buttons:  ");
    for(i = 0; i < size; i++)
        printf("%d:%s  ", i, buttons[i] ?  "on ":"off");
    printf("\n");
}
int buttons[14] = {0};
int axes[6] = {0};
LRESULT CALLBACK WindowProcedure(
        HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int i;
    switch (message)
    {
        case WM_CREATE:
            SetTimer( hwnd, 1, 100, NULL );
            break;
        case WM_DESTROY:
            KillTimer( hwnd, 1 );
            PostQuitMessage(0);
            break;
        case MM_JOY1MOVE:
            i = 0;
            axes[i] = LOWORD(lParam) - 32767;
            axes[i+1] = HIWORD(lParam) - 32767;
            display_buttons(axes, buttons, 13);
            break;
        case MM_JOY1ZMOVE:
            i = 2;
            axes[i] = LOWORD(lParam) - 32767;
            axes[i+1] = HIWORD(lParam) - 32767;
            display_buttons(axes, buttons, 13);
            break;
        case MM_JOY1BUTTONDOWN:
            for(i = 0; i < 13; i++)
                printf("%s", wParam & ( 1 << i ) ?  "1":"0");
            printf("\n");
            process_buttons(buttons, wParam, 1);
            display_buttons(axes, buttons, 13);
            break;
        case MM_JOY1BUTTONUP:
            for(i = 0; i < 13; i++)
                printf("%s", wParam & ( 1 << i ) ?  "1":"0");
            printf("\n");
            process_buttons(buttons, wParam, 0);
            display_buttons(axes, buttons, 13);
            break;
        case WM_TIMER:
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
