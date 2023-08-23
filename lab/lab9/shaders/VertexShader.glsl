#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float time;
uniform bool is_quad;

out vec2 texcoord;

void main()
{
	// Pass v_texture_coord as output to Fragment Shader
	texcoord = v_texture_coord;

	if (is_quad)
	{
		texcoord = vec2(v_texture_coord.x + sin(time), v_texture_coord.y);
	}

	mat4 ModelView = View * Model;


	gl_Position = Projection * ModelView * vec4(v_position, 1.f);
}