#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction[2];
uniform vec3 light_position[2];
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms

uniform int light_type;
uniform vec3 object_color;
uniform float cut_off_angle;

// Output
layout(location = 0) out vec4 out_color;


void main()
{   
    float ambient_light = 0.25 * material_kd;

    float diffuse_light, specular_light;
    float light = ambient_light;

    float cut_off = radians(cut_off_angle);

    for (int i = 0; i < 2; ++i) {
        vec3 L = normalize( light_position[i] - world_position );
        vec3 V = normalize( eye_position - world_position );
        vec3 R = reflect (-L, world_normal);
    
        diffuse_light = material_kd * 1 * max( dot( L, world_normal ), 0.0 );
        specular_light = 0; 

        if (diffuse_light > 0) {
            specular_light = material_ks * 1 * pow( max( dot( R, V ), 0.0 ), material_shininess );
        }

        if (light_type == 0) { 
            float kq = 1 / pow(distance(light_position[i], world_position), 2);
            light += kq * (diffuse_light + specular_light);
        } else {
            
            float spot_light = dot(-L, light_direction[i]);
            float spot_light_limit = cos(cut_off);
            
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);

            if (spot_light < spot_light_limit) {
                light += 0;
            } else {
                light += light_att_factor * (diffuse_light + specular_light);
            }
        }  
    }
     // TODO(student): Write pixel out color
    out_color = vec4(light * object_color, 1.0); 
}