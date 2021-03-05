#include "ez_fft.h"
#include "py/runtime.h"
#include "py/objarray.h"


STATIC mp_obj_t ez_memtest(mp_obj_t len_obj)
{
    int len = mp_obj_get_int(len_obj);
    char* p = (char*)malloc(len);
    if(p == NULL) return mp_obj_new_bool(0);
    for (size_t i = 0; i < len; i++)
    {
        *(p + i) = 0x33;
    }
    free(p);
    return mp_obj_new_bool(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(ez_fft_memtest_obj, ez_memtest);

STATIC mp_obj_t ez_fftN(mp_obj_t buffer)
{
    mp_obj_array_t* array = (mp_obj_array_t *)buffer;
    if(array->typecode != 'f')
    {
        printf("please input float array");
        return mp_obj_new_bool(0);
    }
    return mp_obj_new_bool(!fft_N(array->len, (float*)(array->items)));
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(ez_fft_fftN_obj, ez_fftN);

STATIC mp_obj_t ez_fft_abs(mp_obj_t buffer)
{
    mp_obj_array_t* array = (mp_obj_array_t *)buffer;
    if(array->typecode != 'f')
    {
        printf("please input float array");
        return mp_obj_new_bool(0);
    }
    ezfft_abs(array->items, array->len / 2);
    array->items = (float *)m_realloc(array->items, sizeof(float) * array->len /2);
    array->len = array->len / 2;
    return buffer;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(ez_fft_abs_obj, ez_fft_abs);

STATIC const mp_rom_map_elem_t ezfft_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ezfft) },
    { MP_ROM_QSTR(MP_QSTR_fft), MP_ROM_PTR(&ez_fft_fftN_obj) },
    { MP_ROM_QSTR(MP_QSTR_memtest), MP_ROM_PTR(&ez_fft_memtest_obj) },
    { MP_ROM_QSTR(MP_QSTR_ezabs), MP_ROM_PTR(&ez_fft_abs_obj) },
};
STATIC MP_DEFINE_CONST_DICT(ezfft_module_globals, ezfft_module_globals_table);

// Define module object.
const mp_obj_module_t ezfft_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&ezfft_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_ezfft, ezfft_user_cmodule, 1);
