#version 430

in vec3 vertex_world;
in vec3 in_normal;
in vec3 light_vector;
in vec3 eye_vector;

vec3 light_color = vec3(0.3,0.5,0.5);

float ambient_intensity = 0.8;
float diffuse_intensity = 1.0;
float SpecularPower = 32.0;
float SpecularIntensity = 1.0;
float roughness = 0.02;


vec4 CalcLightInternal(vec3 light_color1, vec3 light_direction1, vec3 Normal)                                                  
{                                                                                           
    vec4 AmbientColor = vec4(light_color1 * ambient_intensity, 1.0f);
	
    float DiffuseFactor = dot(Normal, -light_direction1);                                     
                                                                                            
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                  
                                                                                          
    if (DiffuseFactor > 0) {                                                                
        DiffuseColor = vec4(light_color1 * diffuse_intensity * DiffuseFactor, 1.0f);    
                                                                                            
    }                                                                                       
                                                                                       
    return (DiffuseColor);                  
}

void main()                                                                                 
{      
       //Bring normal to [-1,1]                            
       vec3 normal = normalize(2.0 * in_normal - 1.0);                                                         
       vec3 light_vector0 = normalize(light_vector);
	   vec3 eye_vector0 = normalize(eye_vector);

       float rough_sq = roughness * roughness;

	   float A = 1.0 - 0.5 * (rough_sq/(rough_sq + 0.57));
	   float B = 0.45 * (rough_sq / (rough_sq + 0.09));
	   
	   //Find projected light vector which will help us to find azimuth angles
	   float normalDotLight = dot(normal, light_vector0);	   
	   vec3 light_projected = normalize(light_vector0 - (normal * normalDotLight));
	   
	   float normalDotEye = dot(normal,eye_vector0);
	   vec3 eye_projected = normalize(eye_vector0 - (normal * normalDotEye));
	   
	   float CX = dot(light_projected,eye_projected);

	   float angle_eye = acos(normalDotEye);
	   float angle_light = acos(normalDotLight);

	   float alpha = max(angle_eye,angle_light);
	   float beta =  min(angle_eye,angle_light);

	   float DX = sin(alpha) * tan(beta); 

	   float ON_component = max(0.0, normalDotLight)*(A + (B * max(0.0,CX) *DX));                     
	   
	   vec4 diffuse = CalcLightInternal(light_color, light_vector0, normal);
	   gl_FragColor = diffuse + vec4(light_color * ON_component , 1.0);                           
}
