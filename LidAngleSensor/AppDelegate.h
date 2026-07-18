// AppDelegate.h

#import <Cocoa/Cocoa.h>
#import "HarmoniumAudioEngine.h"
#import "NSLabel.h"
#import "LidAngleSensor.h"
#import "KeyCaptureView.h"

typedef NS_ENUM(NSInteger, NoteNamingMode) {
    NoteNamingModeWestern,
    NoteNamingModeSargam
};

@interface AppDelegate : NSObject <NSApplicationDelegate, KeyCaptureViewDelegate>

// Main Window
@property (strong) NSWindow *window;

// Lid Sensor & Timer
@property (strong) LidAngleSensor *lidSensor;
@property (strong) NSTimer *updateTimer;

@property (nonatomic, assign) double lastLidAngle;
@property (nonatomic, assign) double lastUpdateTime;
@property (nonatomic, assign) double airPressure;


// Harmonium Audio Engine
@property (strong) HarmoniumAudioEngine *harmoniumEngine;

// UI Elements
@property (strong) NSLabel *titleLabel;
@property (strong) NSLabel *angleLabel; // For showing the angle
@property (strong) NSLabel *scaleLabel;
@property (strong) NSPopUpButton *scalePopUpButton;
@property (strong) NSLabel *notationLabel;
@property (strong) NSPopUpButton *namingModePopUpButton;
@property (strong) NSLabel *legendLabel; // For the key map legend
@property (strong) NSLabel *instructionsLabel;

// NEW: UI Elements for the air pressure toggle
@property (strong) NSLabel *airPressureToggleLabel;
@property (strong) NSSwitch *airPressureToggle;

// State Management
@property (strong) NSArray<NSString *> *availableScales;
@property (strong) NSDictionary<NSString *, NSArray<NSNumber *> *> *scaleNoteMapping;
@property (strong) NSString *currentScale;
@property (nonatomic, assign) NoteNamingMode currentNamingMode;

@end
