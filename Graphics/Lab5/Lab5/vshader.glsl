#version 120

attribute vec4 vPosition;//in
attribute vec4 vColor;
varying vec4 color;//out

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;

void main()
{
	color = vColor;
	gl_Position = projection_matrix * model_view_matrix * vPosition / vPosition.w;
}
