#version 330 core
out vec4 fragColor;
  
in vec2 current_position;

uniform float time;
uniform float width, height;

void main() {
    vec2 centre_of_screen = 0.5 * vec2(width, height);
    vec2 val = gl_FragCoord.xy - centre_of_screen;
    float dist = length(val);
    
    float v = 100 * time / dist;

    fragColor = vec4((sin(v) + 1.0) / 2.0, 0.0, (cos(v + 100 * val.x * val.y) * sin(v - 100 * val.x * val.y) + 1.0) / 2.0, 1.0);
}