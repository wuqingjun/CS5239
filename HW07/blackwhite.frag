// Inter-image processing

uniform int       mode;
uniform float     frac;
uniform sampler2D img0;

void main()
{
    if(mode == 0) {
        gl_FragColor = texture2D(img0,gl_TexCoord[0].st);
    }
    else {
        vec4 rgba = texture2D(img0,gl_TexCoord[0].st);
        vec3 rgb = rgba.rgb;
        vec3 diff = rgb - (1.0, 1.0, 1.0);
        float dist = dot(diff, diff) / 3.0;
        if(dist < frac) {
            gl_FragColor = vec4(1.0, 1.0, 1.0, rgba.a);
        }
        else{
            gl_FragColor = vec4(0.0, 0.0, 0.0, rgba.a);
        }
    }
}
