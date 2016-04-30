//
//  ViewController.h
//  Simple Books
//
//  Created by Allen Zhao on 4/28/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#import <GLKit/GLKit.h>
#import <CoreMotion/CoreMotion.h>
#import "Macro.h"
#import "TouchMsgCenter.h"
#import "HSDatePickerViewController.h"
#import <CoreData/CoreData.h>

extern UIView* MainView;

@interface ViewController : GLKViewController<UITextFieldDelegate, HSDatePickerViewControllerDelegate, NSFetchedResultsControllerDelegate>
{
    @public
}

@property (strong, nonatomic) GLKBaseEffect *baseEffect;
@property (strong, nonatomic) CMMotionManager *motionManager;
@property (strong, nonatomic) NSOperationQueue *queue;
@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (strong, nonatomic) NSMutableArray* NSFetchedResultsControllers;

- (NSFetchedResultsController *)GetfetchedResultsController:(NSString*)EntityName sortDescriptor:(NSString*) sortDescriptorName Section:(NSString*)SectionName Cache:(NSString*)CacheName;

@end
extern ViewController* VC;

