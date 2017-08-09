/* Copyright [2013-2016] [Aaron Springstroh, Minimal Graphics Library]

This file is part of the MGLCraft.

MGLCraft is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MGLCraft is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with MGLCraft.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __CONTROLS__
#define __CONTROLS__

#include <iostream>
#include <min/camera.h>
#include <min/ray.h>
#include <min/window.h>
#include <stdexcept>
#include <world.h>

namespace game
{

class controls
{
  private:
    min::window *_window;
    min::camera<float> *_camera;
    game::world *_world;

  public:
    controls(min::window &window, min::camera<float> &camera, game::world &world) : _window(&window), _camera(&camera), _world(&world)
    {
        // Check that pointers are valid
        if (!_window || !_camera || !_world)
        {
            throw std::runtime_error("control: Invalid contorl pointers");
        }

        // Get access to the keyboard
        auto &keyboard = _window->get_keyboard();

        // Register click callback function for placing path
        _window->register_data((void *)this);
        _window->register_lclick(controls::place_block);
        _window->register_rclick(controls::remove_block);

        // Add FPS(WADS) keys to watch
        keyboard.add(min::window::key_code::KEYQ);
        keyboard.add(min::window::key_code::KEYW);
        keyboard.add(min::window::key_code::KEYS);
        keyboard.add(min::window::key_code::KEYA);
        keyboard.add(min::window::key_code::KEYD);
        keyboard.add(min::window::key_code::KEYE);
        keyboard.add(min::window::key_code::SPACE);
        keyboard.add(min::window::key_code::KEYZ);
        keyboard.add(min::window::key_code::KEYX);
        keyboard.add(min::window::key_code::KEYC);
        keyboard.add(min::window::key_code::KEY1);
        keyboard.add(min::window::key_code::KEY2);
        keyboard.add(min::window::key_code::KEY3);
        keyboard.add(min::window::key_code::KEY4);

        // Register callback function for closing window
        keyboard.register_keydown(min::window::key_code::KEYQ, controls::close_window, (void *)_window);

        // Register callback function W
        keyboard.register_keydown(min::window::key_code::KEYW, controls::forward, (void *)this);
        keyboard.set_per_frame(min::window::key_code::KEYW, true);

        // Register callback function A
        keyboard.register_keydown(min::window::key_code::KEYA, controls::left, (void *)this);
        keyboard.set_per_frame(min::window::key_code::KEYA, true);

        // Register callback function D
        keyboard.register_keydown(min::window::key_code::KEYD, controls::right, (void *)this);
        keyboard.set_per_frame(min::window::key_code::KEYD, true);

        // Register callback function E
        keyboard.register_keydown(min::window::key_code::KEYE, controls::reset, (void *)this);

        // Register callback function S
        keyboard.register_keydown(min::window::key_code::KEYS, controls::back, (void *)this);
        keyboard.set_per_frame(min::window::key_code::KEYS, true);

        // Register callback function SPACE
        keyboard.register_keyup(min::window::key_code::SPACE, controls::jump, (void *)this);

        // Register callback function Z
        keyboard.register_keydown(min::window::key_code::KEYZ, controls::add_x, (void *)_world);

        // Register callback function X
        keyboard.register_keydown(min::window::key_code::KEYX, controls::add_y, (void *)_world);

        // Register callback function C
        keyboard.register_keydown(min::window::key_code::KEYC, controls::add_z, (void *)_world);

        // Register callback function KEY1 for switching texture to 'grass'
        keyboard.register_keydown(min::window::key_code::KEY1, controls::switch_grass, (void *)_world);

        // Register callback function KEY2 for switching texture to 'stone'
        keyboard.register_keydown(min::window::key_code::KEY2, controls::switch_stone, (void *)_world);

        // Register callback function KEY2 for switching texture to 'sand'
        keyboard.register_keydown(min::window::key_code::KEY3, controls::switch_sand, (void *)_world);

        // Register callback function KEY2 for switching texture to 'wood'
        keyboard.register_keydown(min::window::key_code::KEY4, controls::switch_wood, (void *)_world);
    }
    min::camera<float> *get_camera()
    {
        return _camera;
    }
    game::world *get_world()
    {
        return _world;
    }
    min::window *get_window()
    {
        return _window;
    }
    static void close_window(void *ptr, double step)
    {
        // Cast to window pointer type and call shut down on window
        min::window *const win = reinterpret_cast<min::window *>(ptr);
        win->set_shutdown();

        // Alert that we received the call back
        std::cout << "controls: Shutdown called by user" << std::endl;
    }
    static void stop(void *ptr, double step)
    {
        // Cast to camera pointer type and move camera
        controls *control = reinterpret_cast<controls *>(ptr);

        // Get the world pointers
        game::world *const world = control->get_world();
        world->character_move(min::vec3<float>());
    }
    static void forward(void *ptr, double step)
    {
        // Cast to camera pointer type and move camera
        controls *control = reinterpret_cast<controls *>(ptr);

        // Get the camera and world pointers
        min::camera<float> *const camera = control->get_camera();
        game::world *const world = control->get_world();
        const min::vec3<float> &direction = camera->get_forward();
        world->character_move(direction);
    }
    static void left(void *ptr, double step)
    {
        // Cast to camera pointer type and move camera
        controls *control = reinterpret_cast<controls *>(ptr);

        // Get the camera and world pointers
        min::camera<float> *const camera = control->get_camera();
        game::world *const world = control->get_world();
        const min::vec3<float> &right = camera->get_frustum().get_right();
        world->character_move(right * -1.0);
    }
    static void right(void *ptr, double step)
    {
        // Cast to camera pointer type and move camera
        controls *control = reinterpret_cast<controls *>(ptr);

        // Get the camera and world pointers
        min::camera<float> *const camera = control->get_camera();
        game::world *const world = control->get_world();
        const min::vec3<float> &right = camera->get_frustum().get_right();
        world->character_move(right);
    }
    static void back(void *ptr, double step)
    {
        // Cast to camera pointer type and move camera
        controls *control = reinterpret_cast<controls *>(ptr);

        // Get the camera and world pointers
        min::camera<float> *const camera = control->get_camera();
        game::world *const world = control->get_world();
        const min::vec3<float> &direction = camera->get_forward();
        world->character_move(direction * -1.0);
    }
    static void jump(void *ptr, double step)
    {
        // Cast to camera pointer type and move camera
        controls *control = reinterpret_cast<controls *>(ptr);

        // Get the world pointer
        game::world *const world = control->get_world();
        world->character_jump(min::vec3<float>(0.0, 1.0, 0.0));
    }
    static void switch_grass(void *ptr, double step)
    {
        // Cast to world pointer type and set atlas id to '0'
        game::world *const world = reinterpret_cast<game::world *>(ptr);
        world->set_atlas_id(0);
    }
    static void switch_stone(void *ptr, double step)
    {
        // Cast to world pointer type and set atlas id to '1'
        game::world *const world = reinterpret_cast<game::world *>(ptr);
        world->set_atlas_id(1);
    }
    static void switch_sand(void *ptr, double step)
    {
        // Cast to world pointer type and set atlas id to '2'
        game::world *const world = reinterpret_cast<game::world *>(ptr);
        world->set_atlas_id(2);
    }
    static void switch_wood(void *ptr, double step)
    {
        // Cast to world pointer type and set atlas id to '3'
        game::world *const world = reinterpret_cast<game::world *>(ptr);
        world->set_atlas_id(3);
    }
    static void add_x(void *ptr, double step)
    {
        // Cast to world pointer type
        game::world *const world = reinterpret_cast<game::world *>(ptr);

        // Increase x scale
        world->set_scale_x(1);
    }
    static void add_y(void *ptr, double step)
    {
        // Cast to world pointer type
        game::world *const world = reinterpret_cast<game::world *>(ptr);

        // Increase x scale
        world->set_scale_y(1);
    }
    static void add_z(void *ptr, double step)
    {
        // Cast to world pointer type
        game::world *const world = reinterpret_cast<game::world *>(ptr);

        // Increase x scale
        world->set_scale_z(1);
    }
    static void reset(void *ptr, double step)
    {
        // Cast to camera pointer type and move camera
        controls *control = reinterpret_cast<controls *>(ptr);

        // Get the world pointer
        game::world *const world = control->get_world();

        // Reset scale
        world->reset_scale();
    }
    static void place_block(void *ptr, const uint16_t x, const uint16_t y)
    {
        // Cast to camera pointer type and move camera
        controls *control = reinterpret_cast<controls *>(ptr);

        // Get the camera and world pointers
        min::camera<float> *const camera = control->get_camera();
        game::world *const world = control->get_world();

        // Calculate new point to add
        const min::vec3<float> point = camera->project_point(3.0);

        // Create a ray from camera to destination
        const min::ray<float, min::vec3> r(camera->get_position(), point);

        // Add block to world
        world->add_block(r);
    }
    static void remove_block(void *ptr, const uint16_t x, const uint16_t y)
    {
        // Cast to camera pointer type and move camera
        controls *control = reinterpret_cast<controls *>(ptr);

        // Get the camera and world pointers
        min::camera<float> *const camera = control->get_camera();
        game::world *const world = control->get_world();

        // Calculate point to remove from
        const min::vec3<float> point = camera->project_point(3.0);

        // Create a ray from camera to destination
        const min::ray<float, min::vec3> r(camera->get_position(), point);

        // Remove block from world
        world->remove_block(r);
    }
};
}

#endif
