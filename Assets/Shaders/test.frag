#version 460 core

uniform vec4 		objectColour;
uniform sampler2D 	mainTex;

uniform sampler2D 	noiseTex;
uniform float		globalTimer;
uniform vec3		facingDir;
uniform vec3		leftDir;
uniform vec3		centre;
uniform vec3		dimension;
uniform vec3		trace[100];		// TODO: [performance issue] change this to UBO
uniform vec3		testingPoint;

uniform int			numOfNodesInTrace;

layout(std430, binding = 3) buffer inktrace
{
    float data_SSBO[];
} INKTRACE;

uniform sampler2DShadow shadowTex;

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
} IN;

out vec4 fragColor;

bool onFarFace(in vec3 worldPosition) {
	float floatingPointUncertainty = 0.001f;
	vec3 frontFaceCentre = centre + facingDir * abs(dimension.z);
	if (
	dot(normalize(worldPosition - frontFaceCentre), facingDir)<=floatingPointUncertainty &&
	dot(normalize(worldPosition - frontFaceCentre), facingDir)>=-floatingPointUncertainty
	) {
		return(true);
	}
	return(false);
}

bool onNearFace(in vec3 worldPosition) {
	float floatingPointUncertainty = 0.001f;
	vec3 nearFaceCentre = centre - facingDir * abs(dimension.z);
	if (
	dot(normalize(worldPosition - nearFaceCentre), -facingDir)<=floatingPointUncertainty &&
	dot(normalize(worldPosition - nearFaceCentre), -facingDir)>=-floatingPointUncertainty
	) {
		return(true);
	}
	return(false);
}

bool onRightFace(in vec3 worldPosition) {
	float floatingPointUncertainty = 0.001f;
	vec3 rightFaceCentre = centre - leftDir * abs(dimension.x);
	if (
	dot(normalize(worldPosition - rightFaceCentre), -leftDir)<=floatingPointUncertainty &&
	dot(normalize(worldPosition - rightFaceCentre), -leftDir)>=-floatingPointUncertainty
	) {
		return(true);
	}
	return(false);
}

bool onLeftFace(in vec3 worldPosition) {
	float floatingPointUncertainty = 0.001f;
	vec3 leftFaceCentre = centre + leftDir * abs(dimension.x);
	if (
	dot(normalize(worldPosition - leftFaceCentre), leftDir)<= floatingPointUncertainty &&
	dot(normalize(worldPosition - leftFaceCentre), leftDir)>= -floatingPointUncertainty
	) {
		return(true);
	}
	return(false);
}

bool onTopFace(in vec3 worldPosition) {
	float floatingPointUncertainty = 0.001f;
	vec3 topDir = cross(facingDir, leftDir);
	vec3 topFaceCentre = centre + topDir * abs(dimension.y);
	if (
	dot(normalize(worldPosition - topFaceCentre), topDir)<=floatingPointUncertainty &&
	dot(normalize(worldPosition - topFaceCentre), topDir)>=-floatingPointUncertainty
	) {
		return(true);
	}
	return(false);
}

bool onBottomFace(in vec3 worldPosition) {
	float floatingPointUncertainty = 0.001f;
	vec3 topDir = cross(facingDir, leftDir);
	vec3 bottomFaceCentre = centre - topDir * abs(dimension.y);
	if (
	dot(normalize(worldPosition - bottomFaceCentre), -topDir)<=floatingPointUncertainty &&
	dot(normalize(worldPosition - bottomFaceCentre), -topDir)>=-floatingPointUncertainty
	) {
		return(true);
	}
	return(false);
}

bool shouldPaint(in vec2 point, in float radius, float amplitude, float amplificationFactor) {
	vec2 d = IN.texCoord - point;
	float angle = atan(d.y/d.x);
	if(d.x * d.x + d.y * d.y < (radius + amplitude * sin(angle*amplificationFactor)) * (radius + amplitude * sin(angle*amplificationFactor)))
	{
		return(true);
	}
	return(false);
}

