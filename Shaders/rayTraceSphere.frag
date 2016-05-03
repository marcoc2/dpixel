#version 120

// variavel de entrada
varying vec3 sphereOrigin;
//varying float lutTextureCoord;
// lut1d
//uniform sampler1D lutTextureSampler;

/** Orignally "uniform" variables */
uniform vec2 screenSize;
vec3 nearHorizontal = vec3(10.0, 1.0, 1.0);
vec3 nearVertical = vec3(10.0, 10.0, 1.0);
vec3 nearCenter = vec3(0.0, 1.0, 1.0);
vec3 farHorizontal = vec3(10.0, 1.0, 10.0);
vec3 farVertical = vec3(10.0, 10.0, 10.0);
vec3 farCenter = vec3(0.0, 1.0, 10.0);
vec3 lightDirection = vec3(10.0, 10.0, 10.0);

float radius = 10;

bool solveQuadratic( in float a, in float b, in float c, out float x0, out float x1)
{
   float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
   else if (discr == 0) x0 = x1 = - 0.5 * b / a;
   else {
        float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
   }
   if( x0 > x1 )
   {
       float temp = x0;
       x0 = x1;
       x1 = temp;
   }
   return true;
}

bool intersectRaySphere( in vec3 rayDir, in vec3 rayOrig, in vec3 sphereOrig, out float time )
{

    vec3 centerToOrigin = rayOrig - sphereOrig;

// coeficientes da equacao quadratica
// A = 1 pois rayDir eh unitario
    float B = 2.0f * dot( centerToOrigin, rayDir );
    float C = dot( centerToOrigin, centerToOrigin ) - radius * radius;

   float t0 = 0.0f;
   float t1 = 0.0f;

   bool isResolved = solveQuadratic( 1, B, C, t0, t1 );

   if( isResolved == false ) return false;

   if( t1 < 0.0f )
       return false;

   if( t0 < 0.0f )
   {
       time = t1;
   }
   else
   {
       time = t0;
   }

    return(true);

}

void main(void)
{

   // normaliza frag coord para ir de - screenSize / 2 até screenSize / 2
   vec2 normalizedFragCoord = 2.0 * gl_FragCoord.xy / screenSize - 1.0;

   vec4 fragmentOrigin = vec4( nearCenter + nearHorizontal * normalizedFragCoord.x + nearVertical * normalizedFragCoord.y, 1 );

   // calcula fragmento no far
   vec4 fragmentFar = vec4( farCenter + farHorizontal * normalizedFragCoord.x + farVertical * normalizedFragCoord.y, 1 );

   // direcao do raio
   vec3 rayDir = normalize( (fragmentFar - fragmentOrigin).xyz );

   // origem do raio
   vec3 rayOrigin = fragmentOrigin.xyz;

   float time = 0;

   // testa intersecao com esfera
   if( !intersectRaySphere( rayDir, rayOrigin, sphereOrigin, time ) )
   {
       discard;
   }
   else
   {

       // computa ponto da intersecao
       vec3 intersectionPoint = rayOrigin + rayDir * time;

       vec3 normal = normalize( sphereOrigin - intersectionPoint );

       float dotProd = dot( normal, lightDirection );

       if( dotProd < 0 ) dotProd = -dotProd;

       // dat magic number...
       float colorCoeficient = ( dotProd + 0.386 ) * 0.7215;

       // consulta lut
       //vec4 lutColor = texture1D( lutTextureSampler, lutTextureCoord );
       vec4 lutColor = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
       //vec4 lutColor = gl_Color;

       // aplica iluminacao
       gl_FragColor = vec4( colorCoeficient * lutColor.rgb, lutColor.a );

       // projeta no espaço de clip o ponto da interceptacao
       vec4 newCoord = gl_ModelViewProjectionMatrix * vec4( intersectionPoint, 1 );
       newCoord.z /= newCoord.w;

       // atualiza profundidade do fragmento como a coordenada z do ponto de intersecao no espaco de clip normalizado de [0..1]
       gl_FragDepth = ( newCoord.z + 1.0f ) / 2.0f;
   }

}
