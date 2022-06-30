#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


uniform vec4 color=vec4(1,1,1,1);
uniform vec4 lightDir=vec4(0,0,1,0);

//Atrybuty
layout (location=0) in vec4 vertex;
layout (location=1) in vec4 normal;
layout (location=2) in vec2 texCoord;

out vec4 l;
out vec4 n;
out vec4 v;
out vec2 i_tc;

void main(void) {
    vec4 lp = vec4(0, 0, 30, 1);
    l = normalize(V * lp - V*M*vertex); //wektor do œwiat³a w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normal); //wektor normalny w przestrzeni oka
    gl_Position=P*V*M*vertex;

    i_tc=texCoord;
}
