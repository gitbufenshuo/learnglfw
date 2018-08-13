#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float xScale;
void main()
{
    gl_Position = vec4(aPos.x * xScale, aPos.y, aPos.z, 1.0);
}
