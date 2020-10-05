/* Copyright 2018 REPLACE_WITH_YOUR_NAME
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef UK80_H
#define UK80_H

#include "quantum.h"


/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
/*
 * ,------------------------------------------------------------
 * |Esc|   | F1| F2| F3| F4| | F5| f6| F7| F8| | F9|F10|Ins|F12|
 * ,------------------------------------------------------------
 * |`¬ |  1|  2|  3|  4|  5|  6|  7|  8|  9|  0| -_| =+|Del|Bsp|
 * |------------------------------------------------------------
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P| [{| ]}|Enter|
 * |------------------------------------------------------.    -
 * |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L| ;:| '@| #~|    |
 * |------------------------------------------------------------
 * |Sft| \||  Z|  X|  C|  V|  B|  N|  M| ,<| .>| /?| Up| Shift |
 * |------------------------------------------------------------
 * |Ctrl| Mag| Alt| space | space | space | Alt|Lft|Dwn|Rgh|Ctl|
 * `------------------------------------------------------------
*/
#define LAYOUT_80_iso( \
  K000,       K002, K003, K004, K005,       K007, K008, K009, K010, K011, K012, K013, K014, \
  K100, K101, K102, K103, K104, K105, K106, K107, K108, K109, K110, K111, K112, K113, K114, \
  K200,       K202, K203, K204, K205, K206, K207, K208, K209, K210, K211, K212, K213,       \
  K300,       K302, K303, K304, K305, K306, K307, K308, K309, K310, K311, K312, K313, K314, \
  K400, K401, K402, K403, K404, K405, K406, K407, K408, K409, K410, K411, K412,       K414, \
  K500, K501,       K503, K504,       K506,       K508,       K510, K511, K512, K513, K514  \
) { \
  { K000,  KC_NO, K002,  K003,  K004,  K005,  KC_NO, K007,  K008,  K009,  K010,  K011,  K012,  K013,  K014 }, \
  { K100,  K101,  K102,  K103,  K104,  K105,  K106,  K107,  K108,  K109,  K110,  K111,  K112,  K113,  K114 }, \
  { K200,  KC_NO, K202,  K203,  K204,  K205,  K206,  K207,  K208,  K209,  K210,  K211,  K212,  K213,  KC_NO }, \
  { K300,  KC_NO, K302,  K303,  K304,  K305,  K306,  K307,  K308,  K309,  K310,  K311,  K312,  K313,  K314 }, \
  { K400,  K401,  K402,  K403,  K404,  K405,  K406,  K407,  K408,  K409,  K410,  K411,  K412,  KC_NO, K414 }, \
  { K500,  K501,  KC_NO, K503,  K504,  KC_NO, K506,  KC_NO, K508,  KC_NO, K510,  K511,  K512,  K513,  K514 }  \
}
#endif
