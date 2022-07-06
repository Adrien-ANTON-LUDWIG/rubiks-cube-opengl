#version 450

in vec4 fragment_color;
in vec2 interpolated_uv;

uniform sampler2D texture_sampler;

layout(location=0) out vec4 output_color;

void main() {
    // output_color = fragment_color;
    output_color = texture(texture_sampler, interpolated_uv);
}
