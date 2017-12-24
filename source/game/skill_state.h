/* Copyright [2013-2018] [Aaron Springstroh, Minimal Graphics Library]

This file is part of the Fractex.

Fractex is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Fractex is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Fractex.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __SKILL_STATE__
#define __SKILL_STATE__

#include <chrono>
#include <game/character.h>
#include <min/sample.h>

namespace game
{

class skill_state
{
  private:
    static constexpr float _max_energy = 100.0;
    enum skill_mode
    {
        jetpack,
        beam,
        grapple,
        missile
    };

    std::chrono::high_resolution_clock::time_point _charge;
    std::chrono::high_resolution_clock::time_point _cool;
    float _energy;
    bool _gun_active;
    bool _locked;
    skill_mode _mode;
    bool _shoot_cooldown;
    bool _charging;

    inline double get_charge_time() const
    {
        // Get the current time
        const std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

        // return time since last sync
        return std::chrono::duration<double, std::milli>(now - _charge).count();
    }
    inline double get_cool_time() const
    {
        // Get the current time
        const std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

        // return time since last sync
        return std::chrono::duration<double, std::milli>(now - _cool).count();
    }

  public:
    skill_state() : _energy(0.0), _gun_active(true), _locked(false), _mode(skill_mode::beam), _shoot_cooldown(false) {}
    inline void add_energy(const float energy)
    {
        // Absorb this amount of energy if not full
        if (_energy < _max_energy)
        {
            _energy += energy;

            // Cap energy at max_energy
            if (_energy > _max_energy)
            {
                _energy = _max_energy;
            }
        }
    }
    inline bool activate_charge()
    {
        // Should we start charging?
        if (!_charging)
        {
            // Check if we have activated beam mode
            if (is_beam_mode() && _locked)
            {
                // If minimum charging has been activated
                if (get_charge_time() > 250.0)
                {
                    // Set charging debounce
                    _charging = true;

                    // Activate charging
                    return true;
                }
            }
        }

        // Do not active charging
        return false;
    }
    inline bool can_consume(const float energy) const
    {
        // Try to consume energy
        if (_energy >= energy)
        {
            return true;
        }

        // Not enough energy
        return false;
    }
    inline void consume(const float energy)
    {
        // Consume energy
        _energy -= energy;
    }
    inline bool will_consume(const float energy)
    {
        // Try to consume energy
        if (_energy >= energy)
        {
            _energy -= energy;
            return true;
        }

        // Not enough energy
        return false;
    }
    inline float get_energy() const
    {
        return _energy;
    }
    inline float get_energy_percent() const
    {
        return _energy / _max_energy;
    }
    inline bool is_beam_charged() const
    {
        return is_beam_mode() && _locked && get_charge_time() > 1000.0;
    }
    inline bool is_gun_active() const
    {
        return _gun_active;
    }
    inline bool is_jetpack_mode() const
    {
        return _mode == skill_mode::jetpack;
    }
    inline bool is_beam_mode() const
    {
        return _mode == skill_mode::beam;
    }
    inline bool is_grapple_mode() const
    {
        return _mode == skill_mode::grapple;
    }
    inline bool is_missile_mode() const
    {
        return _mode == skill_mode::missile;
    }
    inline bool is_locked() const
    {
        return _locked;
    }
    inline bool is_off_cooldown() const
    {
        return !_shoot_cooldown;
    }
    inline void lock()
    {
        _locked = true;
    }
    inline void unlock_beam()
    {
        if (_mode == skill_mode::beam)
        {
            _locked = false;
            _charging = false;
        }
    }
    inline void unlock_grapple()
    {
        if (_mode == skill_mode::grapple)
        {
            _locked = false;
        }
    }
    inline void unlock_missile()
    {
        if (_mode == skill_mode::missile)
        {
            _locked = false;
        }
    }
    inline void unlock_jetpack()
    {
        if (_mode == skill_mode::jetpack)
        {
            _locked = false;
        }
    }
    inline bool check_cooldown()
    {
        if (_shoot_cooldown)
        {
            const double dt = get_cool_time();
            if (dt > 2000.0)
            {
                _shoot_cooldown = !_shoot_cooldown;
            }
        }

        return !_shoot_cooldown;
    }
    inline void start_charge()
    {
        _charge = std::chrono::high_resolution_clock::now();
    }
    inline void start_cooldown()
    {
        // Start the cooldown timer
        _shoot_cooldown = true;

        // Update start time
        _cool = std::chrono::high_resolution_clock::now();
    }
    inline void set_energy(const float energy)
    {
        _energy = energy;
    }
    inline void set_gun_active(const bool mode)
    {
        _gun_active = mode;
    }
    inline void set_jetpack_mode()
    {
        _mode = skill_mode::jetpack;
    }
    inline void set_beam_mode()
    {
        _mode = skill_mode::beam;
    }
    inline void set_grapple_mode()
    {
        _mode = skill_mode::grapple;
    }
    inline void set_missile_mode()
    {
        _mode = skill_mode::missile;
    }
};
}

#endif
