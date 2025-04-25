#pragma once

#include "osg/Geometry.hpp"

#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

namespace gltf
{

struct Subbuffer
{
    std::size_t positions_offset;
    std::size_t positions_byte_size;
    std::size_t normals_offset;
    std::size_t normals_byte_size;
    std::size_t texcoords_offset;
    std::size_t texcoords_byte_size;
    std::size_t indices_offset;
    std::size_t indices_byte_size;

    std::vector<osg::Vec3f> positions;
    std::vector<osg::Vec3f> normals;
    std::vector<osg::Vec2f> texcoords;
    std::vector<std::uint32_t> indices;
};

struct Buffer
{
    using Geometry_index = unsigned int;

    std::map<Geometry_index, Subbuffer> subbuffers;
    std::size_t size;
};

}
