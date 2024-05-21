#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
//layout(location = 1) in vec2 texCoord;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color2;

//out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_Position;
out vec3 v_Color2;

uniform mat4 u_MVP;

void main()
{   
    //  v_TexCoord = texCoord;
    gl_Position = u_MVP * position;
    v_Normal = normal; // pass normal to fragment shader
    v_Position = position.xyz; // pass position to fragment shader
    v_Color2 = color2;
};


#shader fragment
#version 330 core


layout(location = 0) out vec4 color;

//in vec2 v_TexCoord;
in vec3 v_Normal; // added input variable for normal
in vec3 v_Position; // added input variable for position
in vec3 v_Color2;

uniform vec4 u_Color;
//uniform sampler2D u_Texture;
uniform vec3 u_LightPosition; // added uniform for light position
uniform vec3 u_LightColor; // added uniform for light color
uniform vec3 viewPos;

void main()
{
    
    //vec4 texColor = texture(u_Texture, v_TexCoord);
    // if binding texture
    //color = u_Color * texColor;
    //color = u_Color;
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * u_LightColor;
  	
    // diffuse 
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;
    
    // specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - v_Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColor;  
        
    vec3 result = (ambient + diffuse + specular) * v_Color2.xyz;
    color = vec4(result, 1.0);
};