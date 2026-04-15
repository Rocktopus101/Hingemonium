//
//  AppDelegate.m
//  LidAngleSensor
//
//  Created by Sam on 2025-09-06.
//  Modified by Vedaant on 2025-09-27
//

#import "AppDelegate.h"

// Maps keyboard characters to a base chromatic scale (C3 = 48)
static const int kKeyToMidiNote[] = {
    ['z'] = 48, ['s'] = 49, ['x'] = 50, ['d'] = 51, ['c'] = 52, ['v'] = 53,
    ['g'] = 54, ['b'] = 55, ['h'] = 56, ['n'] = 57, ['j'] = 58, ['m'] = 59,
    [','] = 60, ['l'] = 61, ['.'] = 62
};

@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // --- Window and Custom View Setup ---
    self.window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 520, 420)
                                                  styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable)
                                                    backing:NSBackingStoreBuffered
                                                      defer:NO];

    // Create and set our custom view as the window's content view
    KeyCaptureView *keyView = [[KeyCaptureView alloc] initWithFrame:self.window.frame];
    keyView.delegate = self; // Set the delegate to self
    self.window.contentView = keyView;

    [self.window setTitle:@"Hingemonium "];
    [self.window center];
    [self.window makeKeyAndOrderFront:nil];
    [self.window makeFirstResponder:keyView];

    // --- Component Initialization ---
    self.harmoniumEngine = [[HarmoniumAudioEngine alloc] init];
    self.lidSensor = [[LidAngleSensor alloc] init];
    self.currentNamingMode = NoteNamingModeWestern;
    self.lastLidAngle = -1.0;
    self.lastUpdateTime = CACurrentMediaTime();
    self.airPressure = 0.0; // Start with no air
    
    [self.harmoniumEngine startEngine];

    // --- UI and Scale Setup ---
    [self setupUI];
    [self setupScales];
    [self updateLegend]; // Initial legend update

    // --- Start Update Timer ---
    self.updateTimer = [NSTimer scheduledTimerWithTimeInterval:0.02
                                                        target:self
                                                      selector:@selector(update)
                                                      userInfo:nil
                                                       repeats:YES];
}

- (void)keyCaptureView:(KeyCaptureView *)view didReceiveKeyDown:(NSEvent *)event {
    if (event.isARepeat) return;
    
    NSString *keyStr = event.charactersIgnoringModifiers.lowercaseString;
    if (keyStr.length == 0) return;
    
    char character = [keyStr characterAtIndex:0];
    int midiNote = [self getMidiNoteForKey:character];
    
    if (midiNote > 0) {
        [self.harmoniumEngine playNote:midiNote];
    }
}

- (void)keyCaptureView:(KeyCaptureView *)view didReceiveKeyUp:(NSEvent *)event {
    NSString *keyStr = event.charactersIgnoringModifiers.lowercaseString;
    if (keyStr.length == 0) return;
    
    char character = [keyStr characterAtIndex:0];
    int midiNote = [self getMidiNoteForKey:character];
    
    if (midiNote > 0) {
        [self.harmoniumEngine releaseNote:midiNote];
    }
}


