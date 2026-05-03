#version 330 core

out vec4 FragColor;
in float vDist;
in vec3 vBarycentric;

void main()
{
    vec3 d = fwidth(vBarycentric);

    vec3 edge = smoothstep(d * 0.5, d * 1.5, vBarycentric);
    float edgeFactor = min(min(edge.x, edge.y), edge.z);

    float dCam = clamp(vDist / 10.0, 0.0, 1.0);

    vec3 nearColor = vec3(0.0, 0.0, 1.0);
    vec3 midColor  = vec3(0.0, 1.0, 0.0);
    vec3 farColor  = vec3(1.0, 0.0, 0.0);

    vec3 color;
    if (dCam < 0.5)
        color = mix(nearColor, midColor, dCam * 2.0);
    else
        color = mix(midColor, farColor, (dCam - 0.5) * 2.0);

    color = mix(vec3(0.0), color, edgeFactor);

    FragColor = vec4(color, 1.0);
}