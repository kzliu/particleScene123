#version 400
// TODO (Task 2): Declare a "color" uniform
out vec3 fragColor;
uniform vec3 color;

void main() {
    fragColor = vec3(1.0);
    // TODO (Task 2): Set fragColor to a color uniform.
    fragColor = color;
}