- (void)setupUI {
    NSView *contentView = self.window.contentView;

    // --- Create UI Elements ---
    self.titleLabel = [[NSLabel alloc] init];
    self.titleLabel.stringValue = @"Harmonium for Mac";
    [self.titleLabel setFont:[NSFont boldSystemFontOfSize:24]];

    self.angleLabel = [[NSLabel alloc] init];
    [self.angleLabel setStringValue:@"--°"];
    [self.angleLabel setFont:[NSFont monospacedDigitSystemFontOfSize:18 weight:NSFontWeightRegular]];
    [self.angleLabel setTextColor:[NSColor secondaryLabelColor]];

    self.scaleLabel = [[NSLabel alloc] init];
    self.scaleLabel.stringValue = @"Select Scale:";
    
    self.scalePopUpButton = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(0, 0, 200, 25) pullsDown:NO];
    [self.scalePopUpButton setTarget:self];
    [self.scalePopUpButton setAction:@selector(scaleDidChange:)];
    [self.scalePopUpButton setTranslatesAutoresizingMaskIntoConstraints:NO];

    self.notationLabel = [[NSLabel alloc] init];
    self.notationLabel.stringValue = @"Notation:";
        
    self.namingModePopUpButton = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(0, 0, 200, 25) pullsDown:NO];
    [self.namingModePopUpButton addItemsWithTitles:@[@"Western (C, D, E)", @"Sargam (Sa, Re, Ga)"]];
    [self.namingModePopUpButton setTarget:self];
    [self.namingModePopUpButton setAction:@selector(namingModeDidChange:)];
    [self.namingModePopUpButton setTranslatesAutoresizingMaskIntoConstraints:NO];
    
    // NEW: Create the toggle switch and its label
    self.airPressureToggleLabel = [[NSLabel alloc] init];
    self.airPressureToggleLabel.stringValue = @"Max Air:";

    self.airPressureToggle = [[NSSwitch alloc] init];
    [self.airPressureToggle setTarget:self];
    [self.airPressureToggle setAction:@selector(toggleDidChange:)];
    [self.airPressureToggle setTranslatesAutoresizingMaskIntoConstraints:NO];

    self.legendLabel = [[NSLabel alloc] init];
    [self.legendLabel setFont:[NSFont monospacedSystemFontOfSize:12 weight:NSFontWeightRegular]];
    self.legendLabel.stringValue = @"-";
    
    self.instructionsLabel = [[NSLabel alloc] init];
    self.instructionsLabel.stringValue = @"Move the lid to pump the bellows or use the 'Max Air' toggle.";
    self.instructionsLabel.alignment = NSTextAlignmentCenter;
    
    // Add ALL views to the content view
    for (NSView *view in @[self.titleLabel, self.angleLabel, self.scaleLabel, self.scalePopUpButton, self.notationLabel, self.namingModePopUpButton, self.airPressureToggleLabel, self.airPressureToggle, self.legendLabel, self.instructionsLabel]) {
        [contentView addSubview:view];
    }

    // --- Auto Layout ---
    for (NSView *view in @[self.titleLabel, self.angleLabel, self.scaleLabel, self.notationLabel, self.airPressureToggleLabel, self.legendLabel, self.instructionsLabel]) {
        [view setTranslatesAutoresizingMaskIntoConstraints:NO];
    }

    [NSLayoutConstraint activateConstraints:@[
        [self.titleLabel.centerXAnchor constraintEqualToAnchor:contentView.centerXAnchor],
        [self.titleLabel.topAnchor constraintEqualToAnchor:contentView.topAnchor constant:20],
        
        [self.angleLabel.centerXAnchor constraintEqualToAnchor:contentView.centerXAnchor],
        [self.angleLabel.topAnchor constraintEqualToAnchor:self.titleLabel.bottomAnchor constant:8],

        [self.scaleLabel.topAnchor constraintEqualToAnchor:self.angleLabel.bottomAnchor constant:25],
        [self.scaleLabel.centerXAnchor constraintEqualToAnchor:contentView.centerXAnchor constant:-90],
        
        [self.scalePopUpButton.centerYAnchor constraintEqualToAnchor:self.scaleLabel.centerYAnchor],
        [self.scalePopUpButton.leadingAnchor constraintEqualToAnchor:self.scaleLabel.trailingAnchor constant:10],
        
        [self.notationLabel.topAnchor constraintEqualToAnchor:self.scaleLabel.bottomAnchor constant:15],
        [self.notationLabel.trailingAnchor constraintEqualToAnchor:self.scaleLabel.trailingAnchor],
        
        [self.namingModePopUpButton.centerYAnchor constraintEqualToAnchor:self.notationLabel.centerYAnchor],
        [self.namingModePopUpButton.leadingAnchor constraintEqualToAnchor:self.scalePopUpButton.leadingAnchor],
        [self.namingModePopUpButton.trailingAnchor constraintEqualToAnchor:self.scalePopUpButton.trailingAnchor],

        // NEW: Constraints for the toggle switch row
        [self.airPressureToggleLabel.topAnchor constraintEqualToAnchor:self.notationLabel.bottomAnchor constant:15],
        [self.airPressureToggleLabel.trailingAnchor constraintEqualToAnchor:self.notationLabel.trailingAnchor],
        [self.airPressureToggle.centerYAnchor constraintEqualToAnchor:self.airPressureToggleLabel.centerYAnchor],
        [self.airPressureToggle.leadingAnchor constraintEqualToAnchor:self.namingModePopUpButton.leadingAnchor],

        // Adjust legend to be below the new toggle row
        [self.legendLabel.topAnchor constraintEqualToAnchor:self.airPressureToggleLabel.bottomAnchor constant:24],
        [self.legendLabel.centerXAnchor constraintEqualToAnchor:contentView.centerXAnchor],

        [self.instructionsLabel.centerXAnchor constraintEqualToAnchor:contentView.centerXAnchor],
        [self.instructionsLabel.bottomAnchor constraintEqualToAnchor:contentView.bottomAnchor constant:-20]
    ]];
    [self.window setFrame:NSMakeRect(0, 0, 680, 420) display:YES];
    [self.window center];
}

