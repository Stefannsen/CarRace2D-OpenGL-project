
// Shader-ul de fragment / Fragment shader  

 
 #version 400

in vec4 ex_Color;
in vec2 tex_Coord;
out vec4 out_Color;

vec4 red = vec4(1.0,0.0,0.0,1.0);
vec4 green= vec4(0.0,1.0,0.0,1.0);

uniform sampler2D myTexture;
//uniform sampler2D myTexture_1;


void main(void)
  {
  // out_Color=ex_Color;
  // out_Color=mix(red,green,0.9);
   //out_Color = texture(myTexture, tex_Coord);
   out_Color = texture(myTexture, tex_Coord);

	
  }
 