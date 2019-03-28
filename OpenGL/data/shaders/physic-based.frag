// classic physic-based fragment shader
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
uniform float Roughness;
uniform float ReflectionCoefficient;

uniform vec3	m_pointLightPos[4];
uniform vec3	m_lightColors[4];
uniform float	m_lightPower[4];
uniform int		m_lightCount;

uniform sampler2D diffuseTex;

out vec4 FragColour;

// ---------------- Oren-Nayar ------------------
float getDiffuse(vec3 L, vec3 N, vec3 E)
{
	float NdL = max( 0.0f, dot( N, L ) );
	float NdE = max( 0.0f, dot( N, E ) );
	float R2 = Roughness * Roughness;

	// Oren-Nayar Diffuse Term
	float A = 1.0f - 0.5f * R2 / (R2 + 0.33f);
	float B = 0.45f * R2 / (R2 + 0.09f);

	// CX = Max(0, cos(l,e))
	vec3 lightProjected = normalize( L - N * NdL );
	vec3 viewProjected = normalize( E - N * NdE);
	float CX = max( 0.0f, dot( lightProjected, viewProjected ) );

	// DX = sin(alpha) * tan(beta)
	float alpha = sin( max( acos( NdE ), acos( NdL ) ) );
	float beta = tan( min( acos( NdE ), acos( NdL ) ) );
	float DX = alpha * beta;

	// Calculate Oren-Nayar, replaces the Phong L
	float OrenNayar = NdL * (A + B * CX * DX);

	return OrenNayar;
}

// --------------- Cook-Torrance -------------------
float getSpecular(vec3 L, vec3 N, vec3 E)
{
	float NdL = max( 0.0f, dot( N, L ) );
	float NdE = max( 0.0f, dot( N, E ) );
	float R2 = Roughness * Roughness;

	// Vector average H of the light vector L and view vector E
	vec3 H = normalize(( L + E ) / 2 ); 

	float NdH = max( 0.0f, dot( N, H ) );
	float NdH2 = NdH * NdH;
	float e = 2.71828182845904523536028747135f;
	float pi = 3.1415926535897932384626433832f;

	// Beckman's Distribution Function D
	float exponent = -(1 - NdH2) / (NdH2 * R2);
	float D = pow( e, exponent ) / (R2 * NdH2 * NdH2);

	// Fresnel Term F
	float rf = ReflectionCoefficient;
	float F = rf + (1-rf) * pow(1-NdE , 5);
	//float F = ReflectionCoefficient + (1 – ReflectionCoefficient) * pow( 1 - NdE, 5 );

	// Geometric Attenuation Factor G
	float X = 2.0f * NdH / dot( E, H );
	float G = min(1, min(X * NdL, X * NdE));

	// Calculate Cook-Torrance
	float CookTorrance = max( (D*G*F) / (NdE * pi), 0.0f );

	return CookTorrance;
}

void main() 
{
	// Default values set up
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);

	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 E = normalize(CameraPosition - vPosition.xyz);

	vec3 texDiffuse = texture( diffuseTex, vTexCoord ).rgb;

	for (int i = 0; i < m_lightCount; i++)
	{
		vec3 L = m_pointLightPos[i] - vPosition.xyz;

		// get the distance squared for attentuation 
		float attenuation = 1.0f / dot(L, L);

		// normalise for dot products inside the functions
		L = normalize(L);

		diffuse += getDiffuse(L, N, E) * m_lightColors[i] * m_lightPower[i] * attenuation;
		// attenuate specular by a fixed amount, instead of distance-squared

		specular += getSpecular(L, N, E) * m_lightColors[i] * m_lightPower[i] * 0.1f;
	}

	// --------------- Final Colour Output --------------------

	// calculate each colour property
	vec3 ambient = Ia * Ka;
	diffuse = Kd * diffuse;
	specular = Ks * specular;
	
	// output final colour
	FragColour = vec4( ambient + diffuse + specular, 1);

	//FragColour = vec4(OrenNayar * texDiffuse, 1);
}


