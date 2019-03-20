#ifndef SOKOBAN_FORMATTER_H
#define SOKOBAN_FORMATTER_H

#include "sokoban_common.h"
#include <type_traits>
#include <optional>

namespace Sokoban
{

class Formatter {
public:
    static constexpr char alphabet[] = "_#@+$*. ";

private:
    static constexpr auto TILE_COUNT = static_cast<std::underlying_type_t<Tile>>(Tile::Count);
    static_assert(sizeof(alphabet) == TILE_COUNT + 1);

public:
    static constexpr std::optional<Tile> encode(char ch) noexcept {
        std::underlying_type_t<Tile> index = 0u;

        while (alphabet[index] != TILE_COUNT) {
            if (alphabet[index] == ch) {
                return static_cast<Tile>(index);
            }
            index++;
        }
        return std::nullopt;
    }

    static constexpr char decode(Tile tile) noexcept {
        return alphabet[static_cast<size_t>(tile)];
    }
};

}

#endif
