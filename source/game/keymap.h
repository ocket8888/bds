/* Copyright [2013-2018] [Aaron Springstroh, Minimal Graphics Library]

This file is part of the Beyond Dying Skies.

Beyond Dying Skies is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Beyond Dying Skies is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Beyond Dying Skies.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __KEY_MAP__
#define __KEY_MAP__

#include <min/window.h>
#include <string>
#include <vector>

namespace game
{

class key_map
{
  private:
    static constexpr size_t _max_prefix = 26;
    static constexpr size_t _max_keys = 75;
    std::vector<std::string> _prefix;
    std::vector<std::string> _key;

    void load_prefix_strings()
    {
        _prefix[0] = "For: ";
        _prefix[1] = "Back: ";
        _prefix[2] = "Left: ";
        _prefix[3] = "Right: ";
        _prefix[4] = "Reset: ";
        _prefix[5] = "ScaleX: ";
        _prefix[6] = "ScaleY: ";
        _prefix[7] = "ScaleZ: ";
        _prefix[8] = "Item1: ";
        _prefix[9] = "Item2: ";
        _prefix[10] = "Item3: ";
        _prefix[11] = "Item4: ";
        _prefix[12] = "Item5: ";
        _prefix[13] = "Item6: ";
        _prefix[14] = "Item7: ";
        _prefix[15] = "Item8: ";
        _prefix[16] = "Jump: ";
        _prefix[17] = "Dash: ";
        _prefix[18] = "Inv: ";
        _prefix[19] = "Debug: ";
        _prefix[20] = "Wire: ";
        _prefix[21] = "MusicUp: ";
        _prefix[22] = "MusicDown: ";
        _prefix[23] = "Menu: ";
        _prefix[24] = "Use: ";
        _prefix[25] = "Drop: ";
    }
    void load_key_strings()
    {
        _key[0] = "F1";
        _key[1] = "F2";
        _key[2] = "F3";
        _key[3] = "F4";
        _key[4] = "F5";
        _key[5] = "F6";
        _key[6] = "F7";
        _key[7] = "F8";
        _key[8] = "F9";
        _key[9] = "F10";
        _key[10] = "F11";
        _key[11] = "F12";
        _key[12] = "0";
        _key[13] = "1";
        _key[14] = "2";
        _key[15] = "3";
        _key[16] = "4";
        _key[17] = "5";
        _key[18] = "6";
        _key[19] = "7";
        _key[20] = "8";
        _key[21] = "9";
        _key[22] = "A";
        _key[23] = "B";
        _key[24] = "C";
        _key[25] = "D";
        _key[26] = "E";
        _key[27] = "F";
        _key[28] = "G";
        _key[29] = "H";
        _key[30] = "I";
        _key[31] = "J";
        _key[32] = "K";
        _key[33] = "L";
        _key[34] = "M";
        _key[35] = "N";
        _key[36] = "O";
        _key[37] = "P";
        _key[38] = "Q";
        _key[39] = "R";
        _key[40] = "S";
        _key[41] = "T";
        _key[42] = "U";
        _key[43] = "V";
        _key[44] = "W";
        _key[45] = "X";
        _key[46] = "Y";
        _key[47] = "Z";
        _key[48] = "NUM 0";
        _key[49] = "NUM 1";
        _key[50] = "NUM 2";
        _key[51] = "NUM 3";
        _key[52] = "NUM 4";
        _key[53] = "NUM 5";
        _key[54] = "NUM 6";
        _key[55] = "NUM 7";
        _key[56] = "NUM 8";
        _key[57] = "NUM 9";
        _key[58] = "LSHIFT";
        _key[59] = "RSHIFT";
        _key[60] = "LCONTROL";
        _key[61] = "RCONTROL";
        _key[62] = "TAB";
        _key[63] = "ENTER";
        _key[64] = "BACKSPACE";
        _key[65] = "SPACE";
        _key[66] = "LALT";
        _key[67] = "RALT";
        _key[68] = "ESC";
        _key[69] = "APOST";
        _key[70] = "QUOTE";
        _key[71] = ",";
        _key[72] = ".";
        _key[73] = ";";
        _key[74] = "???";
    }

  public:
    key_map(const size_t prefix_size) : _prefix(std::max(prefix_size, _max_prefix), "?"), _key(_max_keys)
    {
        // Load the key strings
        load_prefix_strings();

        // Load the key strings
        load_key_strings();
    }
    const std::string &get_prefix_string(const size_t index) const
    {
        return _prefix[index];
    }
    const std::string &get_key_string(const min::window::key_type key) const
    {
        switch (key)
        {
        case min::window::key_code::F1:
            return _key[0];
        case min::window::key_code::F2:
            return _key[1];
        case min::window::key_code::F3:
            return _key[2];
        case min::window::key_code::F4:
            return _key[3];
        case min::window::key_code::F5:
            return _key[4];
        case min::window::key_code::F6:
            return _key[5];
        case min::window::key_code::F7:
            return _key[6];
        case min::window::key_code::F8:
            return _key[7];
        case min::window::key_code::F9:
            return _key[8];
        case min::window::key_code::F10:
            return _key[9];
        case min::window::key_code::F11:
            return _key[10];
        case min::window::key_code::F12:
            return _key[11];
        case min::window::key_code::KEY0:
            return _key[12];
        case min::window::key_code::KEY1:
            return _key[13];
        case min::window::key_code::KEY2:
            return _key[14];
        case min::window::key_code::KEY3:
            return _key[15];
        case min::window::key_code::KEY4:
            return _key[16];
        case min::window::key_code::KEY5:
            return _key[17];
        case min::window::key_code::KEY6:
            return _key[18];
        case min::window::key_code::KEY7:
            return _key[19];
        case min::window::key_code::KEY8:
            return _key[20];
        case min::window::key_code::KEY9:
            return _key[21];
        case min::window::key_code::KEYA:
            return _key[22];
        case min::window::key_code::KEYB:
            return _key[23];
        case min::window::key_code::KEYC:
            return _key[24];
        case min::window::key_code::KEYD:
            return _key[25];
        case min::window::key_code::KEYE:
            return _key[26];
        case min::window::key_code::KEYF:
            return _key[27];
        case min::window::key_code::KEYG:
            return _key[28];
        case min::window::key_code::KEYH:
            return _key[29];
        case min::window::key_code::KEYI:
            return _key[30];
        case min::window::key_code::KEYJ:
            return _key[31];
        case min::window::key_code::KEYK:
            return _key[32];
        case min::window::key_code::KEYL:
            return _key[33];
        case min::window::key_code::KEYM:
            return _key[34];
        case min::window::key_code::KEYN:
            return _key[35];
        case min::window::key_code::KEYO:
            return _key[36];
        case min::window::key_code::KEYP:
            return _key[37];
        case min::window::key_code::KEYQ:
            return _key[38];
        case min::window::key_code::KEYR:
            return _key[39];
        case min::window::key_code::KEYS:
            return _key[40];
        case min::window::key_code::KEYT:
            return _key[41];
        case min::window::key_code::KEYU:
            return _key[42];
        case min::window::key_code::KEYV:
            return _key[43];
        case min::window::key_code::KEYW:
            return _key[44];
        case min::window::key_code::KEYX:
            return _key[45];
        case min::window::key_code::KEYY:
            return _key[46];
        case min::window::key_code::KEYZ:
            return _key[47];
        case min::window::key_code::NUM0:
            return _key[48];
        case min::window::key_code::NUM1:
            return _key[49];
        case min::window::key_code::NUM2:
            return _key[50];
        case min::window::key_code::NUM3:
            return _key[51];
        case min::window::key_code::NUM4:
            return _key[52];
        case min::window::key_code::NUM5:
            return _key[53];
        case min::window::key_code::NUM6:
            return _key[54];
        case min::window::key_code::NUM7:
            return _key[55];
        case min::window::key_code::NUM8:
            return _key[56];
        case min::window::key_code::NUM9:
            return _key[57];
        case min::window::key_code::LSHIFT:
            return _key[58];
        case min::window::key_code::RSHIFT:
            return _key[59];
        case min::window::key_code::LCONTROL:
            return _key[60];
        case min::window::key_code::RCONTROL:
            return _key[61];
        case min::window::key_code::TAB:
            return _key[62];
        case min::window::key_code::ENTER:
            return _key[63];
        case min::window::key_code::BACKSPACE:
            return _key[64];
        case min::window::key_code::SPACE:
            return _key[65];
        case min::window::key_code::LALT:
            return _key[66];
        case min::window::key_code::RALT:
            return _key[67];
        case min::window::key_code::ESCAPE:
            return _key[68];
        case min::window::key_code::APOSTROPHE:
            return _key[69];
        case min::window::key_code::QUOTE:
            return _key[70];
        case min::window::key_code::COMMA:
            return _key[71];
        case min::window::key_code::PERIOD:
            return _key[72];
        case min::window::key_code::SEMICOLON:
            return _key[73];
        default:
            return _key[74];
        }
    }
};
}

#endif
