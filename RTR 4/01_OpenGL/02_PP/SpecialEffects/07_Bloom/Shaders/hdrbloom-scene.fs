#version 460 core

in vec3 transformedNormals;
in vec3 lightDirection;
in vec3 viewerVector;

uniform vec3 u_la;
uniform vec3 u_ld;
uniform vec3 u_ls;
uniform vec3 u_ka;
uniform vec3 u_ks;
uniform vec3 u_kd;
uniform float u_materialShininnes;
uniform int u_lightingEnabled;

uniform float bloom_thresh_min ;
uniform float bloom_thresh_max ;

out vec4 FragColor0;
out vec4 FragColor1;

void main(void)
{
vec3 phong_ads_color = vec3(1.0,1.0,1.0);
if(u_lightingEnabled == 1)
{
vec3 ambiant = u_la * u_ka;

vec3 normalized_transformed_normals = normalize(transformedNormals);
vec3 normalized_light_direction = normalize(lightDirection);

vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction ,normalized_transformed_normals),0.0);
vec3 reflectionVector = reflect(-normalized_light_direction,normalized_transformed_normals);
vec3 normalized_view_vector = normalize(viewerVector);
vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector , normalized_view_vector), 0.0), u_materialShininnes);

phong_ads_color = ambiant + diffuse +  specular;

FragColor0 =  vec4(phong_ads_color, 1.0);

// Calculate luminance
float Y = dot(phong_ads_color , vec3(0.299, 0.587, 0.144));

// Threshold color based on its luminance and write it to
// the second output
phong_ads_color = phong_ads_color * 4.0 * smoothstep(bloom_thresh_min, bloom_thresh_max, Y);
FragColor1 = vec4(phong_ads_color, 1.0);

}
else
{
phong_ads_color = vec3(1.0,1.0,1.0);
FragColor0 = vec4(phong_ads_color, 1.0);
}
}