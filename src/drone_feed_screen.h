/**
 * @file drone_feed_screen.h
 * @author Akhilesh Warty
 * @brief Drone Feed Screen for Smart Watch (Experimental)
 * @version 0.1
 * @date 2023-09-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "parsecs.h"
#include "video_player.h"
#include "thumb_test.h"

static lv_obj_t* drone_feed_screen;
static lv_obj_t* video_feed;
static lv_color_t feed_buffer[LV_CANVAS_BUF_SIZE_TRUE_COLOR(200,200)];
static lv_obj_t* video_frame;
static lv_obj_t* video_feed_canvas;


lv_timer_t* droneFeedTimer;
long lTime;
char szTemp[64];

// VideoPlayer video_player;

JPEGDEC jpeg;



void init_screen(lv_obj_t* screen);
void init_video_feed(lv_timer_t* timer);
int drawFrame(JPEGDRAW *pDraw);
void init_video_feed_canvas(lv_obj_t* screen);
void init_video_frame(lv_obj_t* screen);

void init_drone_feed_screen(lv_obj_t* screen){
    drone_feed_screen = screen;
    lv_obj_set_style_bg_color(screen,LV_COLOR_BLACK,0);
    // lv_obj_set_style_bg_opa(screen,LV_OPA_0,0);
    // init_video_feed();
    droneFeedTimer = lv_timer_create(init_video_feed,10000,NULL);
    lv_timer_ready(droneFeedTimer);
    // tft.fillScreen(TFT_SKYBLUE);
    // init_video_frame(drone_feed_screen);
    // init_video_feed_canvas(drone_feed_screen);

}

void init_video_feed(lv_timer_t* timer){

   if(jpeg.openFLASH((uint8_t*)thumb_test,sizeof(thumb_test),drawFrame)){
        Serial.println("Successfully opened JPEG image");
        Serial.printf("Image size: %d x %d, orientation: %d, bpp: %d\n", jpeg.getWidth(),
        jpeg.getHeight(), jpeg.getOrientation(), jpeg.getBpp());
        if (jpeg.hasThumb())
            Serial.printf("Thumbnail present: %d x %d\n", jpeg.getThumbWidth(), jpeg.getThumbHeight());
        jpeg.setPixelType(RGB565_BIG_ENDIAN); // The SPI LCD wants the 16-bit pixels in big-endian order
        lTime = micros();
        // Draw the thumbnail image in the middle of the display (upper left corner = 120,100) at 1/4 scale
        if (jpeg.decode(120,100,JPEG_SCALE_QUARTER | JPEG_EXIF_THUMBNAIL))
        {
        lTime = micros() - lTime;
        sprintf(szTemp, "Successfully decoded image in %d us", (int)lTime);
        Serial.println(szTemp);
        // spilcdWriteString(&lcd, 0, 200, szTemp, 0xffe0, 0, FONT_8x8, DRAW_TO_LCD);
        }
        jpeg.close();
    }
    else{
         Serial.println("Failed to open");
   }
}

int drawFrame(JPEGDRAW *pDraw){
    int pixelCount;

    pixelCount = pDraw->iHeight * pDraw->iWidth;

    tft.setAddrWindow(pDraw->x,pDraw->y,pDraw->iHeight,pDraw->iWidth);

    tft.pushPixels(pDraw->pPixels, pixelCount);

    // lv_img_set_src(video_frame,pDraw->pPixels);

    return 1;

}

void init_video_frame(lv_obj_t* screen){
    video_frame = lv_img_create(screen);
}

void init_video_feed_canvas(lv_obj_t* screen){
    video_feed_canvas = lv_canvas_create(screen);
    lv_canvas_set_buffer(video_feed_canvas,feed_buffer,200,200,LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(video_feed_canvas,LV_ALIGN_CENTER,0,0);
    lv_canvas_fill_bg(video_feed_canvas,LV_COLOR_WHITE,LV_OPA_COVER);
}
