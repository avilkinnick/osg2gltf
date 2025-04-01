#pragma once

namespace osg_types
{

struct Mode_list
{
    Mode_list() noexcept
        : GL_LIGHTING(false)
        , GL_DEPTH_TEST(false)
        , GL_BLEND(false)
        , GL_ALPHA_TEST(false)
        , GL_CULL_FACE(false)
        , GL_NORMALIZE(false)
        , GL_RESCALE_NORMAL(false)
        , GL_POLYGON_OFFSET_FILL(false)
        , GL_POLYGON_OFFSET_LINE(false)
        , GL_POLYGON_OFFSET_POINT(false)
        , GL_POLYGON_STIPPLE(false)
        , GL_FOG(false)
        , GL_COLOR_MATERIAL(false)
        , GL_LINE_SMOOTH(false)
        , GL_POINT_SMOOTH(false)
        , GL_STENCIL_TEST(false)
        , GL_CLIP_PLANE0(false)
        , GL_CLIP_PLANE1(false)
        , GL_CLIP_PLANE2(false)
        , GL_CLIP_PLANE3(false)
        , GL_CLIP_PLANE4(false)
        , GL_CLIP_PLANE5(false)
        , GL_PROGRAM_POINT_SIZE(false)
        , GL_VERTEX_PROGRAM_TWO_SIDE(false)
    {
    }

    bool GL_LIGHTING: 1;
    bool GL_DEPTH_TEST: 1;
    bool GL_BLEND: 1;
    bool GL_ALPHA_TEST: 1;
    bool GL_CULL_FACE: 1;
    bool GL_NORMALIZE: 1;
    bool GL_RESCALE_NORMAL: 1;
    bool GL_POLYGON_OFFSET_FILL: 1;
    bool GL_POLYGON_OFFSET_LINE: 1;
    bool GL_POLYGON_OFFSET_POINT: 1;
    bool GL_POLYGON_STIPPLE: 1;
    bool GL_FOG: 1;
    bool GL_COLOR_MATERIAL: 1;
    bool GL_LINE_SMOOTH: 1;
    bool GL_POINT_SMOOTH: 1;
    bool GL_STENCIL_TEST: 1;
    bool GL_CLIP_PLANE0: 1;
    bool GL_CLIP_PLANE1: 1;
    bool GL_CLIP_PLANE2: 1;
    bool GL_CLIP_PLANE3: 1;
    bool GL_CLIP_PLANE4: 1;
    bool GL_CLIP_PLANE5: 1;
    bool GL_PROGRAM_POINT_SIZE: 1;
    bool GL_VERTEX_PROGRAM_TWO_SIDE: 1;
};

}
