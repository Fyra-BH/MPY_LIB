#include "ez_fft.h"
#include "py/runtime.h"
#include "py/objarray.h"
#include <stdio.h>


STATIC mp_obj_t ez_fft8(mp_obj_t buffer)
{
    mp_obj_array_t* array = (mp_obj_array_t *)buffer;
    if(array->typecode != 'f')
    {
        printf("please input float array");
        return mp_obj_new_bool(0);
    }
    if (array->len != 16)
    {
        printf("length wrong");
        return mp_obj_new_bool(0);
    }
    fft_8((float*)(array->items));
    return buffer;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(ez_fft_fft8_obj, ez_fft8);

STATIC mp_obj_t ez_fft16(mp_obj_t buffer)
{
    mp_obj_array_t* array = (mp_obj_array_t *)buffer;
    if(array->typecode != 'f')
    {
        printf("please input float array");
        return mp_obj_new_bool(0);
    }
    if (array->len != 32)
    {
        printf("length wrong");
        return mp_obj_new_bool(0);
    }
    fft_16((float*)(array->items));
    return buffer;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(ez_fft_fft16_obj, ez_fft16);

STATIC mp_obj_t ez_fft32(mp_obj_t buffer)
{
    mp_obj_array_t* array = (mp_obj_array_t *)buffer;
    if(array->typecode != 'f')
    {
        printf("please input float array");
        return mp_obj_new_bool(0);
    }
    if (array->len != 64)
    {
        printf("length wrong");
        return mp_obj_new_bool(0);
    }
    fft_32((float*)(array->items));
    return buffer;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(ez_fft_fft32_obj, ez_fft32);

STATIC const mp_rom_map_elem_t ezfft_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ezfft) },
    { MP_ROM_QSTR(MP_QSTR_fft8), MP_ROM_PTR(&ez_fft_fft8_obj) },
    { MP_ROM_QSTR(MP_QSTR_fft16), MP_ROM_PTR(&ez_fft_fft16_obj) },
    { MP_ROM_QSTR(MP_QSTR_fft32), MP_ROM_PTR(&ez_fft_fft32_obj) },
};
STATIC MP_DEFINE_CONST_DICT(ezfft_module_globals, ezfft_module_globals_table);

// Define module object.
const mp_obj_module_t ezfft_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&ezfft_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_ezfft, ezfft_user_cmodule, 1);

