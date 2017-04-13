#version 430

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent; 

uniform mat4 MVP_matrix;
uniform mat4 World_matrix;

vec3 eye_position = vec3(0.0,0.0,0.0);
vec3 light_position = vec3(0.0,0.0,-150.0);
mat3 TBN;

out vec3 vertex_world;
out vec3 in_normal;
out vec3 light_vector;
out vec3 eye_vector;

void CaclulateTangentSpace()
{
    vec3 Normal0 = normalize(Normal);                                                       
    vec3 Tangent0 = normalize(Tangent);                                                     
    Tangent0 = normalize(Tangent0 - dot(Tangent0, Normal0) * Normal0);                           
    vec3 Bitangent0 = cross(Tangent0, Normal0);                                                
    TBN = mat3(Tangent0, Bitangent0, Normal0);
}

void main()
{
	vertex_world = (World_matrix * vec4(Position,1.0)).xyz;
	in_normal = (World_matrix * vec4(Normal, 0.0)).xyz;

	//Build tangent space
	CaclulateTangentSpace();

	//Compute light vector in Tangent space
	light_vector = normalize(light_position - vertex_world);
	light_vector = TBN * light_vector;

	//Compute eye vector in Tangent space
	eye_vector = normalize(eye_position- vertex_world);
	eye_vector = TBN * eye_vector;

	gl_Position = MVP_matrix * vec4(Position,1.0);
}