void paint(inout vec4 albedo, float time, vec4 color) {  // May need to change how to operate on 'time'
	vec4 n = texture(noiseTex,vec2(IN.texCoord.x, IN.texCoord.y));
    
	if(n.r > (1+time))
	{
		albedo *= color;
	}
	else
	{
		albedo *= texture(mainTex, IN.texCoord);
	}
}

/*
	Texturing Coordinates
	(when facing towards the following face:)
	- For the far face with the rectangle orientation towards the eye: (0,0) at top-left, x-axis being horizontal
	- For the near face with the rectangle orientation towards the front: (0,0) at bottom-right, x-axis being horizontal
	- For the left face with the rectangle orientation towards the left in the eye: (0,0) at bottom-right, x-axis being horizontal
	- For the right face with the rectangle orientation towards the right in the eye: (0,0) at bottom-right, x-axis being horizontal
	- For the top face with the rectangle orientation downwards: (0,0) at top left, x-axis being horizontal
	- For the bottom face with the rectangle orientation upwards: (0,0) at top-left, x-axis being horizontal
*/

// TODO: given a world position, change it to vec2 in the texture space
vec2 GetTexPosOnFarFace(in vec3 worldExplosionPoint) {
	vec2 result;
	
	vec3 faceOrigin = centre + facingDir * abs(dimension.z) - leftDir * abs(dimension.x) + cross(facingDir, leftDir) * abs(dimension.y);
	vec3 faceXAxis = normalize(centre + facingDir * abs(dimension.z) + cross(facingDir, leftDir) * abs(dimension.y) - faceOrigin);
	vec3 faceYAxis = normalize(centre - leftDir * abs(dimension.x) + facingDir * abs(dimension.z) - faceOrigin);
	vec3 faceVector = worldExplosionPoint - faceOrigin;
	float faceVectorX = dot(faceVector, faceXAxis);
	float faceVectorY = dot(faceVector, faceYAxis);
	
	result.x = (0.5f * faceVectorX) / dimension.x;
	result.y = (0.5f * faceVectorY) / dimension.y;

	return(result);
}

vec2 GetTexPosOnNearFace(in vec3 worldExplosionPoint) {
	vec2 result;
	// TODO
	return(result);
}

vec2 GetTexPosOnLeftFace(in vec3 worldExplosionPoint) {
	vec2 result;
	// TODO
	return(result);
}

vec2 GetTexPosOnRightFace(in vec3 worldExplosionPoint) {
	vec2 result;
	// TODO
	return(result);
}

vec2 GetTexPosOnTopFace(in vec3 worldExplosionPoint) {
	vec2 result;
	
	vec3 faceOrigin = centre + cross(facingDir, leftDir) * abs(dimension.y) - facingDir * abs(dimension.z) - leftDir * abs(dimension.x);
	vec3 faceXAxis = normalize(centre + cross(facingDir, leftDir) * abs(dimension.y) - facingDir * abs(dimension.z) - faceOrigin);
	vec3 faceYAxis = normalize(centre - leftDir * abs(dimension.x) + cross(facingDir, leftDir) * abs(dimension.y) - faceOrigin);
	vec3 faceVector = worldExplosionPoint - faceOrigin;
	float faceVectorX = dot(faceVector, faceXAxis);
	float faceVectorY = dot(faceVector, faceYAxis);
	
	result.x = (0.5f * faceVectorX) / dimension.x;
	result.y = (0.5f * faceVectorY) / dimension.z;

	return(result);
}

vec2 GetTexPosOnBottomFace(in vec3 worldExplosionPoint) {
	vec2 result;
	// TODO
	return(result);
}

void FarFaceTransformations(in vec2 explosionPoint, out vec2 pointToLeft, out vec2 pointToRight, out vec2 pointToTop, out vec2 pointToBottom) {
	pointToLeft.x = 2.0f - explosionPoint.x;
	pointToLeft.y = 1.0f - explosionPoint.y;
	pointToRight.x = -explosionPoint.x;
	pointToRight.y = 1.0f - explosionPoint.y;
	pointToTop.x = explosionPoint.x;
	pointToTop.y = 1.0f + explosionPoint.y;
	pointToBottom.x = explosionPoint.x;
	pointToBottom.y = explosionPoint.y - 1.0f;
}

