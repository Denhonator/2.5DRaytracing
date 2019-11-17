uniform sampler2D texture;
uniform vec2 offset;
uniform vec2 tiling;
uniform float angle;

void main()
{
	float rad = radians((1.0 - (gl_FragCoord.y) / 432.0) * 22.0);
	float dist = 0.5 / tan(rad);
	float newAngleDeg = angle + (gl_FragCoord.x - 960.0) / 960.0 * 35.0;
	float newAngle = radians(newAngleDeg);
	vec2 rotation = vec2(-cos((newAngle)), -sin((newAngle)));

	vec4 pixel = texture2D(texture, (offset * 0.1 + dist * rotation));

    gl_FragColor = gl_Color * pixel;
}