#version 460 core

in vec4 a_position;
in vec3 a_normal;
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec3 u_la;
uniform vec3 u_ld;
uniform vec3 u_ls;
uniform vec4 u_lightPosition;
uniform vec3 u_ka;
uniform vec3 u_ks;
uniform vec3 u_kd;
uniform float u_materialShininnes;
uniform int u_lightingEnabled;
out vec3 phong_ads_out;
void main(void)
{
if(u_lightingEnabled == 1)
{
vec3 ambiant = u_la * u_ka;
vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;
mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);
vec3 transformedNormals = normalize(normalMatrix * a_normal);
vec3 lightDirection = normalize(vec3(u_lightPosition) - eyeCoordinates.xyz);
vec3 diffuse = u_ld * u_kd * max(dot(lightDirection ,transformedNormals),0.0);
vec3 reflectionVector = reflect(-lightDirection,transformedNormals);
vec3 viewerVector = normalize(-eyeCoordinates.xyz);
vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector , viewerVector), 0.0), u_materialShininnes);
phong_ads_out = ambiant + diffuse +  specular;
}
else
{
phong_ads_out = vec3(1.0,1.0,1.0);
}
gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
}