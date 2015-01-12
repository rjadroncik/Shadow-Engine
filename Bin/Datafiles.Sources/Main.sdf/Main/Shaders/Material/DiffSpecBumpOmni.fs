///////////////////////////////// UNIFORMS /////////////////////////////////////
uniform sampler2D diffuseSampler;
uniform sampler2D normalMapSampler;

uniform vec3 lightTint;
uniform vec3 lightAmbDifSpe;

uniform vec3  materialTint;
uniform vec4  materialAmbDifSpeEmi;
uniform float materialShininess;

////////////////////////////////// VARYING /////////////////////////////////////
varying vec2 varTexCoord0;

varying vec3  varLightTextureDirection;
varying vec3  varLightTextureHalfVector;
varying float varLightAttenuation;

/////////////////////////////////// MAIN ///////////////////////////////////////
void main(void)
{
	//Discard unlit fragments
	if (varLightAttenuation == 0.0) { discard; } 
	
	vec3 textureNormal;
	textureNormal.xy = (texture2D(normalMapSampler, varTexCoord0).wy * vec2(2.0, 2.0)) + vec2(-1.0, -1.0);
	textureNormal.z  = sqrt(1.0 - dot(textureNormal.xy, textureNormal.xy));
 
 	vec4 ambDifSpeEmi = materialAmbDifSpeEmi * vec4(lightAmbDifSpe, 0.0);
 	vec3 tint         = materialTint * lightTint;

	float diffuseIntensity  = ambDifSpeEmi.y *     max(0.0, dot(textureNormal, normalize(varLightTextureDirection)));
	float specularIntensity = ambDifSpeEmi.z * pow(max(0.0, dot(textureNormal, normalize(varLightTextureHalfVector))), materialShininess);
	
	vec3 diffuseColor = tint * vec3(texture2D(diffuseSampler, varTexCoord0));
	
	gl_FragColor = vec4((ambDifSpeEmi.x * diffuseColor + diffuseIntensity * diffuseColor + specularIntensity * tint) * varLightAttenuation, 1.0);
}