#include <mod/amlmod.h>
#include <mod/logger.h>

#include <fstream>

#include "thirdparty/inipp.h"
inipp::Ini<char> ini;

#include "cleo.h"
cleo_ifs_t* cleo = nullptr;

MYMOD(net.alexblade.rusjj.inifiles, CLEO4 IniFiles, 1.1, Alexander Blade & RusJJ)
BEGIN_DEPLIST()
    ADD_DEPENDENCY_VER(net.rusjj.cleolib, 2.0.1.3)
END_DEPLIST()

#define __decl_op(__name, __int)	const char* NAME_##__name = #__name; const uint16_t OP_##__name = __int;
#define __reg_opcode				cleo->RegisterOpcode
#define __reg_func					cleo->RegisterOpcodeFunction
#define __handler_params 			void *handle, uint32_t *ip, uint16_t opcode, const char *name
#define __op_name_match(x) 			opcode == OP_##x || strcmp(name, NAME_##x) == 0
#define __reg_op_func(x, h) 		__reg_opcode(OP_##x, h); __reg_func(NAME_##x, h);

__decl_op(READ_INT_FROM_INI_FILE, 0x0AF0);      // 0AF0=4,%4d% = read_int_from_ini_file %1s% section %2s% key %3s%
__decl_op(WRITE_INT_TO_INI_FILE, 0x0AF1);       // 0AF1=4,write_int %1d% to_ini_file %2s% section %3s% key %4s%
__decl_op(READ_FLOAT_FROM_INI_FILE, 0x0AF2);    // 0AF2=4,%4d% = read_float_from_ini_file %1s% section %2s% key %3s%
__decl_op(WRITE_FLOAT_TO_INI_FILE, 0x0AF3);     // 0AF3=4,write_float %1d% to_ini_file %2s% section %3s% key %4s%
__decl_op(READ_STRING_FROM_INI_FILE, 0x0AF4);   // 0AF4=4,%4d% = read_string_from_ini_file %1s% section %2s% key %3s%
__decl_op(WRITE_STRING_TO_INI_FILE, 0x0AF5);    // 0AF5=4,write_string %1s% to_ini_file %2s% section %3s% key %4s%

std::string sGameRoot = "/storage/emulated/0/Android/data/com.rockstargames.gtasa/files/";
static char szConvertedValue[16];

void READ_INT_FROM_INI_FILE(__handler_params)
{
    char filename[128], section[64], key[64];
    int result = 0, i = 0;
    cleo->ReadStringLong(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleo->ReadStringLong(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleo->ReadStringLong(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sGameRoot + filename).c_str());
    if(is.is_open())
    {
        ini.parse(is);
        inipp::get_value(ini.sections[section], key, result);
    }
    cleo->GetPointerToScriptVar(handle)->i = result;
}

void WRITE_INT_TO_INI_FILE(__handler_params)
{
    char filename[128], section[64], key[64];
    int i = 0, value = cleo->ReadParam(handle)->i;
    cleo->ReadStringLong(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleo->ReadStringLong(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleo->ReadStringLong(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sGameRoot + filename).c_str());
    if(is.is_open()) ini.parse(is);
    else ini.clear();

    sprintf(szConvertedValue, "%d", value);
    ini.sections[section][key] = szConvertedValue;

    std::ofstream os((sGameRoot + filename).c_str());
    if(os.is_open()) ini.generate(os);
}

void READ_FLOAT_FROM_INI_FILE(__handler_params)
{
    char filename[128], section[64], key[64];
    float result = 0.0f; int i = 0;
    cleo->ReadStringLong(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleo->ReadStringLong(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleo->ReadStringLong(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sGameRoot + filename).c_str());
    if(is.is_open())
    {
        ini.parse(is);
        inipp::get_value(ini.sections[section], key, result);
    }
    cleo->GetPointerToScriptVar(handle)->f = result;
}

void WRITE_FLOAT_TO_INI_FILE(__handler_params)
{
    char filename[128], section[64], key[64];
    int i = 0; float value = cleo->ReadParam(handle)->f;
    cleo->ReadStringLong(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleo->ReadStringLong(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleo->ReadStringLong(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sGameRoot + filename).c_str());
    if(is.is_open()) ini.parse(is);
    else ini.clear();

    sprintf(szConvertedValue, "%f", value);
    ini.sections[section][key] = szConvertedValue;

    std::ofstream os((sGameRoot + filename).c_str());
    if(os.is_open()) ini.generate(os);
}

char valRes[100];
void READ_STRING_FROM_INI_FILE(__handler_params)
{
    char filename[128], section[64], key[64];
    valRes[0] = 0; int i = 0;
    cleo->ReadStringLong(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleo->ReadStringLong(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleo->ReadStringLong(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sGameRoot + filename).c_str());
    if(is.is_open())
    {
        ini.parse(is);
        sprintf(valRes, "%s", ini.sections[section][key].c_str());
    }

    if(**(uint8_t**)((int)handle + 20) > 8)
    {
        char* dst = (char*)cleo->GetPointerToScriptVar(handle);
        memcpy(dst, valRes, 15); dst[15] = 0;
    }
    else
    {
        char* dst = (char*)cleo->ReadParam(handle);
        strcpy(dst, valRes);
    }
}

void WRITE_STRING_TO_INI_FILE(__handler_params)
{
    char filename[128], section[64], key[64], value[128];
    int i = 0;
    if(**(uint8_t**)((int)handle + 20) > 8) // Is this gonna work..?
    {
        cleo->ReadStringLong(handle, value, sizeof(value)); value[sizeof(value)-1] = 0;
    }
    else
    {
        strcpy(value, (char*)cleo->ReadParam(handle));
    }
    cleo->ReadStringLong(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleo->ReadStringLong(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleo->ReadStringLong(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sGameRoot + filename).c_str());
    if(is.is_open()) ini.parse(is);
    else ini.clear();

    ini.sections[section][key] = value;

    std::ofstream os((sGameRoot + filename).c_str());
    if(os.is_open()) ini.generate(os);
}

extern "C" void OnModLoad()
{
    logger->SetTag("[CLEO] IniFiles");
    logger->Info("Starting...");
    if(!(cleo = (cleo_ifs_t*)GetInterface("CLEO")))
    {
        logger->Error("Cannot load a mod: CLEO's interface is unknown!");
        return;
    }
    __reg_op_func(READ_INT_FROM_INI_FILE, READ_INT_FROM_INI_FILE);
    __reg_op_func(WRITE_INT_TO_INI_FILE, WRITE_INT_TO_INI_FILE);
    __reg_op_func(READ_FLOAT_FROM_INI_FILE, READ_FLOAT_FROM_INI_FILE);
    __reg_op_func(WRITE_FLOAT_TO_INI_FILE, WRITE_FLOAT_TO_INI_FILE);
    __reg_op_func(READ_STRING_FROM_INI_FILE, READ_STRING_FROM_INI_FILE);
    __reg_op_func(WRITE_STRING_TO_INI_FILE, WRITE_STRING_TO_INI_FILE);
}