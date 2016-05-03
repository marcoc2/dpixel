uniform vec3 iResolution;
uniform vec4 iMouse;
uniform float iGlobalTime;

const vec3 background  = vec3(0.0, 0.0, 0.2);
const vec3 light_1     = vec3(4.0, 8.0,  3.0);
const vec3 light_2     = vec3(-4.0, 8.0, -7.0);
const vec2 eps         = vec2(0.001, 0.0);
const int maxSteps     = 64;

float time = iGlobalTime;

vec3 shade(vec3 color, vec3 point, vec3 normal, vec3 rd)
{
	
	vec3 dtl       = normalize(light_1 - point);
	float diffuse  = dot(dtl, normal); //diffuse
	float specular = 0.75 * pow(max(dot(reflect(dtl, normal), rd), 0.0), 64.0); //specular
	vec3 c = (diffuse + specular) * color * 0.85;
	
	dtl      =  normalize(light_2 - point);
	diffuse  = dot(dtl, normal); //more diffuse
	specular = 0.9 * pow(max(dot(reflect(dtl, normal), rd), 0.0), 128.0); //more specular
	return clamp( c + (diffuse + specular) * 0.25 * color, 0.0, 1.0);
}

// estimates the distance from Point p to implicit given geometry
float distanceEstimator(vec3 p)
{
	float t = mod(time, 70.0);
	p = p - vec3(t, t * 0.5, t * 0.3);
	
	float rpm = 1.0;
	vec3 repeater = mod(p, vec3(rpm)) - 0.5 * vec3(rpm);

	float sphere = length(repeater) - 0.06 * rpm;
	
	vec2 cylinder = vec2(max(length(repeater.xz) - 0.2, 0.0), repeater.y);
	float doughnut = length(cylinder) - 0.015 * rpm;
	
	return min(doughnut, sphere);
}

void main() {
	
	float ratio  = iResolution.x / iResolution.y;
	vec2 fragment = gl_FragCoord.xy / iResolution.xy;
	
	vec2 uv = -1.0 + 2.0 * fragment;
	uv.x *= ratio;
	
	//camera setup taken from iq's raymarching box: https://www.shadertoy.com/view/Xds3zN
	vec3 ta = vec3( 0.0, 0.0, -3.5 );
	vec3 ro = vec3( -3.0 + 3.2*cos(0.3*time + 6.0), 4.0, 1.0 + 3.2*sin(0.2*time + 6.0) );
	vec3 cw = normalize( ta-ro );
	vec3 cp = vec3( 0.0, 1.0, 0.0 );
	vec3 cu = normalize( cross(cw,cp) );
	vec3 cv = normalize( cross(cu,cw) );
	vec3 rd = normalize( uv.x*cu + uv.y*cv + 2.5*cw );
	
	vec3 col             = background;
	float t              = 0.0;
	vec3 p               = vec3(0.0);
	
	// march
	float steps = 0.0;
	for (int i = 0; i < maxSteps; i++) {
		p = ro + t * rd;
		float distanceEstimation = distanceEstimator(p);
		if (distanceEstimation > 0.005) {
			t += distanceEstimation;
			steps += 1.0;
		} else {
			break;
		}
	}
	
	vec3 c = (cos(p * 0.5 - 0.5) + 1.0) / 2.0;
	vec3 normal = normalize(vec3(distanceEstimator(p + eps.xyy) - distanceEstimator(p - eps.xyy),
								 distanceEstimator(p + eps.yxy) - distanceEstimator(p - eps.yxy),
								 distanceEstimator(p + eps.yyx) - distanceEstimator(p - eps.yyx)));
	
	col = shade(c, p, normal, rd);
	col = mix(col, background, smoothstep(0.8, 1.0, float(steps) / float(maxSteps)));
	
	
	gl_FragColor = vec4(col, 1.0); 

}
