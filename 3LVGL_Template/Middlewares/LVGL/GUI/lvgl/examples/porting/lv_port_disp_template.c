/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp_template.h"
#include <stdbool.h>
#include "lcd.h"

/*********************
 *      DEFINES
 *********************/
#define MY_DISP_HOR_RES    240
#define MY_DISP_VER_RES    320

#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    320
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    240
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_t * disp, const lv_area_t * area, lv_color_t * px_map);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lv_disp_t * disp = lv_disp_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_disp_set_flush_cb(disp, (lv_disp_flush_cb_t)disp_flush);

    /* Example 1
     * One buffer for partial rendering*/
    static lv_color_t buf_1_1[MY_DISP_HOR_RES * 10];                          /*A buffer for 10 rows*/
    lv_disp_set_draw_buffers(disp, buf_1_1, NULL, sizeof(buf_1_1), LV_DISP_RENDER_MODE_PARTIAL);

    /* Example 2
     * Two buffers for partial rendering
     * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
//    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];
//    static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];
//    lv_disp_set_draw_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISP_RENDER_MODE_PARTIAL);

    /* Example 3
     * Two buffers screen sized buffer for double buffering.
     * Both LV_DISP_RENDER_MODE_DIRECT and LV_DISP_RENDER_MODE_FULL works, see their comments*/
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];
//    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];
//    lv_disp_set_draw_buffers(disp, buf_3_1, buf_3_2, sizeof(buf_3_1), LV_DISP_RENDER_MODE_DIRECT);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
	LCD_Init();
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display.
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when it's finished.*/
u16 colorSwitch(lv_color_t * px_map){
	return ((px_map->red & 0xf8)>>3) | ((px_map->green & 0xfC)<<3) | ((px_map->blue & 0xf8)<<8);
}

static void disp_flush(lv_disp_t * disp_drv, const lv_area_t * area, lv_color_t * px_map)
{    

	// printf("rgb:#%x%x%x\t%d\r\n", px_map->red, px_map->green, px_map->blue, (u16)(&px_map));
    LCD_Fill_xy(area->x1, area->y1, area->x2, area->y2, (u16*)px_map);
    lv_disp_flush_ready(disp_drv);
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
