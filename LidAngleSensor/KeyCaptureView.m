//
//  KeyCaptureView.m
//  LidAngleSensor
//
//  Created by Vedaant Rajeshirke on 9/27/25.
//


#import "KeyCaptureView.h"

@implementation KeyCaptureView

// 1. This view must be able to become the "first responder" to receive key events.
- (BOOL)acceptsFirstResponder {
    return YES;
}

// 2. This method is called when a key is pressed. By implementing it, we "handle"
//    the event, which stops the system from making the alert sound.
- (void)keyDown:(NSEvent *)event {
    // Pass the event to our delegate (the AppDelegate) to handle the logic.
    [self.delegate keyCaptureView:self didReceiveKeyDown:event];
}

// This method is called when a key is released.
- (void)keyUp:(NSEvent *)event {
    // Pass the event to our delegate to handle the fade-out logic.
    [self.delegate keyCaptureView:self didReceiveKeyUp:event];
}

- (void)scrollWheel:(NSEvent *)event {
    // Forward the scrolling delta to the delegate
    if ([self.delegate respondsToSelector:@selector(keyCaptureView:didScrollWithDelta:)]) {
        [self.delegate keyCaptureView:self didScrollWithDelta:event.scrollingDeltaY];
    }
}

@end
