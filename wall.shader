uniform sampler2D texture;
uniform vec2 pos[960];

struct Light {
	vec2 pos;
	float strength;
};
Light lights[4] = Light[4](
	Light(vec2(120, 120), 1),
	Light(vec2(120, -120), 1),
	Light(vec2(-120, 120), 1),
	Light(vec2(-120, -120), 1));

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	float b = 0;
	float ldistS;
	vec2 toLight;
	for (int i = 0; i < 4; i++) {
		toLight = pos[gl_FragCoord.x/2] - lights[i].pos;
		ldistS = toLight.x * toLight.x + toLight.y * toLight.y + 100;
		b += lights[i].strength * 1000000.0 / ldistS / ldistS;
	}
	pixel.rgb *= b;

    gl_FragColor = gl_Color * pixel;
}