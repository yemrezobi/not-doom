#include <Game.hpp>

int main(int argc, char *argv[])
{
    // ignore clang warnings
    (void)argc;
    (void)argv;

    auto game = Game();
    game.start();

    return 0;
}
