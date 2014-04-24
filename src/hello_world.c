#include "pebble.h"

static Window *window;

static TextLayer *lattitude_layer;
static TextLayer *longitude_layer;
static TextLayer *tracking_layer;
static TextLayer *activity_layer;

static AppSync sync;
static uint8_t sync_buffer[64];

enum TripchainKey {
  TRIPCHAIN_LATTITUDE_KEY = 0, //TUPLE_CSTRING
  TRIPCHAIN_LONGITUDE_KEY = 1,
  TRIPCHAIN_TRACKING_KEY = 2, // TUPLE_CSTRING
  TRIPCHAIN_ACTIVITY_KEY = 3,
 
};

char *translate_error(AppMessageResult result) {
  switch (result) {
    case APP_MSG_OK: return "APP_MSG_OK";
    case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
    case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
    case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
    case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
    case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
    case APP_MSG_BUSY: return "APP_MSG_BUSY";
    case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
    case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
    case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
    case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
    case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
    case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
    default: return "UNKNOWN ERROR";
  }
}

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "In dropped: %i - %s", app_message_error, translate_error(app_message_error));
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {

    case TRIPCHAIN_LATTITUDE_KEY:
      // App Sync keeps new_tuple in sync_buffer, so we may use it directly
      text_layer_set_text(lattitude_layer, new_tuple->value->cstring);
      break;
    
    case TRIPCHAIN_LONGITUDE_KEY:
      text_layer_set_text(longitude_layer, new_tuple->value->cstring);
      break;

    case TRIPCHAIN_TRACKING_KEY:
      text_layer_set_text(tracking_layer, new_tuple->value->cstring);
      break;
    
    case TRIPCHAIN_ACTIVITY_KEY:
      text_layer_set_text(activity_layer, new_tuple->value->cstring);
      break;
    
  }
}

static void send_cmd(void) {
  Tuplet value = TupletInteger(1, 1);

  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (iter == NULL) {
    return;
  }

  dict_write_tuplet(iter, &value);
  dict_write_end(iter);

  app_message_outbox_send();
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  lattitude_layer = text_layer_create(GRect(0, 15, 144, 68));
  text_layer_set_text_color(lattitude_layer, GColorWhite);
  text_layer_set_background_color(lattitude_layer, GColorClear);
  text_layer_set_font(lattitude_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(lattitude_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(lattitude_layer));

  longitude_layer = text_layer_create(GRect(0, 35, 144, 68));
  text_layer_set_text_color(longitude_layer, GColorWhite);
  text_layer_set_background_color(longitude_layer, GColorClear);
  text_layer_set_font(longitude_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(longitude_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(longitude_layer));
  
  tracking_layer = text_layer_create(GRect(0, 65, 144, 68));
  text_layer_set_text_color(tracking_layer, GColorWhite);
  text_layer_set_background_color(tracking_layer, GColorClear);
  text_layer_set_font(tracking_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(tracking_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(tracking_layer));
  
  activity_layer = text_layer_create(GRect(0, 95, 144, 68));
  text_layer_set_text_color(activity_layer, GColorWhite);
  text_layer_set_background_color(activity_layer, GColorClear);
  text_layer_set_font(activity_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(activity_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(activity_layer));

  Tuplet initial_values[] = {
    TupletCString(TRIPCHAIN_LATTITUDE_KEY, "60.1833\u00B0 N"),
    TupletCString(TRIPCHAIN_LONGITUDE_KEY, "24.8333\u00B0 E"),
    TupletCString(TRIPCHAIN_TRACKING_KEY, "Not tracking"),
    TupletCString(TRIPCHAIN_ACTIVITY_KEY, "default"),
    
    
  };

  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

  send_cmd();
}

static void window_unload(Window *window) {
  app_sync_deinit(&sync);

  text_layer_destroy(lattitude_layer);
  text_layer_destroy(longitude_layer);
  text_layer_destroy(tracking_layer);
  text_layer_destroy(activity_layer); 
}

static void init(void) {
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_set_fullscreen(window, true);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });

  const int inbound_size = 128;
  const int outbound_size = 128;
  app_message_open(inbound_size, outbound_size);

  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
