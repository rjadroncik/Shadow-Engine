//////////////////////////////// ATTRIBUTES/////////////////////////////////////
attribute vec2 texCoord0;
attribute vec3 tangent;
attribute vec3 normal;

///////////////////////////////// UNIFORMS /////////////////////////////////////
uniform vec3 cameraPosition;

uniform vec3 lightPosition;
uniform vec4 lightParameters;

uniform mat4 lightProjection;

////////////////////////////////// VARYING /////////////////////////////////////
varying vec2 varTexCoord0;
varying vec4 varTexCoord1;

varying vec3  varLightTextureDirection;
varying vec3  varLightTextureHalfVector;
varying float varLightAttenuation;

//////////////////////////// ComputeLightVaryings //////////////////////////////
void ComputeLightVaryings()
{
	mat3 TBN = mat3(tangent, cross(tangent, normal), normal);

	vec3 lightDirection  = (lightPosition  - vec3(gl_Vertex));
	vec3 cameraDirection = (cameraPosition - vec3(gl_Vertex));

	varLightTextureDirection  = normalize(lightDirection  * TBN);
	varLightTextureHalfVector = normalize(cameraDirection * TBN) + varLightTextureDirection;
	varLightAttenuation       = 1.0 - length(lightDirection) / lightParameters.x;
}

/////////////////////////////////// MAIN ///////////////////////////////////////
void main(void)
{
	varTexCoord0 = texCoord0;
	gl_Position  = ftransform();

	varTexCoord1 = gl_ModelViewMatrix * gl_Vertex;
	varTexCoord1 = lightProjection    * varTexCoord1; 
	
	ComputeLightVaryings();
}