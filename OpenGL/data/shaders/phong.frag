// classic Phong fragment shader
#version 410

in vec2 vTexCoord;
in vec4 vPosition;
in vec3 vNormal;

uniform vec3 Ka;				// ambient material colour
uniform vec3 Kd;				// diffuse material colour
uniform vec3 Ks;				// specular material colour

uniform float specularPower;	// material specular power

uniform vec3 Ia;				// ambient light colour

uniform vec3 Id;				// diffuse light colour
uniform vec3 Is;				// specular light colour

uniform vec3 LightDirection;
uniform vec3 CameraPosition;

uniform vec3	m_pointLightPos[4];
uniform vec3	m_lightColors[4];
uniform float	m_lightPower[4];
uniform int		m_lightCount;

uniform sampler2D diffuseTex;

out vec4 FragColour;

float getDiffuse(vec3 L){
	return max(0, dot(vNormal.xyz, L));
}

float getSpecular(vec3 L, vec3 N)
{
	// calculate view vector and reflection vector
	vec3 V = normalize(CameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	return pow( max( 0, dot( R, V ) ), specularPower );
}


void main() 
{
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);


	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);

	vec3 texDiffuse = texture( diffuseTex, vTexCoord ).rgb;
	
	vec3 V = normalize(CameraPosition - vPosition.xyz);

	for (int i = 0; i < m_lightCount; i++)
	{
		vec3 L = m_pointLightPos[i] - vPosition.xyz;

		// get the distance squared for attentuation 
		float attenuation = 1.0f / dot(L, L);

		// normalise for dot products inside the functions
		L = normalize(L);

		diffuse += getDiffuse(L) * m_lightColors[i] * m_lightPower[i] * attenuation;

		// attenuate specular by a fixed amount, instead of distance-squared
		specular += getSpecular(L, N) * m_lightColors[i] * m_lightPower[i] * 0.1f;
	}

	// calculate each colour property
	vec3 ambient = Ia * Ka;
	diffuse = Kd * diffuse /* * texDiffuse */;
	specular = Ks * specular;
	
	// output final colour
	FragColour = vec4( ambient + diffuse + specular, 1);

	//FragColour = vec4(texDiffuse, 1);
}