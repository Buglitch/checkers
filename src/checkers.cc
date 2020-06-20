#include <iostream>

#include "engine.hh"

int main(void)
{
    Game *game = new Game();
    struct pos p;
    while (game->is_running()) {
        std::cout << "Checkers" << std::endl;
        std::cout << "========" << std::endl;
        std::cout << *game;
        std::cin >> p.x;
        std::cin >> p.y;
        game->play(p);
    }
    std::cout << "Checkers" << std::endl;
    std::cout << "========" << std::endl;
    std::cout << *game;
    return 0;
}
