//
//  ViewController.m
//  Simple Books
//
//  Created by Allen Zhao on 4/28/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#import "ViewController.h"
#import "Enum.h"
#import "PolygonTouchResponder.h"
#import "ios_sys.h"
#import "math.h"
#import "memorypool.h"
#import "rendercenter.h"
#import "Global.h"
#import "PhysicalInterpolation.h"
#import "SimpleBooks.h"
#import "GraphicsProcessingCenter.h"
#import "GyroProcessingCenter.h"
#import "gyro.h"
#import "TouchMsgCenter.h"
#import "HSDatePickerViewController.h"
#import "AppDelegate.h"
#import "Global.h"
#include "avcodec.h"
UIView* MainView;
ViewController* VC;

class ConstantFieldModel;
@interface ViewController () <HSDatePickerViewControllerDelegate>
@end

@implementation ViewController

@synthesize baseEffect;
@synthesize motionManager;
@synthesize queue;

- (BOOL)shouldAutorotate
{
    // Automatic rotation is disabled.
    return NO;
}

-(void)single_finger_single_click_handle:(UITapGestureRecognizer *)recognizer{
    CGPoint point = [recognizer locationInView:self.view];
    point_2d tpoint = ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point.x, point.y));
    
    MsgCenter.single_finger_single_click(tpoint);
}

-(void)single_finger_double_click_handle:(UITapGestureRecognizer *)recognizer{
    CGPoint point = [recognizer locationInView:self.view];
    MsgCenter.single_finger_double_click(ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point.x, point.y)));
}

-(void)two_finger_single_click_handle:(UITapGestureRecognizer *)recognizer{
    CGPoint point1 = [recognizer locationOfTouch:0 inView:self.view];
    CGPoint point2 = [recognizer locationOfTouch:1 inView:self.view];
    MsgCenter.two_finger_single_click(
                                      ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point1.x, point1.y)),
                                      ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point2.x, point2.y)));
}

-(void)two_finger_double_click_handle:(UITapGestureRecognizer *)recognizer{
    CGPoint point1 = [recognizer locationOfTouch:0 inView:self.view];
    CGPoint point2 = [recognizer locationOfTouch:1 inView:self.view];
    MsgCenter.two_finger_double_click(
                                      ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point1.x, point1.y)),
                                      ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point2.x, point2.y)));
}

-(void)longPress_handle:(UILongPressGestureRecognizer *)recognizer{
    if (recognizer.state == UIGestureRecognizerStateBegan) {
        CGPoint point = [recognizer locationInView:self.view];
        return MsgCenter.long_press_began(ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point.x, point.y)));
    }
    if (recognizer.state == UIGestureRecognizerStateEnded) {
        CGPoint point = [recognizer locationInView:self.view];
        return MsgCenter.long_press_end(ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point.x, point.y)));
    }
}

-(void)PinchGesture_handle:(UIPinchGestureRecognizer *)recognizer{
    if(recognizer.state == UIGestureRecognizerStateBegan)
    {
        CGPoint point1 = [recognizer locationOfTouch:0 inView:self.view];
        CGPoint point2 = [recognizer locationOfTouch:1 inView:self.view];
        
        return MsgCenter.pinching(recognizer,
                           ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point1.x, point1.y)),
                           ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point2.x, point2.y)));
    }
    return MsgCenter.pinching(recognizer);
}

-(void)rotation_handle:(UIRotationGestureRecognizer *)recognizer{
    if(recognizer.state == UIGestureRecognizerStateBegan)
    {
        CGPoint point1 = [recognizer locationOfTouch:0 inView:self.view];
        CGPoint point2 = [recognizer locationOfTouch:1 inView:self.view];
        return MsgCenter.rotating(recognizer,
                                  ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point1.x, point1.y)),
                                  ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point2.x, point2.y)));
    }
    return MsgCenter.rotating(recognizer);
}

