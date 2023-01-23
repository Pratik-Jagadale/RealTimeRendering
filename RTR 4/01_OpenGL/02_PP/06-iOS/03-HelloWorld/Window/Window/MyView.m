//
//  MyView.m
//  Window
//
//  Created by user226085 on 1/25/23.
//

#import "MyView.h"

@implementation MyView
{
    @private
    NSString* string;
}

-(id)initWithFrame:(CGRect)frame
{
    // Code
    
    self = [super initWithFrame:frame];
    
    if(self)
    {
        string = @"Hello World !!!";
        
        // Gesture recognizer code
        
        // Single Tap
        UITapGestureRecognizer* singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        // Double Tap
        UITapGestureRecognizer* doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        // Differentiate between single tap and double tap
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        // Swipe
        UISwipeGestureRecognizer *swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector (onSwipe:)];
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        // LongPress
        UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector (onLongPress:)];
        [self addGestureRecognizer:longPressGestureRecognizer];
        
    }
    
    return self;
}

// Similar to WM_PAINT
-(void)drawRect:(CGRect)dirtyRect
{
    // Code
    UIColor* backgroundColor = [UIColor blackColor];
    [backgroundColor set];
    UIRectFill(dirtyRect);
    
    UIFont* font = [UIFont fontWithName:@"Helvetica" size:24];
    UIColor* textColor = [UIColor greenColor];
    NSDictionary* dictionary = [NSDictionary dictionaryWithObjectsAndKeys:font,NSFontAttributeName, textColor,NSForegroundColorAttributeName, nil];
    CGSize textSize = [string sizeWithAttributes:dictionary];
    
    CGPoint point;
    point.x = dirtyRect.size.width/2 - textSize.width/2;
    point.y = dirtyRect.size.height/2 - textSize.height/2;
    
    [string drawAtPoint:point withAttributes:dictionary];
}

-(BOOL)acceptsFirstResponder
{
    return YES;
}

-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    // Code
}

-(void)onSingleTap:(UITapGestureRecognizer*)tgr
{
    // Code
    string = @"Single Tap";
    [self setNeedsDisplay];
}

-(void)onDoubleTap:(UITapGestureRecognizer*)tgr
{
    // Code
    string = @"Double Tap";
    [self setNeedsDisplay];
}

-(void)onSwipe:(UISwipeGestureRecognizer*)stgr
{
    // Code
    
    [self release];
    exit(0);
}

-(void)onLongPress:(UILongPressGestureRecognizer*)lptgr
{
    // Code
    string = @"Long Press";
    [self setNeedsDisplay];
    
}

-(void)dealloc
{
    // Code
    
    [super dealloc];
    
}

@end
