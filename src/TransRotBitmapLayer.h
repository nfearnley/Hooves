typedef struct
{
    RotBitmapLayer *black_layer;
    RotBitmapLayer *white_layer;    
} TransRotBitmapLayer;

TransRotBitmapLayer * trans_rot_bitmap_layer_create(GBitmap *black_image, GBitmap *white_image);
void trans_rot_bitmap_layer_destroy(TransRotBitmapLayer *layer);
void trans_rot_bitmap_layer_set_frame(TransRotBitmapLayer *layer, GRect frame);
void trans_rot_bitmap_layer_add_child(Layer *parent, TransRotBitmapLayer *layer);
void trans_rot_bitmap_layer_set_angle(TransRotBitmapLayer *layer, int32_t angle);
