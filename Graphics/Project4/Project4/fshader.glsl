#version 120
varying vec2 texCoord;
varying vec4 color;
uniform sampler2D texture;
uniform int fEnable;

void main() {
    if (fEnable == 1)//normal frag color
    {
        gl_FragColor = color;
    }
    else if (fEnable == 2)
    {
        gl_FragColor = texture2D(texture, texCoord);
    }
    
}
