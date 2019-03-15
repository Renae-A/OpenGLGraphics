// a normal map vertex shader
#version 410

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Normal;
layout( location = 2 ) in vec2 TexCoord;
layout( location = 3 ) in vec4 Tangent;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiTangent;
out vec4 vPosition;

uniform mat4 ProjectionViewModel;

// we need the model matrix seperate
uniform mat4 ModelMatrix;

// we need this matrix to transform the normal
uniform mat3 NormalMatrix;

void main() 
{
	vTexCoord = TexCoord;
	vPosition = ModelMatrix * Position;
	vNormal = NormalMatrix * Normal.xyz;
	vTangent = NormalMatrix * Tangent.xyz;
	vBiTangent = cross(vNormal, vTangent) * Tangent.w;
	gl_Position = ProjectionViewModel * Position;
}