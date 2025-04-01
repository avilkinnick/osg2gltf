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

enum class Color_control
{
    SINGLE_COLOR,
    SEPARATE_SPECULAR_COLOR
};

enum class Color_mode
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

enum class Depth_function
{
    NEVER,
    LESS,
    EQUAL,
    LEQUAL,
    GREATER,
    NOTEQUAL,
    GEQUAL,
    ALWAYS
};

enum class Face_enum
{
    FRONT,
    BACK,
    FRONT_AND_BACK
};

enum class Filter_mode
{
    NEAREST,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR
};

enum class Internal_format
{
    RGB,
    RGBA,
    RGB8,
    RGBA8,
    RGB16F,
    RGBA16F,
    DEPTH_COMPONENT,
    DEPTH_COMPONENT24
};

enum class Polygon_mode
{
    POINT,
    LINE,
    FILL
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

enum class Tex_env_mode
{
    DECAL,
    MODULATE,
    BLEND,
    REPLACE,
    ADD
};

enum class Tex_gen_mode
{
    OBJECT_LINEAR,
    EYE_LINEAR,
    SPHERE_MAP,
    NORMAL_MAP,
    REFLECTION_MAP
};

enum class Wrap_mode
{
    CLAMP,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
    REPEAT,
    MIRROR
};

}
