#version 330 core

uniform mat4 uni_t_model;
uniform mat4 uni_t_view;
uniform mat4 uni_t_proj;
uniform mat4 uni_t_normal;
uniform mat4 uni_t_mvp;

layout(location = 1) in vec3 in_position;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in vec3 in_normal;

out vec2 var_uv;
out vec3 var_normal;

void main() {
	//vec4 pos = uni_t_mvp * vec4(in_position, 1.0);
	//var_normal = (uni_t_normal * vec4(in_normal, 1.0)).xyz;
	//
	//var_uv = in_uv;
	var_uv = in_uv;
	gl_Position = vec4 (in_position.xyz, 1.0);
}