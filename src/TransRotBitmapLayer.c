#include <pebble.h>
#include "TransRotBitmapLayer.h"

TransRotBitmapLayer * trans_rot_bitmap_layer_create(GBitmap *black_image, GBitmap *white_image)
{
    TransRotBitmapLayer *layer;
    layer = malloc(sizeof(TransRotBitmapLayer));
    layer->black_layer = rot_bitmap_layer_create(black_image);
    layer->white_layer = rot_bitmap_layer_create(white_image);
    rot_bitmap_set_compositing_mode(layer->black_layer, GCompOpAnd);
    rot_bitmap_set_compositing_mode(layer->white_layer, GCompOpSet);
    return layer;
}

void trans_rot_bitmap_layer_destroy(TransRotBitmapLayer *layer)
{
    rot_bitmap_layer_destroy(layer->white_layer);
    rot_bitmap_layer_destroy(layer->black_layer);
    free(layer);
}

void trans_rot_bitmap_layer_set_frame(TransRotBitmapLayer *layer, GRect frame)
{
    layer_set_frame(bitmap_layer_get_layer((BitmapLayer*)layer->black_layer), frame);
    layer_set_frame(bitmap_layer_get_layer((BitmapLayer*)layer->white_layer), frame);
}

void trans_rot_bitmap_layer_add_child(Layer *parent, TransRotBitmapLayer *layer)
{   
    layer_add_child(parent, bitmap_layer_get_layer((BitmapLayer*)layer->black_layer));
    layer_add_child(parent, bitmap_layer_get_layer((BitmapLayer*)layer->white_layer));
}

void trans_rot_bitmap_layer_set_angle(TransRotBitmapLayer *layer, int32_t angle)
{
    rot_bitmap_layer_set_angle(layer->black_layer, angle);
    rot_bitmap_layer_set_angle(layer->white_layer, angle);
}