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
uniform sampler2D textureSampler;
uniform vec4 colorOffset;

void main(){
   if(texture(textureSampler, textCoord).rgb != vec3(0.0))
      color = vec4( textCoord.x  + colorOffset.x, textCoord.y + textCoord.x, textCoord.y + colorOffset.y, 1.0f);
   else
      color = vec4(0.0);

   color += texture(textureSampler, textCoord);

};