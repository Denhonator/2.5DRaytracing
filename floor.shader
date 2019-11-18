uniform sampler2D texture;
uniform vec2 offset;
uniform vec2 tiling;
uniform float angle;

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
	float rad = radians((1.0 - gl_FragCoord.y / 540.0) * 22.0);
	float dist = 0.5 / tan(rad);
	float newAngleDeg = angle + (gl_FragCoord.x - 960.0) / 960.0 * 35.0;
	float newAngle = radians(newAngleDeg);
	vec2 rotation = vec2(-cos((newAngle)), -sin((newAngle)));

	vec4 pixel = texture2D(texture, (offset*0.05 + dist * rotation));

	float b = 0;
	float ldistS;
	vec2 toLight;
	for (int i = 0; i < 4; i++) {
		toLight = (offset + dist * rotation * 20) - lights[i].pos;
		ldistS = toLight.x * toLight.x + toLight.y * toLight.y + 1000;
		b += lights[i].strength * 1000.0 / ldistS;
	}
	pixel *= b;

    gl_FragColor = gl_Color * pixel;
}