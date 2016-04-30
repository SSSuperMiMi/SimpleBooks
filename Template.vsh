attribute vec4 Color;
attribute vec4 Position;

varying lowp vec4 fs_Color;

uniform mat4 MVP;
uniform vec4 UColor;

void main(void)
{
    gl_Position = MVP * Position;
    fs_Color = UColor;
}