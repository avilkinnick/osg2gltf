#pragma once

namespace gltf
{

struct Mesh
{
    unsigned int positions_index;
    unsigned int normals_index;
    unsigned int texcoords_index;
    unsigned int indices_index;
    unsigned int material_index;
};

}
