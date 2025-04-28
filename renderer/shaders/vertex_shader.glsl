uniform sampler2D u_texture;

void main()
{
    vec2 uv = (gl_FragCoord.xy / vec2(800.0, 600.0)); // adjust to your window size
    FragColor = texture(u_texture, uv);
}