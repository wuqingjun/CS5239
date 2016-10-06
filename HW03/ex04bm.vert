//  Per-vertex Blinn-Phong lighting
//  Vertex shader

float my_dot(vec3 a, vec3 b)
{
	vec3 c = a * b;
	return c.x + c.y + c.z;
}

vec3 my_norm(vec3 v)
{
	return v / sqrt(dot(v,v));
}

vec4 blinn()
{
   //  P is the vertex coordinate on body
   vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex);
   //  N is the object normal at P
   vec3 N = my_norm(gl_NormalMatrix * gl_Normal);
   //  Light Position for light 0
   vec3 LightPos = vec3(gl_LightSource[0].position);
   //  L is the light vector
   vec3 L = my_norm(LightPos - P);

   //  Emission and ambient color
   vec4 color = gl_FrontMaterial.emission + gl_FrontLightProduct[0].ambient + gl_LightModel.ambient*gl_FrontMaterial.ambient;

   //  Diffuse light intensity is cosine of light and normal vectors
   float Id = my_dot(L,N);
   if (Id>0.0)
   {
      //  Add diffuse
      color += Id*gl_FrontLightProduct[0].diffuse;
      //  Specular is cosine of half and normal vectors
      float Is = my_dot(N,gl_LightSource[0].halfVector.xyz);
      if (Is>0.0) color += pow(Is,gl_FrontMaterial.shininess)*gl_FrontLightProduct[0].specular;
   }

   //  Return sum of color components
   return color;
}

void main()
{
   //  Vertex color
   gl_FrontColor = blinn();

   //  Texture coordinate
   gl_TexCoord[0] = gl_MultiTexCoord0;

   //  Return fixed transform coordinates for this vertex
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
