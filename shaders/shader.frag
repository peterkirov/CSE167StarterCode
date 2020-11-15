#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 normalColor;
in vec3 fragPosition;
in vec3 transNorm;

uniform vec3 color;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specularpassed;
uniform float shininess;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float phong;
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec4 outFrag = vec4(1);
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    if(phong == 0.0f){
        outFrag = vec4(normalColor, 1.0);
    }else if (phong == 1.0f){
        
        vec3 ambient = lightColor * ambient;
  	    // diffuse 
        vec3 norm = normalize(transNorm);
        vec3 lightDir = normalize(lightPos - fragPosition);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lightColor * (diff * diffuse);
    
        // specular
        vec3 viewDir = normalize(viewPos - fragPosition);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = (spec * specularpassed);  
        
        float attenuate = 1.0/(0.1 *distance(lightPos, transNorm));

        vec3 result = attenuate*ambient + attenuate*diffuse + attenuate*specular;

        outFrag = vec4(result, 1.0);

    }
    fragColor = outFrag;
}
