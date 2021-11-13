#shader vertex
#version 330 core 


layout(location = 0) in vec3 postion;
layout(location = 1) in vec2 texCoord;

out vec2 textureCoord;

uniform mat4 viewProjection;
uniform mat4 transform;

void main(){
   gl_Position = viewProjection * transform * vec4(postion, 1.0);
   textureCoord = (texCoord - 0.15)/2.2 + 0.15*2.2;
};




#shader fragment
#version 330 core 

layout(location = 0) out vec4 color;

in vec2 textureCoord;
uniform sampler2D textureSampler;
uniform vec4 rgba;

void main(){
   if(texture(textureSampler, textureCoord).a != 0.0)
      color = vec4( textureCoord.x, textureCoord.y , 0.0, 1.0);
   else
      color = vec4(0.0);
   color += texture(textureSampler, textureCoord);
};