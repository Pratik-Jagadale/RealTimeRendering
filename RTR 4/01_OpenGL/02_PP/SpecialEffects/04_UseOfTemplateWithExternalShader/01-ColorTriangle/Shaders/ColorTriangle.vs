#version 460 core

in vec4 a_position;
in vec3 a_color;
uniform mat4 u_mvpMatrix;
out vec3 a_color_out;
void main(void)
{
a_color_out = a_color;
gl_Position = u_mvpMatrix * a_position;
}