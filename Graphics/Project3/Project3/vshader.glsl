#version 120

attribute vec4 vPosition;
attribute vec4 vNormal;
varying vec4 color;

uniform int isShadow;
uniform mat4 model_view, projection;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct, LightPosition, black;
uniform float shininess, attenuation_constant, attenuation_linear, attenuation_quadratic;
vec4 ambient, diffuse, specular;

void main()
{
    if (isShadow == 1)
    {
        color = black;
        float x = LightPosition.x-LightPosition.y*((LightPosition.x-vPosition.x)/(LightPosition.y-vPosition.y));
        float y = 0.001;
        float z = LightPosition.z-LightPosition.y*((LightPosition.z - vPosition.z)/(LightPosition.y-vPosition.y));
        gl_Position = projection * model_view * vec4(x, y, z, 1.0);
    }
    else
    {
        ambient = AmbientProduct;
        vec4 N = normalize(model_view * vNormal);
        vec4 L_temp = model_view * (LightPosition - vPosition);
        vec4 L = normalize(L_temp);
        diffuse = max(dot(L,N), 0.0) * DiffuseProduct;
        vec4 EyePoint = vec4(0.0, 0.0, 0.0, 1.0);
        vec4 V = normalize(EyePoint - (model_view * vPosition));
        vec4 H = normalize(L + V);
        specular = pow(max(dot(N, H), 0.0), shininess) * SpecularProduct;
        float distance = length(L_temp);
        float attenuation = 1/(attenuation_constant + (attenuation_linear * distance) + (attenuation_quadratic * distance * distance));
        vec4 c = vNormal;
        c.w = 1.0;
        //color = c;
        color = ambient + (attenuation * (diffuse + specular));
        //color = c;//(attenuation * (diffuse + specular));
        gl_Position = projection * model_view * vPosition;
    }
    
    
}
