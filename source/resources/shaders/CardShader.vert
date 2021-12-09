#version 330 core

// Vertex Attribute Array/Object
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aVertexColor;
layout (location = 2) in vec2 aTexCoords;

// Output variables
out vec3 vertexColor;
out vec2 texCoords;

// Uniform variables
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
  vertexColor = aVertexColor;
  texCoords = aTexCoords;

  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPosition, 1.0);
}
