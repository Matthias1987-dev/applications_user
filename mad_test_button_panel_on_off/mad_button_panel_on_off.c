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
// This is the position of the image on the screen
// 1,1 is the first line left
// 2,1 is the first line right
// 1,2 is the second line left
// 2,2 is the second line right
// 1,3 is the third line left
// 2,3 is the third line right
// 1,4 is the fourth line left
// 2,4 is the fourth line right
//
//List of the x and y
//
//enter_1   x=1     Y=1
//enter_2   x=2     Y=1
//enter_3   x=1     y=2
//enter_4   x=2     y=2
//enter_5   x=1     y=3
//enter_6   x=2     y=3
//enter_7   x=1     y=4
//enter_8   x=2     y=4
//
*/

/**
my_draw_background is a function that draw the background of the application
greatet from me.
my_draw_enter_1 to my_draw_enter_8 are function that draw the background and
the icon of the button that is selected
greatet from me.
my_input_callback is a function that put the event in the queue
greatet from jamison.
mad_test_button_panel_on_off_app is the main function of the application
that call the other function
greatet from me.
*/

void my_draw_background(Canvas* canvas, void* context) {
    UNUSED(context);
    canvas_draw_icon(canvas, 12, 5, &I_Off_25x27);
    canvas_draw_icon(canvas, 12, 35, &I_On_25x27);
    canvas_draw_icon(canvas, 41, 5, &I_Off_25x27);
    canvas_draw_icon(canvas, 41, 35, &I_On_25x27);
    canvas_draw_icon(canvas, 70, 5, &I_Off_25x27);
    canvas_draw_icon(canvas, 70, 35, &I_On_25x27);
    canvas_draw_icon(canvas, 99, 5, &I_Off_25x27);
    canvas_draw_icon(canvas, 99, 35, &I_On_25x27);
}

void my_input_callback(InputEvent* event, void* context) {
    furi_message_queue_put((FuriMessageQueue*)context, event, FuriWaitForever);
}

void my_draw_enter_1(Canvas* canvas, void* context) {
    UNUSED(context);
    my_draw_background(canvas, context);
    canvas_draw_icon(canvas, 12, 5, &I_Off_hvr_25x27);
}

void my_draw_enter_2(Canvas* canvas, void* context) {
    UNUSED(context);
    my_draw_background(canvas, context);
    canvas_draw_icon(canvas, 12, 35, &I_On_hvr_25x27);
}

void my_draw_enter_3(Canvas* canvas, void* context) {
    UNUSED(context);
    my_draw_background(canvas, context);
    canvas_draw_icon(canvas, 41, 5, &I_Off_hvr_25x27);
}

void my_draw_enter_4(Canvas* canvas, void* context) {
    UNUSED(context);
    my_draw_background(canvas, context);
    canvas_draw_icon(canvas, 41, 35, &I_On_hvr_25x27);
}

void my_draw_enter_5(Canvas* canvas, void* context) {
    UNUSED(context);
    my_draw_background(canvas, context);
    canvas_draw_icon(canvas, 70, 5, &I_Off_hvr_25x27);
}

void my_draw_enter_6(Canvas* canvas, void* context) {
    UNUSED(context);
    my_draw_background(canvas, context);
    canvas_draw_icon(canvas, 70, 35, &I_On_hvr_25x27);
}

void my_draw_enter_7(Canvas* canvas, void* context) {
    UNUSED(context);
    my_draw_background(canvas, context);
    canvas_draw_icon(canvas, 99, 5, &I_Off_hvr_25x27);
}

void my_draw_enter_8(Canvas* canvas, void* context) {
    UNUSED(context);
    my_draw_background(canvas, context);
    canvas_draw_icon(canvas, 99, 35, &I_On_hvr_25x27);
}

int32_t mad_test_button_panel_on_off_app(void* p) {
    UNUSED(p);
    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    //Show directions to user.
    Gui* gui = furi_record_open(RECORD_GUI);
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, my_draw_background, NULL);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    return 0;
}