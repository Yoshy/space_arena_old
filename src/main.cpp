// (C) 2009 Yoshy
// ����� ����� � ����

#include "spacearena.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT) {
    SpaceArena::SpaceArena* spaceArena = new SpaceArena::SpaceArena();

    if (!spaceArena->init()) return false;
    spaceArena->run();
    delete spaceArena;
    return 0;
};
