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

out vec2 i_tc;
out float i_nl;

void main(void) {
    gl_Position=P*V*M*vertex;

    mat4 G=mat4(inverse(transpose(mat3(M))));
    vec4 n=normalize(V*G*normal);

    i_nl=clamp(dot(n,lightDir),0,1);
    i_tc=texCoord;
}
