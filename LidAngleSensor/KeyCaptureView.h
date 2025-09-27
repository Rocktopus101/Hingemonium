//
//  KeyCaptureView.h
//  LidAngleSensor
//
//  Created by Vedaant Rajeshirke on 9/27/25.
//


//
//  KeyCaptureView.h
//  LidAngleSensor
//

#import <Cocoa/Cocoa.h>

@class KeyCaptureView;

// A protocol to delegate key events back to the controller (our AppDelegate)
@protocol KeyCaptureViewDelegate <NSObject>
- (void)keyCaptureView:(KeyCaptureView *)view didReceiveKeyDown:(NSEvent *)event;
@end

@interface KeyCaptureView : NSView

@property (nonatomic, weak) id<KeyCaptureViewDelegate> delegate;

@end