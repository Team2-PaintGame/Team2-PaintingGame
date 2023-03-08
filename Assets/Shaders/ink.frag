#version 400 core

//#define iTim (time + fract(1e4*sin(dot(gl_FragCoord.xy,vec2(137,-13))))* dt)

uniform vec4 		objectColour;
uniform sampler2D 	mainTex;
//uniform sampler2DShadow shadowTex;
uniform float time;
uniform float dt;

uniform vec3	lightPos;
uniform float	lightRadius;
uniform vec4	lightColour;

uniform vec3	cameraPos;

uniform bool hasTexture;

in Vertex
{
	vec4 colour;
	vec2 texCoord;
	//vec4 shadowProj;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColor;

void main(void)
{
	float shadow = 1.0; // New !
	
	//if( IN . shadowProj . w > 0.0) { // New !
	//	shadow = textureProj ( shadowTex , IN . shadowProj ) * 0.5f;
	//}

	vec3  incident = normalize ( lightPos - IN.worldPos );
	float lambert  = max (0.0 , dot ( incident , IN.normal )) * 0.9; 
	
	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
	float sFactor = pow ( rFactor , 1000000.0 );
	
	vec4 albedo = objectColour;//IN.colour;
	
	if(hasTexture) {
	 albedo *= texture(mainTex, IN.texCoord);
	 //fragColor = texture(mainTex, IN.texCoord);
	}

	//else {
		//fragColor = objectColour;
	//}
	
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	
	fragColor.rgb = albedo.rgb * 0.05f; //ambient
	
	//fragColor.rgb += albedo.rgb * lightColour.rgb * lambert * shadow; //diffuse light
	
	fragColor.rgb += albedo.rgb + lightColour.rgb * sFactor * shadow; //specular light
	
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
	
	fragColor.a = albedo.a;

//	fragColor = texture(mainTex, IN.texCoord);

	//float T = iTim;   
    
	//vec4 o = vec4(0);

    //for(vec4 p = vec4(10.*cos(T), 0, 0, 30.+30.*sin(T)-40.*T), i = o-2.; o.x > (p*.1*i+i).y; i+=.002) {
	//	o = i* texture(mainTex, p.xw/2e2),
	//	p.xy += IN.texCoord/1280 * 0.1;
	//	p-=.05;
	//	o++;
	//}

	//fragColor = o;

//fragColor.rgb = IN.normal;

	//fragColor = IN.colour;
	
	//fragColor.xy = IN.texCoord.xy;
	
	//fragColor = IN.colour;
}