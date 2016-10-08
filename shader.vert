#version 450

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec2 heightmapDimensions;
uniform sampler2D heightmap;

layout(location = 0) in vec3 in_Position;
out vec3 pass_Color;

void main(void) {
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position.x * 10, texture(heightmap, vec2(in_Position.x / heightmapDimensions.x, in_Position.y / heightmapDimensions.y)).r * 300, in_Position.y * 10, 1.0);
	pass_Color = texture(heightmap, vec2(in_Position.x / heightmapDimensions.x, in_Position.y / heightmapDimensions.y)).rgb;
}
