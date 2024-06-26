#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/extras/pane.h>

#include <algorithm>
#include <memory>
#include <span>
#include <string>
#include <vector>

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();

    std::vector<std::string> lryics_1 = {
        "Should Old Acquaintance be forgot,",    "and never thought upon;",
        "The flames of Love extinguished,",      "and fully past and gone:",
        "Is thy sweet Heart now grown so cold,", "that loving Breast of thine;",
        "That thou canst never once reflect",    "On old long syne.",
    };

    std::vector<std::string> lryics_2 = {
        "Is this the real life?",
        "Is this just fantasy?",
        "Caught in a landslide",
        "No escape from reality",
        "Open your eyes",
        "Look up to the skies and see",
        "I'm just a poor boy, I need no sympathy",
        "Because I'm easy come, easy go",
        "Little high, little low",
        "Any way the wind blows",
        "Doesn't really matter to me, to me",
        "Mama, just killed a man",
        "Put a gun against his head",
        "Pulled my trigger, now he's dead",
        "Mama, life had just begun",
        "But now I've gone and thrown it all away",
        "Mama, ooh",
        "Didn't mean to make you cry",
        "If I'm not back again this time tomorrow",
        "Carry on, carry on as if nothing really matters",
        "Too late, my time has come",
        "Sends shivers down my spine",
        "Body's aching all the time",
        "Goodbye, everybody, I've got to go",
        "Gotta leave you all behind and face the truth",
        "Mama, ooh (Any way the wind blows)",
        "I don't wanna die",
        "I sometimes wish I'd never been born at all",
        "I see a little silhouetto of a man",
        "Scaramouche, Scaramouche, will you do the Fandango?",
        "Thunderbolt and lightning very, very frightening me",
        "(Galileo) Galileo",
        "(Galileo) Galileo",
        "Galileo Figaro",
        "Magnifico-o-o-o-o",
        "I'm just a poor boy, nobody loves me",
        "He's just a poor boy from a poor family",
        "Spare him his life from this monstrosity",
        "Easy come, easy go, will you let me go?",
        "Bismillah! No, we will not let you go (Let him go!)",
        "Bismillah! We will not let you go (Let him go!)",
        "Bismillah! We will not let you go (Let me go!)",
        "Will not let you go (Let me go!)",
        "Never let you go (Never, never, never, never let me go)",
        "Oh oh oh oh",
        "No, no, no, no, no, no, no",
        "Oh, mamma mia, mamma mia (Mamma mia, let me go)",
        "Beelzebub has a devil put aside for me, for me, for me",
        "So you think you can stone me and spit in my eye?",
        "So you think you can love me and leave me to die?",
        "Oh, baby, can't do this to me, baby",
        "Just gotta get out, just gotta get right outta here",
        "Ooh, ooh yeah, ooh yeah",
        "Nothing really matters",
        "Anyone can see",
        "Nothing really matters",
        "Nothing really matters to me",
        "Any way the wind blows",
    };

    const rawterm::Pos term_size = rawterm::get_term_size();
    for (auto&& line : lryics_1) {
        line = rawterm::set_foreground(line, rawterm::Colors::red);
    }
    for (auto&& line : lryics_2) {
        line = rawterm::set_foreground(line, rawterm::Colors::yellow);
    }

    auto pane_mgr = rawterm::PaneManager(rawterm::get_term_size());
    pane_mgr.set_content(std::vector(
        lryics_2.begin(),
        lryics_2.begin() + std::min(term_size.vertical, static_cast<int>(lryics_2.size()))));

    pane_mgr.set_pane_background(rawterm::Colors::aqua);

    // Open a second pane, green bg, red text, under the first
    auto k = rawterm::wait_for_input();
    pane_mgr.split_horizontal(lryics_1);
    pane_mgr.switch_active();
    pane_mgr.set_pane_background(rawterm::Colors::lime);

    // Open a third pane to the right of the first, red bg
    k = rawterm::wait_for_input();
    pane_mgr.switch_active(2);
    pane_mgr.split_vertical({"foo", "bar", "baz"});
    pane_mgr.switch_active();
    pane_mgr.set_pane_background(rawterm::Colors::red);

    // close the green pane at the bottom of the screen
    k = rawterm::wait_for_input();
    pane_mgr.switch_active(3);
    pane_mgr.close_active();

    // Create a new pane with the default background
    k = rawterm::wait_for_input();
    pane_mgr.split_horizontal({"hello", "world"});

    k = rawterm::wait_for_input();
    rawterm::exit_alt_screen();
    return 0;
}
