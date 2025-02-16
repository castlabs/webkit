/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "config.h"
#import "TestsController.h"

extern "C" void _BeginEventReceiptOnThread(void);

int main(int argc, char** argv)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    [[NSUserDefaults standardUserDefaults] removePersistentDomainForName:@"TestWebKitAPI"];

    // Set a user default for TestWebKitAPI to bypass all linked-on-or-after checks in WebKit
    NSMutableDictionary *argumentDomain = [[[NSUserDefaults standardUserDefaults] volatileDomainForName:NSArgumentDomain] mutableCopy];
    if (!argumentDomain)
        argumentDomain = [[NSMutableDictionary alloc] init];
    
    NSDictionary *dict = @{ @"WebKitLinkedOnOrAfterEverything": @YES };

    [argumentDomain addEntriesFromDictionary:dict];
    [[NSUserDefaults standardUserDefaults] setVolatileDomain:argumentDomain forName:NSArgumentDomain];

    [NSApplication sharedApplication];
    _BeginEventReceiptOnThread(); // Makes window visibility notifications work (and possibly more).

    bool passed = TestWebKitAPI::TestsController::singleton().run(argc, argv);

    [pool drain];

    return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
