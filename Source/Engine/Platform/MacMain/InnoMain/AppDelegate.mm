//
//  AppDelegate.m
//  InnoMain
//
//  Created by zhangdoa on 14/04/2019.
//  Copyright © 2019 InnocenceEngine. All rights reserved.
//

#import "AppDelegate.h"
#import "MacWindowDelegate.h"

#import "../../../Common/InnoApplication.h"
#import "MacWindowSystemBridgeImpl.h"
#import "MTRenderingBackendBridgeImpl.h"

@implementation AppDelegate
MacWindowSystemBridgeImpl* m_macWindowSystemBridge;
MTRenderingBackendBridgeImpl* m_metalRenderingBackendBridge;
MacWindowDelegate* m_macWindowDelegate;
MetalDelegate* m_metalDelegate;

-(void) drawLoop:(NSTimer*) timer{
    if(![m_macWindowDelegate isAlive]){
        [m_macWindowDelegate close];
        InnoApplication::terminate();
        return;

    }
    if([m_macWindowDelegate isVisible]){
        if (!InnoApplication::update())
        {
            InnoApplication::terminate();
            return;
        }
    }
}

- (id)init
{
    m_macWindowDelegate = [MacWindowDelegate alloc];
    m_metalDelegate = [MetalDelegate alloc];
    
    m_macWindowSystemBridge = new MacWindowSystemBridgeImpl(m_macWindowDelegate, m_metalDelegate);
    m_metalRenderingBackendBridge = new MTRenderingBackendBridgeImpl(m_macWindowDelegate, m_metalDelegate);
        
    //Start the engine c++ code
    const char* l_args = "-renderer 4 -mode 0";
    if (!InnoApplication::setup(m_macWindowSystemBridge, m_metalRenderingBackendBridge, (char*)l_args))
    {
        return 0;
    }
    if (!InnoApplication::initialize())
    {
        return 0;
    }
    
    [NSTimer scheduledTimerWithTimeInterval:0.000001 target:self selector:@selector(drawLoop:) userInfo:nil repeats:YES];
    
    return self;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication
{
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification{
    InnoApplication::terminate();
    
    delete m_metalRenderingBackendBridge;
    delete m_macWindowSystemBridge;
    
    [m_macWindowDelegate applicationWillTerminate:aNotification];
}
@end