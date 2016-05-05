#version 120

void main()
{
	vec4 v = vec4(gl_Vertex);
	v.z = 0.0;

        //gl_TexCoord[0]=gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * v;
}
