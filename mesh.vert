#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 MVP;
uniform vec3 uCameraPos;

out float vDist;
out vec3 vBarycentric;

void main()
{
    vec3 worldPos = position;
    vDist = distance(worldPos, uCameraPos);

    int localIdx = gl_VertexID % 3; //to jest wbudowane

    if (localIdx == 0)
        vBarycentric = vec3(1.0, 0.0, 0.0);
    else if (localIdx == 1)
        vBarycentric = vec3(0.0, 1.0, 0.0);
    else
        vBarycentric = vec3(0.0, 0.0, 1.0);

    gl_Position = MVP * vec4(position, 1.0);
}