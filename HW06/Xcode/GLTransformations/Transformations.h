//
//  Transformations.h
//  GLTransformations
//
//  Created by RRC on 9/8/13.
//  Copyright (c) 2013 Ricardo Rendon Cepeda. All rights reserved.
//

#import <GLKit/GLKit.h>

@interface Transformations : NSObject

typedef enum TransformationState
{
    S_NEW,
    S_SCALE,
    S_TRANSLATION,
    S_ROTATION
}
TransformationState;

@property (readwrite) TransformationState state;

- (id)initWithDepth:(float)z Scale:(float)s Translation:(GLKVector2)t Rotation:(GLKVector3)r;
- (void)start;
- (void)scale:(float)s;
- (void)translate:(GLKVector2)t withMultiplier:(float)m;
- (void)rotate:(GLKVector3)r withMultiplier:(float)m;
- (GLKMatrix4)getModelViewMatrix;

@end
