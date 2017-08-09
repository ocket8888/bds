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
#ifndef __WORLD__
#define __WORLD__

#include <cgrid.h>
#include <cmath>
#include <cstdint>
#include <explode_particle.h>
#include <min/bmp.h>
#include <min/camera.h>
#include <min/physics.h>
#include <min/program.h>
#include <min/shader.h>
#include <min/stream_vertex.h>
#include <min/texture_buffer.h>
#include <min/tree.h>
#include <min/uniform_buffer.h>
#include <min/vertex_buffer.h>
#include <stdexcept>
#include <vector>

namespace game
{

class world
{
  private:
    // Opengl stuff
    min::shader _tv;
    min::shader _tf;
    min::program _terrain_program;
    min::uniform_buffer<float> _preview;
    min::uniform_buffer<float> _geom;
    min::vertex_buffer<float, uint32_t, min::stream_vertex, GL_FLOAT, GL_UNSIGNED_INT> _pb;
    min::vertex_buffer<float, uint32_t, min::stream_vertex, GL_FLOAT, GL_UNSIGNED_INT> _gb;
    min::texture_buffer _tbuffer;
    GLuint _bmp_id;
    min::bmp _bmp;

    // User stuff
    min::vec3<unsigned> _scale;

    // Grid
    cgrid _grid;

    // Physics stuff
    min::vec3<float> _gravity;
    min::physics<float, uint16_t, uint32_t, min::vec3, min::aabbox, min::aabbox, min::tree> _simulation;
    size_t _char_id;

    // Particle stuff
    explode_particle _particles;

    inline void draw_placemark() const
    {
        // Bind VAO
        _pb.bind();

        // Draw placemarker
        _pb.draw_all(GL_TRIANGLES);
    }
    inline void draw_terrain() const
    {
        // Bind VAO
        _gb.bind();

        // Draw graph-mesh
        _gb.draw_all(GL_TRIANGLES);
    }
    // Generate all geometry in grid and adds it to geometry buffer
    void generate_gb()
    {
        // Reset the buffer
        _gb.clear();

        // Get surrounding chunks for drawing
        const std::vector<size_t> keys = _grid.get_view_chunks();

        // Add all chunks to the geometry buffer
        for (const auto &key : keys)
        {
            const auto &chunk = _grid.get_chunk(key);
            if (chunk.vertex.size() > 0)
            {
                _gb.add_mesh(chunk);
            }
        }

        // Bind the gb VAO
        _gb.bind();

        // Upload contents to the vertex buffer
        _gb.upload();
    }
    // Generates the preview geometry and adds it to preview buffer
    void generate_pb()
    {
        // Reset the buffer
        _pb.clear();

        // Store start point => (0,0,0)
        min::vec3<float> p;

        // x axis
        for (size_t i = 0; i < _scale.x(); i++)
        {
            // y axis
            p.y(0.0);
            for (size_t j = 0; j < _scale.y(); j++)
            {
                // z axis
                p.z(0.0);
                for (size_t k = 0; k < _scale.z(); k++)
                {
                    const min::mesh<float, uint32_t> box_mesh = _grid.atlas_box(p);
                    _pb.add_mesh(box_mesh);
                    p.z(p.z() + 1.0);
                }
                p.y(p.y() + 1.0);
            }
            p.x(p.x() + 1.0);
        }

        //Bind the pb VAO
        _pb.bind();

        // Upload contents to the vertex buffer
        _pb.upload();
    }
    void load_character(const min::vec3<float> &p)
    {
        // Create a box for camera movement and add to simulation
        const min::vec3<float> half_extent(0.45, 0.95, 0.45);
        const min::aabbox<float, min::vec3> box(p - half_extent, p + half_extent);
        _char_id = _simulation.add_body(box, 10.0, 1);

        // Get the physics body for editing
        min::body<float, min::vec3> &body = _simulation.get_body(_char_id);

        // Set this body to be unrotatable
        body.set_no_rotate();
    }
    void load_uniform()
    {
        // Load the uniform buffer with program we will use
        _preview.set_program(_terrain_program);
        _geom.set_program(_terrain_program);

        // Let the particle system use this uniform buffer
        _particles.set_uniforms(_preview);

        // Change light alpha for placemark
        const min::vec4<float> col1(1.0, 1.0, 1.0, 1.0);
        const min::vec4<float> pos1(0.0, 100.0, 0.0, 1.0);
        const min::vec4<float> pow1(0.5, 1.0, 0.0, 0.25);
        _preview.add_light(min::light<float>(col1, pos1, pow1));

        // Add light to scene
        const min::vec4<float> col2(1.0, 1.0, 1.0, 1.0);
        const min::vec4<float> pos2(0.0, 100.0, 0.0, 1.0);
        const min::vec4<float> pow2(0.5, 1.0, 0.0, 1.0);
        _geom.add_light(min::light<float>(col2, pos2, pow2));

        // Load projection and view matrix into uniform buffer
        _preview.add_matrix(min::mat4<float>());
        _preview.add_matrix(min::mat4<float>());
        _preview.add_matrix(min::mat4<float>());
        _preview.add_matrix(min::mat4<float>());
        _geom.add_matrix(min::mat4<float>());
        _geom.add_matrix(min::mat4<float>());
        _geom.add_matrix(min::mat4<float>());
        _geom.add_matrix(min::mat4<float>());

        // Load the buffer with data
        _preview.update();
        _geom.update();
    }
    inline min::vec3<float> snap(const min::vec3<float> &point) const
    {
        const float x = point.x();
        const float y = point.y();
        const float z = point.z();

        return min::vec3<float>(std::floor(x) + 0.5, std::floor(y) + 0.5, std::floor(z) + 0.5);
    }
    inline void update_uniform(min::camera<float> &cam)
    {
        // Calculate new placemark point
        const min::vec3<float> dest = cam.project_point(3.0);

        // Create a ray from camera to destination
        const min::ray<float, min::vec3> r(cam.get_position(), dest);

        // Trace a ray to the destination point to find placement position, return point is snapped
        const min::vec3<float> translate = _grid.ray_trace_before(r);

        // Set camera at the character position
        const min::vec3<float> &p = character_position();
        cam.set_position(p + min::vec3<float>(0.0, 1.0, 0.0));

        // Update geom matrix uniforms
        _geom.set_matrix(cam.get_pv_matrix(), 0);
        _geom.set_matrix(cam.get_v_matrix(), 1);
        _geom.set_matrix(translate, 2);
        _geom.update_matrix();

        // Update preview matrix uniforms
        _preview.set_matrix(cam.get_pv_matrix(), 0);
        _preview.set_matrix(cam.get_v_matrix(), 1);
        _preview.set_matrix(translate, 2);
        _preview.set_matrix(min::mat4<float>(cam.get_position()), 3);
        _preview.update_matrix();
    }

