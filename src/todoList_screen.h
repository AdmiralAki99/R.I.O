#include "parsecs.h"

static lv_obj_t* todo_screen;
static lv_obj_t* add_scr;
static lv_obj_t* todoList;
static lv_style_t* todoList_style;
static lv_obj_t* todoList_check_boxes;
static lv_obj_t* add_todoList_button;
static lv_obj_t* add_todoList_button_label;
static lv_style_t* add_todoList_button_style;

lv_timer_t* updateTasks;

char *tasks[20] = {"Geek", "Geeks", "Geekfor"};

void init_todoList_screen(lv_obj_t* act_scr);
void init_todoList(lv_obj_t* scr,lv_obj_t* list,lv_style_t* list_style);
void init_addTodoList(lv_obj_t* scr,lv_obj_t* list,lv_style_t* list_style,lv_obj_t* add_button);
void updateTasksMethod(lv_timer_t* timer);
static void event_handler(lv_event_t * e);
void addTask(char* task);
void update_todoList(lv_obj_t* scr,lv_obj_t* list,lv_style_t* list_style);
void createAddScreen(lv_obj_t* scr);
void todo_screen_back_event_cb(lv_event_t* event);


void init_todoList_screen(lv_obj_t* act_scr){
    todo_screen = act_scr;
    lv_obj_set_style_bg_color(todo_screen, LV_COLOR_BLACK, 0);
    lv_obj_add_event_cb(todo_screen, todo_screen_back_event_cb, LV_EVENT_GESTURE, NULL);
    lv_scr_load(todo_screen);
    init_todoList(todo_screen,todoList,todoList_style);
    updateTasks = lv_timer_create(updateTasksMethod, 10000 , NULL);
    // lv_timer_ready(updateTasks);
}

static void todo_scroll_event_cb(lv_event_t * e){
    lv_obj_t * cont = lv_event_get_target(e);

    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;

    lv_coord_t r = lv_obj_get_height(cont) * 7 / 10;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_cnt(cont);
    for(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        lv_coord_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y);

        /*Get the x of diff_y on a circle.*/
        lv_coord_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if(diff_y >= r) {
            x = r;
        }
        else {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000);   /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        lv_obj_set_style_translate_x(child, x, 0);

        /*Use some opacity with larger translations*/
        lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
    }
}


static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        const char * txt = lv_checkbox_get_text(obj);
        const char * state = lv_obj_get_state(obj) & LV_STATE_CHECKED ? "Checked" : "Unchecked";
        LV_LOG_USER("%s: %s", txt, state);
    }
}

void init_todoList(lv_obj_t* scr,lv_obj_t* list,lv_style_t* list_style){
    lv_obj_set_flex_flow(lv_scr_act(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(lv_scr_act(), LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    list = lv_obj_create(lv_scr_act());
    lv_obj_set_size(list, 240, 240);
    lv_obj_center(list);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    // lv_obj_add_event_cb(list, todo_scroll_event_cb, LV_EVENT_SCROLL, NULL);
    lv_obj_set_style_radius(list, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(list, true, 0);
    lv_obj_set_scroll_dir(list, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(list, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_bg_color(list, LV_COLOR_BLACK, 0);
    lv_obj_set_style_border_color(list, LV_COLOR_BLACK, 0);
    lv_obj_set_style_border_width(list, 0, 0);

    uint32_t i = 0;
    while(tasks[i] != NULL){
         todoList_check_boxes = lv_checkbox_create(list);
        lv_checkbox_set_text(todoList_check_boxes, tasks[i]);
        lv_obj_set_style_text_color(todoList_check_boxes,LV_COLOR_WHITE, 0);
        i++;
        // lv_obj_add_event_cb(todoList_check_boxes, add_button_pressed, LV_E, NULL);
    }

    /*Update the buttons position manually for first*/
    lv_event_send(list, LV_EVENT_SCROLL, NULL);

    /*Be sure the fist button is in the middle*/
    lv_obj_scroll_to_view(lv_obj_get_child(list, 0), LV_ANIM_OFF);
}

void updateTasksMethod(lv_timer_t* timer){
    addTask("New Task");
}

void update_todoList(lv_obj_t* scr,lv_obj_t* list,lv_style_t* list_style){
    lv_obj_set_flex_flow(lv_scr_act(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(lv_scr_act(), LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    list = lv_obj_create(lv_scr_act());
    lv_obj_set_size(list, 240, 240);
    lv_obj_center(list);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    lv_obj_add_event_cb(list, todo_scroll_event_cb, LV_EVENT_SCROLL, NULL);
    lv_obj_set_style_radius(list, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(list, true, 0);
    lv_obj_set_scroll_dir(list, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(list, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);

    uint32_t i = 0;
    while(tasks[i] != NULL){
         todoList_check_boxes = lv_checkbox_create(list);
        lv_checkbox_set_text(todoList_check_boxes, tasks[i]);
        i++;
        // lv_obj_add_event_cb(todoList_check_boxes, add_button_pressed, LV_E, NULL);
    }

    lv_obj_t * btn = lv_btn_create(list);
    lv_obj_set_width(btn, lv_pct(200));
    // lv_obj_add_event_cb(btn, add_button_pressed, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text_fmt(label,"Add Event");

    /*Update the buttons position manually for first*/
    lv_event_send(list, LV_EVENT_SCROLL, NULL);

    /*Be sure the fist button is in the middle*/
    lv_obj_scroll_to_view(lv_obj_get_child(list, 0), LV_ANIM_OFF);
}

// Implement a Function to add string to character array

void addTask(char *task)
{
    uint8_t i = 0;
    while(tasks[i] != NULL && i<20){
        i++;
    }
    tasks[i] = task;
    lv_obj_del(todo_screen);
    todo_screen = lv_obj_create(NULL);
    lv_scr_load(todo_screen);
    update_todoList(todo_screen,todoList,todoList_style);
}

void todo_screen_back_event_cb(lv_event_t* event){
    if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT){
        lv_obj_t* scr = (lv_obj_t*)lv_obj_get_user_data(todo_screen);
        lv_scr_load(scr);
        lv_obj_del(todo_screen);
        // music_screen = NULL;
    }
}