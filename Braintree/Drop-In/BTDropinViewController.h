#import <UIKit/UIKit.h>
#import "Braintree-API.h"

@class BTUI;
@protocol BTDropInViewControllerDelegate;

/// A view controller that provides a quick and easy payment experience.
///
/// When initialized with a Braintree client, the Drop In will prompt a user for payment details,
/// based on your Gatweay configuration. The Drop In payment form supports cards and PayPal. When
/// using Drop In, you don't need to worry about which methods are already on file with Braintree;
/// newly created methods are saved as part of the Drop In flow as needed.
///
/// Upon successful form submission, you will receive a payment method nonce, which you can
/// transact with on your server. Client and validation errors are handled internally by Drop In;
/// other types of Errors are rare and generally irrecoverable.
///
/// The Drop In view controller delegates presentation and dismissal to the developer. It has been
/// most thoroughly tested in the context of a UINavigationController.
///
/// The Drop In can send success and failure messages to the developer via a block
/// or a delegate. See `paymentMethodCompletionBlock` and `delegate`.
///
/// You can customize Drop In in various ways, for example, you can change the primary Call To
/// Action button text. For visual customzation options see `theme` and `BTUI`. Like any
/// UIViewController, you can setup properties like `title` or `navigationBar.rightBarButtonItem`.
@interface BTDropInViewController : UIViewController

/// Initialize a new Drop In.
///
/// @param client a client token that has been initialized with a client token
///
/// @return A new Drop In view controller that is ready to be presented.
- (instancetype)initWithClient:(BTClient *)client;

/// The Braintree client used internally for communication with the Gateway. This property is exposed
/// to enable the use of other UIViewController initializers, for example, when using Storyboards.
///
/// @see BTClient
@property (nonatomic, strong) BTClient *client;

#pragma mark State Change Notifications

/// The delgate that, if set, is notified of success or failure.
@property (nonatomic, weak) id<BTDropInViewControllerDelegate> delegate;

/// Functionally equivalent to the `BTDropInViewControllerDelegate` above, you may also register a
/// single block callback for success and failure notifications.
///
/// @note The block (and thus all objects it retains) will be retained for the lifespan of this
/// view controller instance.
@property (nonatomic, strong) void (^paymentMethodCompletionBlock)(BTPaymentMethod *paymentMethod, NSError *error);


#pragma mark Customization

/// The presentation theme to use for the Drop In.
@property (nonatomic, strong) BTUI *theme;

/// Primary text to display in the summary view.
///
/// Intended to provide a name the overall transaction taking place. For example, "1 Item", "1 Year Subscription", "Yellow T-Shirt", etc.
///
/// If summaryTitle or summaryDescription are nil, then the summary view is not shown.
@property (nonatomic, copy) NSString *summaryTitle;

/// Detail text to display in the summary view.
///
/// Intended to provide a few words of detail. For example, "Ships in Five Days", "15 feet by 12 feet" or "We know you'll love it"
///
/// If summaryTitle or summaryDescription are nil, then the summary view is not shown.
@property (nonatomic, copy) NSString *summaryDescription;

/// A string representation of the grand total amount
///
/// For example, "$12.95"
@property (nonatomic, copy) NSString *displayAmount;

/// The text to display in the primary call-to-action button. For example: "$19 - Purchase" or "Subscribe Now".
@property (nonatomic, copy) NSString *callToActionText;

/// Whether or not to display payment methods on file. If false, Drop-In will always
/// display the new payment method form.
///
/// Defaults to `YES`.
@property (nonatomic, assign) BOOL shouldDisplayPaymentMethodsOnFile;

/// Whether to hide the call to action control.
///
/// When true, a submit button will be added as a bar button item (which
/// relies on the drop-in view controller being embedded in a navigation controller.
///
/// Defaults to `NO`.
///
/// @see callToAction
/// @see callToActionAmount
@property (nonatomic, assign) BOOL shouldHideCallToAction;


@end

/// A protocol for BTDropInViewController completion notifications.
@protocol BTDropInViewControllerDelegate <NSObject>

/// Informs the delegate when the user has successfully provided a payment method.
///
/// @param viewController The Drop In view controller informing its delegate of success
/// @param paymentMethod The selected (and possibly newly created) payment method.
- (void)dropInViewController:(BTDropInViewController *)viewController didSucceedWithPaymentMethod:(BTPaymentMethod *)paymentMethod;

/// Informs the delegate when the Drop In itself has failed. Normally, Drop In handles its own error
/// cases, so errors sent to this method are generally irrecoverable.
///
///  @param viewController The Drop In view controller informing its delegate of failure.
///  @param error An error that describes the failure.
- (void)dropInViewController:(BTDropInViewController *)viewController didFailWithError:(NSError *)error;

@optional

/// Informs the delegate when the user has entered or selected payment information.
///
/// @param viewController The Drop In view controller informing its delegate
- (void)dropInViewControllerWillComplete:(BTDropInViewController *)viewController;

@end