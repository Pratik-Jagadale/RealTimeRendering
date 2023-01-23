//
//  main.m
//  Window
//
//  Created by user226085 on 1/25/23.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

int main(int argc, char* argv[])
{
    // Code
    
    // Create AutoreleasePool for reference counting for memory de-allocation
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];
    
    NSString* delegateClassName = NSStringFromClass([AppDelegate class]);
    
    int result = UIApplicationMain(argc, argv, nil, delegateClassName);
    
    // Tell AutoreleasePool to release all objects created by this application
    [pool release];
    
    return (result);
}
