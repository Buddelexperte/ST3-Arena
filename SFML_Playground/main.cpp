#pragma once
#include "SFML_Arena.h"

int main()
{
    // Game Instance init
    GI_Arena* gameInstance = &GI_Arena::getInstance();

    gameInstance->start();

    return 0;
}