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

//uniform vec3 PointLightDirections[4];
uniform int PointLightCount;

uniform sampler2D diffuseTex;

out vec4 FragColour;

vec3 PointLightDirections[4];


void main() 
{
    PointLightDirections[0] = vec3(5.0f, 90.0f, 0.0f);
    PointLightDirections[1] = vec3(-5.0f, 35.0f, 78.0f);
    PointLightDirections[2] = vec3(60.0f, -81.0f, -90.0f);
    PointLightDirections[3] = vec3(-60.0f, 0.0f, -13.0f);


	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);

	vec3 texDiffuse = texture( diffuseTex, vTexCoord ).rgb;
	
	vec3 V = normalize(CameraPosition - vPosition.xyz);

	float lambertTerm = 0;
	float specularTerm = 0;
	for (int i = 0; i < PointLightCount; i++)
	{
		vec3 L = normalize(PointLightDirections[i]);

		// calculate lambert term (negate light direction)
		lambertTerm += max( 0, min( 1, dot( N, -L ) ) );

		// calculate view vector and reflection vector
		
		vec3 R = reflect( L, N );

		// calculate specular term
		specularTerm += pow( max( 0, dot( R, V ) ), specularPower );
	}

	// calculate each colour property
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd * lambertTerm;
	vec3 specular = Is * Ks * specularTerm;
	
	// output final colour
	FragColour = vec4( ambient + diffuse + specular, 1);

	//FragColour = vec4(texDiffuse, 1);


	//// ensure normal and light direction are normalised
	//vec3 N = normalize(vNormal);
	//vec3 L = normalize(LightDirection);
	//
	//vec3 texDiffuse = texture( diffuseTex, vTexCoord ).rgb;
	//
	//// calculate lambert term (negate light direction)
	//float lambertTerm = max( 0, min( 1, dot( N, -L ) ) );
	//
	//// calculate view vector and reflection vector
	//vec3 V = normalize(CameraPosition - vPosition.xyz);
	//vec3 R = reflect( L, N );
	//
	//// calculate specular term
	//float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );
	//
	//// calculate each colour property
	//vec3 ambient = Ia * Ka;
	//vec3 diffuse = Id * Kd * texDiffuse * lambertTerm;
	//vec3 specular = Is * Ks * specularTerm;
	//
	//// output final colour
	//FragColour = vec4( ambient + diffuse + specular, 1);
	//
	//FragColour = vec4(texDiffuse, 1);
}