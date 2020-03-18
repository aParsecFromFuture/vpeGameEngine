#version 130

struct ray{
    vec3 origin;
    vec3 direction;
};

struct sphere{
    vec3 center;
    vec3 color;
    float radius;
    bool reflection;
};

uniform float TRANSFORM_X;
uniform float TRANSFORM_Y;

uniform int SPHERE_COUNT = 3;

uniform sphere SPHERES[3] =  sphere[](sphere(vec3(0, 0, 0), vec3(1.0f, 0.0f, 0.0f), 120.0f, false), 
                                                sphere(vec3(0, 0, 0), vec3(0.0f, 1.0f, 0.0f), 4.0f, false),
                                                sphere(vec3(0, 0, 0), vec3(0.0f, 0.0f, 1.0f), 2.0f, false));

uniform vec3 LIGHT = vec3(100.0f, 0, -40.0f);
uniform vec3 CAMERA_POSITION;
uniform vec3 CAMERA_DIRECTION;
uniform vec3 CAMERA_UP;

const vec3 SPACE_COLOR = vec3(0, 0, 0);
const bool SHADE_MODE_ON = true;

const float DIFFUSE_CONSTANT = 0.55f;
const float SPECULAR_CONSTANT = 0.45f;
const float AMBIENT_CONSTANT = 0.05f;

const int REFLECTION_COUNT = 2;

vec3 CAMERA_RIGHT = cross(CAMERA_UP, CAMERA_DIRECTION);

vec3 imagePlane(vec2 point)
{
    return (TRANSFORM_X * point.x - 8) * CAMERA_RIGHT + (4.5 - TRANSFORM_Y * point.y) * CAMERA_UP + 10 * CAMERA_DIRECTION;
}

float ray_intersect_sphere(ray ry, sphere sph)
{
    float s, r, q, s2, r2, l2, m2;
    vec3 l;
    
    l = sph.center - ry.origin;
    r = sph.radius;
    l2 = dot(l, l);
    s = dot(ry.direction, l);
    r2 = r * r;
    
    if(s < 0 && l2 > r2){
        return 0.0f;
    }
    
    s2 = s * s;
    m2 = l2 - s2;
    
    if(m2 > r2){
        return 0.0f;
    }
    
    q = sqrt(r2 - m2);
    return (l2 > r2)? (s - q) : (s + q);
}

vec3 get_pixel_color(ray r, float t, vec3 intersect_normal, vec3 color)
{
    vec3 pixel_coord, pixel_color, intersect_point, to_eye, from_light, to_light, reflected_light;
    float diffuse_value, specular_value, ambient_value;
    int i, the_nearest;
    
    intersect_point = r.origin + t * r.direction;
    to_light = normalize(LIGHT - intersect_point);
    pixel_color = vec3(0, 0, 0);

    if(dot(intersect_normal, to_light) > 0){
        
        if(SHADE_MODE_ON){
            ray r2 = ray(intersect_point, to_light);
            for(i = 0; i < SPHERE_COUNT; i++){
                t = ray_intersect_sphere(r2, SPHERES[i]); 
                if(t < length(LIGHT - intersect_point) && t > 0.1f){
                    pixel_color = AMBIENT_CONSTANT * color;
                    return pixel_color;
                }
            }
        }
        
        from_light = -to_light;
    
        to_eye = normalize(r.origin - intersect_point);
        reflected_light = reflect(from_light, intersect_normal);
        
        diffuse_value = dot(intersect_normal, to_light);
        pixel_color += DIFFUSE_CONSTANT * diffuse_value * color;
        
        specular_value = dot(reflected_light, to_eye);
        specular_value = (specular_value > 0)? pow(specular_value, 8) : 0;
        pixel_color += SPECULAR_CONSTANT * specular_value * vec3(1, 1, 1);
    }
    pixel_color += AMBIENT_CONSTANT * color;
    return pixel_color;
}

void main()
{
    vec3 pixel_coord, pixel_color, intersect_point, intersect_normal;
    ray r, the_last_ray;
    float the_last_t, t ,t_now;
    int i, the_nearest, the_last, step;
    bool intersection = false;
    
    pixel_coord = imagePlane(gl_FragCoord.xy);
    pixel_color = SPACE_COLOR;
    
    r = ray(CAMERA_POSITION, normalize(pixel_coord));
    t = 10000.0f;
    for(i = 0; i < SPHERE_COUNT; i++){
        t_now = ray_intersect_sphere(r, SPHERES[i]);
        if(t_now > 0.1f && t > t_now){
            t = t_now;
            the_nearest = i;
            intersection = true;
        }
    }
    
    for(i = 0; i < SPHERE_COUNT; i++){
        t_now = ray_intersect_sphere(r, SPHERES[i]);
        if(t_now > 0.1f && t > t_now){
            t = t_now;
            the_nearest = i;
            intersection = true;
        }
    }
    
    if(intersection){
        intersect_normal = normalize((r.origin + t * r.direction) - SPHERES[the_nearest].center);
        pixel_color = get_pixel_color(r, t, intersect_normal, SPHERES[the_nearest].color);
    }else{
        gl_FragColor = vec4(pixel_color, 0);
    }
    
    if(intersection){
        if(SPHERES[the_nearest].reflection){
            intersect_normal = normalize((r.origin + t * r.direction) - SPHERES[the_nearest].center);
            pixel_color = get_pixel_color(r, t, intersect_normal, SPHERES[the_nearest].color);
            the_last_ray = r;
            the_last = the_nearest;
            the_last_t = t;
            step = 0;
            while(SPHERES[the_last].reflection && step++ < REFLECTION_COUNT){
                intersection = false;
                intersect_point = r.origin + t * r.direction;
                r = ray(intersect_point, normalize(intersect_point - SPHERES[the_last].center));
                
                t = 10000.0f;
                for(i = 0; i < SPHERE_COUNT; i++){
                    t_now = ray_intersect_sphere(r, SPHERES[i]);
                    if(t_now > 0.1f && t > t_now){
                        t = t_now;
                        the_last = i;
                        the_last_t = t;
                        intersection = true;
                    }
                }
                
                if(!intersection)break;
                the_last_ray = r;
            }
            intersect_normal = normalize((the_last_ray.origin + the_last_t * the_last_ray.direction) - SPHERES[the_last].center);
            pixel_color = 0.8*pixel_color + 0.2*get_pixel_color(the_last_ray, the_last_t, intersect_normal, SPHERES[the_last].color);
        }else{
            intersect_normal = normalize((r.origin + t * r.direction) - SPHERES[the_nearest].center);
            pixel_color = get_pixel_color(r, t, intersect_normal, SPHERES[the_nearest].color);
        }
    }
    gl_FragColor = vec4(pixel_color, 0);
} 