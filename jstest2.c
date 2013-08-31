#include <windows.h>
#include <stdio.h>
int main()
{
    int i;
    JOYINFOEX pji;
    int update;
    DWORD axes[6] = {0};
    UINT buttons[13] = {0};
    ZeroMemory(&pji, sizeof(pji));
    pji.dwSize = sizeof(pji);
    pji.dwFlags = JOY_RETURNBUTTONS | JOY_RETURNX | JOY_RETURNY
        | JOY_RETURNZ | JOY_RETURNR | JOY_RETURNU | JOY_RETURNV;
    while(JOYERR_NOERROR == joyGetPosEx(0, &pji))
    {
        update = 0;
        for(i = 0; i < 6; i++)
        {
            DWORD axis = ((DWORD*) &(pji.dwXpos))[i];
            if(axis != axes[i])
            {
                axes[i] = axis;
                update = 1;
            }
        }
        for(i = 0; i < 13; i++)
        {
            int button = (pji.dwButtons & (1 << i)) != 0;
            if(button != buttons[i])
            {
                buttons[i] = button;
                update = 1;
            }
        }
        if(update)
        {
            printf("Axes:", pji.dwButtons);
            for(i = 0; i < 6; i++)
            {
                printf("  %d: %5d", i, axes[i] + 32767);
            }
            printf(" Buttons:  ", pji.dwButtonNumber);
            for(i = 0; i < 13; i++)
                printf("%d:%s  ", i, buttons[i] ? "on ":"off");
            printf("\n");
        }
    }
}
