#version 460

in vec4 fragColor;
out vec4 outColor;

void main()
{
    if (fragColor.a == 0.0)
        discard;

    outColor = vec4(fragColor.rgb, 1.0);
}
