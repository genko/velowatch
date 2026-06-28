#include "bike_layer.h"

#define IMAGE_SIZE_X 200
#define IMAGE_SIZE_Y 100

static Layer *s_bike_layer;
static BitmapLayer *s_bike_image_layer;
static GBitmap *s_bike_image;

void bike_layer_create(Layer *parent_layer, GRect frame) {
    s_bike_layer = layer_create(frame);

    // Load image once
    s_bike_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BIKE);

    // Position image centered at the bottom
    GRect bounds = layer_get_bounds(s_bike_layer);

    int x = (bounds.size.w - IMAGE_SIZE_X) / 2;
    int y = bounds.size.h - IMAGE_SIZE_Y;

    s_bike_image_layer = bitmap_layer_create(
        GRect(x, y, IMAGE_SIZE_X, IMAGE_SIZE_Y));

    bitmap_layer_set_bitmap(s_bike_image_layer, s_bike_image);
    bitmap_layer_set_compositing_mode(s_bike_image_layer, GCompOpSet);

    layer_add_child(
        s_bike_layer,
        bitmap_layer_get_layer(s_bike_image_layer));

    layer_add_child(parent_layer, s_bike_layer);
}

void bike_layer_refresh(void) {
    // Only needed if you later change the bitmap
    layer_mark_dirty(s_bike_layer);
}

void bike_layer_set_hidden(bool hidden) {
    if (s_bike_layer) {
        layer_set_hidden(s_bike_layer, hidden);
    }
}

void bike_layer_destroy(void) {
    bitmap_layer_destroy(s_bike_image_layer);
    gbitmap_destroy(s_bike_image);
    layer_destroy(s_bike_layer);
}
