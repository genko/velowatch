#pragma once

#include <pebble.h>

void weather_status_layer_create(Layer *parent_layer, GRect frame);

void weather_status_layer_refresh();

void weather_status_layer_set_hidden(bool hidden);

void weather_status_layer_destroy();