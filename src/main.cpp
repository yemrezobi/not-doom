#include "Doom.hpp"

int main(int argc, char *argv[])
{
    // ignore clang warnings
    (void)argc;
    (void)argv;

    Doom game{};
    game.start();

    return 0;
}
