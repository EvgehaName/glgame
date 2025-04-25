#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec3 Normal;
out vec2 TextureCoord;
out vec3 FragPos;

void main() 
{ 
    FragPos = vec3(uModel * vec4(position,1.0));
    Normal = mat3(transpose(inverse(uModel))) * normal;
    TextureCoord = textureCoord;
    gl_Position = uProjection * uView * uModel  * vec4(position, 1.0); 
}