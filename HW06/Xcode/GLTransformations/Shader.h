

#import <GLKit/GLKit.h>

@interface Shader : NSObject

- (GLuint)BuildProgram:(const char*)vertexShaderSource with:(const char*)fragmentShaderSource;

@end
