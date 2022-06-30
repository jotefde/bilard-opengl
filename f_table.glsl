#version 330

uniform sampler2D fieldTexture;

layout(location=0) out vec4 pixelColor;

in vec2 i_tc;
in float i_nl;

void main(void) {
	vec4 color=texture(fieldTexture, i_tc);
	pixelColor=vec4(color.rgb*i_nl,color.a);
}
