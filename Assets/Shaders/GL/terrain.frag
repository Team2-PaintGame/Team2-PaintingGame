#version 400 core

uniform vec4 		objectColour;
uniform vec3		fogColour;
uniform sampler2DShadow shadowTex;
uniform sampler2D 	skyboxTex;

uniform sampler2D 	splatMapTex;
uniform sampler2D 	rTex;
uniform sampler2D 	gTex;
uniform sampler2D 	bTex;
uniform sampler2D 	bgTex;

uniform vec3	lightPos;
uniform float	lightRadius;
uniform vec4	lightColour;

uniform vec3	cameraPos;

uniform bool hasTexture;

in Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 worldPos;
	float visibility;
} IN;

out vec4 fragColor;

void main(void)
{
	vec4 splatMapColor = texture(splatMapTex, IN.texCoord); //what is the color value on the splatmap
	vec2 tiledTexCoord = IN.texCoord * 20;
	
	vec4 rTexColor 		= texture(rTex, tiledTexCoord) * splatMapColor.r;
	vec4 gTexColor 		= texture(gTex, tiledTexCoord) * splatMapColor.g;
	vec4 bTexColor 		= texture(bTex, tiledTexCoord) * splatMapColor.b;
	vec4 bgTexColor 	= texture(bgTex, tiledTexCoord) * (1 - (splatMapColor.r + splatMapColor.g + splatMapColor.b));

	vec4 finalTexColor = bgTexColor + rTexColor + gTexColor + bTexColor;

	float shadow = 1.0; // New !
	
	if( IN . shadowProj . w > 0.0) { // New !
		shadow = textureProj ( shadowTex , IN . shadowProj ) * 0.5f;
	}

	vec3  incident = normalize ( lightPos - IN.worldPos );
	float lambert  = max (0.0 , dot ( incident , IN.normal )) * 0.9; 
	
	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
	float sFactor = pow ( rFactor , 1000000.0 );

	//fragColor = finalTexColor;
	
	vec4 albedo = IN.colour;
	albedo *= finalTexColor;
	
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	
	fragColor.rgb = albedo.rgb * 0.05f; //ambient
	
	fragColor.rgb += albedo.rgb * lightColour.rgb * lambert * shadow; //diffuse light
	
	fragColor.rgb += lightColour.rgb * sFactor * shadow; //specular light
	
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
	
	fragColor.a = albedo.a;

	vec4 skyColour = texture(skyboxTex, gl_FragCoord.xy / vec2(1280, 720));
	fragColor.rgb = mix(skyColour.rgb, fragColor.rgb, IN.visibility);
}