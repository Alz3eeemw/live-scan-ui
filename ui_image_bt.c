#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#elif defined(LV_BUILD_TEST)
#include "../lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_BT
#define LV_ATTRIBUTE_IMG_BT
#endif

static const
LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_BT
uint8_t img_bt_map[] = {

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x41,0x00,0x26,0x19,0x89,0x11,0xec,0x19,0xad,0x01,0x8d,0x01,0x8d,0x01,0x0d,0x1a,0xab,0x11,0x69,0x11,0x47,0x19,0x62,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x21,0x00,0x8a,0x19,0x52,0x0a,0x75,0x02,0x54,0x02,0x75,0x02,0x95,0x02,0x95,0x02,0x95,0x02,0x75,0x02,0x55,0x02,0x75,0x02,0x55,0x02,0x53,0x0a,0x8a,0x11,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x20,0x00,0x0f,0x12,0x54,0x02,0x95,0x02,0x95,0x02,0x31,0x0a,0x94,0x02,0xb5,0x02,0xb5,0x02,0xb5,0x02,0x75,0x02,0x93,0x12,0x72,0x12,0x95,0x02,0x95,0x02,0x54,0x02,0xac,0x09,0x00,0x00,0x00,0x00,
    0x00,0x00,0x8a,0x11,0x34,0x02,0xb5,0x02,0x95,0x02,0x51,0x0a,0x00,0x00,0x07,0x09,0x74,0x02,0xb5,0x02,0x75,0x02,0x52,0x12,0xa3,0x08,0xc4,0x08,0x74,0x02,0xb5,0x02,0xb5,0x02,0x54,0x02,0xc5,0x08,0x00,0x00,
    0x41,0x08,0x11,0x02,0x95,0x02,0xb5,0x02,0x95,0x02,0x75,0x02,0x6e,0x32,0x00,0x00,0x07,0x09,0x74,0x02,0x52,0x12,0xc3,0x10,0x82,0x08,0x31,0x12,0x75,0x02,0x95,0x02,0x95,0x02,0x54,0x02,0xed,0x19,0x00,0x00,
    0xa3,0x10,0x54,0x02,0x92,0x12,0x72,0x0a,0x73,0x0a,0x73,0x0a,0x33,0x02,0x2d,0x2a,0x00,0x00,0xc6,0x08,0x83,0x08,0x82,0x08,0x10,0x12,0x73,0x0a,0x73,0x0a,0x73,0x02,0x52,0x0a,0x94,0x0a,0x30,0x0a,0x63,0x00,
    0x04,0x19,0x55,0x02,0xec,0x19,0x00,0x00,0x21,0x00,0x62,0x00,0x42,0x00,0x83,0x00,0x21,0x00,0x00,0x00,0x00,0x00,0x42,0x00,0x83,0x00,0x62,0x00,0x62,0x00,0x20,0x00,0x82,0x08,0x92,0x1a,0xf1,0x01,0x63,0x00,
    0x62,0x08,0x33,0x02,0x54,0x02,0xed,0x19,0x00,0x00,0xc5,0x08,0x11,0x02,0xcd,0x09,0x20,0x00,0xe5,0x10,0x62,0x08,0x82,0x08,0x0f,0x12,0x10,0x0a,0x82,0x08,0x41,0x00,0x91,0x22,0x34,0x02,0x30,0x0a,0x62,0x00,
    0x20,0x00,0xf0,0x01,0x95,0x02,0x54,0x02,0x2e,0x22,0x00,0x00,0x07,0x11,0xa3,0x08,0x06,0x11,0x54,0x0a,0x11,0x02,0x62,0x08,0x05,0x19,0x05,0x11,0x82,0x08,0x51,0x12,0x75,0x02,0x75,0x02,0x0d,0x1a,0x20,0x00,
    0x00,0x00,0xaa,0x11,0x54,0x02,0xb5,0x02,0x54,0x02,0x2f,0x1a,0x00,0x00,0x47,0x19,0x74,0x0a,0x95,0x02,0xb5,0x02,0x11,0x02,0x62,0x00,0x83,0x08,0x72,0x1a,0x75,0x02,0xb5,0x02,0x54,0x02,0xc5,0x08,0x00,0x00,
    0x00,0x00,0x20,0x00,0xef,0x09,0x54,0x02,0x95,0x02,0x75,0x02,0x92,0x1a,0x73,0x0a,0x95,0x02,0xb5,0x02,0xb5,0x02,0x95,0x02,0x72,0x12,0x72,0x12,0x75,0x02,0x95,0x02,0x75,0x02,0x2a,0x01,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x41,0x00,0xab,0x11,0x52,0x02,0x75,0x02,0x34,0x02,0x75,0x02,0x95,0x02,0x95,0x02,0x95,0x02,0x95,0x02,0x54,0x02,0x34,0x02,0x74,0x02,0x31,0x0a,0x48,0x11,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x41,0x00,0x47,0x19,0xeb,0x21,0xcd,0x09,0x0f,0x0a,0xcf,0x01,0x0f,0x0a,0xee,0x11,0xec,0x19,0x89,0x21,0xc4,0x10,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};

const lv_image_dsc_t img_bt = {
  .header.magic = LV_IMAGE_HEADER_MAGIC,
  .header.cf = LV_COLOR_FORMAT_RGB565,
  .header.flags = 0,
  .header.w = 20,
  .header.h = 13,
  .header.stride = 40,
  .data_size = sizeof(img_bt_map),
  .data = img_bt_map,
};

