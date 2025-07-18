#version 460 core

uniform sampler2D hdr_image;
uniform sampler2D bloom_image;
uniform float exposure = 0.9;
uniform float bloom_factor = 1.0;
uniform float scene_factor = 1.0;

out vec4 Frag_Color;

void main(void)
{
    vec4 c = vec4(0.0);

    c += texelFetch(hdr_image, ivec2(gl_FragCoord.xy), 0) * scene_factor;
    c += texelFetch(bloom_image, ivec2(gl_FragCoord.xy), 0) * bloom_factor;

    c.rgb = vec3(1.0) - exp(-c.rgb * exposure);
    Frag_Color = c;
}
