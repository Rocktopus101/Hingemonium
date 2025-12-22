//
//  KeyCaptureView.h
//  LidAngleSensor
//
//  Created by Vedaant Rajeshirke on 9/27/25.
//

#import <Cocoa/Cocoa.h>

@class KeyCaptureView;

// A protocol to delegate key events back to the controller (our AppDelegate)
@protocol KeyCaptureViewDelegate <NSObject>
- (void)keyCaptureView:(KeyCaptureView *)view didReceiveKeyDown:(NSEvent *)event;
- (void)keyCaptureView:(KeyCaptureView *)view didReceiveKeyUp:(NSEvent *)event;
- (void)keyCaptureView:(KeyCaptureView *)view didScrollWithDelta:(CGFloat)deltaY;
@end

@interface KeyCaptureView : NSView

@property (nonatomic, weak) id<KeyCaptureViewDelegate> delegate;

@end
