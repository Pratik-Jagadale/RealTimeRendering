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
out vec4 FragColor;
void main(void)
{
vec3 phong_ads_color;
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
}
else
{
phong_ads_color = vec3(1.0,1.0,1.0);
}
FragColor = vec4(phong_ads_color, 1.0);
}