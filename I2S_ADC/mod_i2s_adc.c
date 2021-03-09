
#include "py/runtime.h"
#include "py/objarray.h"

#include <stdlib.h>

extern int my_i2s_read(int Nbytes, void* buff);
extern void example_i2s_init(void);
extern void s16_to_float(int len_of_int, __uint16_t* src, float* dst);

STATIC mp_obj_t i2s_adc_init(void)
{
    example_i2s_init();
    return mp_obj_new_bool(1);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(i2s_adc_init_obj, i2s_adc_init);

STATIC mp_obj_t i2s_read_buff(mp_obj_t buffer)
{
    mp_obj_array_t* array = (mp_obj_array_t *)buffer;
    if(array->typecode != 'f') return mp_obj_new_bool(0);
    if(array->len == 0) return mp_obj_new_bool(0);
    uint16_t *buff_ints = (uint16_t*)malloc(array->len  * sizeof(uint16_t));
    if(buff_ints == NULL) return mp_obj_new_bool(0);
    my_i2s_read(array->len * 4 / 2,buff_ints);
    s16_to_float(array->len, buff_ints, array->items);
    free(buff_ints);
    return mp_obj_new_bool(1);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(i2s_read_buff_obj, i2s_read_buff);

STATIC const mp_rom_map_elem_t i2s_adc_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_i2s_adc) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&i2s_adc_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_to_array), MP_ROM_PTR(&i2s_read_buff_obj) },
};
STATIC MP_DEFINE_CONST_DICT(i2s_adc_module_globals, i2s_adc_module_globals_table);

// Define module object.
const mp_obj_module_t i2s_adc_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&i2s_adc_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_i2s_adc, i2s_adc_user_cmodule, 1);

