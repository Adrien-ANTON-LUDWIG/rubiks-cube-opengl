#version 450

in vec3 position;
in vec3 normalSmooth;
in vec3 color;

vec3 light_color = vec3(1.0, 1.0, 1.0);
vec3 light_position = vec3(1.0, 1.0, 5.0);

out vec4 fragment_color;

uniform mat4 model_view_matrix = mat4(
			      0.57735, -0.33333, 0.57735, 0.00000,
			      0.00000, 0.66667, 0.57735, 0.00000,
			      -0.57735, -0.33333, 0.57735, 0.00000,
			      0.00000, 0.00000, -17, 1.00000);
uniform mat4 projection_matrix = mat4(
			      15.00000, 0.00000, 0.00000, 0.00000,
			      0.00000, 15.00000, 0.00000, 0.00000,
			      0.00000, 0.00000, -1.00020, -1.00000,
			      0.00000, 0.00000, -10.00100, 0.00000);

void main() {
	gl_Position = projection_matrix * model_view_matrix * vec4(position, 1.0);
	fragment_color = vec4(color, 1.0);
}
