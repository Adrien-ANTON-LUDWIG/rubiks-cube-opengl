#version 450

in vec4 fragment_color;
in vec2 interpolated_uv;

uniform sampler2D texture_sampler;
uniform float opacity;

layout(location=0) out vec4 output_color;

void main() {
    // output_color = fragment_color;
    vec4 texture_color = texture(texture_sampler, interpolated_uv);
    output_color = vec4(texture_color.rgb, opacity);
}