- (void)sysInit
{
    srand((unsigned)time(0));

    UITapGestureRecognizer* single_finger_single_click = [[UITapGestureRecognizer alloc] init];
    [single_finger_single_click addTarget:self action:@selector(single_finger_single_click_handle:)];
    [single_finger_single_click setNumberOfTapsRequired:1];
    [single_finger_single_click setNumberOfTouchesRequired:1];
    [self.view addGestureRecognizer:single_finger_single_click];
    
    UITapGestureRecognizer* single_finger_double_click = [[UITapGestureRecognizer alloc] init];
    [single_finger_double_click addTarget:self action:@selector(single_finger_double_click_handle:)];
    [single_finger_double_click setNumberOfTapsRequired:2];
    [single_finger_double_click setNumberOfTouchesRequired:1];
    [self.view addGestureRecognizer:single_finger_double_click];
    
    UITapGestureRecognizer* two_finger_single_click = [[UITapGestureRecognizer alloc] init];
    [two_finger_single_click addTarget:self action:@selector(two_finger_single_click_handle:)];
    [two_finger_single_click setNumberOfTapsRequired:1];
    [two_finger_single_click setNumberOfTouchesRequired:2];
    [self.view addGestureRecognizer:two_finger_single_click];
    
    UITapGestureRecognizer* two_finger_double_click = [[UITapGestureRecognizer alloc] init];
    [two_finger_double_click addTarget:self action:@selector(two_finger_double_click_handle:)];
    [two_finger_double_click setNumberOfTapsRequired:2];
    [two_finger_double_click setNumberOfTouchesRequired:2];
    [self.view addGestureRecognizer:two_finger_double_click];
    
    UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc]
                                                    initWithTarget:self action:@selector(longPress_handle:)];
    [self.view addGestureRecognizer:longPress];
    UIPinchGestureRecognizer* PinchGesture = [[UIPinchGestureRecognizer alloc]
                                              initWithTarget:self action:@selector(PinchGesture_handle:)];
    [self.view addGestureRecognizer:PinchGesture];
    
    UIRotationGestureRecognizer* rotation = [[UIRotationGestureRecognizer alloc]
                                             initWithTarget:self action:@selector(rotation_handle:)];
    [self.view addGestureRecognizer:rotation];
    
    AspectRatio = [[UIScreen mainScreen] bounds].size.width / [[UIScreen mainScreen] bounds].size.height;
    
    self.motionManager = [[CMMotionManager alloc] init];
    self.motionManager.accelerometerUpdateInterval = .01;
    self.motionManager.gyroUpdateInterval = .01;
    self.queue = [[NSOperationQueue alloc] init];
    self.motionManager.deviceMotionUpdateInterval = .01;
    [self.motionManager startDeviceMotionUpdatesToQueue:self.queue withHandler:^(CMDeviceMotion *motion, NSError *error) {
        GyroData::pinch = motion.attitude.pitch;
        GyroData::yaw =   motion.attitude.yaw;
        GyroData::roll =  motion.attitude.roll;
    }];
    
    MainView = self.view;
    VC = self;
    VC.managedObjectContext = App.managedObjectContext;
    VC.NSFetchedResultsControllers = [NSMutableArray arrayWithCapacity:5];
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* obj;
    for(obj in event.allTouches)
    {
        os_package osp;
        osp.touch = obj;
        osp.Type = _touchesBegan;
        MsgCenter.direct_event(osp);
    }
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* obj;
    for(obj in event.allTouches)
    {
        os_package osp;
        osp.touch = obj;
        osp.Type = _touchesMoved;
        MsgCenter.direct_event(osp);
    }
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* obj;
    for(obj in event.allTouches)
    {
        os_package osp;
        osp.touch = obj;
        osp.Type = _touchesEnded;
        MsgCenter.direct_event(osp);
    }
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* obj;
    for(obj in event.allTouches)
    {
        os_package osp;
        osp.touch = obj;
        osp.Type = _touchesCancelled;
        MsgCenter.direct_event(osp);
    }
}

- (void)dataInit
{
    pSimpleBooks = new SimpleBooks();
}

- (void)changeTimeAtTimedisplay
{
}

- (void)glInit
{
    GLKView *view = (GLKView *)self.view;
    NSAssert([view isKindOfClass:[GLKView class]],
             @"View controller's view is not a GLKView");

    view.context = [[EAGLContext alloc]
                    initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:view.context];
    
    self.baseEffect = [[GLKBaseEffect alloc] init];
    self.baseEffect.useConstantColor = GL_TRUE;
    self.baseEffect.constantColor = GLKVector4Make(
                                                   1.0f,
                                                   1.0f,
                                                   1.0f,
                                                   1.0f);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    self.preferredFramesPerSecond = 60;
}

