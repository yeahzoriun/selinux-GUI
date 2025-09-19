#include <iostream>
#include <printf.h>
#include <fstream>

#include <selinux/selinux.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "src/window.hpp"
#include "src/colorbutton.hpp"
#include "src/textbutton.hpp"
#include "src/rectangle.hpp"
#include "src/text.hpp"
#include "src/selinuxGUIHelper.hpp"

// Example Log
// type=AVC msg=audit(1695091245.782:123): avc:  denied  { read write } for  pid=4321 comm="nginx" path="/var/www/html/index.html" dev="sda1" ino=1234567 scontext=system_u:system_r:httpd_t:s0 tcontext=unconfined_u:object_r:var_t:s0 tclass=file

int main() {
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_AUDIO);
    TTF_Init();

    window MainWindow("SELINUX-GUI", 1280, 720);

    textbutton refreshLogsButton;
    refreshLogsButton.structInit(MainWindow.rendererOBJ, "Refresh Log(s)", 0, 0, 15*(int)sizeof("Refresh Log(s)"), 40, {255,255,255}, {0,0,0});
    refreshLogsButton.x = (1280/2) - refreshLogsButton.w/2; refreshLogsButton.y = 700 - refreshLogsButton.h;
    refreshLogsButton.onClickFunc = [&MainWindow](){
        //initDenials();
        setupAVCDenialsUI(MainWindow.rendererOBJ);
    };

    initDenials();
    setupAVCDenialsUI(MainWindow.rendererOBJ);

    while (MainWindow.running == true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED){
                MainWindow.running = false;
            }
        }

        SDL_SetRenderDrawColor(MainWindow.rendererOBJ, 150, 150, 150, 255);
        SDL_RenderClear(MainWindow.rendererOBJ);

        refreshLogsButton.handleFrame(true, MainWindow.windowOBJ);
        renderAVCDenialsUI(MainWindow.windowOBJ);

        SDL_RenderPresent(MainWindow.rendererOBJ);
    }

    TTF_Quit();
    SDL_Quit();
    return 0;
}