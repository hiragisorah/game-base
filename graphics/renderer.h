#pragma once

constexpr unsigned int kShaderResourceMax = 9;
constexpr unsigned int kRenderTargetMax = 9;
constexpr unsigned int kViewPortMax = 9;

enum class VIEWPORT_TYPE
{
	DEFAULT
};

enum class RENDER_TARGET : unsigned int
{
	BACK_BUFFER
};

enum class SYNC_INTERVAL
{
	UNLIMITED,
	LIMITED
};

enum class DIMENSION_TYPE
{
	_2D,
	_3D,
	_3D_BB
};
enum class GEOMETRY_TYPE
{
	_POINT,
	_LINE,
	_QUAD,
	_CUBE,
	_BALL
};
enum class SHADER_TYPE
{
	_DEFAULT
};
enum class TEX_RESOURCE
{
	_NONE
};
enum class SAMPLER_STATE
{
	_NONE
};
enum class BLEND_STATE
{
	_NONE
};
enum class CULL_MODE
{
	_CW,
	_CCW,
	_NONE,
	_WIRE_FRAME
};