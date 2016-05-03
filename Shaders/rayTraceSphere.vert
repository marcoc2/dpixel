#version 120

varying vec3 sphereOrigin;
float radius = 10;
//varying float lutTextureCoord;

/** Orignally "uniform" variables */
float minValue = -10.0;
float maxValue = 10.0;
float verticalScale = 1.0;
uniform vec2 screenSize;
//vec4 voxelColor = vec4( 1.0, 0.0, 0.0, 1.0 );

void main(void)
{

   //radius = gl_MultiTexCoord0.s;

   // repassa posicao
   vec3 sphereOriginAux = gl_Vertex.xyz * vec3( 1.0f, 1.0f, verticalScale);
   vec4 vertexPosition = vec4( sphereOriginAux, 1.0f );

   // repassa cor
   gl_FrontColor = gl_Color;

   // calcula coordenadad e textura
   //lutTextureCoord = (gl_FogCoord - minValue) / ( maxValue - minValue );

   // 8 pontos do cubo no espaco de tela
   vec4 p1 = gl_ModelViewProjectionMatrix * ( vertexPosition + vec4( -radius, -radius, -radius, 0 ) );
   vec4 p2 = gl_ModelViewProjectionMatrix * ( vertexPosition + vec4( radius, -radius, -radius, 0 ) );
   vec4 p3 = gl_ModelViewProjectionMatrix * ( vertexPosition + vec4( -radius, radius, -radius, 0 ) );
   vec4 p4 = gl_ModelViewProjectionMatrix * ( vertexPosition + vec4( radius, radius, -radius, 0 ) );
   vec4 p5 = gl_ModelViewProjectionMatrix * ( vertexPosition + vec4( -radius, -radius, radius, 0 ) );
   vec4 p6 = gl_ModelViewProjectionMatrix * ( vertexPosition + vec4( radius, -radius, radius, 0 ) );
   vec4 p7 = gl_ModelViewProjectionMatrix * ( vertexPosition + vec4( -radius, radius, radius, 0 ) );
   vec4 p8 = gl_ModelViewProjectionMatrix * ( vertexPosition + vec4( radius, radius, radius, 0 ) );

   float oneOverW = 1.0f / p1.w;
   p1.xy *= oneOverW;

   oneOverW = 1.0f / p2.w;
   p2.xy *= oneOverW;

   oneOverW = 1.0f / p3.w;
   p3.xy *= oneOverW;

   oneOverW = 1.0f / p4.w;
   p4.xy *= oneOverW;

   oneOverW = 1.0f / p5.w;
   p5.xy *= oneOverW;

   oneOverW = 1.0f / p6.w;
   p6.xy *= oneOverW;

   oneOverW = 1.0f / p7.w;
   p7.xy *= oneOverW;

   oneOverW = 1.0f / p8.w;
   p8.xy *= oneOverW;

   float minX = min( min( min( p1.x, p2.x ), min( p3.x, p4.x ) ), min( min( p5.x, p6.x ), min( p7.x, p8.x ) ) );
   float maxX = max( max( max( p1.x, p2.x ), max( p3.x, p4.x ) ), max( max( p5.x, p6.x ), max( p7.x, p8.x ) ) );

   float minY = min( min( min( p1.y, p2.y ), min( p3.y, p4.y ) ), min( min( p5.y, p6.y ), min( p7.y, p8.y ) ) );
   float maxY = max( max( max( p1.y, p2.y ), max( p3.y, p4.y ) ), max( max( p5.y, p6.y ), max( p7.y, p8.y ) ) );

   float width = (maxX - minX) * screenSize.x;
   float height = (maxY - minY) * screenSize.y;

   gl_PointSize = max( width, height );

   gl_Position = gl_ModelViewProjectionMatrix * vertexPosition;

}
