precision highp float;
uniform vec2 u_resolution;
uniform vec2 u_mouse; 
#define PI 3.14159265359
#define ITER 200.0

float atan2(float x, float y){
    return 2.0 * atan(y / (sqrt(y*y + x*x) + x));
}

vec3 julia(vec2 z, vec2 c) {
    float angle;
    for (int i = 0; i < int(ITER); i++) {
        z = vec2(z.x*z.x - z.y*z.y, 2.0 * z.x * z.y) + c;
        if (dot(z, z) > 4.0){
            angle = atan2(z.x, z.y);
            return vec3(
                sqrt(float(i) / ITER),// * (1.0 + sin(angle)) / 2.0, 
                sqrt(float(i) / ITER),// * (1.0 + sin(angle + 2.0 * PI / 3.0))/2.0, 
                sqrt(float(i) / ITER)// * (1.0 + sin(angle + 4.0 * PI / 3.0))/2.0
            );
        }
    }
    angle = atan2(z.x, z.y);
    return vec3(
        (1.0 + sin(angle)) / 2.0, 
        (1.0 + sin(angle + 2.0 * PI / 3.0))/2.0, 
        (1.0 + sin(angle + 4.0 * PI / 3.0))/2.0
    );
}



void main() {
    vec2 uv = (gl_FragCoord.xy / u_resolution - 0.5) * 2.0;
    uv.x *= u_resolution.x / u_resolution.y;

    vec2 m = (u_mouse / u_resolution - 0.5) * 2.0;
    m.x *= u_resolution.x / u_resolution.y;

    vec3 c = julia(uv * 1.5, m);

    gl_FragColor = vec4(c.r,c.g,c.b, 1.0);
}