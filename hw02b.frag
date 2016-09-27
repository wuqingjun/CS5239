/* Sine Wave texture shader
* Derived from http://computer-graphics.se/TSBK07-files/pdf/PDF09/7b.pdf
* Added Lighting effects, optimized calculations
*/


//  Light from vertex shader
varying float LightIntensity;

// Given in main application
uniform float time;

vec4 permute ( vec4 x) { return mod ((34.0 * x + 1.0) * x , 289.0) ; }
vec2 cellular ( vec2 P) {
const float K = 1.0/7.0;
const float K2 = 0.5/7.0;
const float jitter = 0.8; // jitter 1.0 makes F1 wrong more often
vec2 Pi = mod ( floor (P ) , 289.0) ;
vec2 Pf = fract ( P);
vec4 Pfx = Pf .x + vec4 ( -0.5 , -1.5 , -0.5 , -1.5) ;
vec4 Pfy = Pf .y + vec4 ( -0.5 , -0.5 , -1.5 , -1.5) ;
vec4 p = permute ( Pi .x + vec4 (0.0 , 1.0 , 0.0 , 1.0) );
p = permute (p + Pi .y + vec4 (0.0 , 0.0 , 1.0 , 1.0) );
vec4 ox = mod (p , 7.0) *K+ K2 ;
vec4 oy = mod ( floor (p *K) ,7.0) * K+ K2 ;
vec4 dx = Pfx + jitter * ox ;
vec4 dy = Pfy + jitter * oy ;
vec4 d = dx * dx + dy * dy ; // distances squared
// Cheat and pick only F1 for the return value
d. xy = min (d.xy , d. zw ) ;
d.x = min (d.x , d. y);
return d. xx ; // F1 duplicated , F2 not computed
}


void main()
{
	// Create a simple sin function based off of texture coordinates
	// Multiply by time to rotate the resulting patterns
	// Multiply by 0.75 to adjust speed of rotation
    /*
	float a = sin(gl_TexCoord[0].s*30.0*time*0.75)*0.5+0.5;	
	float b = sin(gl_TexCoord[0].t*30.0*time*0.75)*0.5+0.5;

	// Create color for fragment
    gl_FragColor = vec4(a,b,1.0,1.0);

	// Adjust color intensity for lighting (interpolated from vertex shader values)
     gl_FragColor *= LightIntensity;
     */

    vec2 F = cellular(gl_TexCoord[0].st*10.0);
    float facets = 0.1 + (F.y - F.x);
    gl_FragColor = vec4(vec3(facets), 1.0);
    gl_FragColor *= LightIntensity;
}
