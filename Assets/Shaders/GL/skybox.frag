#version 330 core

uniform samplerCube cubeTexDay;
uniform samplerCube cubeTexNight;

uniform bool useFog = false;
uniform vec3 fogColour;
uniform float dayNightRatio;

in Vertex {
	vec3 viewDir;
} IN;

out vec4 fragColour;
const float belowHorizonLevel = 0.0f;
const float aboveHorizonLevel = 0.25f;
const float offset = 1.0 / 300.0;  

void main(void)	{

	vec3 viewDir = normalize(IN.viewDir);
	vec4 dayTexture = texture(cubeTexDay, viewDir);
	vec4 nightTexture = texture(cubeTexNight, viewDir);
	vec4 finalTexture = mix(dayTexture, nightTexture, dayNightRatio);

	fragColour = pow(finalTexture, vec4(2.2f));

	if (useFog) {
		//float ratio = clamp((viewDir.y - belowHorizonLevel) / (aboveHorizonLevel - belowHorizonLevel), 0.0, 1.0);
		fragColour.rgb = mix(fogColour, fragColour.rgb, 0.85);
	}
}