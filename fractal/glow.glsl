precision highp float;
uniform vec2 u_resolution;
uniform vec2 u_mouse; 
#define PI 3.14159265359
#define ITER 200.0

vec3 palette( float t ) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(.5,.5,.5);
    vec3 d = vec3(.6,0.416,0.557);

    return a + b*cos( 6.28318*(c*t+d) );
}

vec3 julia_glow(vec2 z, vec2 c) {
    float brightness;
    for (int i = 0; i < int(ITER); i++) {
        z = vec2(z.x*z.x - z.y*z.y, 2.0 * z.x * z.y) + c;
        if (dot(z, z) > 4.0){
            brightness = smoothstep(0.0, 0.4, float(i) / ITER * 2.0 - .1);
            if (brightness < 0.01){
                return vec3(0.0,0.0,0.0);
            }
            return palette(brightness);
        }
    }
    
    return palette(1.0);
}

void main() {
    vec2 uv = (gl_FragCoord.xy / u_resolution - 0.5) * 1.8;
    uv.x *= u_resolution.x / u_resolution.y;

    vec2 m = (u_mouse / u_resolution - 0.5) * 2.0;
    m.x *= u_resolution.x / u_resolution.y;

    vec3 c = julia_glow(uv * 1.5, m);

    gl_FragColor = vec4(c.r,c.g,c.b, 1.0);
}