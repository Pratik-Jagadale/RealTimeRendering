//
//  AppDelegate.m
//  Window
//
//  Created by user226085 on 1/25/23.
//

#import "AppDelegate.h"
#import <UIKit/UIKit.h>
#import "ViewController.h"
#import "GLESView.h"

@implementation AppDelegate
{
    @private
    UIWindow* window;
    UIViewController* viewController;
    GLESView* view;
}

-(BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    // Code
    // Create a window
    window = [UIWindow new];
    
    // Set bkg color of window
    [window setBackgroundColor:[UIColor blackColor] ];
    
    //create viewController of window
    viewController = [[ViewController alloc]init];
    [window setRootViewController:viewController];
    
    CGRect rect =  [[window screen]bounds]; //CGRect rect = window.screen.bounds;
    view = [[GLESView alloc]initWithFrame:rect];
    [viewController setView:view];
    
    // Reference counting
    [view release];
    
    // Make the window key and visible
    [window makeKeyAndVisible];
    
    [view startAnimation];
    
    return YES;
}

-(void)ApplicationWillResignActive:(UIApplication*)application
{
    // Code
    [view stopAnimation];
}

-(void)ApplicationDidEnterBackground:(UIApplication*)application
{
    // Code
}

-(void)ApplicationWillEnterForeground:(UIApplication*)application
{
    // Code
}

-(void)ApplicationDidBecomeActive:(UIApplication*)application
{
    // Code
    [view startAnimation];
}

-(void)ApplicationWillTerminate:(UIApplication*)application
{
    // Code
    [view stopAnimation];
}

-(void)dealloc
{
    // Code
    
    if(view)
    {
        [view release];
        view = nil;
    }
    
    if(viewController)
    {
        [viewController release];
        viewController = nil;
    }
    
    if(window)
    {
        [window release];
        window = nil;
    }
    
    [super dealloc];
}

@end
