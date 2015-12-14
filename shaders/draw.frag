#version 400


uniform vec4 particlecolor;
//in vec2 velocity;

const float DELTA = 0.2;

out vec4 gl_FragColor;

void main() {
    vec2 p = 2.0 * (gl_PointCoord - 0.5);
    float a = smoothstep(1.0 - DELTA, 1.0, length(p));
//    float e = 0.0 + length(velocity) / 3.0;
//    gl_FragColor = pow(mix(particlecolor, vec4(0, 0, 0, 0), a), vec4(e));
    gl_FragColor = mix(particlecolor, vec4(0, 0, 0, 0), a);
//    gl_FragColor = vec4(1, 1, 1, 1);
}
