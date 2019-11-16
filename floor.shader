uniform sampler2D texture;
uniform vec2 offset;
uniform vec2 tiling;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy * tiling + offset / 20);
	pixel -= gl_FragCoord.y / 1000;
	pixel -= abs(980-gl_FragCoord.x) / 7000;

    gl_FragColor = gl_Color * pixel;
}