-(BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
    textField.text = [textField.text substringWithRange:NSMakeRange(0, textField.text.length - 1)];
    return true;
}

-(BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
    textField.text = [textField.text stringByAppendingString:@"$"];
    pSimpleBooks->m_CurrentBillMoney = [textField.text floatValue];
    
    return true;
}

-(BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    NSCharacterSet *nonNumberSet = [[NSCharacterSet characterSetWithCharactersInString:@"0123456789."]invertedSet];
    
    // allow backspace
    if (range.length > 0 && [string length] == 0) {
        return YES;
    }
    // do not allow . at the beggining
    if (range.location == 0 && [string isEqualToString:@"."]) {
        return NO;
    }
    
    NSString *currentText = pSimpleBooks->Money->EditingNotes.text;  // currentField指的是当前确定的那个输入框
    if ([string isEqualToString:@"."]&&[currentText rangeOfString:@"." options:NSBackwardsSearch].length == 0) {
        
    }else if([string isEqualToString:@"."]&&[currentText rangeOfString:@"." options:NSBackwardsSearch].length== 1) {
        string = @"";
        //alreay has a decimal point
    }
    // set the text field value manually
    NSString *newValue = [[textField text] stringByReplacingCharactersInRange:range withString:string];
    newValue = [[newValue componentsSeparatedByCharactersInSet:nonNumberSet]componentsJoinedByString:@""];
    textField.text = newValue;
    // return NO because we're manually setting the value
    return NO;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self glInit];
    [self sysInit];
    [self dataInit];
}

- (void)update
{
    render.Update();

    _GyroPC_.Run();
    _gpc_.Run();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClear(GL_COLOR_BUFFER_BIT);

    render.Rendering();
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    
    // Make the view's context current
    GLKView *view = (GLKView *)self.view;
    [EAGLContext setCurrentContext:view.context];
    
    // Stop using the context created in -viewDidLoad
    ((GLKView *)self.view).context = nil;
    [EAGLContext setCurrentContext:nil];
    
    [self setMotionManager:nil];
}

#pragma mark - HSDatePickerViewControllerDelegate
- (void)hsDatePickerPickedDate:(NSDate *)date {
    IncreaseFrame();
    pSimpleBooks->UpdateTimeBlock(date);
    pSimpleBooks->m_HasBeenSetTimeManually = true;
}

//optional
- (void)hsDatePickerDidDismissWithQuitMethod:(HSDatePickerQuitMethod)method {
    IncreaseFrame();
    // NSLog(@"Picker did dismiss with %lu", method);
}

//optional
- (void)hsDatePickerWillDismissWithQuitMethod:(HSDatePickerQuitMethod)method {
    IncreaseFrame();
    // NSLog(@"Picker will dismiss with %lu", method);
}

- (NSFetchedResultsController *)GetfetchedResultsController:(NSString*)EntityName sortDescriptor:(NSString*) sortDescriptorName Section:(NSString*)SectionName Cache:(NSString*)CacheName
{
    int count = self.NSFetchedResultsControllers.count;
    for( int i = 0; i < count; ++i){
        NSFetchedResultsController* obj = [self.NSFetchedResultsControllers objectAtIndex:i];
        BOOL res = [obj.fetchRequest.entityName isEqualToString:EntityName];
        if (res == true) {
            return obj;
        }
    }
    
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    // Edit the entity name as appropriate.
    NSEntityDescription *entity = [NSEntityDescription entityForName:EntityName inManagedObjectContext:self.managedObjectContext];
    [fetchRequest setEntity:entity];
    
    // Set the batch size to a suitable number.
    [fetchRequest setFetchBatchSize:20];
    
    // Edit the sort key as appropriate.
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:sortDescriptorName ascending:NO];
    NSArray *sortDescriptors = @[sortDescriptor];
    
    [fetchRequest setSortDescriptors:sortDescriptors];
    
    // Edit the section name key path and cache name if appropriate.
    // nil for section name key path means "no sections",
    // while the @"dayStamp" allows us to group into sections based on days.
    
    NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:self.managedObjectContext sectionNameKeyPath:SectionName cacheName:CacheName];
    aFetchedResultsController.delegate = self;
    [self.NSFetchedResultsControllers addObject:aFetchedResultsController];
    NSError *error = nil;
    
    if (![aFetchedResultsController performFetch:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
    
    return aFetchedResultsController;
}


@end















