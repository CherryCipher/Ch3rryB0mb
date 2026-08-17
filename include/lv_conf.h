#ifndef LV_CONF_H
#define LV_CONF_H

/* Color depth */
#define LV_COLOR_DEPTH 16

/* Memory */
#define LV_MEM_SIZE (64U * 1024U)

/* Default display refresh period */
#define LV_DEF_REFR_PERIOD 33

/* Default DPI */
#define LV_DPI_DEF 130

/* Logging */
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

/* Fonts */
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_20 1

#define LV_FONT_DEFAULT &lv_font_montserrat_14

#endif