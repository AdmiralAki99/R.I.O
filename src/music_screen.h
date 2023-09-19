#include "parsecs.h"
#include <math.h>

#define LV_COLOR_BLACK  LV_COLOR_MAKE(0x00, 0x00, 0x00)
#define LV_COLOR_WHITE LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)
#define LV_COLOR_GREY LV_COLOR_MAKE(0x80, 0x80, 0x80)

/* Define the number of bars in the spectrum */
#define NUM_BARS 20

/* Define the height of the spectrum bars */
#define BAR_HEIGHT 60

/* Define the spacing between bars */
#define BAR_SPACING 10

/* Define the width of each bar */
#define BAR_WIDTH ((LV_HOR_RES - (NUM_BARS - 1) * BAR_SPACING) / NUM_BARS)

/* Define the maximum height of the spectrum bars */
#define MAX_BAR_HEIGHT (BAR_HEIGHT - 5)

/* Create a new style for the spectrum bars */
static lv_style_t bar_style;

/* Create an array to hold the bar objects */
static lv_obj_t *bars[NUM_BARS];

/* Create an array to hold the bar values */
static uint8_t bar_values[NUM_BARS];

lv_obj_t* music_screen = NULL;

static lv_obj_t* background_dial;
static lv_style_t backgorund_dial_style;

static lv_obj_t* play_button;
static lv_style_t play_button_style;
lv_obj_t* play_icon;

static lv_obj_t* forward_button;
static lv_style_t forward_button_style;

static lv_obj_t* rewind_button;
static lv_style_t rewind_button_style;

static lv_obj_t* song_label;
static lv_style_t song_label_style;
lv_obj_t* song_label_name;

static lv_obj_t* artist_label;
static lv_style_t artist_label_style;
lv_obj_t* artist_label_name;

static lv_timer_t* spectrum_task;
static lv_timer_t* play_button_task;
static lv_timer_t* update_song_name;

static char* title;
static char* artist;

static bool is_playing = false;

void initialize_music_bg_dial_style(lv_style_t* background_dial_style);
void initialize_music_bg_dial(lv_obj_t* dial,lv_style_t* dial_style,lv_obj_t* act_scr);
void initialize_music_play_button_style(lv_style_t* play_button_style);
void init_music_play_button(lv_obj_t* button,lv_style_t* button_style,lv_obj_t* act_scr);
void initialize_music_forward_button_style(lv_style_t* forward_button_style);
void init_music_forward_button(lv_obj_t* button,lv_style_t* button_style,lv_obj_t* act_scr);
void initialize_music_rewind_button_style(lv_style_t* rewind_button_style);
void init_music_rewind_button(lv_obj_t* button,lv_style_t* button_style,lv_obj_t* act_scr);
void initialize_music_song_label_style(lv_style_t* song_label_style);
void init_music_song_label(lv_obj_t* label,lv_style_t* label_style,lv_obj_t* act_scr);
void initialize_music_artist_label_style(lv_style_t* artist_label_style);
void init_music_artist_label(lv_obj_t* label,lv_style_t* label_style,lv_obj_t* act_scr);
void update_music_audio_spectrum(lv_timer_t *task);
void initialize_music_spectrum_style(lv_style_t* bar_style);
void init_spectrum_bars(lv_obj_t* bars[],lv_style_t* bar_style,lv_obj_t* act_scr);
void update_play_button_cb(lv_timer_t *task);
void play_button_cb(lv_event_t* event);
void update_song_label(lv_timer_t * timer);
void artist_label_cb(lv_event_t* event);
void forward_button_cb(lv_event_t* event);
void rewind_button_cb(lv_event_t* event);
void screen_event_cb(lv_event_t* event);


void init_music_screen(lv_obj_t* scr){
    lv_obj_set_style_bg_color(scr, LV_COLOR_BLACK, 0);
    music_screen = scr;
    // initialize_music_bg_dial_style(&backgorund_dial_style);
    // initialize_music_bg_dial(background_dial,&backgorund_dial_style,scr);

    initialize_music_play_button_style(&play_button_style);
    init_music_play_button(play_button,&play_button_style,scr);
    initialize_music_forward_button_style(&forward_button_style);
    init_music_forward_button(forward_button,&forward_button_style,scr);
    initialize_music_rewind_button_style(&rewind_button_style);
    init_music_rewind_button(rewind_button,&rewind_button_style,scr);
    initialize_music_song_label_style(&song_label_style);
    init_music_song_label(song_label,&song_label_style,scr);
    initialize_music_artist_label_style(&artist_label_style);
    init_music_artist_label(artist_label,&artist_label_style,scr);
    initialize_music_spectrum_style(&bar_style);
    init_spectrum_bars(bars,&bar_style,scr);
    play_button_task = lv_timer_create(update_play_button_cb, 10, NULL);
    lv_timer_ready(play_button_task);

    update_song_name = lv_timer_create(update_song_label, 20, NULL);
    lv_timer_ready(update_song_name);

    lv_obj_add_event_cb(scr,screen_event_cb,LV_EVENT_GESTURE,NULL);

}