// TODO:
/*void NearFaceTransformations(in vec2 explosionPoint, out vec2 pointToLeft, out vec2 pointToRight, out vec2 pointToTop, out vec2 pointToBottom) {
	pointToLeft.x = ;
	pointToLeft.y = ;
	pointToRight.x = ;
	pointToRight.y = ;
	pointToTop.x = ;
	pointToTop.y = ;
	pointToBottom.x = ;
	pointToBottom.y = ;
}

void LeftFaceTransformations(in vec2 explosionPoint, out vec2 pointToLeft, out vec2 pointToRight, out vec2 pointToTop, out vec2 pointToBottom) {
	pointToLeft.x = ;
	pointToLeft.y = ;
	pointToRight.x = ;
	pointToRight.y = ;
	pointToTop.x = ;
	pointToTop.y = ;
	pointToBottom.x = ;
	pointToBottom.y = ;
}

void RightFaceTransformations(in vec2 explosionPoint, out vec2 pointToLeft, out vec2 pointToRight, out vec2 pointToTop, out vec2 pointToBottom) {
	pointToLeft.x = ;
	pointToLeft.y = ;
	pointToRight.x = ;
	pointToRight.y = ;
	pointToTop.x = ;
	pointToTop.y = ;
	pointToBottom.x = ;
	pointToBottom.y = ;
}*/

void TopFaceTransformations(in vec2 explosionPoint, out vec2 pointToLeft, out vec2 pointToRight, out vec2 pointToFar, out vec2 pointToNear) {
	pointToLeft.x = explosionPoint.y;
	pointToLeft.y = 2.0f - explosionPoint.x;
	pointToRight.x = 1.0f - explosionPoint.y;
	pointToRight.y = 1.0f + explosionPoint.x;
	pointToFar.x = explosionPoint.x;
	pointToFar.y = explosionPoint.y - 1.0f;
	pointToNear.x = explosionPoint.x;
	pointToNear.y = 1.0f + explosionPoint.y;
}

// TODO:
/*void BottomFaceTransformations(in vec2 explosionPoint, out vec2 pointToLeft, out vec2 pointToRight, out vec2 pointToTop, out vec2 pointToBottom) {
	pointToLeft.x = ;
	pointToLeft.y = ;
	pointToRight.x = ;
	pointToRight.y = ;
	pointToTop.x = ;
	pointToTop.y = ;
	pointToBottom.x = ;
	pointToBottom.y = ;
}*/

