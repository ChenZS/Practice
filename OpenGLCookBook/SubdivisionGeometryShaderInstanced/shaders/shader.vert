#version 330 core
  
layout(location=0) in vec3 vVertex;		//object space vertex position
uniform mat4 M[4];

void main()
{ 
    gl_Position = M[gl_InstanceID]*vec4(vVertex,1);
}