#pragma once

namespace osg_types
{

enum class Blend_func_enum
{
    ZERO,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA,
    SRC_ALPHA_SATURATE
};

enum Color_mode
{
    AMBIENT,
    DIFFUSE,
    SPECULAR,
    EMISSION,
    AMBIENT_AND_DIFFUSE,
    OFF
};

enum class Data_variance
{
    STATIC,
    DYNAMIC,
    UNSPECIFIED
};

enum class Face_type
{
    FRONT,
    BACK,
    FRONT_AND_BACK
};

enum class Render_bin_mode
{
    INHERIT,
    USE,
    OVERRIDE,
    USE_RENDERBIN_DETAILS
};

enum class Rendering_hint
{
    DEFAULT_BIN,
    OPAQUE_BIN,
    TRANSPARENT_BIN
};

}
