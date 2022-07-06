#version 450

in vec3 position;
in vec3 normalSmooth;
in vec3 color;
in vec2 uv;

vec3 light_color = vec3(1.0, 1.0, 1.0);
vec3 light_position = vec3(1.0, 1.0, 5.0);

out vec4 fragment_color;
out vec2 interpolated_uv;

uniform mat4 transform;
uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;

void main() {
	gl_Position = projection_matrix * model_view_matrix * transform * vec4(position, 1.0);
	fragment_color = vec4(color, 1.0);
	interpolated_uv = uv;
}
