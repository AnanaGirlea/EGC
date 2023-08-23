#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Position;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_color;
out vec2 frag_coord;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_position = v_position;
    frag_normal = v_normal;
    frag_coord = v_coord;
    frag_color = v_color;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position + vec4(v_position, 1);

}
