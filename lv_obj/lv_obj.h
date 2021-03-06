/**
 * @file lv_base_obj.h
 * 
 */

#ifndef LV_OBJ_H
#define LV_OBJ_H

/*********************
 *      INCLUDES
 *********************/
#include <lvgl/lv_misc/2d.h>
#include <stddef.h>
#include <stdbool.h>
#include "misc/mem/dyn_mem.h"
#include "misc/mem/linked_list.h"
#include "misc/others/color.h"

/*********************
 *      DEFINES
 *********************/
/*Error check of lv_conf.h*/
#if LV_HOR_RES == 0 || LV_VER_RES == 0
#error "LV: LV_HOR_RES and LV_VER_RES must be greater then 0"
#endif

#if LV_DOWNSCALE != 1 && LV_DOWNSCALE != 2 && LV_DOWNSCALE != 4
#error "LV: LV_DOWNSCALE can be only 1, 2 or 4"
#endif

#if LV_VDB_SIZE == 0 && (LV_DOWNSCALE != 1 || LV_UPSCALE_MAP != 0 || LV_UPSCALE_FONT != 0 || LV_UPSCALE_STYLE != 0)
#error "LV: If LV_VDB_SIZE == 0 then LV_DOWNSCALE must be 1, LV_UPSCALE_MAP 0, LV_UPSCALE_FONT 0, LV_UPSCALE_STYLE 0"
#endif

/*New defines*/
#if LV_UPSCALE_SYTLE != 0
#define LV_STYLE_MULT LV_DOWNSCALE
#else
#define LV_STYLE_MULT 1
#endif

#define LV_OBJ_DEF_WIDTH  (80 * LV_DOWNSCALE)
#define LV_OBJ_DEF_HEIGHT  (60 * LV_DOWNSCALE)

/**********************
 *      TYPEDEFS
 **********************/

struct __LV_OBJ_T;

typedef enum
{
    LV_DESIGN_DRAW,
    LV_DESIGN_COVER_CHK,
}lv_design_mode_t;

typedef bool (* lv_design_f_t) (struct __LV_OBJ_T* obj_dp, const area_t * mask_p, lv_design_mode_t mode); 

typedef enum
{
	LV_SIGNAL_CLEANUP,
    LV_SIGNAL_PRESSED,
    LV_SIGNAL_PRESS_LOST,
    LV_SIGNAL_RELEASED,
    LV_SIGNAL_LONG_PRESS,
    LV_SIGNAL_DRAG_BEGIN,
    LV_SIGNAL_DRAG_END,        
    LV_SIGNAL_CHILD_CHG,
    LV_SIGNAL_CORD_CHG,
    LV_SIGNAL_STYLE_CHG,
}lv_signal_t;

typedef bool (* lv_signal_f_t) (struct __LV_OBJ_T* obj_dp, lv_signal_t sign, void * param);

typedef struct __LV_OBJ_T
{
    struct __LV_OBJ_T* par_dp;
    ll_dsc_t child_ll;
    
    area_t cords;
    
    /*Basic appearance*/
    opa_t opa;

    /*Attributes and states*/
    uint8_t click_en     :1;    /*1: can be pressed by a display input device*/
    uint8_t drag_en      :1;    /*1: enable the dragging*/
    uint8_t drag_throw_en:1;    /*1: Enable throwing with drag*/
    uint8_t drag_parent  :1;    /*1. Parent will be dragged instead*/
    uint8_t style_iso 	 :1;	/*1: The object has got an own style*/
    uint8_t hidden       :1;    /*1: Object is hidden*/
    uint8_t top_en       :1;	/*1: If the object or its children  is clicked it goes to the foreground*/
    uint8_t res			 :1;
    
    uint8_t free_num; 		/*Application specific identifier (set it freely)*/
    
    lv_signal_f_t signal_f;
    lv_design_f_t design_f;
    
    void * ext_dp;        /*The object attributes can be extended here*/
    void * style_p;       /*Object specific style*/

#if LV_OBJ_FREE_P != 0
    void * free_p;        /*Application specific pointer (set it freely)*/
#endif
}lv_obj_t;

typedef enum
{
    LV_ALIGN_CENTER = 0,
	LV_ALIGN_IN_TOP_LEFT,
	LV_ALIGN_IN_TOP_MID,
	LV_ALIGN_IN_TOP_RIGHT,
	LV_ALIGN_IN_BOTTOM_LEFT,
	LV_ALIGN_IN_BOTTOM_MID,
	LV_ALIGN_IN_BOTTOM_RIGHT,
	LV_ALIGN_IN_LEFT_MID,
	LV_ALIGN_IN_RIGHT_MID,
	LV_ALIGN_OUT_TOP_LEFT,
	LV_ALIGN_OUT_TOP_MID,
	LV_ALIGN_OUT_TOP_RIGHT,
	LV_ALIGN_OUT_BOTTOM_LEFT,
	LV_ALIGN_OUT_BOTTOM_MID,
	LV_ALIGN_OUT_BOTTOM_RIGHT,
	LV_ALIGN_OUT_LEFT_TOP,
	LV_ALIGN_OUT_LEFT_MID,
	LV_ALIGN_OUT_LEFT_BOTTOM,
	LV_ALIGN_OUT_RIGHT_TOP,
	LV_ALIGN_OUT_RIGHT_MID,
	LV_ALIGN_OUT_RIGHT_BOTTOM,
}lv_align_t;

typedef struct
{
	color_t color;
}lv_objs_t;

typedef enum
{
	LV_OBJS_DEF,
	LV_OBJS_SCR,
}lv_objs_builtin_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_init(void);
void lv_obj_inv(lv_obj_t* obj_dp);
void lv_obj_refr_style(lv_obj_t* obj_dp);
void lv_style_refr_all(void * style_p);

