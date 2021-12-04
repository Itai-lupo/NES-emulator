#shader vertex
#version 330 core 

layout(location = 0) in vec3 postion;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 MatColor;
layout(location = 3) in float texSlot;

out vec4 MatrialColor;
out vec2 textCoord;
out float textureIndex;

uniform mat4 viewProjection;

void main(){
   gl_Position = viewProjection * vec4(postion, 1.0);
   MatrialColor = MatColor;
   textCoord = texCoord;
   textureIndex  = texSlot;
};




#shader fragment
#version 330 core 

layout(location = 0) out vec4 color;

in vec4 MatrialColor;
in vec2 textCoord;
in float textureIndex;

uniform sampler2D textures[8];

vec4 calcColor(vec4 textureColor)
{
   if(int(textureIndex) == 0)
      return MatrialColor;

   if(textureColor.rgb == vec3(0.0))
      return vec4(0.0);
      
   return textureColor + vec4( textCoord.x  + MatrialColor.x, textCoord.y + textCoord.x, textCoord.y + MatrialColor.y, 1.0f);
}

void main(){
   vec4 textureColor;
   switch (int(textureIndex)) {
      case 0:
         textureColor = vec4(0.0);
         break;
      case 1:
         textureColor = texture(textures[1], textCoord);
         break;
      case 2:
         textureColor = texture(textures[2], textCoord);
         break;
      case 3:
         textureColor = texture(textures[3], textCoord);
         break;
      case 4:
         textureColor = texture(textures[4], textCoord);
         break;
      case 5:
         textureColor = texture(textures[5], textCoord);
         break;
      case 6:
         textureColor = texture(textures[6], textCoord);
         break;
      case 7:
         textureColor = texture(textures[7], textCoord);
         break;
   }
   color = calcColor(textureColor);
};