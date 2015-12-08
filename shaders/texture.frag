#version 400



in vec2 uv;
// TODO (Task 8): Declare a sampler2D uniform
uniform sampler2D texture1;

out vec4 color;

void main(){
    // TODO (Task 8): Set the output color to the corresponding texel

    color = texture(texture1, uv);

}
