// a normal map fragment shader
#version 410

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;

out vec4 FragColour;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 Ka;					// material ambient
uniform vec3 Kd;					// material diffuse
uniform vec3 Ks;					// material specular
uniform float specularPower;

uniform vec3 Ia;					// light ambient
uniform vec3 Id;					// light diffuse
uniform vec3 Is;					// light specular

uniform vec3 CameraPosition;

uniform vec3	m_pointLightPos[4];
uniform vec3	m_lightColors[4];
uniform float	m_lightPower[4];
uniform int		m_lightCount;

float getDiffuse(vec3 L, vec3 N)
{
	return max(0, dot(N, L));
}

float getSpecular(vec3 L, vec3 N)
{
	// calculate view vector and reflection vector
	vec3 V = normalize(CameraPosition - vPosition.xyz);
	vec3 R = reflect( -L, N );

	return pow( max( 0, dot( R, V ) ), specularPower );
}

void main() 
{
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);

	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);

	mat3 TBN = mat3(T, B, N);

	vec3 texDiffuse = texture( diffuseTexture, vTexCoord ).rgb;
	vec3 texSpecular = texture( specularTexture, vTexCoord ).rgb;
	vec3 texNormal = texture( normalTexture, vTexCoord ).rgb;

	N = TBN * (texNormal * 2 - 1);

	for (int i = 0; i < m_lightCount; i++)
	{
		vec3 L = m_pointLightPos[i] - vPosition.xyz;

		// get the distance squared for attentuation 
		float attenuation = 1.0f / dot(L, L);

		// normalise for dot products inside the functions
		L = normalize(L);

		diffuse += getDiffuse(L, N) * m_lightColors[i] * m_lightPower[i] * attenuation;

		// attenuate specular by a fixed amount, instead of distance-squared
		specular += getSpecular(L, N) * m_lightColors[i] * m_lightPower[i] * 0.1f;
	}

	// calculate each light property
	vec3 ambient = Ia * Ka;
	diffuse = Kd * texDiffuse * diffuse;
	specular = Ks * texSpecular * specular;

	FragColour = vec4(ambient + diffuse + specular, 1);
}