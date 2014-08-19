#import "BTAppSwitchHandler.h"

#import "BTPayPalAppSwitchHandler.h"
#import "BTPayPalPaymentMethod.h"
#import "BTVenmoAppSwitchReturnURL.h"

@interface BTAppSwitchHandler () <BTPayPalAppSwitchHandlerDelegate>

@end

@implementation BTAppSwitchHandler

+ (instancetype)sharedHandler {
    static BTAppSwitchHandler *instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[BTAppSwitchHandler alloc] init];
    });
    return instance;
}

- (void)setAppSwitchCallbackURLScheme:(NSString *)appSwitchCallbackURLScheme {
    [BTPayPalAppSwitchHandler sharedHandler].appSwitchCallbackURLScheme = appSwitchCallbackURLScheme;
}

- (NSString *)appSwitchCallbackURLScheme {
    return [BTPayPalAppSwitchHandler sharedHandler].appSwitchCallbackURLScheme;
}

- (BTClient *)client {
    return [BTPayPalAppSwitchHandler sharedHandler].client;
}

- (void)setDelegate:(id<BTAppSwitchHandlerDelegate>)delegate {
    _delegate = delegate;
    [BTPayPalAppSwitchHandler sharedHandler].delegate = self;
}

- (BOOL)initiateAuthWithClient:(BTClient *)client delegate:(id<BTAppSwitchHandlerDelegate>)delegate {
    BOOL success = [[BTPayPalAppSwitchHandler sharedHandler] initiatePayPalAuthWithClient:client delegate:self];
    if (success) {
        self.delegate = delegate;
    }
    return success;
}

- (BOOL)handleAppSwitchURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication {
    if ([BTVenmoAppSwitchReturnURL isValidURL:url sourceApplication:sourceApplication]) {
        BTVenmoAppSwitchReturnURL *venmoReturnURL = [[BTVenmoAppSwitchReturnURL alloc] initWithURL:url];

        switch (venmoReturnURL.state) {
            case BTVenmoAppSwitchReturnURLStateSucceeded: {
                BTPaymentMethod *venmoPaymentMethod = venmoReturnURL.paymentMethod;
                [self.delegate appSwitchHandler:self didCreatePaymentMethod:venmoPaymentMethod];
                break;
            }
            default:
                @throw [NSException exceptionWithName:NSInternalInconsistencyException reason:@"Not implemented" userInfo:nil];
        }

    } else {
        return [[BTPayPalAppSwitchHandler sharedHandler] handleAppSwitchURL:url sourceApplication:sourceApplication];
    }
    return NO;
}


#pragma mark PayPalAppSwitchHandler delegate methods

- (void)payPalAppSwitchHandlerWillCreatePayPalPaymentMethod:(__unused BTPayPalAppSwitchHandler *)appSwitchHandler {
    [self.delegate appSwitchHandlerWillCreatePaymentMethod:self];
}

- (void)payPalAppSwitchHandler:(__unused BTPayPalAppSwitchHandler *)appSwitchHandler didCreatePayPalPaymentMethod:(BTPayPalPaymentMethod *)paymentMethod {
    [self.delegate appSwitchHandler:self didCreatePaymentMethod:paymentMethod];
}

- (void)payPalAppSwitchHandler:(__unused BTPayPalAppSwitchHandler *)appSwitchHandler didFailWithError:(NSError *)error {
    [self.delegate appSwitchHandler:self didFailWithError:error];
}

- (void)payPalAppSwitchHandlerAuthenticatorAppDidCancel:(__unused BTPayPalAppSwitchHandler *)appSwitchHandler {
    [self.delegate appSwitchHandlerAuthenticatorAppDidCancel:self];
}

@end