void initialize_music_bg_dial_style(lv_style_t* background_dial_style){
    lv_style_init(&backgorund_dial_style);
    lv_style_set_bg_color(background_dial_style, LV_COLOR_BLACK);
    lv_style_set_border_width(background_dial_style, 0);
    lv_style_set_outline_width(background_dial_style,0);
}

void initialize_music_bg_dial(lv_obj_t* dial,lv_style_t* dial_style,lv_obj_t* act_scr){
    dial = lv_btn_create(act_scr);
    lv_obj_set_size(dial,240,240);
    lv_obj_align(dial,LV_ALIGN_CENTER,0,0);
    lv_obj_add_style(dial,dial_style,0);
    lv_obj_set_style_bg_color(dial,LV_COLOR_BLACK,0);
}

void initialize_music_play_button_style(lv_style_t* play_button_style){
    lv_style_init(play_button_style);
    lv_style_set_bg_color(play_button_style,LV_COLOR_BLACK);
    lv_style_set_radius(play_button_style,LV_RADIUS_CIRCLE);
    lv_style_set_border_width(play_button_style, 2);
    lv_style_set_outline_width(play_button_style, 0);
    lv_style_set_border_color(play_button_style,LV_COLOR_WHITE );
}

void init_music_play_button(lv_obj_t* play_button,lv_style_t* play_button_style,lv_obj_t* act_scr){
    play_button = lv_btn_create(act_scr);
    lv_obj_set_size(play_button,40,40);
    lv_obj_align(play_button,LV_ALIGN_CENTER,0,60);
    lv_obj_add_style(play_button,play_button_style,0);
    lv_obj_set_style_bg_opa(play_button,0,0);
    lv_obj_add_event_cb(play_button,play_button_cb,LV_EVENT_CLICKED,NULL);
    if(is_playing){
        play_icon = lv_label_create(play_button);
        lv_label_set_text(play_icon,LV_SYMBOL_PAUSE);
        lv_obj_set_style_text_color(play_icon,LV_COLOR_WHITE,0);
        lv_obj_align(play_icon,LV_ALIGN_CENTER,0,0);
    }else{
        play_icon = lv_label_create(play_button);
        lv_label_set_text(play_icon,LV_SYMBOL_PLAY);
        lv_obj_set_style_text_color(play_icon,LV_COLOR_WHITE,0);
        lv_obj_align(play_icon,LV_ALIGN_CENTER,0,0);
    }
   

    lv_obj_set_parent(play_button,act_scr);
}

void initialize_music_forward_button_style(lv_style_t* forward_button_style){
    lv_style_init(forward_button_style);
    lv_style_set_bg_color(forward_button_style, LV_COLOR_BLACK);
    lv_style_set_radius(forward_button_style, LV_RADIUS_CIRCLE);
    lv_style_set_border_width(forward_button_style, 2);
    lv_style_set_outline_width(forward_button_style, 0);
    lv_style_set_border_color(forward_button_style, LV_COLOR_WHITE);
}

void init_music_forward_button(lv_obj_t* forward_button,lv_style_t* forward_button_style,lv_obj_t* act_scr){
    forward_button = lv_btn_create(act_scr);
    lv_obj_set_size(forward_button,40,40);
    lv_obj_align(forward_button,LV_ALIGN_CENTER,50,60);
    lv_obj_add_style(forward_button,forward_button_style,0);
    lv_obj_set_style_bg_opa(forward_button,0,0);
    lv_obj_add_event_cb(forward_button,forward_button_cb,LV_EVENT_CLICKED,NULL);
    
    lv_obj_t* forward_icon = lv_label_create(forward_button);
    lv_label_set_text(forward_icon,LV_SYMBOL_NEXT);
    lv_obj_set_style_text_color(forward_icon,LV_COLOR_WHITE,0);
    lv_obj_align(forward_icon,LV_ALIGN_CENTER,0,0);

    lv_obj_set_parent(forward_button,act_scr);
}

