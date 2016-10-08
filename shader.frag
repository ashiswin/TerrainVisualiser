#version 450

in vec3 pass_Color;
out vec3 out_Color;


void main(void) {
	out_Color = pass_Color;
	if(pass_Color.x < 0.1) {
		out_Color = vec3(0.57, 0.6, 0.95);
	}
}
