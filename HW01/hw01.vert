//  NDC to RGB Basic vertex shader


void main()
{
   //  Set vertex coordinates
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   
    // Get pixel location in mapping vector
    // Map NDC (-1 to 1) to RGB (0 to 1):
    // -1 to 1 -> 0 to 2 to be positive like RGB
    // 0 to 2 -> NGB space is 2x as large as RBG -> divide by 2
    // 
    // 4D -> 3D
    // w might not be 1 -> need to divide by w to get
    // correct depth mapping
    gl_FrontColor = (((gl_Position/gl_Position.w)+1.0 ) * 0.5);
}