// NEW: Action method for the toggle switch
- (void)toggleDidChange:(NSSwitch *)sender {
    // The main logic is handled in the update loop.
    // This just ensures the UI updates instantly when toggled.
    [self update];
}

- (void)update {
    if (!self.lidSensor.isAvailable && self.airPressureToggle.state == NSControlStateValueOff) {
        self.angleLabel.stringValue = @"Sensor N/A";
        // Set air pressure to 0 if sensor is unavailable and toggle is off
        self.airPressure = 0.0;
        [self.harmoniumEngine updateVolume:(float)self.airPressure];
        return;
    }
    
    double angle = [self.lidSensor lidAngle];
    double currentTime = CACurrentMediaTime();
    
    // Calculate deltaTime for fade processing (always needed)
    double deltaTime = currentTime - self.lastUpdateTime;
    if (deltaTime <= 0 || deltaTime > 0.5) {
        deltaTime = 0.02; // Use a reasonable default if deltaTime is invalid
    }
    
    // --- 2. Update Air Pressure Model ---
    if (self.airPressureToggle.state == NSControlStateValueOn) {
        // If the toggle is ON, force air pressure to 100%
        self.airPressure = 1.0;
        // Update lastUpdateTime so deltaTime calculation is correct next frame
        self.lastUpdateTime = currentTime;
    } else {
        // Otherwise, use the existing lid sensor logic
        if (angle < 0) {
            self.angleLabel.stringValue = @"Read Error";
            // Still update lastUpdateTime to prevent deltaTime issues
            self.lastUpdateTime = currentTime;
            return;
        }

        if (self.lastLidAngle < 0) {
            self.lastLidAngle = angle;
            self.lastUpdateTime = currentTime;
            return;
        }

        double lidDeltaTime = currentTime - self.lastUpdateTime;
        if (lidDeltaTime <= 0 || lidDeltaTime > 0.5) {
            self.lastLidAngle = angle;
            self.lastUpdateTime = currentTime;
            return;
        }

        double instantVelocity = fabs(angle - self.lastLidAngle) / lidDeltaTime;
        double pumpFactor = 0.015;
        self.airPressure += instantVelocity * pumpFactor;
        
        double decayRate = 0.5;
        self.airPressure -= decayRate * lidDeltaTime;
        self.airPressure = fmax(0.0, fmin(1.0, self.airPressure));

        // Save state for the next frame
        self.lastLidAngle = angle;
        self.lastUpdateTime = currentTime;
    }

    [self.harmoniumEngine processFadesWithDeltaTime:deltaTime];

    // --- 3. Update UI and Audio Engine ---
    self.angleLabel.stringValue = [NSString stringWithFormat:@"Angle: %.1f° | Air Pressure: %.0f%%", angle, self.airPressure * 100];
    [self.harmoniumEngine updateVolume:(float)self.airPressure];
}
- (void)namingModeDidChange:(NSPopUpButton *)sender {
    self.currentNamingMode = (NoteNamingMode)sender.indexOfSelectedItem;
    [self updateLegend];
}

- (void)setupScales {
    // Bilaval is the same as the Western Major scale
    // Kafi is similar to the Dorian mode
    // Bhairavi uses all flat (komal) notes except Sa and Pa
    self.availableScales = @[@"Chromatic", @"Major / Bilaval Thaat", @"Natural Minor", @"Kafi Thaat", @"Bhairavi Thaat", @"Minor Pentatonic"];
    self.scaleNoteMapping = @{
        @"Chromatic":             @[@0, @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11],
        @"Major / Bilaval Thaat": @[@0, @2, @4, @5, @7, @9, @11], // Sa, Re, Ga, Ma, Pa, Dha, Ni
        @"Natural Minor":         @[@0, @2, @3, @5, @7, @8, @10],
        @"Kafi Thaat":            @[@0, @2, @3, @5, @7, @9, @10], // Sa, Re, ga, Ma, Pa, Dha, ni
        @"Bhairavi Thaat":        @[@0, @1, @3, @5, @7, @8, @10], // Sa, re, ga, Ma, Pa, dha, ni
        @"Minor Pentatonic":      @[@0, @3, @5, @7, @10]
    };
    self.mappedKeys = @[@'z', @'s', @'x', @'d', @'c', @'v', @'g', @'b', @'h', @'n', @'j', @'m', @',', @'l', @'.'];

    [self.scalePopUpButton addItemsWithTitles:self.availableScales];
    self.currentScale = self.availableScales[0];
}


