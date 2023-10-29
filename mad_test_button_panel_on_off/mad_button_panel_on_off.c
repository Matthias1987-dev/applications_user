#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/widget.h>
#include <gui/modules/variable_item_list.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

#include <stdlib.h>
#include <input/input.h>
#include "mad_test_button_panel_on_off_icons.h"
#include "mad_test_button_panel_on_off.h"

#define TAG "SubGhz Remote"

// Change this to BACKLIGHT_AUTO if you don't want the backlight to be continuously on.
#define BACKLIGHT_Auto

// Our application menu has 3 items.  You can add more items if you want.
typedef enum {
    SubGhzRemoteSubmenuIndexPlay,
    SubGhzRemoteSubmenuIndexConfigure,
    SubGhzRemoteSubmenuIndexAbout,
} SubGhzRemoteSubmenuIndex;

// Each view is a screen we show the user.
typedef enum {
    SubGhzRemoteViewTextInput, // Input for configuring text settings
    SubGhzRemoteViewSubmenu, // The menu when the app starts
    SubGhzRemoteViewConfigure, // The configuration screen
    SubGhzRemoteViewPlay, // The main screen
    SubGhzRemoteViewAbout, // The about screen with directions, link to social channel, etc.
} SubGhzRemoteView;

// Not use for the moment.
/**
 * typedef enum {
    SubGhzRemoteEventIdRedrawScreen = 0, // Custom event to redraw the screen
    SubGhzRemoteEventIdOkPressed = 42, // Custom event to process OK button getting pressed down
} SubGhzRemoteEventId;
*/

typedef struct {
    ViewDispatcher* view_dispatcher; // Switches between our views
    NotificationApp* notifications; // Used for controlling the backlight
    Submenu* submenu; // The application menu
    TextInput* text_input; // The text input screen
    VariableItemList* variable_item_list_config; // The configuration screen
    View* view_play; // The main screen
    Widget* widget_about; // The about screen

    VariableItem* setting_2_item; // The name setting item (so we can update the text)
    char* temp_buffer; // Temporary buffer for text input
    uint32_t temp_buffer_size; // Size of temporary buffer

    FuriTimer* timer; // Timer for redrawing the screen
} SubGhzRemoteApp;

typedef struct {
    uint32_t setting_1_index; // The team color setting index
    FuriString* setting_2_name; // The name setting
    uint8_t x; // The x coordinate
} SubGhzRemotePlayModel;

/**
 * @brief      Callback for exiting the application.
 * @details    This function is called when user press back button.  We return VIEW_NONE to
 *            indicate that we want to exit the application.
 * @param      _context  The context - unused
 * @return     next view id
*/
static uint32_t subghzremote_navigation_exit_callback(void* _context) {
    UNUSED(_context);
    return VIEW_NONE;
}

/**
 * @brief      Callback for returning to submenu.
 * @details    This function is called when user press back button.  We return VIEW_NONE to
 *            indicate that we want to navigate to the submenu.
 * @param      _context  The context - unused
 * @return     next view id
*/
static uint32_t subghzremote_navigation_submenu_callback(void* _context) {
    UNUSED(_context);
    return SubGhzRemoteViewSubmenu;
}

/**
 * @brief      Callback for returning to configure screen.
 * @details    This function is called when user press back button.  We return VIEW_NONE to
 *            indicate that we want to navigate to the configure screen.
 * @param      _context  The context - unused
 * @return     next view id
*/
static uint32_t subghzremote_navigation_configure_callback(void* _context) {
    UNUSED(_context);
    return SubGhzRemoteViewConfigure;
}

/**
 * @brief      Handle submenu item selection.
 * @details    This function is called when user selects an item from the submenu.
 * @param      context  The context - SkeletonApp object.
 * @param      index     The SkeletonSubmenuIndex item that was clicked.
*/
static void subghzremote_submenu_callback(void* context, uint32_t index) {
    SubGhzRemoteApp* app = (SubGhzRemoteApp*)context;
    switch(index) {
    case SubGhzRemoteSubmenuIndexPlay:
        view_dispatcher_switch_to_view(app->view_dispatcher, SubGhzRemoteViewPlay);
        break;
    case SubGhzRemoteSubmenuIndexConfigure:
        view_dispatcher_switch_to_view(app->view_dispatcher, SubGhzRemoteViewConfigure);
        break;
    case SubGhzRemoteSubmenuIndexAbout:
        view_dispatcher_switch_to_view(app->view_dispatcher, SubGhzRemoteViewAbout);
        break;
    default:
        break;
    }
}
