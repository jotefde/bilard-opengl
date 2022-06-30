#version 330

uniform sampler2D tex;

layout(location=0) out vec4 pixelColor;

in vec4 n;
in vec4 l;
in vec4 v;
in vec2 i_tc;

void main(void) {
	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);	
	
	vec4 mr = reflect(-ml, mn);

	//Parametry powierzchni
	vec4 kd = texture(tex, i_tc); 	
	vec4 ks = vec4(0.9);

	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1), 10);

	pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 0);
}
