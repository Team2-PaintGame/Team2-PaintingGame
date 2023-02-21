

#version 430 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(rgba8ui/*, binding = 12*/) writeonly uniform uimage2D tex;


uniform int width;
uniform int height;

uniform int radius;

uniform int leftS;
uniform int rightS;
uniform int topT;
uniform int bottomT;

uniform ivec2 center;
uniform int teamID;

void main() {

        ivec2 IMAGE_COORD = ivec2(leftS + gl_GlobalInvocationID.x, topT + gl_GlobalInvocationID.y);

        if(IMAGE_COORD.x >= rightS || IMAGE_COORD.y >= bottomT){return;}
        
		vec2 noiseVec = vec2(IMAGE_COORD.x,IMAGE_COORD.y)/(radius/6);
		
		if (length(IMAGE_COORD - center) < radius) {
			imageStore(tex, IMAGE_COORD, ivec4(teamID, 0, 0, 0));
		}
}