#include "screen.h"

#include <algorithm>

namespace rawterm {
    std::ostream& operator<<(std::ostream& os, const Pos& p) {
        return os << "Pos{" << p.vertical << ", " << p.horizontal << "}";
    }

    [[nodiscard]] bool Pos::operator==(const Pos& other) const {
        return this->vertical == other.vertical && this->horizontal == other.horizontal;
    }

    Pos& Pos::operator+=(const Pos& other) {
        this->vertical += other.vertical;
        this->horizontal += other.horizontal;
        return *this;
    }

    Pos& Pos::operator-=(const Pos& other) {
        this->vertical -= other.vertical;
        this->horizontal -= other.horizontal;
        return *this;
    }

    [[nodiscard]] Pos Pos::operator+(const Pos& other) const {
        return Pos(this->vertical + other.vertical, this->horizontal + other.horizontal);
    }

    [[nodiscard]] Pos Pos::operator-(const Pos& other) const {
        return Pos(this->vertical - other.vertical, this->horizontal - other.horizontal);
    }

    Pos& Pos::operator+=(const int other) {
        this->vertical += other;
        this->horizontal += other;
        return *this;
    }

    Pos& Pos::operator-=(const int other) {
        this->vertical -= other;
        this->horizontal -= other;
        return *this;
    }

    [[nodiscard]] bool Pos::partial_cmp(const Pos& rhs) {
        return this->vertical == rhs.vertical || this->horizontal == rhs.horizontal;
    }

    [[nodiscard]] bool Region::contains(const Pos& cmp) const {
        return cmp.vertical >= top_left.vertical && cmp.vertical <= bottom_right.vertical &&
               cmp.horizontal >= top_left.horizontal & cmp.horizontal <= bottom_right.horizontal;
    }

    // TODO: Verify that this code actually works/makes sense
    [[nodiscard]] Region Region::intersect(const Region& other) {
        // Calculate the intersection's top-left corner
        Pos intersection_top_left;
        intersection_top_left.horizontal = std::max(top_left.horizontal, other.top_left.horizontal);
        intersection_top_left.vertical = std::max(top_left.vertical, other.top_left.vertical);

        // Calculate the intersection's bottom-right corner
        Pos intersection_bottom_right;
        intersection_bottom_right.horizontal =
            std::min(bottom_right.horizontal, other.bottom_right.horizontal);
        intersection_bottom_right.vertical =
            std::min(bottom_right.vertical, other.bottom_right.vertical);

        // Check if the resulting region is valid
        if (intersection_top_left.horizontal > intersection_bottom_right.horizontal ||
            intersection_top_left.vertical > intersection_bottom_right.vertical) {
            // Return an invalid region if there's no intersection
            return {{0, 0}, {0, 0}};
        }

        // Return the intersection region
        return {intersection_top_left, intersection_bottom_right};
    }
}  // namespace rawterm
