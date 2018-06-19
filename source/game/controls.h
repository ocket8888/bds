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
#ifndef __CONTROLS__
#define __CONTROLS__

#include <game/id.h>
#include <game/sound.h>
#include <game/state.h>
#include <game/ui_overlay.h>
#include <game/world.h>
#include <iostream>
#include <min/camera.h>
#include <min/ray.h>
#include <min/window.h>
#include <stdexcept>

namespace game
{

class controls
{
  private:
    static constexpr float _project_dist = 3.0;
    min::window *_window;
    min::camera<float> *_camera;
    character *_character;
    state *_state;
    ui_overlay *_ui;
    world *_world;
    sound *_sound;

  public:
    controls(min::window &window, min::camera<float> &camera, character &ch,
             state &state, ui_overlay &ui, world &world, sound &sound)
        : _window(&window), _camera(&camera), _character(&ch),
          _state(&state), _ui(&ui), _world(&world), _sound(&sound) {}

    min::camera<float> *get_camera()
    {
        return _camera;
    }
    character *get_character()
    {
        return _character;
    }
    sound *get_sound()
    {
        return _sound;
    }
    state *get_state()
    {
        return _state;
    }
    ui_overlay *get_ui()
    {
        return _ui;
    }
    world *get_world()
    {
        return _world;
    }
    min::window *get_window()
    {
        return _window;
    }
    void register_control_callbacks()
    {
        // Get the player skill beam string
        const inventory &inv = _world->get_player().get_inventory();

        // Enable the console and set default message
        _ui->enable_console();
        _ui->set_console_string(inv.get_name(inv.get_key(0).id()));

        // Get access to the keyboard
        auto &keyboard = _window->get_keyboard();

        // Clear any keys mapped to keyboard
        keyboard.clear();

        // Register data and function callbacks
        _window->register_data((void *)this);
        _window->register_lclick_down(controls::left_click_down);
        _window->register_lclick_up(controls::left_click_up);
        _window->register_rclick_down(controls::right_click_down);
        _window->register_rclick_up(controls::right_click_up);
        _window->register_update(controls::on_resize);

        // Add FPS(WADS) keys to watch
        keyboard.add(min::window::key_code::F1);
        keyboard.add(min::window::key_code::F2);
        keyboard.add(min::window::key_code::F3);
        keyboard.add(min::window::key_code::F4);
        keyboard.add(min::window::key_code::ESCAPE);
        keyboard.add(min::window::key_code::KEYW);
        keyboard.add(min::window::key_code::KEYS);
        keyboard.add(min::window::key_code::KEYA);
        keyboard.add(min::window::key_code::KEYD);
        keyboard.add(min::window::key_code::KEYR);
        keyboard.add(min::window::key_code::KEYE);
        keyboard.add(min::window::key_code::KEYZ);
        keyboard.add(min::window::key_code::KEYX);
        keyboard.add(min::window::key_code::KEYC);
        keyboard.add(min::window::key_code::KEY1);
        keyboard.add(min::window::key_code::KEY2);
        keyboard.add(min::window::key_code::KEY3);
        keyboard.add(min::window::key_code::KEY4);
        keyboard.add(min::window::key_code::KEY5);
        keyboard.add(min::window::key_code::KEY6);
        keyboard.add(min::window::key_code::KEY7);
        keyboard.add(min::window::key_code::KEY8);
        keyboard.add(min::window::key_code::SPACE);
        keyboard.add(min::window::key_code::TAB);
        keyboard.add(min::window::key_code::LSHIFT);
        keyboard.add(min::window::key_code::KEYQ);

        // Register callback function F1
        keyboard.register_keydown(min::window::key_code::F1, controls::toggle_text, (void *)this);

        // Register callback function F2
        keyboard.register_keydown(min::window::key_code::F2, controls::toggle_wireframe, (void *)this);

        // Register callback function F3
        keyboard.register_keydown(min::window::key_code::F3, controls::music_down, (void *)_sound);

        // Register callback function F4
        keyboard.register_keydown(min::window::key_code::F4, controls::music_up, (void *)_sound);

        // Register callback function ESCAPE
        keyboard.register_keydown(min::window::key_code::ESCAPE, controls::toggle_pause, (void *)this);

        // Register callback function W
        keyboard.register_keydown_per_frame(min::window::key_code::KEYW, controls::forward, (void *)this);

        // Register callback function S
        keyboard.register_keydown_per_frame(min::window::key_code::KEYS, controls::back, (void *)this);

        // Register callback function A
        keyboard.register_keydown_per_frame(min::window::key_code::KEYA, controls::left, (void *)this);

        // Register callback function D
        keyboard.register_keydown_per_frame(min::window::key_code::KEYD, controls::right, (void *)this);

        // Register callback function E
        keyboard.register_keydown(min::window::key_code::KEYE, controls::select, (void *)this);

        // Register callback function R
        keyboard.register_keydown(min::window::key_code::KEYR, controls::reset, (void *)this);

        // Register callback function Z
        keyboard.register_keydown(min::window::key_code::KEYZ, controls::add_x, (void *)this);

        // Register callback function X
        keyboard.register_keydown(min::window::key_code::KEYX, controls::add_y, (void *)this);

        // Register callback function C
        keyboard.register_keydown(min::window::key_code::KEYC, controls::add_z, (void *)this);

        // Register callback function KEY1 for switching texture
        keyboard.register_keydown(min::window::key_code::KEY1, controls::key1_down, (void *)this);
        keyboard.register_keyup(min::window::key_code::KEY1, controls::key1_up, (void *)this);

        // Register callback function KEY2 for switching texture
        keyboard.register_keydown(min::window::key_code::KEY2, controls::key2_down, (void *)this);
        keyboard.register_keyup(min::window::key_code::KEY2, controls::key2_up, (void *)this);

        // Register callback function KEY3 for switching texture
        keyboard.register_keydown(min::window::key_code::KEY3, controls::key3_down, (void *)this);
        keyboard.register_keyup(min::window::key_code::KEY3, controls::key3_up, (void *)this);

        // Register callback function KEY4 for switching texture
        keyboard.register_keydown(min::window::key_code::KEY4, controls::key4_down, (void *)this);
        keyboard.register_keyup(min::window::key_code::KEY4, controls::key4_up, (void *)this);

        // Register callback function KEY5 for switching texture
        keyboard.register_keydown(min::window::key_code::KEY5, controls::key5_down, (void *)this);
        keyboard.register_keyup(min::window::key_code::KEY5, controls::key5_up, (void *)this);

        // Register callback function KEY6 for switching texture
        keyboard.register_keydown(min::window::key_code::KEY6, controls::key6_down, (void *)this);
        keyboard.register_keyup(min::window::key_code::KEY6, controls::key6_up, (void *)this);

        // Register callback function KEY7 for switching texture
        keyboard.register_keydown(min::window::key_code::KEY7, controls::key7_down, (void *)this);
        keyboard.register_keyup(min::window::key_code::KEY7, controls::key7_up, (void *)this);

        // Register callback function KEY8 for switching texture
        keyboard.register_keydown(min::window::key_code::KEY8, controls::key8_down, (void *)this);
        keyboard.register_keyup(min::window::key_code::KEY8, controls::key8_up, (void *)this);

        // Register callback function SPACE
        keyboard.register_keydown(min::window::key_code::SPACE, controls::jump, (void *)this);

        // Register callback function TAB
        keyboard.register_keydown(min::window::key_code::TAB, controls::ui_extend, (void *)this);

        // Register callback function LSHIFT
        keyboard.register_keydown(min::window::key_code::LSHIFT, controls::shift_down, (void *)this);
        keyboard.register_keyup(min::window::key_code::LSHIFT, controls::shift_up, (void *)this);

        // Register callback function KEYQ
        keyboard.register_keydown(min::window::key_code::KEYQ, controls::drop_item, (void *)this);
    }
    static void toggle_text(void *const ptr, double step)
    {
        // Get the state pointers
        controls *const control = reinterpret_cast<controls *>(ptr);
        ui_overlay *const ui = control->get_ui();

        // Enable / disable drawing debug text
        ui->toggle_debug_text();
    }
    static void toggle_wireframe(void *const ptr, double step)
    {
        // Get the state pointers
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Enable / disable drawing wireframe
        state->toggle_wireframe();
    }
    static void music_down(void *const ptr, double step)
    {
        // Get the sound pointer
        sound *const sound = reinterpret_cast<game::sound *>(ptr);

        // Decrease the music gain
        sound->bg_gain_down();
    }
    static void music_up(void *const ptr, double step)
    {
        // Get the sound pointer
        sound *const sound = reinterpret_cast<game::sound *>(ptr);

        // Increase the music gain
        sound->bg_gain_up();
    }
    static void toggle_pause(void *const ptr, double step)
    {
        // Get the state, window and ui pointers
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();
        min::window *const win = control->get_window();
        ui_overlay *const ui = control->get_ui();

        // If the UI is extended and we are pausing
        if (!state->get_pause() && ui->is_extended())
        {
            // Close the extended UI
            ui->toggle_extend();
        }

        // Toggle pause, adjust cursor, and set user input
        const bool paused = state->toggle_pause();
        state->set_user_input(paused);
        win->display_cursor(paused);

        // Set the game mode caption from toggle state
        if (paused)
        {
            // Switch to menu mode
            ui->switch_mode_menu();
        }
        else
        {
            // Switch to anti menu mode
            ui->switch_mode_no_menu();
        }

        // Center cursor in middle of window
        win->set_cursor(win->get_width() / 2, win->get_height() / 2);
    }
    static void forward(void *const ptr, double step)
    {
        // Get the state, camera, world pointers
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        min::camera<float> *const camera = control->get_camera();
        world *const world = control->get_world();

        // Move the character
        const min::vec3<float> &direction = camera->get_forward();
        world->get_player().move(direction);
    }
    static void left(void *const ptr, double step)
    {
        // Get the camera and world pointers
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        min::camera<float> *const camera = control->get_camera();
        world *const world = control->get_world();

        // Move the character
        const min::vec3<float> &right = camera->get_frustum().get_right();
        world->get_player().move(right * -1.0);
    }
    static void right(void *const ptr, double step)
    {
        // Get the camera and world pointers
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        min::camera<float> *const camera = control->get_camera();
        world *const world = control->get_world();

        // Move the character
        const min::vec3<float> &right = camera->get_frustum().get_right();
        world->get_player().move(right);
    }
    static void back(void *const ptr, double step)
    {
        // Get the camera and world pointers
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        min::camera<float> *const camera = control->get_camera();
        world *const world = control->get_world();

        // Move the character
        const min::vec3<float> &direction = camera->get_forward();
        world->get_player().move(direction * -1.0);
    }
    void key_down(const size_t index)
    {
        // Early exit if pause
        if (_state->get_pause())
        {
            return;
        }

        // Get the skills pointer
        player &play = _world->get_player();
        skills &skill = play.get_skills();
        stats &stat = play.get_stats();

        // Lookup key in inventory
        const inventory &inv = play.get_inventory();
        const item &it = inv.get_key(index);

        // Equip item
        stat.equip_item(it);

        // Get the item id and type
        const item_type type = it.type();
        const item_id id = it.id();

        // Is the gun locked?
        const bool locked = skill.is_locked();

        // What type of item is it?
        const bool is_empty = (type == item_type::empty);
        const bool is_block = (type == item_type::block);
        const bool is_item = (type == item_type::item);
        const bool is_skill = (type == item_type::skill);
        const bool is_ether = (id == item_id::CONS_ETHER);

        // Selecting multiple times?
        const bool multi_click = _ui->get_selected() == ui_id(index).to_key();

        // If not locked
        if (!locked)
        {
            // Modify ui toolbar
            _ui->set_key_down(index);

            // Set the console string from item
            _ui->set_console_string(inv.get_name(it.id()));

            // Cancel edit mode
            if (!is_block && !is_ether)
            {
                // Disable edit mode
                _world->set_edit_mode(false, false, false);
            }

            // Play selection sound
            if (!is_empty && !is_item)
            {
                // Play selection sound
                _sound->play_click();
            }
            else
            {
                play.set_mode(play_mode::none);
            }

            // Choose action
            if (is_block)
            {
                // Reset scale
                _world->reset_scale();

                // Enable edit mode
                _world->set_edit_mode(true, false, false);

                // Set atlas id
                _world->set_atlas_id(id_to_atlas(id));

                // Set place mode
                play.set_mode(play_mode::place);
            }
            else if (is_skill)
            {
                // Set skill mode
                switch (id)
                {
                case item_id::AUTO_BEAM:
                    play.set_mode(play_mode::gun);
                    skill.set_auto_mode();
                    break;
                case item_id::BEAM:
                    play.set_mode(play_mode::gun);
                    skill.set_beam_mode();
                    break;
                case item_id::CHARGE:
                    play.set_mode(play_mode::gun);
                    skill.set_charge_mode();
                    break;
                case item_id::GRAPPLE:
                    play.set_mode(play_mode::gun);
                    skill.set_grapple_mode();
                    break;
                case item_id::GRENADE:
                    play.set_mode(play_mode::gun);
                    skill.set_grenade_mode();
                    break;
                case item_id::JET:
                    play.set_mode(play_mode::skill);
                    skill.set_jetpack_mode();
                    break;
                case item_id::MISSILE:
                    play.set_mode(play_mode::gun);
                    skill.set_missile_mode();
                    break;
                case item_id::PORTAL:
                    play.set_mode(play_mode::gun);
                    skill.set_portal_mode();
                    break;
                case item_id::SCAN:
                    play.set_mode(play_mode::skill);
                    skill.set_scan_mode();
                    break;
                case item_id::SCATTER:
                    play.set_mode(play_mode::gun);
                    skill.set_scatter_mode();
                    break;
                case item_id::SPEED:
                    play.set_mode(play_mode::none);
                    skill.set_speed_mode();
                    break;
                default:
                    play.set_mode(play_mode::none);
                    break;
                }
            }
            else if (is_ether)
            {
                if (multi_click && _world->is_edit_mode())
                {
                    // Swap copy / place mode
                    _world->toggle_swatch_copy_place();
                }
                else
                {
                    // Reset scale
                    _world->reset_scale();

                    // Turn on swatches
                    _world->set_edit_mode(true, true, true);
                }

                // Load the current swatch data
                _world->load_swatch();
            }
            else if (is_item && _ui->action_select())
            {
                // Play selection sound on item action
                _sound->play_click();
            }
        }
        else
        {
            // Failed to change state
            _ui->set_key_down_fail(index);
        }
    }
    static void key1_down(void *const ptr, double step)
    {
        // Get the skills pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        control->key_down(0);
    }
    static void key2_down(void *const ptr, double step)
    {
        // Get the skills pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        control->key_down(1);
    }
    static void key3_down(void *const ptr, double step)
    {
        // Get the skills pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        control->key_down(2);
    }
    static void key4_down(void *const ptr, double step)
    {
        // Get the skills pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        control->key_down(3);
    }
    static void key5_down(void *const ptr, double step)
    {
        // Get the skills pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        control->key_down(4);
    }
    static void key6_down(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        control->key_down(5);
    }
    static void key7_down(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        control->key_down(6);
    }
    static void key8_down(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        control->key_down(7);
    }
    static void key1_up(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);