/*Create and delete*/
lv_obj_t* lv_obj_create(lv_obj_t* parent_dp, lv_obj_t * copy_dp);
void lv_obj_del(lv_obj_t* obj_dp);

/*Virtual functions*/
bool lv_obj_signal(lv_obj_t* obj_dp, lv_signal_t sign, void * param);

/*SETTER FUNCTIONS*/
/*Parent/children set*/
void lv_obj_set_parent(lv_obj_t* obj_dp, lv_obj_t* parent_dp);
/*Coordinate set (set_cord_f will be called)*/
void lv_obj_set_pos(lv_obj_t* obj_dp, cord_t x, cord_t y);
void lv_obj_set_pos_us(lv_obj_t* obj_dp, cord_t x, cord_t y);
void lv_obj_set_x(lv_obj_t* obj_dp, cord_t x);
void lv_obj_set_x_us(lv_obj_t* obj_dp, cord_t x);
void lv_obj_set_y(lv_obj_t* obj_dp, cord_t y);
void lv_obj_set_y_us(lv_obj_t* obj_dp, cord_t y);
void lv_obj_set_size(lv_obj_t* obj_dp, cord_t w, cord_t h);
void lv_obj_set_size_us(lv_obj_t* obj_dp, cord_t w, cord_t h);
void lv_obj_set_width(lv_obj_t* obj_dp, cord_t w);
void lv_obj_set_width_us(lv_obj_t* obj_dp, cord_t w);
void lv_obj_set_height(lv_obj_t* obj_dp, cord_t h);
void lv_obj_set_height_us(lv_obj_t* obj_dp, cord_t h);
void lv_obj_align(lv_obj_t* obj_dp,lv_obj_t* base_dp, lv_align_t align, cord_t x_mod, cord_t y_mod);
void lv_obj_align_us(lv_obj_t* obj_dp,lv_obj_t* base_dp, lv_align_t align, cord_t x_mod, cord_t y_mod);
/*Appearance set*/
void lv_obj_set_hidden(lv_obj_t* obj_dp, bool hidden_en);
void lv_obj_set_opa(lv_obj_t* obj_dp, opa_t opa);
void lv_obj_set_opar(lv_obj_t* obj_dp, opa_t opa);
/*Attribute set*/
void lv_obj_set_click(lv_obj_t* obj_dp, bool click_en);
void lv_obj_set_top(lv_obj_t* obj_dp, bool click_en);
void lv_obj_set_drag(lv_obj_t* obj_dp, bool drag_en);
void lv_obj_set_drag_throw(lv_obj_t* obj_dp, bool dragthr_en);
void lv_obj_set_drag_parent(lv_obj_t* obj_dp, bool dragpar_en);
void lv_obj_set_signal_f(lv_obj_t* obj_dp, lv_signal_f_t fp);
void lv_obj_set_design_f(lv_obj_t* obj_dp, lv_design_f_t fp);
/*Other set*/
void * lv_obj_alloc_ext(lv_obj_t* obj_dp, uint16_t ext_size);
void lv_obj_set_style(lv_obj_t* obj_dp, void * style_p);
void * lv_obj_iso_style(lv_obj_t * obj_dp, uint32_t style_size);
void lv_obj_set_free_num(lv_obj_t* obj_dp, uint8_t free_num);
void lv_obj_set_free_p(lv_obj_t* obj_dp, void * free_p);
void lv_obj_merge_style(lv_obj_t* obj_dp);

/*GETTER FUNCTIONS*/
/*Screen get*/
lv_obj_t* lv_scr_act(void);
void lv_scr_load(lv_obj_t* scr_dp);
/*Parent/children get*/
lv_obj_t* lv_obj_get_scr(lv_obj_t* obj_dp);
lv_obj_t* lv_obj_get_parent(lv_obj_t* obj_dp);
lv_obj_t * lv_obj_get_child(lv_obj_t * obj_dp, lv_obj_t * child_dp);
/*Coordinate get*/
void lv_obj_get_cords(lv_obj_t* obj_dp, area_t * cords_p);
cord_t lv_obj_get_x(lv_obj_t* obj_dp);
cord_t lv_obj_get_y(lv_obj_t* obj_dp);
cord_t lv_obj_get_width(lv_obj_t* obj_dp);
cord_t lv_obj_get_height(lv_obj_t* obj_dp);
/*Appearance get*/
bool lv_obj_get_hidden(lv_obj_t* obj_dp);
opa_t lv_obj_get_opa(lv_obj_t* obj_dp);
/*Attribute get*/
bool lv_obj_get_click(lv_obj_t* obj_dp);
bool lv_obj_get_top(lv_obj_t* obj_dp);
bool lv_obj_get_drag(lv_obj_t* obj_dp);
bool lv_obj_get_drag_throw(lv_obj_t* obj_dp);
bool lv_obj_get_drag_parent(lv_obj_t* obj_dp);

/*Virtual functions get*/
lv_design_f_t lv_obj_get_design_f(lv_obj_t* obj_dp);
lv_signal_f_t  lv_obj_get_signal_f(lv_obj_t* obj_dp);
/*Other get*/
void * lv_obj_get_ext(lv_obj_t* obj_dp);
void * lv_obj_get_style(lv_obj_t* obj_dp);
uint8_t lv_obj_get_free_num(lv_obj_t* obj_dp);

lv_objs_t * lv_objs_get(lv_objs_builtin_t style, lv_objs_t * copy_p);

/**********************
 *      MACROS
 **********************/

#define LV_SA(obj_dp, style_type) ((style_type *) obj_dp->style_p)
#define LV_EA(obj_dp, obj_type) ((obj_type *) obj_dp->ext_dp)

#endif
