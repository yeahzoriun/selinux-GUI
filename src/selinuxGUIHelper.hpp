#pragma once

#include <iostream>
#include <printf.h>
#include <fstream>
#include <vector>

#include <selinux/selinux.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "window.hpp"
#include "colorbutton.hpp"
#include "textbutton.hpp"
#include "rectangle.hpp"
#include "text.hpp"

struct AVCDenial
{
    std::string PermissionDenied;
    std::string Process;
    std::string Path;
    bool Destroyed = false;
};

std::vector<AVCDenial> Denials;

void initDenials() {
    
    Denials.clear();

    std::ifstream Log;
    std::string Line;
    Log = std::ifstream("/var/log/audit/audit.log");

    int errors = 0;

    if (!Log.is_open()){
        std::printf("log couldn't open!");
        return;
    }

    while (getline(Log, Line))
    {

        if (Line.find("type=AVC") != std::string::npos){

            AVCDenial DenialToAdd;
            
            int permissionDeniedIndexStart = Line.find("avc:");
            int permissionDeniedIndexEnd = Line.find("for");
            std::string permissionDenied;

            if (permissionDeniedIndexStart != std::string::npos && permissionDeniedIndexEnd != std::string::npos){
                permissionDenied = Line.substr(permissionDeniedIndexStart, permissionDeniedIndexEnd - permissionDeniedIndexStart);
                DenialToAdd.PermissionDenied = permissionDenied;
            }

            int processIDIndexStart = Line.find("pid=");
            int processIDIndexEnd = Line.find("path");
            std::string process;

            if (processIDIndexStart != std::string::npos && processIDIndexEnd != std::string::npos){
                process = Line.substr(processIDIndexStart, processIDIndexEnd - processIDIndexStart);
                DenialToAdd.Process = process;
            }

            int pathIndexStart = Line.find("path=");
            int pathIndexEnd = Line.find("dev=");
            std::string path;

            if (pathIndexStart != std::string::npos && pathIndexEnd != std::string::npos){
                path = Line.substr(pathIndexStart, pathIndexEnd - pathIndexStart);
                DenialToAdd.Path = path;
            }

            Denials.push_back(DenialToAdd);
        }

    }
}

std::vector<rectangle> rectangles;
std::vector<text> texts;
std::vector<textbutton> textbuttons;

void setupAVCDenialsUI(SDL_Renderer* renderer){

    int YOffset = 10;
    rectangles.clear();
    texts.clear();
    textbuttons.clear();

    for (int i = 0; i < Denials.size(); i++){

        if (Denials[i].Destroyed == true){
            Denials.erase(Denials.begin() + i);
            continue;
        }

        rectangle DenialRec;
        DenialRec.structInit(renderer, 0, 0, 1000, 100, {255,255,255});
        DenialRec.x = (1280/2) - (DenialRec.w/2); // centers X
        DenialRec.y = YOffset;

        text DenialText;
        DenialText.structInit(renderer, 0, 0, 25, "- AVC Denial -", {150, 150, 150});
        DenialText.x = (1280/2) - (DenialText.w/2); // centers X
        DenialText.y = YOffset;

        text PermissionsDeniedText;
        PermissionsDeniedText.structInit(renderer, 0, 0, 20, Denials[i].PermissionDenied, {255, 0, 0});
        PermissionsDeniedText.x = (1280/2) - (PermissionsDeniedText.w/2); // centers X
        PermissionsDeniedText.y = YOffset+30;

        text ProcessText;
        ProcessText.structInit(renderer, 0, 0, 20, Denials[i].Process, {255, 0, 0});
        ProcessText.x = (1280/2) - (ProcessText.w/2); // centers X
        ProcessText.y = YOffset+55;

        text PathText;
        PathText.structInit(renderer, 0, 0, 15, Denials[i].Path, {255, 0, 0});
        PathText.x = (1280/2) - (PathText.w/2); // centers X
        PathText.y = YOffset+80;

        textbutton AllowButton;
        AllowButton.structInit(renderer, "Allow", 0, 0, 70, 30, {0,255,0}, {0,0,0});
        AllowButton.x = ((1280/2) - (AllowButton.w/2)) + 200;
        AllowButton.y = YOffset+10;

        textbutton BlockButton;
        BlockButton.structInit(renderer, "Block", 0, 0, 70, 30, {255,0,0}, {0,0,0});
        BlockButton.x = ((1280/2) - (BlockButton.w/2)) + 200;
        BlockButton.y = YOffset+60;
        BlockButton.onClickFunc = [savedI = i](){
            //Denials.erase(Denials.begin() + savedI);
            Denials[savedI].Destroyed = true;
        };

        rectangles.push_back(DenialRec);

        texts.push_back(DenialText);
        texts.push_back(PermissionsDeniedText);
        texts.push_back(ProcessText);
        texts.push_back(PathText);

        textbuttons.push_back(AllowButton);
        textbuttons.push_back(BlockButton);

        YOffset+=110;
    }

}

void renderAVCDenialsUI(SDL_Window* window){

    for (int i = 0; i < rectangles.size(); i++){
        rectangles[i].handleFrame();
    }

    for (int i = 0; i < texts.size(); i++){
        texts[i].handleFrame();
    }

    for (int i = 0; i < textbuttons.size(); i++){
        textbuttons[i].handleFrame(true, window);
    }

}