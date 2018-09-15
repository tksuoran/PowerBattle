#include "game.hpp"
#include "texture_util.hpp"

int main(int argc, const char *const *const argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    apply_mask();

    {
        Game game;
        game.init();
        game.main_loop();
        game.deinit();
    }

	return 0;
}
