#include <pebble.h>
#include "TransBitmapLayer.h"

TransBitmapLayer * trans_bitmap_layer_create(GBitmap *black_image, GBitmap *white_image, GRect frame)
{
    TransBitmapLayer *layer;
    layer = malloc(sizeof(TransBitmapLayer));
    layer->black_layer = bitmap_layer_create(frame);
    layer->white_layer = bitmap_layer_create(frame);
    bitmap_layer_set_bitmap(layer->black_layer, black_image);
    bitmap_layer_set_bitmap(layer->white_layer, white_image);
    bitmap_layer_set_compositing_mode(layer->black_layer, GCompOpAnd);
    bitmap_layer_set_compositing_mode(layer->white_layer, GCompOpSet);
    return layer;
}

void trans_bitmap_layer_destroy(TransBitmapLayer *layer)
{
    bitmap_layer_destroy(layer->white_layer);
    bitmap_layer_destroy(layer->black_layer);
    free(layer);
}

void trans_bitmap_layer_set_frame(TransBitmapLayer *layer, GRect frame)
{
    layer_set_frame(bitmap_layer_get_layer(layer->black_layer), frame);
    layer_set_frame(bitmap_layer_get_layer(layer->white_layer), frame);
}

void trans_bitmap_layer_add_child(Layer *parent, TransBitmapLayer *layer)
{   
    layer_add_child(parent, bitmap_layer_get_layer(layer->black_layer));
    layer_add_child(parent, bitmap_layer_get_layer(layer->white_layer));
}