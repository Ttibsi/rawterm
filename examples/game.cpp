#include <rawterm/core.h>
#include <rawterm/cursor.h>

#include <format>
#include <iostream>
#include <random>

// A recreation of a simple "collect the coin" game from this tutorial in Go.
// See: https://www.youtube.com/watch?v=H1X16MWKrCA

struct Sprite {
    char rune;
    int vert;
    int horiz;

    Sprite(char c, int x, int y) : rune(c), vert(x), horiz(y) {}
    void draw(rawterm::Cursor& cur) {
        cur.move({vert, horiz});
        std::cout << rune;
    }
};

std::vector<Sprite> setupCoins(const int& lvl, const rawterm::Pos& term_size) {
    // setup random
    // https://stackoverflow.com/a/7560564
    std::random_device rd;   // obtain a random number from hardware
    std::mt19937 gen(rd());  // seed the generator
    std::uniform_int_distribution<> distr_vert(2, term_size.vertical);  // define the range
    std::uniform_int_distribution<> distr_horiz(2, term_size.horizontal);

    std::vector<Sprite> coins = {};

    for (int i = 0; i < lvl + 3; i++) {
        coins.emplace_back('0', distr_vert(gen), distr_horiz(gen));
    }

    return coins;
}

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();

    rawterm::Cursor::cursor_hide();
    rawterm::Cursor cur;
    cur.reset();

    unsigned short score = 0;
    unsigned int lvl = 1;
    Sprite player = Sprite('@', 10, 10);
    std::vector<Sprite> coins = setupCoins(lvl, rawterm::get_term_size());

    // Helper lambda for redrawing term
    auto draw_all = [&player, &coins, &cur, &score, &lvl] {
        rawterm::clear_screen();
        cur.move({1, 1});
        std::cout << std::format("Score: {} Level: {}", score, lvl);
        player.draw(cur);

        for (auto&& coin : coins) {
            coin.draw(cur);
        }  // Doesn't draw last coin?
    };

    // Game loop
    while (true) {
        draw_all();
        auto k = rawterm::wait_for_input();

        if (k == rawterm::Key('q')) {
            break;
        } else if (k == rawterm::Key('h')) {
            player.horiz--;
        } else if (k == rawterm::Key('j')) {
            player.vert++;
        } else if (k == rawterm::Key('k')) {
            player.vert--;
        } else if (k == rawterm::Key('l')) {
            player.horiz++;
        }

        // Remove collected coin
        for (int i = 0; i < coins.size(); i++) {
            Sprite* c = &coins.at(i);
            if (player.vert == c->vert && player.horiz == c->horiz) {
                coins.erase(coins.begin() + i);
                score++;

                // Check for next level
                if (coins.size() == 1) {
                    lvl++;
                    coins = setupCoins(lvl, rawterm::get_term_size());
                }
                break;
            }
        }
    }

    rawterm::Cursor::cursor_show();
    return 0;
}