- (void)scaleDidChange:(NSPopUpButton *)sender {
    self.currentScale = sender.selectedItem.title;
    [self updateLegend];
}

- (void)updateLegend {
    NSMutableString *legendText = [NSMutableString string];
    [legendText appendString:@"Keyboard Mapping:\n\n"];
    
    for (int i = 0; i < self.mappedKeys.count; i++) {
        char key = [self.mappedKeys[i] charValue];
        int midiNote = [self getMidiNoteForKey:key];
        NSString *noteName = [self noteNameForMidi:midiNote];
        
        // Format as "Z: C3 " with padding
        [legendText appendFormat:@"%c: %-4s", toupper(key), [noteName UTF8String]];
        
        // Add a newline after every 5 keys for readability
        if ((i + 1) % 5 == 0) {
            [legendText appendString:@"\n"];
        }
    }
    self.legendLabel.stringValue = legendText;
}

- (int)getMidiNoteForKey:(char)key {
    int baseNote = kKeyToMidiNote[key];
    if (baseNote == 0) return -1;
    
    int finalNote = baseNote;
    if (![self.currentScale isEqualToString:@"Chromatic"]) {
        NSArray<NSNumber *> *scaleIntervals = self.scaleNoteMapping[self.currentScale];
        int rootNote = 48; // C3
        
        int closestNoteInScale = -1;
        int minDistance = 100;
        
        for (int octave = -1; octave <= 2; octave++) {
            for (NSNumber *interval in scaleIntervals) {
                int noteInScale = rootNote + interval.intValue + (octave * 12);
                int distance = abs(noteInScale - baseNote);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestNoteInScale = noteInScale;
                }
            }
        }
        finalNote = closestNoteInScale;
    }
    return finalNote;
}
- (NSString *)noteNameForMidi:(int)midiNote {
    if (midiNote < 0) return @"-";

    if (self.currentNamingMode == NoteNamingModeSargam) {
        // --- Sargam Naming Logic ---
        // Let's assume C is our tonic (Sa)
        int rootNoteC = 48;
        int interval = (midiNote - rootNoteC) % 12;
        if (interval < 0) interval += 12;

        // Lowercase for Komal (flat), Uppercase for Shuddha (natural) or Tivra (sharp)
        NSArray<NSString *> *sargamNames = @[
            @"Sa", @"re", @"Re", @"ga", @"Ga", @"Ma", @"MA", @"Pa", @"dha", @"Dha", @"ni", @"Ni"
        ];
        NSString *note = sargamNames[interval];

        // Add octave dots (Saptak)
        int octave = midiNote / 12;
        if (octave < 4) { // Mandra Saptak (Lower)
            return [NSString stringWithFormat:@"%@̣", note]; // Dot below
        } else if (octave > 4) { // Taar Saptak (Higher)
            return [NSString stringWithFormat:@"%@̇", note]; // Dot above
        } else { // Madhya Saptak (Middle)
            return note;
        }

    } else {
        // --- Western Naming Logic (Original) ---
        NSArray<NSString *> *names = @[@"C", @"C#", @"D", @"D#", @"E", @"F", @"F#", @"G", @"G#", @"A", @"A#", @"B"];
        int octave = (midiNote / 12) - 1;
        NSString *note = names[midiNote % 12];
        return [NSString stringWithFormat:@"%@%d", note, octave];
    }
}


- (BOOL)handleKeyDown:(NSEvent *)event {
    if (event.isARepeat) return NO;
    
    NSString *keyStr = event.charactersIgnoringModifiers.lowercaseString;
    if (keyStr.length == 0) return NO;
    
    char character = [keyStr characterAtIndex:0];
    int midiNote = [self getMidiNoteForKey:character];
    
    if (midiNote > 0) {
        [self.harmoniumEngine playNote:midiNote];
        return YES; // We handled it, suppress the beep!
    }
    
    return NO; // Not a key we handle
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    [self.updateTimer invalidate];
    [self.harmoniumEngine stopEngine];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

@end
