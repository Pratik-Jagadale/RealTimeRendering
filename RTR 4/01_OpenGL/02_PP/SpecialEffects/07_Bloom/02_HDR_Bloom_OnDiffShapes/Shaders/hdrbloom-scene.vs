#version 460 core
			
in vec4 a_position;
in vec3 a_normal;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec4 u_lightPosition;
uniform int u_lightingEnabled;

out vec3 transformedNormals;
out vec3 lightDirection;
out vec3 viewerVector;

out flat int material_index;

void main(void)
{

if(u_lightingEnabled == 1)
{
vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;
mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);
transformedNormals = normalMatrix * a_normal;
lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;
viewerVector = -eyeCoordinates.xyz;
}

gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
}
