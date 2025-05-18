#pragma once
#pragma once

struct Hex {
    int q, r;

    Hex(int q = 0, int r = 0) : q(q), r(r) {}

    bool operator==(const Hex& other) const {
        return q == other.q && r == other.r;
    }
};
namespace std {
    template <>
    struct hash<Hex> {
        size_t operator()(const Hex& hex) const {
            return hash<int>()(hex.q) ^ (hash<int>()(hex.r) << 1);
        }
    };
}
