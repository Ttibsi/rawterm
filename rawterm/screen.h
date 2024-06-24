#ifndef RAWTERM_SCREEN_H
#define RAWTERM_SCREEN_H

#include <iostream>

namespace rawterm {
    // 0 == vertical == ^v, 1 == horizontal == <>
    struct Pos {
        int vertical;
        int horizontal;

        constexpr Pos(int x, int y) : vertical(x), horizontal(y) {}
        constexpr Pos() : vertical(0), horizontal(0) {}
        friend std::ostream& operator<<(std::ostream& os, const Pos& p);
        [[nodiscard]] bool operator==(const Pos& other) const;
        Pos& operator+=(const Pos& other);
        Pos& operator-=(const Pos& other);
        [[nodiscard]] Pos operator+(const Pos& other) const;
        [[nodiscard]] Pos operator-(const Pos& other) const;
        Pos& operator+=(const int other);
        Pos& operator-=(const int other);
        [[nodiscard]] bool partial_cmp(const Pos&);
    };

    struct Region {
        Pos top_left;
        Pos bottom_right;

        Region(const Pos& tl, const Pos& br) : top_left(tl), bottom_right(br) {}
        [[nodiscard]] bool contains(const Pos& cmp) const;
        [[nodiscard]] Region intersect(const Region& other);
    };
}  // namespace rawterm
#endif  // RAWTERM_SCREEN_H
