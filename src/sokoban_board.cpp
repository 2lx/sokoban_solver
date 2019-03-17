#include "sokoban_board.h"
#include "sokoban_boxstate.h"

#include <algorithm>

using namespace Sokoban;
using namespace std;

void Board::initialize(std::vector<Tile> && maze,
                       unsigned short _width, unsigned short _height) {
    tiles = move(maze);
    width = _width;
    height = _height;
    const unsigned short count = static_cast<unsigned short>(width * height);

    gmoves.set_node_count(count);
    gpushes.set_node_count(count);

    for (unsigned short i = 0; i < count; i++) {
        const auto tile = tiles[i];
        if (is_passable(tile)) {
            if ( tile == Tile::Player
              || tile == Tile::PlayerOnGoal) { player = i; }

            if ( tile == Tile::Goal
              || tile == Tile::BoxOnGoal
              || tile == Tile::PlayerOnGoal) { goals.push_back(i); }

            if ( tile == Tile::Box
              || tile == Tile::BoxOnGoal)    { boxes.push_back(i); }

            bool is_topmost    = i < width;
            bool is_leftmost   = i % width == 0;
            bool is_rightmost  = i % width + 1 == width;
            bool is_bottommost = i / width + 1 == height;

            bool is_passableup    = !is_topmost    && is_passable(tiles[i - width]);
            bool is_passableleft  = !is_leftmost   && is_passable(tiles[i - 1]);
            bool is_passableright = !is_rightmost  && is_passable(tiles[i + 1]);
            bool is_passabledown  = !is_bottommost && is_passable(tiles[i + width]);

            if (is_passableup && is_passabledown) {
                gpushes.add_edge(static_cast<unsigned short>(i - width), i);
                gpushes.add_edge(static_cast<unsigned short>(i + width), i);
            }
            if (is_passableleft && is_passableright) {
                gpushes.add_edge(static_cast<unsigned short>(i - 1), i);
                gpushes.add_edge(static_cast<unsigned short>(i + 1), i);
            }

            if (is_passableup)    { gmoves.add_edge(i, static_cast<unsigned short>(i - width)); }
            if (is_passableleft)  { gmoves.add_edge(i, static_cast<unsigned short>(i - 1)); }
            if (is_passableright) { gmoves.add_edge(i, static_cast<unsigned short>(i + 1)); }
            if (is_passabledown)  { gmoves.add_edge(i, static_cast<unsigned short>(i + width)); }
        }
    }

    gpushes.remove_impassable(goals);
    gpushes.transpose();
}

void Board::print_graphs() const {
    cout << "Possible moves:\n";
    gmoves.print();
    cout << endl;

    cout << "Possible pushes:\n";
    gpushes.print();
}

BoxState Board::current_state() {
    BoxState state;

    auto gmovesbox {gmoves};
    for (const auto b: boxes) {
        gmovesbox.remove_vertice_edges(b);
    }

    state.player_position = gmovesbox.find_adjacent_min(player);

    sort(begin(boxes), end(boxes));
    copy(begin(boxes), end(boxes), begin(state.box_positions));

    return state;
}

void Board::push(PushType pt) {
    /* for (unsigned short i = 0; i < boxes.size(); i++) { */
    /*     if (boxes[i] == get<0>(pt)) { */
    /*         boxes[i] = get<1>(pt); */
    /*  */
    /*         if (get<0>(pt) < get<1>(pt)) { */
    /*             auto j = i; */
    /*             while (j + 1 < boxes.size() && boxes[j + 1] < boxes[j]) { */
    /*                 swap(boxes[j], boxes[j + 1]); */
    /*                 j++; */
    /*             } */
    /*         } */
    /*         else if (get<0>(pt) > get<1>(pt)) { */
    /*             auto j = i; */
    /*             while (j > 0 && boxes[j - 1] > boxes[j]) { */
    /*                 swap(boxes[j - 1], boxes[j]); */
    /*                 j--; */
    /*             } */
    /*         } */
    /*         break; */
    /*     } */
    /* } */

    auto it = find(begin(boxes), end(boxes), get<0>(pt));
    *it = get<1>(pt);
    player = get<0>(pt);
}

void Board::set_boxstate(const BoxState & bs) {
    for (size_t i = 0; i < boxes.size(); ++i) {
        boxes[i] = bs.box_positions[i];
    }
    player = bs.player_position;
}

bool Board::is_complete() const {
    for (auto ind: boxes) {
        const Tile tile = tiles[ind];
        if ( tile == Tile::Goal
          || tile == Tile::PlayerOnGoal
          || tile == Tile::BoxOnGoal) { continue; }
        else { return false; }
    }
    return true;
}

vector<PushType> Board::possible_pushes() const {
    vector<PushType> result;
    auto gmovesbox {gmoves};
    for (const auto b: boxes) {
        gmovesbox.remove_vertice_edges(b);
    }
    for (const auto ind: boxes) {
        /* cout << "POS: " << ind << endl; */
        for (auto ni: gpushes.siblings(ind)) {
            /* cout << "ind: " << ni << endl; */

            // we can omit the check of pushind correctness, because
            // it was checked during the initialize the 'gpushes' graph
            Direction dir;
            unsigned short pushind;
            if (ni == static_cast<unsigned short>(ind - width)) {
                dir = Direction::Up;
                pushind = static_cast<unsigned short>(ind + width);
            }
            else if (ni == static_cast<unsigned short>(ind - 1)) {
                dir = Direction::Left;
                pushind = static_cast<unsigned short>(ind + 1);
            }
            else if (ni == static_cast<unsigned short>(ind + 1))     {
                dir = Direction::Right;
                pushind = static_cast<unsigned short>(ind - 1);
            }
            else if (ni == static_cast<unsigned short>(ind + width)) {
                dir = Direction::Down;
                pushind = static_cast<unsigned short>(ind - width);
            }

            // check if the destination of the pushed box is occupied
            // or player's place is occupied
            if (end(boxes) != find_if(begin(boxes), end(boxes),
                [ni, pushind](auto ind){return ni == ind || pushind == ind; }) ) { continue; }

            // check if player has access to the cell to push
            if (!gmovesbox.is_joined(player, pushind)) { continue; }

            result.push_back(make_tuple(ind, ni, dir));
        }
    }

    return result;
}
