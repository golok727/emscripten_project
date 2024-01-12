#type vertex
#version 300 es

layout (location = 0) in vec3 aPos;
uniform mat4 u_Projection; 
uniform mat4 u_TranslationMat;

void main() {
	vec4 position = vec4(aPos, 1.0);
	gl_Position = u_Projection * u_TranslationMat * position; 
}

#type fragment
#version 300 es
precision mediump float; 
out vec4 color;
void main() {
	color = vec4(1.0, 1.0, 0.0, 1.0); 
}