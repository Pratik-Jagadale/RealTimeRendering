#version 460 core
		
in vec4 a_position;
in vec3 a_normal;
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec3 u_la[2];
uniform vec3 u_ld[2];
uniform vec3 u_ls[2];
uniform vec4 u_lightPosition[2];
uniform vec3 u_ka;
uniform vec3 u_ks;
uniform vec3 u_kd;
uniform float u_materialShininnes;
uniform int u_lightingEnabled;
out vec3 phong_ads_light;
void main(void)
{
phong_ads_light = vec3(0.0,0.0,0.0);
if(u_lightingEnabled == 1)
{
vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;
mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);
vec3 transformedNormals = normalize(normalMatrix * a_normal);
vec3 viewerVector = normalize(-eyeCoordinates.xyz);
vec3 ambiant[2];
vec3 lightDirection[2];
vec3 diffuse[2];
vec3 reflectionVector[2];
vec3 specular[2];
for(int i = 0 ; i < 2 ; i++)
{
ambiant[i] = u_la[i] * u_ka;
lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);
diffuse[i] = u_ld[i] * u_kd * max(dot(lightDirection[i] ,transformedNormals),0.0);
reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);
specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], viewerVector), 0.0), u_materialShininnes);
phong_ads_light = phong_ads_light + ambiant[i] + diffuse[i] +  specular[i];
}
}
else
{
phong_ads_light = vec3(1.0,1.0,1.0);
}
gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
}