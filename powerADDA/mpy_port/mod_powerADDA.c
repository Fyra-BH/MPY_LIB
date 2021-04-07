#include "py/runtime.h"
#include "py/objarray.h"

#include "stdio.h"
#include "rque_fifo.h"

extern void POWERADDA_TIM4_Init(void);
extern void timer4_start_it(void);
extern void POWERADDA_DAC_Init(void);
extern void dac_start(void);

STATIC mp_obj_t powerADDA_init(mp_obj_t freq_obj)
{
    int freq = mp_obj_get_int(freq_obj);
    POWERADDA_TIM4_Init();
    timer4_start_it();
    POWERADDA_DAC_Init();
    dac_start();
    printf("freq=%d\n", freq);
    return mp_obj_new_bool(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(powerADDA_init_obj, powerADDA_init);

STATIC const mp_rom_map_elem_t powerADDA_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_powerADDA)},
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&powerADDA_init_obj)},
};
STATIC MP_DEFINE_CONST_DICT(powerADDA_module_globals, powerADDA_module_globals_table);

// Define module object.
const mp_obj_module_t powerADDA_user_cmodule = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&powerADDA_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_powerADDA, powerADDA_user_cmodule, 1);