  public:
    world(const std::string &texture_file, const size_t grid_size, const size_t chunk_size, const size_t view_chunk_size)
        : _tv("data/shader/terrain.vertex", GL_VERTEX_SHADER),
          _tf("data/shader/terrain.fragment", GL_FRAGMENT_SHADER),
          _terrain_program(_tv, _tf),
          _preview(1, 4),
          _geom(1, 4),
          _bmp(texture_file),
          _scale(3, 3, 3),
          _grid(grid_size, chunk_size, view_chunk_size),
          _gravity(0.0, -10.0, 0.0),
          _simulation(_grid.get_world(), _gravity)
    {
        // Check if chunk_size is valid
        if (grid_size % chunk_size != 0)
        {
            throw std::runtime_error("world: chunk_size must be integer multiple of grid_size");
        }

        // Load texture buffer
        _bmp_id = _tbuffer.add_bmp_texture(_bmp);

        // Set the collision elasticity of the physics simulation
        _simulation.set_elasticity(0.1);

        // Load uniform buffers
        load_uniform();

        // Reload the character
        min::vec3<float> position(0.0, 2.0, 0.0);
        load_character(position);

        // Update recent chunk
        _grid.update(position);

        // Remove geometry around player
        remove_block(min::vec3<float>(-1.0, -0.0, -1.0), position);

        // Reset scale
        _scale = min::vec3<unsigned>(1, 1, 1);

        // Generate the preview buffer
        generate_pb();

        // generate new mesh
        generate_gb();
    }
    void remove_block(const min::vec3<float> &point, const min::vec3<float> &position)
    {
        // Try to remove geometry from the grid
        const unsigned removed = _grid.set_geometry(snap(point), _scale, -1);

        // If we removed geometry, play particles
        if (removed > 0)
        {
            // generate new mesh
            generate_gb();

            // Add particle effects
            const min::vec3<float> direction = (position - point).normalize();
            _particles.load(point, direction, 5.0);
        }
    }
    void remove_block(const min::ray<float, min::vec3> &r)
    {
        // Trace a ray to the destination point to find placement position, return point is snapped
        const min::vec3<float> traced = _grid.ray_trace_after(r);

        // Try to remove geometry from the grid
        const unsigned removed = _grid.set_geometry(traced, _scale, -1);

        // If we removed geometry, play particles
        if (removed > 0)
        {
            // generate new mesh
            generate_gb();

            // Add particle effects
            _particles.load(traced, r.get_direction() * -1.0, 5.0);
        }
    }
    void add_block(const min::vec3<float> &center)
    {
        // Add to grid
        _grid.set_geometry(snap(center), _scale, _grid.get_atlas());

        // generate new mesh
        generate_gb();
    }
    void add_block(const min::ray<float, min::vec3> &r)
    {
        // Trace a ray to the destination point to find placement position, return point is snapped
        const min::vec3<float> traced = _grid.ray_trace_before(r);

        // Add to grid
        _grid.set_geometry(traced, _scale, _grid.get_atlas());

        // generate new mesh
        generate_gb();
    }
    void character_jump(const min::vec3<float> &vel)
    {
        min::body<float, min::vec3> &body = _simulation.get_body(_char_id);

        // If not jumping or falling, allow jump
        if (std::abs(body.get_linear_velocity().y()) < 1.0)
        {
            // Update the position
            body.add_force(vel * 4000.0 * body.get_mass());
        }
    }
    void character_move(const min::vec3<float> &vel)
    {
        min::body<float, min::vec3> &body = _simulation.get_body(_char_id);

        // Get the current position and set y movement to zero
        const min::vec3<float> dxz = min::vec3<float>(vel.x(), 0.0, vel.z()).normalize();

        // Update the position
        body.add_force(dxz * 1E2 * body.get_mass());
    }
    const min::vec3<float> &character_position() const
    {
        const min::body<float, min::vec3> &body = _simulation.get_body(_char_id);

        // Return the character position
        return body.get_position();
    }
    void draw(min::camera<float> &cam, const float dt)
    {
        // Get the player physics object
        min::body<float, min::vec3> &body = _simulation.get_body(_char_id);

        // Get player position
        const min::vec3<float> &p = body.get_position();

        // Detect if we crossed a chunk boundary
        bool is_valid = true;
        const size_t current_chunk = _grid.chunk_key(p, is_valid);
        const size_t recent_chunk = _grid.get_recent_chunk();
        if (is_valid && recent_chunk != current_chunk)
        {
            // Update the recent chunk
            _grid.update(current_chunk);

            // Generate a new geometry mesh
            generate_gb();
        }

        // Solve the physics simulation
        for (int i = 0; i < 10; i++)
        {
            const std::vector<min::aabbox<float, min::vec3>> col_blocks = _grid.create_collision_cells(snap(p));

            // Add friction force
            const min::vec3<float> &vel = body.get_linear_velocity();
            const min::vec3<float> xz(vel.x(), 0.0, vel.z());

            // Add friction force opposing lateral motion
            body.add_force(xz * body.get_mass() * -2.0);

            _simulation.solve_static(col_blocks, _char_id, dt / 10.0, 10.0);
        }

        // Bind this texture for drawing
        _tbuffer.bind(_bmp_id, 0);

        // update camera matrices
        update_uniform(cam);

        // Use the terrain program for drawing
        _terrain_program.use();

        // Activate the uniform buffer
        _geom.bind();

        // Draw the world geometry
        draw_terrain();

        // Activate the uniform buffer
        _preview.bind();

        // Draw the placemark
        draw_placemark();

        // Draw the particles
        _particles.draw(_preview, cam, dt);
    }
    void set_atlas_id(const int8_t id)
    {
        _grid.set_atlas(id);

        // Regenerate the preview mesh
        generate_pb();
    }
    void set_scale_x(unsigned dx)
    {
        if (_scale.x() < 5)
        {
            _scale.x(_scale.x() + dx);

            // Regenerate the preview mesh
            generate_pb();
        }
    }
    void set_scale_y(unsigned dy)
    {
        if (_scale.y() < 5)
        {
            _scale.y(_scale.y() + dy);

            // Regenerate the preview mesh
            generate_pb();
        }
    }
    void set_scale_z(unsigned dz)
    {
        if (_scale.z() < 5)
        {
            _scale.z(_scale.z() + dz);

            // Regenerate the preview mesh
            generate_pb();
        }
    }
    void reset_scale()
    {
        _scale = min::vec3<unsigned>(1, 1, 1);

        // Regenerate the preview mesh
        generate_pb();
    }
};
}

#endif