        // Get the ui pointer
        ui_overlay *const ui = control->get_ui();
        ui->set_key_up(0);
    }
    static void key2_up(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);

        // Get the ui pointer
        ui_overlay *const ui = control->get_ui();
        ui->set_key_up(1);
    }
    static void key3_up(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);

        // Get the ui pointer
        ui_overlay *const ui = control->get_ui();
        ui->set_key_up(2);
    }
    static void key4_up(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);

        // Get the ui pointer
        ui_overlay *const ui = control->get_ui();
        ui->set_key_up(3);
    }
    static void key5_up(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);

        // Get the ui pointer
        ui_overlay *const ui = control->get_ui();
        ui->set_key_up(4);
    }
    static void key6_up(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);

        // Get the ui pointer
        ui_overlay *const ui = control->get_ui();
        ui->set_key_up(5);
    }
    static void key7_up(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);

        // Get the ui pointer
        ui_overlay *const ui = control->get_ui();
        ui->set_key_up(6);
    }
    static void key8_up(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);

        // Get the ui pointer
        ui_overlay *const ui = control->get_ui();
        ui->set_key_up(7);
    }
    static void add_x(void *const ptr, double step)
    {
        // Get the state pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        // Get the world pointer
        world *const world = control->get_world();

        // Increase x scale
        world->set_scale_x(1);
    }
    static void add_y(void *const ptr, double step)
    {
        // Get the state pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        // Get the world pointer
        world *const world = control->get_world();

        // Increase x scale
        world->set_scale_y(1);
    }
    static void add_z(void *const ptr, double step)
    {
        // Get the state pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        // Get the world pointer
        world *const world = control->get_world();

        // Increase x scale
        world->set_scale_z(1);
    }
    static void reset(void *const ptr, double step)
    {
        // Get the state pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        // Get the world pointer
        world *const world = control->get_world();

        // Reset scale
        world->reset_scale();
    }
    static void select(void *const ptr, double step)
    {
        // Get the state pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        // Get the world, ui, and player pointers
        world *const world = control->get_world();
        ui_overlay *const ui = control->get_ui();
        player &play = world->get_player();

        // Get current target
        const target &t = play.get_target();

        // Reset scale
        const auto p = world->select_target(t);
        if (p.second == static_id::CHEST)
        {
            if (p.first)
            {
                ui->set_alert_item();
            }
            else
            {
                ui->set_alert_item_fail();
            }
        }
    }
    static void ui_extend(void *const ptr, double step)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();
        ui_overlay *const ui = control->get_ui();

        // Do not toggle UI if paused
        if (!state->get_pause() && ui->toggle_extend())
        {
            // Get the ui and window pointer
            min::window *const win = control->get_window();

            // Center cursor when changing user state
            const uint_fast16_t w = win->get_width();
            const uint_fast16_t h = win->get_height();

            // Center cursor in middle of window
            win->set_cursor(w / 2, h / 2);

            // Hide or show the cursor
            const bool input = state->toggle_user_input();
            win->display_cursor(input);
        }
    }
    static void shift_down(void *const ptr, double step)
    {
        // Get the state pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        // Get the UI pointer
        ui_overlay *const ui = control->get_ui();
        if (ui->is_extended())
        {
            // Set the multiplier to 4
            ui->set_multiplier(4);
        }
        else
        {
            // Get the world pointer
            world *const world = control->get_world();
            world->get_player().dash();
        }
    }
    static void shift_up(void *const ptr, double step)
    {
        // Get the state pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        // Get the UI pointer
        ui_overlay *const ui = control->get_ui();

        // Set the multiplier to 4
        if (ui->is_extended())
        {
            ui->set_multiplier(1);
        }
    }
    static void drop_item(void *const ptr, double step)
    {
        // Get the state pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        // Get the UI pointer
        ui_overlay *const ui = control->get_ui();

        // Drop item if hovering
        ui->drop();
    }
    static void left_click_down(void *const ptr, const uint_fast16_t x, const uint_fast16_t y)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);

        // Get the ALL the pointers
        character *const character = control->get_character();
        state *const state = control->get_state();
        world *const world = control->get_world();
        player &play = world->get_player();
        skills &skill = play.get_skills();
        stats &stat = play.get_stats();
        sound *const sound = control->get_sound();
        ui_overlay *const ui = control->get_ui();

        // Are we dead?
        const bool dead = play.is_dead();
        if (dead)
        {
            // Signal for game to respawn
            state->set_respawn(true);

            // return early
            return;
        }

        // Does user have control of cursor?
        const bool input = state->get_user_input();
        const bool pause = state->get_pause();
        if (input)
        {
            // Do mouse click on UI
            if (ui->click_down())
            {
                // Play click sound
                sound->play_click();
            }

            // Early exit
            return;
        }
        else if (pause)
        {
            return;
        }

        // Activate action if active
        if (play.is_action_mode())
        {
            if (skill.is_auto_mode() && skill.is_off_cooldown())
            {
                if (stat.can_consume_beam())
                {
                    // Lock the gun in beam mode
                    skill.lock();
                }
                else
                {
                    sound->play_voice_low_power();
                    ui->set_alert_low_power();
                }
            }
            else if (skill.is_beam_mode() && skill.is_off_cooldown())
            {
                if (stat.can_consume_beam())
                {
                    // Lock the gun in beam mode
                    skill.lock();
                }
                else
                {
                    sound->play_voice_low_power();
                    ui->set_alert_low_power();
                }
            }
            else if (skill.is_charge_mode() && skill.is_off_cooldown())
            {
                if (stat.can_consume_charge())
                {
                    // Play the charge sound
                    sound->play_charge();

                    // Record the start charge time
                    skill.start_charge();

                    // Lock the gun in beam mode
                    skill.lock();
                }
                else
                {
                    sound->play_voice_low_power();
                    ui->set_alert_low_power();
                }
            }
            else if (skill.is_grapple_mode())
            {
                if (stat.is_dynamics_online())
                {
                    if (stat.can_consume_grapple())
                    {
                        // Fire grappling hook
                        const bool hit = world->hook_set();
                        if (hit)
                        {
                            // Consume energy
                            stat.consume_grapple();

                            // Activate grapple animation
                            character->set_animation_grapple(play.get_hook_point());

                            // Play the grapple sound
                            sound->play_grapple();

                            // Lock the gun in grapple mode
                            skill.lock();
                        }
                    }
                    else
                    {
                        sound->play_voice_low_power();
                        ui->set_alert_low_power();
                    }
                }
                else
                {
                    ui->set_alert_dynamics_unlock();
                }
            }
            else if (skill.is_jetpack_mode())
            {
                if (stat.is_dynamics_online())
                {
                    if (stat.can_consume_jet())
                    {
                        // Turn on the jets
                        play.set_jet(true);

                        // Play the jet sound
                        sound->play_jet();

                        // Lock the gun in jetpack mode
                        skill.lock();
                    }
                    else
                    {
                        sound->play_voice_low_power();
                        ui->set_alert_low_power();
                    }
                }
                else
                {
                    ui->set_alert_dynamics_unlock();
                }
            }
            else if (skill.is_missile_mode() && skill.is_off_cooldown())
            {
                // Try to consume energy to create missile
                if (stat.can_consume_missile())
                {
                    // Lock the gun in missile mode
                    skill.lock();
                }
                else
                {
                    sound->play_voice_low_power();
                    ui->set_alert_low_power();
                }
            }
            else if (skill.is_portal_mode())
            {
                // Try to consume energy to power this resource
                if (stat.can_consume_portal())
                {
                    // Consume energy
                    stat.consume_portal();

                    // Play the charge sound
                    sound->play_charge();

                    // Record the start charge time
                    skill.start_charge();

                    // Lock the gun in portal mode
                    skill.lock();
                }
                else
                {
                    sound->play_voice_low_power();
                    ui->set_alert_low_power();
                }
            }
            else if (skill.is_scan_mode())
            {
                // Lock the gun in scan mode
                skill.lock();
            }
            else if (skill.is_grenade_mode() && skill.is_off_cooldown())
            {
                // Try to consume energy to create grenade
                if (stat.can_consume_grenade())
                {
                    // Lock the gun in grenade mode
                    skill.lock();
                }
                else
                {
                    sound->play_voice_low_power();
                    ui->set_alert_low_power();
                }
            }
            else if (skill.is_scatter_mode() && skill.is_off_cooldown())
            {
                if (stat.can_consume_scatter())
                {
                    // Lock the gun in scatter mode
                    skill.lock();
                }
                else
                {
                    sound->play_voice_low_power();
                    ui->set_alert_low_power();
                }
            }
        }
    }
    static void left_click_up(void *const ptr, const uint_fast16_t x, const uint_fast16_t y)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);

        // Get the ALL the pointers
        min::camera<float> *const cam = control->get_camera();
        character *const character = control->get_character();
        world *const world = control->get_world();
        player &play = world->get_player();
        inventory &inv = play.get_inventory();
        skills &skill = play.get_skills();
        stats &stat = play.get_stats();
        state *const state = control->get_state();
        sound *const sound = control->get_sound();
        ui_overlay *const ui = control->get_ui();

        // Get the cooldown multiplier
        const float cd_mult = play.get_stats().get_cooldown_mult();

        // Click up event
        ui->click_up();

        // Do we need to release skill lock
        if (!skill.is_locked())
        {
            // Does user have control of cursor?
            const bool input = state->get_user_input();
            const bool pause = state->get_pause();
            const bool dead = play.is_dead();
            if (input || pause || dead)
            {
                return;
            }
        }

        // Create explode callback to play sound
        const auto play_ex = [sound](min::body<float, min::vec3> &body, const min::vec3<float> &point) -> void {
            // Play the missile explosion sound - !TODO!
            sound->play_explode(point);
        };

        // Check if we are in edit mode
        const bool mode = world->is_edit_mode();
        if (mode)
        {
            // Try to consume energy to create this resource
            if (world->can_add_block())
            {
                // Get selected index
                const size_t select_index = ui->get_selected().index();

                // Consume the resource
                bool consumed = false;
                if (world->is_swatch_mode())
                {
                    if (world->is_swatch_copy())
                    {
                        // Swap copy / place mode
                        world->toggle_swatch_copy_place();

                        // Load the current swatch data
                        world->load_swatch();

                        // Early return
                        return;
                    }
                    else
                    {
                        const unsigned count = world->get_swatch_cost();
                        consumed = inv.consume_multi(item_id::CONS_ETHER, count);
                    }
                }
                else
                {
                    const item_id id = id_from_atlas(world->get_atlas_id());
                    uint_fast8_t count = world->get_scale_size();
                    consumed = inv.consume(select_index, id, count);
                }

                // If we consumed a resource
                if (consumed)
                {
                    // Calculate new point to add
                    const min::vec3<float> point = cam->project_point(_project_dist);

                    // Create a ray from camera to destination
                    const min::ray<float, min::vec3> r(cam->get_position(), point);

                    // Add block to world
                    world->add_block(r);

                    // If we need to reset edit mode
                    const item &it = inv[select_index];
                    if (it.id() == item_id::EMPTY)
                    {
                        // Reset scale
                        world->reset_scale();

                        // Disable edit mode
                        world->set_edit_mode(false, false, false);
                    }
                }
                else
                {
                    ui->set_alert_low_resource();
                }
            }
            else
            {
                ui->set_alert_block_inside();
            }
        }
        else if (play.is_action_mode())
        {
            // Abort the charge animation
            character->abort_animation_shoot();

            // If skill is locked into a mode
            if (skill.is_locked())
            {
                if (skill.is_auto_mode())
                {
                    // Unlock the gun if auto mode
                    skill.unlock_auto();
                }
                else if (skill.is_charged())
                {
                    // Fire a charged explosion ray
                    const min::vec3<unsigned> radius(3, 3, 3);
                    const block_id hit = world->explode_ray(radius, 100.0, true, play_ex);
                    if (hit == block_id::EMPTY)
                    {
                        ui->add_stream_text("Miss!");
                    }

                    // Set recoil
                    state->set_recoil();

                    // Activate shoot animation
                    character->set_animation_shoot();

                    // Start gun cooldown timer
                    skill.start_cooldown(cd_mult);

                    // Consume energy
                    stat.consume_charge();

                    // Stop the charge sound
                    sound->stop_charge();

                    // Unlock the gun if charge mode
                    skill.unlock_charge();
                }
                else if (skill.is_beam_mode() || skill.is_charge_mode())
                {
                    // Remove block from world, get the removed block id
                    if (stat.can_consume_beam())
                    {
                        control->shoot_beam();
                    }
                    else
                    {
                        sound->play_voice_low_power();
                        ui->set_alert_low_power();
                    }

                    // Stop the charge sound
                    sound->stop_charge();

                    // Unlock the gun if beam mode
                    skill.unlock_beam();

                    // Unlock the gun if charge mode
                    skill.unlock_charge();
                }
                else if (skill.is_grapple_mode())
                {
                    //Abort grappling hook
                    play.hook_abort();

                    // Stop grapple animation
                    character->abort_animation_grapple();

                    // Stop the grapple sound
                    sound->stop_grapple();

                    // Unlock the gun if grapple mode
                    skill.unlock_grapple();
                }
                else if (skill.is_grenade_mode())
                {
                    uint_fast8_t count = 1;
                    const ui_id id = ui->get_selected();
                    const bool consumed = inv.consume(id.index(), item_id::GRENADE, count);
                    if (consumed)
                    {
                        // Ran out of ammo
                        if (count == 0)
                        {
                            play.set_mode(play_mode::none);
                        }

                        // Launch an explosive
                        const bool launched = world->launch_explosive(cam->get_up());
                        if (launched)
                        {
                            // Set recoil
                            state->set_recoil();

                            // Activate shoot animation
                            character->set_animation_shoot();

                            // Play the shot sound
                            sound->play_shot();

                            // Start gun cooldown timer
                            skill.start_cooldown(cd_mult);

                            // Consume energy
                            stat.consume_grenade();
                        }
                    }
                    else
                    {
                        play.set_mode(play_mode::none);
                    }

                    // Unlock the gun if grenade mode
                    skill.unlock_grenade();
                }
                else if (skill.is_jetpack_mode())
                {
                    // Turn off the jets
                    play.set_jet(false);

                    // Stop the jet sound
                    sound->stop_jet();

                    // Unlock the gun if jetpack mode
                    skill.unlock_jetpack();
                }
                else if (skill.is_missile_mode())
                {
                    uint_fast8_t count = 1;
                    const ui_id id = ui->get_selected();
                    const bool consumed = inv.consume(id.index(), item_id::MISSILE, count);
                    if (consumed)
                    {
                        // Ran out of ammo
                        if (count == 0)
                        {
                            play.set_mode(play_mode::none);
                        }

                        // Launch a missile
                        const bool launched = world->launch_missile();
                        if (launched)
                        {
                            // Set recoil
                            state->set_recoil();

                            // Activate shoot animation
                            character->set_animation_shoot();

                            // Start gun cooldown timer
                            skill.start_cooldown(cd_mult);

                            // Consume energy
                            stat.consume_missile();
                        }
                    }
                    else
                    {
                        play.set_mode(play_mode::none);
                    }

                    // Unlock the gun if missile mode
                    skill.unlock_missile();
                }
                else if (skill.is_portal_mode())
                {
                    // Stop portal animation
                    character->abort_animation_portal();

                    // Stop the charge sound
                    sound->stop_charge();

                    // If portal is charged
                    if (skill.is_portal_charged())
                    {
                        // Launch portal
                        world->portal();

                        // Play portal sound
                        sound->play_shot();
                    }

                    // Unlock the gun if portal mode
                    skill.unlock_portal();
                }
                else if (skill.is_scan_mode() && play.is_target_block())
                {
                    // Is player targetting a block?
                    const block_id atlas = play.get_target_atlas();

                    // Scan the block on this ray
                    const item_id it_id = id_from_atlas(atlas);
                    const std::string &text = inv.get_name(it_id);

                    // Set the console text
                    ui->set_console_string(text);

                    // Unlock the gun if scan mode
                    skill.unlock_scan();
                }
                else if (skill.is_scatter_mode())
                {
                    // Remove block from world, get the removed block id
                    if (stat.can_consume_scatter())
                    {
                        // Dummy callback
                        const auto f = [](min::body<float, min::vec3> &body, const min::vec3<float> &point) -> void {
                        };

                        // Shot scatter
                        const min::vec3<unsigned> radius(1, 1, 1);
                        const size_t hits = world->scatter_ray(radius, 20.0, f);
                        if (hits == 0)
                        {
                            ui->add_stream_text("Miss!");
                        }

                        // Set recoil
                        state->set_recoil();

                        // Activate shoot animation
                        character->set_animation_shoot();

                        // Play the shot sound
                        sound->play_shot();

                        // Start gun cooldown timer
                        skill.start_cooldown(cd_mult);

                        // Consume energy
                        stat.consume_scatter();
                    }
                    else
                    {
                        sound->play_voice_low_power();
                        ui->set_alert_low_power();
                    }

                    // Unlock the gun if scatter mode
                    skill.unlock_scatter();
                }
            }
        }
    }
    static void right_click_down(void *const ptr, const uint_fast16_t x, const uint_fast16_t y)
    {
        // Cast to control pointer
        controls *const control = reinterpret_cast<controls *>(ptr);

        // Get the camera and state pointers
        state *const state = control->get_state();
        world *const world = control->get_world();
        player &play = world->get_player();
        sound *const sound = control->get_sound();
        ui_overlay *const ui = control->get_ui();

        // Are we dead?
        const bool dead = play.is_dead();
        if (dead)
        {
            // Signal for game to respawn
            state->set_respawn(true);

            // return early
            return;
        }

        // Does user have control of cursor?
        const bool input = state->get_user_input();
        const bool pause = state->get_pause();
        if (input)
        {
            // Do mouse action on UI
            if (ui->action_hover())
            {
                // Play click sound
                sound->play_click();
            }

            // Early exit
            return;
        }
        else if (pause)
        {
            // Early exit
            return;
        }

        // Start camera tracking
        state->set_tracking(true);

        // Set tracking of target
        const target &t = play.get_target();
        state->set_target(t.get_position());
    }
    static void right_click_up(void *const ptr, const uint_fast16_t x, const uint_fast16_t y)
    {
        // Get the state and ui pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();
        ui_overlay *const ui = control->get_ui();

        // Stop camera tracking
        state->set_tracking(false);
        ui->set_draw_focus(false);
    }
    static void jump(void *const ptr, double step)
    {
        // Get the state pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        state *const state = control->get_state();

        // Early exit if paused
        if (state->get_pause())
        {
            return;
        }

        // Get the world pointer
        world *const world = control->get_world();

        // Make player jump
        world->get_player().jump();
    }
    static void on_resize(void *const ptr, const uint_fast16_t width, const uint_fast16_t height)
    {
        // Get the ui pointer
        controls *const control = reinterpret_cast<controls *>(ptr);
        ui_overlay *const ui = control->get_ui();

        // Ignore minimizing window
        if (width == 0 && height == 0)
        {
            // Set ui minimized
            ui->set_minimized(true);

            // Early return
            return;
        }

        // Unset ui minimized
        ui->set_minimized(false);

        // Get the frustum
        min::camera<float> *const camera = control->get_camera();
        auto &f = camera->get_frustum();

        // Update the aspect ratio
        f.set_aspect_ratio(width, height);
        camera->make_dirty();
        camera->force_update();

        // Get the screen dimensions
        const uint_fast16_t w2 = width / 2;
        const uint_fast16_t h2 = height / 2;

        // Update the screen size for ui and text
        ui->set_screen(min::vec2<float>(w2, h2), width, height);
    }
    void die()
    {
        // Stop camera tracking
        _state->set_tracking(false);
        _ui->set_draw_focus(false);

        // If extended close it
        if (_ui->is_extended())
        {
            ui_extend((void *)this, 0.0);
        }
    }
    void respawn()
    {
        // Simulate a key down to change equipment
        key_down(0);
    }
    inline void shoot_beam()
    {
        // Get player, skill and stats
        player &play = _world->get_player();
        skills &skill = play.get_skills();
        stats &stat = play.get_stats();

        // Dummy callback
        const auto f = [](min::body<float, min::vec3> &body, const min::vec3<float> &point) -> void {
        };

        // Fire an explosion ray
        const min::vec3<unsigned> radius(1, 1, 1);
        const block_id hit = _world->explode_ray(radius, 20.0, false, f);
        if (hit == block_id::EMPTY)
        {
            _ui->add_stream_text("Miss!");
        }

        // Set recoil
        _state->set_recoil();

        // Activate shoot animation
        _character->set_animation_shoot();

        // Play the shot sound
        _sound->play_shot();

        // Get the cooldown multiplier
        const float cd_mult = stat.get_cooldown_mult();

        // Start beam cooldown timer
        skill.start_cooldown(cd_mult);

        // Consume energy
        stat.consume_beam();
    }
    void update_stat_ui()
    {
        player &play = _world->get_player();
        stats &stat = play.get_stats();

        // If low energy
        if (stat.is_low_energy_flag())
        {
            // Play critical warning
            _sound->play_voice_power();

            // Reset low energy
            stat.clear_low_energy_flag();
        }

        // If low health
        if (stat.is_low_health_flag())
        {
            // Play critical warning
            _sound->play_voice_critical();

            // Reset low health
            stat.clear_low_health_flag();

            // Stream low health
            _ui->stream_low_health();
        }

        // If low oxygen
        if (stat.is_low_oxygen_flag())
        {
            // Play oxygen warning
            _sound->play_oxygen();

            // Reset low oxygen
            stat.clear_low_oxygen_flag();

            // Stream low oxygen
            _ui->stream_low_oxygen();
        }
        else if (!stat.is_low_oxygen())
        {
            _sound->stop_oxygen();
        }

        // Update the ui energy bar
        const float energy = stat.get_energy_fraction();
        _ui->set_energy(energy);

        // Update the ui experience bar
        const float exp = stat.get_experience_fraction();
        _ui->set_experience(exp);

        // Update the ui health bar
        const float health = stat.get_health_fraction();
        _ui->set_health(health);

        // Update the ui oxygen bar
        const float oxy = stat.get_oxygen_fraction();
        _ui->set_oxygen(oxy);
    }
    void update_skills()
    {
        player &play = _world->get_player();
        skills &skill = play.get_skills();
        stats &stat = play.get_stats();

        // Check if in jetpack mode
        if (skill.is_locked() && skill.is_auto_mode() && skill.is_off_cooldown())
        {
            if (stat.can_consume_beam())
            {
                // Lock the gun in beam mode
                shoot_beam();
            }
            else
            {
                // Unlock the gun if auto mode
                skill.unlock_auto();

                // Low resources
                _sound->play_voice_low_power();
                _ui->set_alert_low_power();
            }
        }
        if (skill.is_locked() && skill.is_jetpack_mode())
        {
            const bool active = play.is_jet();
            if (!active)
            {
                // Stop the jet sound
                _sound->stop_jet();

                // Unlock the jetpack
                skill.unlock_jetpack();
            }
        }

        // Activate charge animation
        if (skill.activate_charge())
        {
            _character->set_animation_charge(*_camera);
        }

        // Activate portal animation
        if (skill.activate_portal())
        {
            _character->set_animation_portal();
        }

        // Track block if player's target is a block
        if (_state->get_tracking())
        {
            const target &t = play.get_track_target();
            if (t.get_id() != target_id::INVALID)
            {
                // Set tracking of target
                _state->set_target(t.get_position());

                // Update the ui focus
                _ui->set_draw_focus(true);

                // Get the target info
                const auto info = _world->get_target_info(t);
                _ui->set_focus(info.second);

                // Update the focus text info
                if (play.is_target_update())
                {
                    // Set the focus text info
                    _ui->set_focus_string(*info.first);

                    // Clear target update debouncer
                    play.clear_target_update();

                    // Play the focus sound
                    _sound->play_focus();
                }
            }
            else
            {
                // Abort
                _state->set_tracking(false);
                _ui->set_draw_focus(false);
            }
        }
    }
    void update_ui()
    {
        // Check cursor state
        player &player = _world->get_player();
        skills &skill = player.get_skills();
        if (!skill.check_cooldown())
        {
            // Reloading
            _ui->set_cursor_reload();
        }
        else if (player.is_target_block() && player.get_target_atlas() == block_id::SODIUM && _world->in_range_explosion(player.get_target().get_position()))
        {
            _ui->set_cursor_target();
        }
        else
        {
            _ui->set_cursor_aim();
        }
    }
    void update()
    {
        // Update energy regen
        update_stat_ui();

        // Update ui
        update_ui();

        // Update skills
        update_skills();
    }
};
}

#endif
