
#import "PhongShader.h"

@interface Model : NSObject

typedef enum MyModels
{
    M_CUBE,
    M_STARSHIP,
    M_LAMP
}
MyModels;

@property (strong, nonatomic) PhongShader* shader;

- (id)initWithModel:(MyModels)m;
- (void)loadModel:(MyModels)m;
- (void)renderModel;
- (void)setMatricesProjection:(GLKMatrix4)p ModelView:(GLKMatrix4)mv Normal:(GLKMatrix3)n;

@end
