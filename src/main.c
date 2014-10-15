#include <pebble.h>
  
#define MIN_BUFFER_SIZE 9
#define HOUR_BUFFER_SIZE 6
  
#define BUFFER_SIZE 44
  
static GFont font;
static GFont fontBold;
  
static Window *s_main_window;
static TextLayer *s_line1;
static TextLayer *s_line2;
static TextLayer *s_line3;

static const char* HOURS[] = {
  "doze",
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
  "onze"
};

static const char* MIN[] = {
  "hora",
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

static const char* TENS_MIN[] = {
  "zero",
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

static int lastTenMinute;
static int lastHour;

static void initialize_text_layer(TextLayer *text_layer, GFont fontToUse){
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_font(text_layer, fontToUse);
  text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
  text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);
  
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(text_layer));
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  int minute = tick_time->tm_min;
  int hour = tick_time->tm_hour;
  //int minute = 17;
  //int hour = 4;
  
  
  if(minute < 20){
    memset(row_2_buffer, 0, BUFFER_SIZE);
    strcat(row_2_buffer, MIN[minute]);
    text_layer_set_text(s_line2, row_2_buffer);
    
    if(!layer_get_hidden((Layer *)s_line3)){
      layer_set_hidden((Layer *)s_line3, true); 
    }
  }else{
    if(minute % 10 != 0){
      if(layer_get_hidden((Layer *)s_line3)){
        layer_set_hidden((Layer *)s_line3, false); 
      }    
      
      memset(row_3_buffer, 0, BUFFER_SIZE);
      strcat(row_3_buffer, MIN[minute % 10]);
      text_layer_set_text(s_line3, row_3_buffer);
    }else{
      if(!layer_get_hidden((Layer *)s_line3)){
        layer_set_hidden((Layer *)s_line3, true); 
      }
    }
    
    if(lastTenMinute != ((int) minute / 10)){
      memset(row_2_buffer, 0, BUFFER_SIZE);
      strcat(row_2_buffer, TENS_MIN[(int) minute/10]);
      text_layer_set_text(s_line2, row_2_buffer);  
    }
  }
  lastTenMinute = (int) minute / 10;
  
  if(lastHour == -1 || lastHour != hour){
    memset(row_1_buffer, 0, BUFFER_SIZE);
    strcat(row_1_buffer, HOURS[hour]);
    text_layer_set_text(s_line1, row_1_buffer);  
  }
  lastHour = hour;
  
}

static void main_window_load(Window *window){
  s_line1 = text_layer_create(GRect(0, 22, 144, 50));
  initialize_text_layer(s_line1, fontBold);
  s_line2 = text_layer_create(GRect(0, 59, 144, 50));
  initialize_text_layer(s_line2, font);
  s_line3 = text_layer_create(GRect(0, 93, 144, 50));
  initialize_text_layer(s_line3, font);
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
  
  fontBold = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
  font = fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT);
  
  lastHour = -1;
  lastTenMinute = -1;

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
