#version 330 core

in vec2 fragCoord;
out vec3 FragColor;

uniform vec3 u_pixelColor;  // Uniform variable to pass in the color

void main()
{
    FragColor = u_pixelColor;  // Set the fragment color to the passed color
}
