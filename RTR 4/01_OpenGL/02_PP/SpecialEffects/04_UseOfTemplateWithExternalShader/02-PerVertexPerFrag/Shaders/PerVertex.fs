#version 460 core

in vec3 phong_ads_out;
out vec4 FragColor;
void main(void)
{
FragColor = vec4(phong_ads_out, 1.0);
}