// classic Oren-Nayar fragment shader
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

uniform sampler2D diffuseTex;

out vec4 FragColour;

void main() 
{
	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);
	vec3 E = normalize(CameraPosition - vPosition.xyz);

	vec3 texDiffuse = texture( diffuseTex, vTexCoord ).rgb;

	// calculate lambert term (negate light direction)
	float lambertTerm = max( 0, min( 1, dot( N, -L ) ) );

	// calculate view vector and reflection vector
	vec3 V = normalize(CameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );

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

	// calculate each colour property
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd * OrenNayar;
	vec3 specular = Is * Ks * specularTerm;
	
	// output final colour
	FragColour = vec4( ambient + diffuse + specular, 1);

	//FragColour = vec4(OrenNayar * texDiffuse, 1);
}