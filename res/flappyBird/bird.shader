#shader vertex
#version 330 core 


layout(location = 0) in vec3 postion;
layout(location = 1) in vec2 texCoord;

out vec2 textCoord;

uniform mat4 viewProjection;
uniform mat4 transform;

void main(){
   gl_Position = viewProjection * transform * vec4(postion, 1.0);
   textCoord = texCoord;
};




#shader fragment
#version 330 core 

layout(location = 0) out vec4 color;

in vec2 textCoord;

uniform vec4 colorOffset;

void main(){
   color = vec4( textCoord.x, textCoord.y  + colorOffset.y, colorOffset.z, 1.0f);
};