void main(void)
{
	float shadow = 1.0;
	
	if( IN . shadowProj . w > 0.0) {
		shadow = textureProj ( shadowTex , IN . shadowProj ) * 0.5f;
	}

	vec3  incident = normalize ( lightPos - IN.worldPos );
	float lambert  = max (0.0 , dot ( incident , IN.normal )) * 0.9; 
	
	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
	float sFactor = pow ( rFactor , 80.0 );
	
	vec4 albedo = IN.colour;
	
	if(hasTexture) {
		float sceneTimeEmerge = -globalTimer;
		float time = sceneTimeEmerge-int(sceneTimeEmerge);
		float radius = (1.0/(200*2)) - time * (1.0/(200*2));
		float amplificationFactor = 6.0f;
		float amplitude = 0.1/(200*2);

		//vec2 explosionPoint[2];
		//explosionPoint[0] = vec2(0.1f, 0.95f);
		//explosionPoint[1] = vec2(0.5f, 0.5f);
		
		vec4 colors = vec4(1.0,0.0,0.0,1.0);
		
		if (true)		// explode on top plane
		{
			vec2 pointToLeft;
			vec2 pointToRight;
			vec2 pointToTop;
			vec2 pointToBottom;
			vec2 pointToFar;
			vec2 pointToNear;

			bool painted = false;

			//vec2 explosionPoint = vec2(0.5,0.5);
			//if (GetTexPosOnTopFace(testingPoint).y >= 0.4 && GetTexPosOnTopFace(testingPoint).y <= 0.6)
			//{
			//colors = vec4(0.0,1.0,0.0,1.0);
			//}
			//else
			//{
			//colors = vec4(1.0,0.0,0.0,1.0);
			//}
			

			for (int i = 0; i < int(numOfNodesInTrace); i++) {

				vec3 worldPos = trace[i];
				//worldPos = vec3(INKTRACE.data_SSBO[i],INKTRACE.data_SSBO[i+1],INKTRACE.data_SSBO[i+2]);

				vec2 explosionPoint = GetTexPosOnTopFace(worldPos);
				//vec2 explosionPoint = GetTexPosOnTopFace(testingPoint);
				//vec2 explosionPoint = vec2(0.5,0.5);

				TopFaceTransformations(explosionPoint, pointToLeft, pointToRight, pointToFar, pointToNear);
			
				if ( shouldPaint(explosionPoint, radius, amplitude, amplificationFactor) && onTopFace(IN.worldPos) )
				{
					paint(albedo, time, colors);
					painted = true;
					break;
				}
				else if ( shouldPaint(pointToLeft, radius, amplitude, amplificationFactor) && onLeftFace(IN.worldPos) )
				{
					paint(albedo, time, colors);
					painted = true;
				
					break;

				}
				else if ( shouldPaint(pointToRight, radius, amplitude, amplificationFactor) && onRightFace(IN.worldPos) )
				{
					paint(albedo, time, colors);
					painted = true;

					break;

				}
				else if ( shouldPaint(pointToFar, radius, amplitude, amplificationFactor) && onFarFace(IN.worldPos) )
				{
					paint(albedo, time, colors);
					painted = true;

					break;

				}
				else if ( shouldPaint(pointToNear, radius, amplitude, amplificationFactor) && onNearFace(IN.worldPos) )
				{
					paint(albedo, time, colors);
					painted = true;

					break;

				}
			
			}  // for-loop

			if (painted == false)
			{
				albedo *= texture(mainTex, IN.texCoord);
			}
		}
		/*else if (false) // explode on far plane	// TODO: explosion on other faces
		{
			vec2 pointToLeft;
			vec2 pointToRight;
			vec2 pointToTop;
			vec2 pointToBottom;

			bool painted = false;
			for (int i = 0; i < 2; i++) {

			FarFaceTransformations(explosionPoint[i], pointToLeft, pointToRight, pointToTop, pointToBottom);
			
			if ( shouldPaint(explosionPoint[i], radius, amplitude, amplificationFactor) && onFarFace(IN.worldPos) )
			{
				paint(albedo, time, colors);
				painted = true;
				break;
			}
			else if ( shouldPaint(pointToLeft, radius, amplitude, amplificationFactor) && onLeftFace(IN.worldPos) )
			{
				paint(albedo, time, colors);
				painted = true;
				
				break;

			}
			else if ( shouldPaint(pointToRight, radius, amplitude, amplificationFactor) && onRightFace(IN.worldPos) )
			{
				paint(albedo, time, colors);
				painted = true;

				break;

			}
			else if ( shouldPaint(pointToTop, radius, amplitude, amplificationFactor) && onTopFace(IN.worldPos) )
			{
				paint(albedo, time, colors);
				painted = true;

				break;

			}
			else if ( shouldPaint(pointToBottom, radius, amplitude, amplificationFactor) && onBottomFace(IN.worldPos) )
			{
				paint(albedo, time, colors);
				painted = true;

				break;

			}
			
			}
			if (painted == false)
			{
				albedo *= texture(mainTex, IN.texCoord);
			}

		}*/
    
		
	}
	
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	
	fragColor.rgb = albedo.rgb * 0.05f; //ambient
	
	fragColor.rgb += albedo.rgb * lightColour.rgb * lambert * shadow; //diffuse light
	
	fragColor.rgb += lightColour.rgb * sFactor * shadow; //specular light
	
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
	
	fragColor.a = albedo.a;

//fragColor.rgb = IN.normal;

	//fragColor = IN.colour;
	
	//fragColor.xy = IN.texCoord.xy;
	
	//fragColor = IN.colour;
}