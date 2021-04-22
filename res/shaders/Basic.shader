#shader vertex
#version 330 core 


layout(location = 0) in vec4 postion;

out vec2 v_TextCoord;

uniform float xOffset;
uniform float yOffset;

void main(){
   gl_Position = vec4(postion.x + xOffset, postion.y + yOffset, 0.0f, 1.0f);
   v_TextCoord.xy = postion.xy;
};




#shader fragment
#version 330 core 

layout(location = 0) out vec4 color;

in vec2 v_TextCoord;

uniform vec4 colorOffset;

void main(){
   color = vec4( v_TextCoord.x  + colorOffset.x, v_TextCoord.y + v_TextCoord.x, v_TextCoord.y + colorOffset.y, 1.0f);
};