void initialize_music_rewind_button_style(lv_style_t* rewind_button_style){
    lv_style_init(rewind_button_style);
    lv_style_set_bg_color(rewind_button_style, LV_COLOR_WHITE);
    lv_style_set_radius(rewind_button_style, LV_RADIUS_CIRCLE);
    lv_style_set_border_width(rewind_button_style, 2);
    lv_style_set_outline_width(rewind_button_style, 0);
    lv_style_set_border_color(rewind_button_style, LV_COLOR_WHITE);
}

void init_music_rewind_button(lv_obj_t* rewind_button,lv_style_t* rewind_button_style,lv_obj_t* act_scr){
    rewind_button = lv_btn_create(act_scr);
    lv_obj_set_size(rewind_button,40,40);
    lv_obj_align(rewind_button,LV_ALIGN_CENTER,-50,60);
    lv_obj_add_style(rewind_button,rewind_button_style,0);
    lv_obj_set_style_bg_opa(rewind_button,0,0);
    lv_obj_add_event_cb(rewind_button,rewind_button_cb,LV_EVENT_CLICKED,NULL);

    lv_obj_t* rewind_icon = lv_label_create(rewind_button);
    lv_label_set_text(rewind_icon,LV_SYMBOL_PREV);
    lv_obj_set_style_text_color(rewind_icon,LV_COLOR_WHITE,0);
    lv_obj_align(rewind_icon,LV_ALIGN_CENTER,0,0);

    lv_obj_set_parent(rewind_button,act_scr);
}

void initialize_music_song_label_style(lv_style_t* song_label_style){
    lv_style_init(song_label_style);
    // lv_style_set_text_color(song_label_style, LV_COLOR_WHITE);
    lv_style_set_border_width(song_label_style, 0);
    lv_style_set_outline_width(song_label_style, 0);
    lv_style_set_border_color(song_label_style, LV_COLOR_BLACK);
    lv_style_set_shadow_width(song_label_style, 0);
}

void init_music_song_label(lv_obj_t* song_label,lv_style_t* song_label_style,lv_obj_t* act_scr){
    
    song_label = lv_btn_create(act_scr);
    lv_obj_add_style(song_label,song_label_style,0);
    lv_obj_set_style_bg_opa(song_label,0,0);
    lv_obj_align(song_label,LV_ALIGN_CENTER,0,0);
    // lv_obj_add_event_cb(song_label,song_label_cb,LV_EVENT_CLICKED,NULL);
    
    song_label_name = lv_label_create(song_label);
    lv_label_set_text(song_label_name,"Do I Wanna Know?");
    lv_obj_set_style_text_color(song_label_name,LV_COLOR_WHITE,0);
    lv_obj_align(song_label_name,LV_ALIGN_CENTER,0,0);
}

void initialize_music_artist_label_style(lv_style_t* artist_label_style){
    lv_style_init(artist_label_style);
    // lv_style_set_text_color(artist_label_style, LV_COLOR_GREY);
    lv_style_set_border_width(artist_label_style, 0);
    lv_style_set_outline_width(artist_label_style, 0);
    lv_style_set_border_color(artist_label_style, LV_COLOR_BLACK);
    lv_style_set_shadow_width(artist_label_style, 0);
}

void init_music_artist_label(lv_obj_t* artist_label,lv_style_t* artist_label_style,lv_obj_t* act_scr){

    artist_label = lv_btn_create(act_scr);
    lv_obj_add_style(artist_label,artist_label_style,0);
    lv_obj_set_style_bg_opa(artist_label,0,0);
    lv_obj_align(artist_label,LV_ALIGN_CENTER,0,20);
    lv_obj_set_size(artist_label,240,20);


    artist_label_name = lv_label_create(artist_label);
    lv_label_set_text(artist_label_name,"Arctic Monkeys");
    lv_obj_set_style_text_color(artist_label_name,LV_COLOR_GREY,0);
    lv_obj_align(artist_label_name,LV_ALIGN_CENTER,0,0);
    // lv_obj_set_style_local_text_font(artist_label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_FONT_MONTSERRAT_16);

}

void play_button_cb(lv_event_t* event){
    if(lv_event_get_code(event) == LV_EVENT_CLICKED){
        if(is_playing){
            lv_label_set_text(play_icon,LV_SYMBOL_PLAY);
            is_playing = false;
        }else{
            lv_label_set_text(play_icon,LV_SYMBOL_PAUSE);
            is_playing = true;
        }
    }
}

