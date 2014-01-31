#include <pebble.h>

#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 168

static Window *window;

static GBitmap *bg_image;
static GBitmap *hand_black_image;
static GBitmap *hand_white_image;

static BitmapLayer *bg_layer;
static RotBitmapLayer *hour_black_layer;
static RotBitmapLayer *hour_white_layer;
static RotBitmapLayer *minute_black_layer;
static RotBitmapLayer *minute_white_layer;

static Window *window;

static int get_hour_angle(int hour, int minute)
{
    int minutes_past_midnight = (hour * 60) + minute;
    return minutes_past_midnight * 65536 / (12 * 60);
}

static int get_minute_angle(int minute)
{
    return minute * 65536 / 60;
}

// Called once per second
static void handle_tick(struct tm *tick_time, TimeUnits units_changed)
{
    int hour_angle = get_hour_angle(tick_time->tm_hour, tick_time->tm_min);
    int minute_angle = get_minute_angle(tick_time->tm_min);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Time: %d:%d", tick_time->tm_hour, tick_time->tm_min);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "HourAngle: %d", hour_angle);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "MinuteAngle: %d", minute_angle);
    rot_bitmap_layer_set_angle(hour_black_layer, hour_angle);
    rot_bitmap_layer_set_angle(hour_white_layer, hour_angle);
    rot_bitmap_layer_set_angle(minute_black_layer, minute_angle);
    rot_bitmap_layer_set_angle(minute_white_layer, minute_angle);
}

static void init(void)
{
    // Create window
    window = window_create();
    window_stack_push(window, true);
    
    // Load images
    bg_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG);
    hand_black_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HAND_BLACK);
    hand_white_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HAND_WHITE);
    
    // Create bg layer
    bg_layer = bitmap_layer_create(GRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    bitmap_layer_set_bitmap(bg_layer, bg_image);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bg_layer));
    
    // Create hand layers
    hour_black_layer = rot_bitmap_layer_create(hand_black_image);
    hour_white_layer = rot_bitmap_layer_create(hand_white_image);
    layer_set_frame(bitmap_layer_get_layer((BitmapLayer*)hour_black_layer), GRect(-39, -27, 221, 221));
    layer_set_frame(bitmap_layer_get_layer((BitmapLayer*)hour_white_layer), GRect(-39, -27, 221, 221));
    rot_bitmap_set_compositing_mode(hour_black_layer, GCompOpAnd);
    rot_bitmap_set_compositing_mode(hour_white_layer, GCompOpSet);
    layer_add_child(bitmap_layer_get_layer(bg_layer), bitmap_layer_get_layer((BitmapLayer*)hour_black_layer));
    layer_add_child(bitmap_layer_get_layer(bg_layer), bitmap_layer_get_layer((BitmapLayer*)hour_white_layer));
    
    minute_black_layer = rot_bitmap_layer_create(hand_black_image);
    minute_white_layer = rot_bitmap_layer_create(hand_white_image);
    layer_set_frame(bitmap_layer_get_layer((BitmapLayer*)minute_black_layer), GRect(-39, -27, 221, 221));
    layer_set_frame(bitmap_layer_get_layer((BitmapLayer*)minute_white_layer), GRect(-39, -27, 221, 221));
    rot_bitmap_set_compositing_mode(minute_black_layer, GCompOpAnd);
    rot_bitmap_set_compositing_mode(minute_white_layer, GCompOpSet);
    layer_add_child(bitmap_layer_get_layer(bg_layer), bitmap_layer_get_layer((BitmapLayer*)minute_black_layer));
    layer_add_child(bitmap_layer_get_layer(bg_layer), bitmap_layer_get_layer((BitmapLayer*)minute_white_layer));
    
    // Subscribe minute handler
    tick_timer_service_subscribe(SECOND_UNIT, &handle_tick);
}

static void deinit(void)
{
    rot_bitmap_layer_destroy(minute_white_layer);
    rot_bitmap_layer_destroy(minute_black_layer);
    rot_bitmap_layer_destroy(hour_white_layer);
    rot_bitmap_layer_destroy(hour_black_layer);
    bitmap_layer_destroy(bg_layer);
    
    gbitmap_destroy(hand_white_image);
    gbitmap_destroy(hand_black_image);
    gbitmap_destroy(bg_image);
    
    window_destroy(window);
}

int main(void)
{
    init();
  
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
  
    app_event_loop();
    deinit();
}
