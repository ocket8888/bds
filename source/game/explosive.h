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
#ifndef __EXPLOSIVE__
#define __EXPLOSIVE__

#include <functional>
#include <game/static_instance.h>
#include <min/aabbox.h>
#include <min/grid.h>
#include <min/physics_nt.h>
#include <min/vec3.h>
#include <vector>

namespace game
{
class explosive
{
  private:
    size_t _body_id;
    size_t _inst_id;
    int8_t _atlas;

  public:
    explosive(const size_t body_id, const size_t inst_id, const int8_t atlas)
        : _body_id(body_id), _inst_id(inst_id), _atlas(atlas) {}

    inline int8_t atlas() const
    {
        return _atlas;
    }
    inline size_t body_id() const
    {
        return _body_id;
    }
    inline void dec_inst()
    {
        _inst_id--;
    }
    inline size_t inst_id() const
    {
        return _inst_id;
    }
};

class explosives
{
  private:
    static constexpr size_t _ex_size = 10;
    static constexpr float _rotation_rate = 2.0;
    typedef min::physics<float, uint16_t, uint32_t, min::vec3, min::aabbox, min::aabbox, min::grid> physics;
    typedef std::function<void(min::body<float, min::vec3> &, min::body<float, min::vec3> &)> coll_call;
    typedef std::function<void(const min::vec3<float> &point, const min::vec3<float> &direction, const min::vec3<unsigned> &scale, const int8_t value)> ex_call;
    physics *_sim;
    static_instance *_inst;
    std::vector<std::pair<min::aabbox<float, min::vec3>, int8_t>> _col_cells;
    std::vector<explosive> _ex;
    const min::vec3<unsigned> _scale;
    float _angle;
    size_t _oldest;
    coll_call _f;

    inline min::body<float, min::vec3> &body(const size_t index)
    {
        return _sim->get_body(_ex[index].body_id());
    }
    inline const min::body<float, min::vec3> &body(const size_t index) const
    {
        return _sim->get_body(_ex[index].body_id());
    }
    inline const min::vec3<float> &position(const size_t index) const
    {
        // Return the explosive position
        return body(index).get_position();
    }
    inline const min::vec3<float> &velocity(const size_t index) const
    {
        // Return the explosive position
        return body(index).get_linear_velocity();
    }
    inline void remove(const size_t index)
    {
        // Clear explosives at index
        _inst->clear_explosive(_ex[index].inst_id());
        _sim->clear_body(_ex[index].body_id());
        _ex.erase(_ex.begin() + index);

        // Adjust the remaining explosive indices
        const size_t size = _ex.size();
        for (size_t i = index; i < size; i++)
        {
            // Adjust the instance id
            _ex[i].dec_inst();

            // Adjust the body data index
            body(i).set_data(min::body_data(i));
        }
    }
    inline void reserve_memory()
    {
        // Reserve space for collision cells
        _col_cells.reserve(27);
        _ex.reserve(_ex_size);
    }

  public:
    explosives(physics *sim, static_instance *inst)
        : _sim(sim), _inst(inst),
          _scale(3, 3, 3), _angle(0.0), _oldest(0), _f(nullptr)
    {
        // Reserve memory for collision cells
        reserve_memory();
    }

    inline void explode(const size_t index, const int8_t atlas, const ex_call &f)
    {
        // Get direction opposing body velocity
        const min::vec3<float> v = min::vec3<float>(velocity(index)).normalize() * -1.0;

        // Call the explosion callback function
        f(position(index), v, _scale, atlas);

        // Blow up the grenade
        remove(index);
    }
    inline bool launch(const min::vec3<float> &p, const min::vec3<float> &dir, const int8_t atlas)
    {
        // If all explosives have been allocated fail to launch
        if (_inst->explosive_full())
        {
            return false;
        }

        // Create a explosive instance
        const size_t inst_id = _inst->add_explosive(p, atlas);

        // Create a box for the explosive
        const min::aabbox<float, min::vec3> box = _inst->box_explosive(inst_id);

        // Add to physics simulation
        const size_t body_id = _sim->add_body(box, 10.0, 3);

        // Register player collision callback
        _sim->register_callback(body_id, _f);

        // Get the physics body for editing
        min::body<float, min::vec3> &body = _sim->get_body(body_id);

        // Set body linear velocity
        const min::vec3<float> lv = min::vec3<float>(0.0, 5.0, 0.0) + dir * 20.0;
        body.set_linear_velocity(lv);

        // Create a new explosive
        _ex.emplace_back(body_id, inst_id, atlas);

        // Store the explosive index as body data
        const size_t index = _ex.size() - 1;
        body.set_data(min::body_data(index));

        // Return launch success
        return true;
    }
    inline void set_collision_callback(const coll_call &f)
    {
        _f = f;
    }
    inline void update_frame(const cgrid &grid, const float dt, const ex_call &f)
    {
        // Do explosive collisions
        const size_t size = _ex.size();
        for (size_t i = 0; i < size; i++)
        {
            // Get all cells that could collide
            grid.explosive_collision_cells(_col_cells, position(i));

            // Solve static collisions
            const size_t body = _ex[i].body_id();
            for (const auto &cell : _col_cells)
            {
                if (_sim->collide(body, cell.first))
                {
                    // Explode the explosive
                    explode(i, cell.second, f);

                    // Decrement current index
                    i--;

                    // Abort inner loop
                    break;
                }
            }
        }
    }
    inline void update(const cgrid &grid)
    {
        // Update the explosive rotation angle
        _angle += _rotation_rate;
        if (_angle > 360.0)
        {
            _angle -= 360.0;
        }

        // Calculate quaternion around Y axis
        const min::quat<float> q(min::vec3<float>::up(), _angle);

        // Update all explosive positions
        const size_t size = _ex.size();
        for (size_t i = 0; i < size; i++)
        {
            // Update the instance matrix
            const size_t inst_id = _ex[i].inst_id();

            // Update body positions
            const min::vec3<float> &p = body(i).get_position();
            _inst->update_explosive_position(inst_id, p);

            // Update body rotations
            _inst->update_explosive_rotation(inst_id, q);
        }
    }
};
}

#endif