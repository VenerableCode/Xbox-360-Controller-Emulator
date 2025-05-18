#include <Windows.h>
#include "ViGEmClient.h"
#include <iostream>
#include <algorithm>
#include <cmath>

bool running = true;

#define XBOX_THUMB_MAX 32767
#define XBOX_THUMB_MIN -32768

bool IsKeyPressed(int key) {
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

int main() {

    PVIGEM_CLIENT client = vigem_alloc();
    if (client == nullptr) {
        std::cerr << "Failed" << std::endl;
        return -1;
    }

    if (vigem_connect(client) != VIGEM_ERROR_NONE) {
        std::cerr << "Failed to connect to controller emulator" << std::endl;
        vigem_free(client);
        return -1;
    }

    PVIGEM_TARGET target = vigem_target_x360_alloc();
    if (vigem_target_add(client, target) != VIGEM_ERROR_NONE) {
        std::cerr << "Failed to add controller connection" << std::endl;
        vigem_target_free(target);
        vigem_free(client);
        return -1;
    }

    XUSB_REPORT report = {};

    std::string opendate = __DATE__ " " __TIME__;
    std::string lastupdated = "5-17-2025";

    std::cout << "Xbox 360 controller emulator" << std::endl;
    std::cout << "Started: " << opendate << std::endl;
    std::cout << "last updated: " << lastupdated << std::endl;
    std::cout << "Virtual controller created" << std::endl;
    

    std::cout << "(CONTROLS)" << std::endl;
    std::cout << "Xbox 360 Controller Emulator" << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "WASD         = Left Thumbstick" << std::endl;
    std::cout << "Arrow Keys   = Right Thumbstick" << std::endl;
    std::cout << "Space        = A Button" << std::endl;
    std::cout << "X            = X Button" << std::endl;
    std::cout << "E            = Y Button" << std::endl;
    std::cout << "B            = B Button" << std::endl;
    std::cout << "1            = Left Bumper (LB)" << std::endl;
    std::cout << "2            = Right Bumper (RB)" << std::endl;
    std::cout << "Shift        = Right Thumbstick Press (R3)" << std::endl;
    std::cout << "Enter        = Right Trigger (RT)" << std::endl;
    std::cout << "Backspace    = Left Trigger (LT)" << std::endl;
    std::cout << "Ctrl + Q     = Exit Emulator" << std::endl;
    std::cout << std::endl;


    while (running) {
        
        //default report to 0
        report.wButtons = 0;
        report.sThumbLX = 0;
        report.sThumbLY = 0;
        report.sThumbRX = 0;
        report.sThumbRY = 0;


        //joysticks
        if (IsKeyPressed('W')) {
            report.sThumbLY = XBOX_THUMB_MAX;
        }
        if (IsKeyPressed('S')) {
            report.sThumbLY = XBOX_THUMB_MIN;
        }
        if (IsKeyPressed('A')) {
            report.sThumbLX = XBOX_THUMB_MIN;
        }
        if (IsKeyPressed('D')) {
            report.sThumbLX = XBOX_THUMB_MAX;
        }

        if (IsKeyPressed(VK_UP)) {
            report.sThumbRY = XBOX_THUMB_MAX;
        }
        if (IsKeyPressed(VK_DOWN)) {
            report.sThumbRY = XBOX_THUMB_MIN;
        }
        if (IsKeyPressed(VK_LEFT)) {
            report.sThumbRX = XBOX_THUMB_MIN;
        }
        if (IsKeyPressed(VK_RIGHT)) {
            report.sThumbRX = XBOX_THUMB_MAX;
        }




        
        if (IsKeyPressed(VK_SHIFT)) {
            report.wButtons |= XUSB_GAMEPAD_RIGHT_THUMB; //crouch
        }

        if (IsKeyPressed(VK_SPACE)) {
            report.wButtons |= XUSB_GAMEPAD_A; //jump
        }

        if (IsKeyPressed('E')) {
            report.wButtons |= XUSB_GAMEPAD_Y;
        }

        if (IsKeyPressed('1')) {
            report.wButtons |= XUSB_GAMEPAD_LEFT_SHOULDER;
        }

        if (IsKeyPressed('2')) {
            report.wButtons |= XUSB_GAMEPAD_RIGHT_SHOULDER;
        }

        if (IsKeyPressed('B')) {
            report.wButtons |= XUSB_GAMEPAD_B;
        }

        if (IsKeyPressed('X')) {
            report.wButtons |= XUSB_GAMEPAD_X;
        }

        if (IsKeyPressed(VK_RETURN)) {
            report.bRightTrigger = 255;
        }
        else {
            report.bRightTrigger = 0;
        }

        if (IsKeyPressed(VK_BACK)) {
            report.bLeftTrigger = 255;
        }
        else {
            report.bLeftTrigger = 0;
        }

        vigem_target_x360_update(client, target, report);

        Sleep(10);
    }



    vigem_target_remove(client, target);
    vigem_target_free(target);
    vigem_disconnect(client);
    vigem_free(client);

    return 0;
}
