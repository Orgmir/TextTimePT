#include <pebble.h>
  
#define BUFFER_SIZE 10
  
static GFont font,fontBold;
  
static Window *s_main_window;
static TextLayer *s_line1;
static TextLayer *s_line2;
static TextLayer *s_line3;

static const char* HOURS[] = {
  "zero",
  "uma",
  "duas",
  "três",
  "quatro",
  "cinco",
  "seis",
  "sete",
  "oito",
  "nove",
  "dez",
  "onze",
  "doze",
  "treze",
  "catorze",
  "quinze",
  "dezasseis",
  "dezassete",
  "dezoito",
  "dezanove"
};

static const char* MIN[] = {
  "em ponto",
  "um",
  "dois",
  "três",
  "quatro",
  "cinco",
  "seis",
  "sete",
  "oito",
  "nove",
  "dez",
  "onze",
  "doze",
  "treze",
  "catorze",
  "quinze",
  "dezasseis",
  "dezassete",
  "dezoito",
  "dezanove"
};


static const char* TENS_NUMBERS[] = {
  "dez",
  "vinte",
  "trinta",
  "quarenta",
  "cinquenta",
  "sessenta"
};

static char row_1_buffer[BUFFER_SIZE];
static char row_2_buffer[BUFFER_SIZE];
static char row_3_buffer[BUFFER_SIZE];

static void initialize_text_layer(TextLayer *text_layer, GFont fontToUse){
  text_layer_set_background_color(text_layer, GColorBlack);
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_font(text_layer, fontToUse);
  text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
  
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(text_layer));
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  memset(row_1_buffer, 0, BUFFER_SIZE);
  memcpy(row_1_buffer, HOURS[tick_time->tm_hour], BUFFER_SIZE);
  
  memset(row_2_buffer, 0, BUFFER_SIZE);
  memcpy(row_2_buffer, MIN[tick_time->tm_min], BUFFER_SIZE);
  
  text_layer_set_text(s_line1, row_1_buffer);
  text_layer_set_text(s_line2, row_2_buffer);
}

static void main_window_load(Window *window){
  
  s_line1 = text_layer_create(GRect(0, 50, 144, 50));
  initialize_text_layer(s_line1, fontBold);
  s_line2 = text_layer_create(GRect(0, 70, 144, 50));
  initialize_text_layer(s_line2, font);
}

static void main_window_unload(Window *window){
  text_layer_destroy(s_line1);
  text_layer_destroy(s_line2);
  text_layer_destroy(s_line3);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  fontBold = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SANSATION_BOLD_30));
  font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SANSATION_LIGHT_28));

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  window_set_fullscreen(s_main_window, true);
  window_set_background_color(s_main_window, GColorBlack);
  
  update_time();
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
