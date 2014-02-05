#include <pebble.h>
#include "TransBitmapLayer.h"
#include "TransRotBitmapLayer.h"
#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 168
#define PONY_WIDTH 61
#define PONY_HEIGHT 114
#define PONY_X 48
#define PONY_Y 18

static Window *window;

static GBitmap *bg_image;
static GBitmap *pony_black_image;
static GBitmap *pony_white_image;
static GBitmap *hour_black_image;
static GBitmap *hour_white_image;
static GBitmap *minute_black_image;
static GBitmap *minute_white_image;

static BitmapLayer *bg_layer;
static TransBitmapLayer *pony_layer;
static TransRotBitmapLayer *hour_layer;
static TransRotBitmapLayer *minute_layer;

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
    trans_rot_bitmap_layer_set_angle(hour_layer, hour_angle);
    trans_rot_bitmap_layer_set_angle(minute_layer, minute_angle);
}

static void init(void)
{
    // Create window
    window = window_create();
    window_stack_push(window, true);
    
    // Load images
    bg_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG);
    pony_black_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PONY_BLACK);
    pony_white_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PONY_WHITE);
    hour_black_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HOUR_BLACK);
    hour_white_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HOUR_WHITE);
    minute_black_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MINUTE_BLACK);
    minute_white_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MINUTE_WHITE);
    
    // Create bg layer
    bg_layer = bitmap_layer_create(GRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    bitmap_layer_set_bitmap(bg_layer, bg_image);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bg_layer));
    
    // Create minute_layer
    minute_layer = trans_rot_bitmap_layer_create(minute_black_image, minute_white_image);
    trans_rot_bitmap_layer_set_frame(minute_layer, GRect(-39, -27, 221, 221));
    trans_rot_bitmap_layer_add_child(bitmap_layer_get_layer(bg_layer), minute_layer);
    
    // Create pony layer
    pony_layer = trans_bitmap_layer_create(pony_black_image, pony_white_image, GRect(PONY_X, PONY_Y, PONY_WIDTH, SCREEN_WIDTH));
    //trans_rot_bitmap_layer_set_frame(pony_layer, GRect(-39, -27, 221, 221));
    trans_bitmap_layer_add_child(bitmap_layer_get_layer(bg_layer), pony_layer);
    
    // Create hour layer
    hour_layer = trans_rot_bitmap_layer_create(hour_black_image, hour_white_image);
    trans_rot_bitmap_layer_set_frame(hour_layer, GRect(-39, -27, 221, 221));
    trans_rot_bitmap_layer_add_child(bitmap_layer_get_layer(bg_layer), hour_layer);
    
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    handle_tick(current_time, MINUTE_UNIT);
    // Subscribe minute handler
    tick_timer_service_subscribe(MINUTE_UNIT, &handle_tick);
}

static void deinit(void)
{
    trans_rot_bitmap_layer_destroy(hour_layer);
    trans_bitmap_layer_destroy(pony_layer);
    trans_rot_bitmap_layer_destroy(minute_layer);
    bitmap_layer_destroy(bg_layer);
    
    gbitmap_destroy(minute_white_image);
    gbitmap_destroy(minute_black_image);
    gbitmap_destroy(hour_white_image);
    gbitmap_destroy(hour_black_image);
    gbitmap_destroy(pony_white_image);
    gbitmap_destroy(pony_black_image);
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
