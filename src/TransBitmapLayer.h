typedef struct
{
    BitmapLayer *black_layer;
    BitmapLayer *white_layer;    
} TransBitmapLayer;

TransBitmapLayer * trans_bitmap_layer_create(GBitmap *black_image, GBitmap *white_image, GRect frame);
void trans_bitmap_layer_destroy(TransBitmapLayer *layer);
void trans_bitmap_layer_set_frame(TransBitmapLayer *layer, GRect frame);
void trans_bitmap_layer_add_child(Layer *parent, TransBitmapLayer *layer);
