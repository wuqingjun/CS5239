varying float LightIntensity;
void main()
{
    float a = gl_TexCoord[0].s * 0.5;
    //float b = sin(gl_TexCoord[0].t*30.0)*0.5+0.5;
    float b = mod(floor(10.0 * gl_TexCoord[0].s) + floor(10.0 * gl_TexCoord[0].t), 2.0);
    //  Create color for fragment
    gl_FragColor = vec4(a,b,1.0,1.0);

    //  Adjust color intensity for lighting (interpolated from vertex shader values)
    gl_FragColor *= LightIntensity;
}
