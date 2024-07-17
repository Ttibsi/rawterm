#ifndef RAWTERM_SCREEN_H
#define RAWTERM_SCREEN_H

#include <iostream>
#include <string>

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
        [[nodiscard]] const std::string toStr() const;
    };

    struct Region {
        Pos top_left;
        Pos bottom_right;

        Region(const Pos& tl, const Pos& br) : top_left(tl), bottom_right(br) {}
        [[nodiscard]] bool contains(const Pos& cmp) const;
        [[nodiscard]] Region intersect(const Region& other);
    };
}  // namespace rawterm

namespace std {
    template <>
    struct hash<rawterm::Pos> {
        std::size_t operator()(const rawterm::Pos& pos) const noexcept {
            std::size_t h1 = std::hash<int> {}(pos.vertical);
            std::size_t h2 = std::hash<int> {}(pos.horizontal);

            // Combine the hash values (a common approach is to use XOR and bit shifting)
            return h1 ^ (h2 << 1);
        }
    };
}  // namespace std

#endif  // RAWTERM_SCREEN_H
