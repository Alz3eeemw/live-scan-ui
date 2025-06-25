#ifndef EEZ_LVGL_UI_GUI_H
#define EEZ_LVGL_UI_GUI_H

#include <lvgl.h>



#if defined(EEZ_FOR_LVGL)
#include <eez/flow/lvgl_api.h>
#endif

#if !defined(EEZ_FOR_LVGL)
#include "screens.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif



void ui_init();
void ui_tick();

#if !defined(EEZ_FOR_LVGL)
void loadScreen(enum ScreensEnum screenId);
#endif

#ifdef __cplusplus
}
#endif

#endif // EEZ_LVGL_UI_GUI_H


#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/

#ifndef EEZ_LVGL_UI_STRUCTS_H
#define EEZ_LVGL_UI_STRUCTS_H



#if defined(EEZ_FOR_LVGL)

#include <eez/flow/flow.h>
#include <stdint.h>
#include <stdbool.h>

#include "vars.h"

using namespace eez;





#endif

#endif /*EEZ_LVGL_UI_STRUCTS_H*/





#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_rgb;
extern const lv_img_dsc_t img_back_live;
extern const lv_img_dsc_t img_n;
extern const lv_img_dsc_t img_bt;
extern const lv_img_dsc_t img_battry;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[5];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/


#ifndef EEZ_LVGL_UI_FONTS_H
#define EEZ_LVGL_UI_FONTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_FONTS_H*/


#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_lvgl(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/


#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_TESLA_VALUE = 0,
    FLOW_GLOBAL_VARIABLE_PITCH_ANGLE = 1,
    FLOW_GLOBAL_VARIABLE_ROLL_ANGLE = 2,
    FLOW_GLOBAL_VARIABLE_YAW_ANGLE = 3,
    FLOW_GLOBAL_VARIABLE_BATTERY_LEVEL = 4,
    FLOW_GLOBAL_VARIABLE_BLUETOOTH_CONNECTED = 5
};

// Native global variables

extern float get_var_tesla_value();
extern void set_var_tesla_value(float value);
extern float get_var_pitch_angle();
extern void set_var_pitch_angle(float value);
extern float get_var_roll_angle();
extern void set_var_roll_angle(float value);
extern float get_var_yaw_angle();
extern void set_var_yaw_angle(float value);
extern int32_t get_var_battery_level();
extern void set_var_battery_level(int32_t value);
extern bool get_var_bluetooth_connected();
extern void set_var_bluetooth_connected(bool value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/

#ifndef EEZ_LVGL_UI_STRUCTS_H
#define EEZ_LVGL_UI_STRUCTS_H



#if defined(EEZ_FOR_LVGL)

#include <eez/flow/flow.h>
#include <stdint.h>
#include <stdbool.h>

#include "vars.h"

using namespace eez;





#endif

#endif /*EEZ_LVGL_UI_STRUCTS_H*/




