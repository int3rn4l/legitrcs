#include <Windows.h>
#include "osfet.h"

// vec3 class
struct Vec3 {
    float x, y, z;
    
    Vec3 operator+(Vec3 d) {
        return { x + d.x,
                 y + d.y,
                 z + d.z };
    }
    Vec3 operator-(Vec3 d) {
        return { x - d.x,
                 y - d.y,
                 z - d.z };
    }
    Vec3 operator*(float d) {
        return { x * d,
                 y * d,
                 z * d };
    }

    
    void Normalhali() {

    while(y < -180)
    {    y += 360;}

    while (y > 180)
    {    y -= 360;}

    if (x > 89) 
    {  x = 89;}

    if (x < -89) 
    {  x = -89;}
  }
};

int WINAPI HackThread(HMODULE hModule) {

    // data
    // clientmodül
    uintptr_t clientModule = (uintptr_t)GetModuleHandle(L"client_panorama.dll");
    // enginemodül
    uintptr_t engineModule = (uintptr_t)GetModuleHandle(L"engine.dll");
    // localplayher pointer
    uintptr_t localPlayer = *(uintptr_t*)(clientModule + dwLocalPlayer);
    // viewAngles ptr
    Vec3* viewAngles = (Vec3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_viewAngles);
    // shotsFired ptr
    int* iShotsFired = (int*)(localPlayer + m_iShotsFired);
    // aimPunch ptr
    Vec3* aimPunhAngle = (Vec3*)(localPlayer + m_aimPunchAngle);

    Vec3 ooPunch{ 0,0,0 };

    while (!GetAsyncKeyState(panickey)) {
        Vec3 punchAngle = *aimPunhAngle * 2;
        if (*iShotsFired > 1) {

            Vec3 newAngle = *viewAngles + ooPunch - punchAngle;

            newAngle.Normalhali();

            *viewAngles = newAngle;
        }
        ooPunch = punchAngle;
    }

    FreeLibraryAndExitThread(hModule, 0);
    CloseHandle(hModule);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
    return TRUE;
}