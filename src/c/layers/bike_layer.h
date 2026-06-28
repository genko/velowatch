#pragma once

#include <pebble.h>

void bike_layer_create(Layer *parent_layer, GRect frame);

void bike_layer_refresh();

void bike_layer_set_hidden(bool hidden);

void bike_layer_destroy();