void forward_button_cb(lv_event_t* event){
    if(lv_event_get_code(event) == LV_EVENT_CLICKED){
        Serial.println("Forward button clicked");
    //    lv_label_set_text(song_label,"Clicked");
    //     lv_obj_del(song_label);
        // a2dp_sink.next();
    }
}

void rewind_button_cb(lv_event_t* event){
    if(lv_event_get_code(event) == LV_EVENT_CLICKED){
        Serial.println("Rewind button clicked");
        // a2dp_sink.next();
    }
}

void update_music_screen(){

    // lv_label_set_text(song_label,"Updating song title");
    // lv_obj_align(song_label,NULL,LV_ALIGN_CENTER,0,0);
    // lv_obj_set_style_local_text_font(song_label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_FONT_MONTSERRAT_16);
    // lv_obj_add_style(song_label,LV_LABEL_PART_MAIN,song_label_style);

    // lv_label_set_text(artist_label,"Updating artist");
    // lv_obj_align(artist_label,NULL,LV_ALIGN_CENTER,0,20);
    // lv_obj_set_style_local_text_font(artist_label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_FONT_MONTSERRAT_16);
    // lv_obj_add_style(artist_label,LV_LABEL_PART_MAIN,artist_label_style);

    lv_task_handler();
}

void initialize_music_spectrum_style(lv_style_t* bar_style){
    lv_style_init(bar_style);
    lv_style_set_bg_color(bar_style,LV_COLOR_BLACK);
    lv_style_set_border_color(bar_style, LV_COLOR_WHITE);
    lv_style_set_border_width(bar_style, 2);
}

void init_spectrum_bars(lv_obj_t* bars[],lv_style_t* bar_style,lv_obj_t* act_scr){
     /* Create the bars */
    for (int i = 0; i < NUM_BARS; i++) {
        bars[i] = lv_obj_create(act_scr);
        lv_obj_set_style_bg_color(bars[i], LV_COLOR_WHITE,0);
        lv_obj_set_style_border_color(bars[i],LV_COLOR_WHITE,0);
        lv_obj_set_style_border_width(bars[i],1,0);
        lv_obj_set_size(bars[i], BAR_WIDTH, 0);
        lv_obj_align(bars[i], LV_ALIGN_LEFT_MID, i * (BAR_WIDTH + BAR_SPACING), 0);
    }

    spectrum_task = lv_timer_create(update_music_audio_spectrum, 30,NULL);
    lv_timer_ready(spectrum_task);
}



void update_music_audio_spectrum(lv_timer_t *task) {
    /* Generate some sample data */
    if(!is_playing){
        return;
    }
    float sample_data[NUM_BARS];
    for (int i = 0; i < NUM_BARS; i++) {
        sample_data[i] = sinf((float)lv_tick_get() / 1000.0f * (float)(i + 1)) * 0.5f + 0.5f;
    }
    
    /* Update the bar values */
    for (int i = 0; i < NUM_BARS; i++) {
        uint8_t new_value = (uint8_t)(sample_data[i] * (float)MAX_BAR_HEIGHT);
        if (new_value != bar_values[i]) {
            bar_values[i] = new_value;
            lv_obj_set_height(bars[i], new_value);
            lv_obj_align(bars[i], LV_ALIGN_LEFT_MID, i * (BAR_WIDTH + BAR_SPACING), -40);
        }
    }

    
}

void update_play_button_cb(lv_timer_t * timer){
    if(gpio_get_level(GPIO_NUM_0) == 0)
  {
    if(is_playing){
            lv_label_set_text(play_icon,LV_SYMBOL_PLAY);
            is_playing = false;
            // a2dp_sink.pause();
        }else{
            lv_label_set_text(play_icon,LV_SYMBOL_PAUSE);
            is_playing = true;
            // a2dp_sink.play();
        }

        Serial.println("Play button clicked");
  }
}

void screen_event_cb(lv_event_t* event){
    if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT){
        lv_obj_t* scr = (lv_obj_t*)lv_obj_get_user_data(music_screen);
        lv_scr_load(scr);
        lv_obj_del(music_screen);
        // music_screen = NULL;
    }
}

void update_song_label(lv_timer_t * timer){
    // lv_label_set_text_fmt(song_label_name,"%s",ble_music_value.c_str());
    if(has_music_metadata_changed){
        title = (char*) ble_music_value.c_str();
        lv_label_set_text(song_label_name,title);
        lv_label_set_text(artist_label_name,"John Mayer");
        has_music_metadata_changed = false;
    }
   
}


