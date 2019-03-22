#include "sokoban_board_state.h"
#include "sokoban_boxstate.h"
#include "sokoban_pushinfo.h"
#include "sokoban_formatter.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <iterator>

using namespace Sokoban;
using namespace std;

bool BoardState::initialize(std::vector<Tile> && tiles, size_t width, size_t height) {
    _tiles  = move(tiles);
    _width  = width;
    _height = height;

    for (index_t i = 0; i < _tiles.size(); i++) {
        Tile tile = _tiles[i];

        if (tile_is_wall(tile))   { _is_wall[i] = true; continue; }
        if (tile_is_player(tile)) { _player = i; }
        if (tile_is_goal(tile))   { _goals.push_back(i); _is_goal[i] = true; }
        if (tile_is_box(tile))    { _boxes.push_back(i); _is_box[i] = true; }
    }

    return _boxes.size() == _goals.size();
}

BoxState BoardState::current_boxstate() const {
    BoxState bs;

    bs.player_index = _player;
    copy(begin(_boxes), end(_boxes), begin(bs.box_indexes));
    bs.box_bits = _is_box;

    return bs;
}

void BoardState::set_boxstate(const BoxState & bs) {
    copy_n(begin(bs.box_indexes), _boxes.size(), begin(_boxes));
    _is_box = bs.box_bits;
    _player = bs.player_index;
}

void BoardState::apply_push(const PushInfo & pi) {
    _is_box[pi.from()] = false;
    _is_box[pi.to()] = true;
    replace(begin(_boxes), end(_boxes), pi.from(), pi.to());

    _player = pi.from();
}

string BoardState::level_as_string(bool draw_boxes) const {
    string result(_tiles.size() + 1, ' ');

    for (size_t i = 0; i < _tiles.size(); ++i) {
        const Tile tile = get_tile(_is_wall[i], draw_boxes && _player == i,
                                   draw_boxes && _is_box[i], _is_goal[i]);
        result[i] = Formatter::decode(tile);
    }
    return result;
}

void BoardState::print_level_string(const std::string & level) const {
    auto it = level.cbegin();
    for (size_t i = 0u; i < _height; ++i) {
        cout << setw(4) << distance(cbegin(level), it) << ": ";
        copy_n(it, _width, ostream_iterator<char>(cout));
        advance(it, _width);
        cout << " :" << distance(cbegin(level), it) - 1 << '\n';
    }
    cout << endl;
}

void BoardState::print_distances(const vector<size_t> & distances) const {
    string level = level_as_string(false);
    for (size_t i = 0; i < distances.size(); ++i) {
        if (distances[i] != 0) {
            level[i] = Formatter::number_to_char(distances[i]);
        }
    }

    print_level_string(level);
}

void BoardState::print(const vector<index_t> & marked) const {
    string level = level_as_string(marked.empty());
    for_each(begin(marked), end(marked), [&level](index_t ind){ level[ind] = '?'; });

    print_level_string(level);
}

