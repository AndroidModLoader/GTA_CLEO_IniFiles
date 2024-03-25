#include <mod/amlmod.h>
#include <mod/logger.h>

#include <fstream>

#include "thirdparty/inipp.h"

#include "cleo.h"
cleo_ifs_t* cleo = nullptr;

#include "cleoaddon.h"
cleo_addon_ifs_t* cleoaddon = nullptr;

MYMOD(net.alexblade.rusjj.inifiles, CLEO4 IniFiles, 1.3, Alexander Blade & RusJJ)
BEGIN_DEPLIST()
    ADD_DEPENDENCY_VER(net.rusjj.cleolib, 2.0.1.6)
END_DEPLIST()

#define CLEO_RegisterOpcode(x, h) cleo->RegisterOpcode(x, h); cleo->RegisterOpcodeFunction(#h, h)
#define CLEO_Fn(h) void h (void *handle, uint32_t *ip, uint16_t opcode, const char *name)  

std::string sConfigsRoot;
static char szConvertedValue[16];

CLEO_Fn(READ_INT_FROM_INI_FILE)
{
    inipp::Ini<char> ini;
    char filename[128], section[64], key[64];
    int result = 0, i = 0;
    cleoaddon->ReadString(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleoaddon->ReadString(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleoaddon->ReadString(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sConfigsRoot + filename).c_str());
    if(is.is_open())
    {
        ini.parse(is);
        inipp::get_value(ini.sections[section], key, result);
        is.close();
    }
    cleo->GetPointerToScriptVar(handle)->i = result;
}

CLEO_Fn(WRITE_INT_TO_INI_FILE)
{
    inipp::Ini<char> ini;
    char filename[128], section[64], key[64];
    int i = 0, value = cleo->ReadParam(handle)->i;
    cleoaddon->ReadString(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleoaddon->ReadString(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleoaddon->ReadString(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sConfigsRoot + filename).c_str());
    if(is.is_open())
    {
        ini.parse(is);
        is.close();
    }
    else ini.clear();

    sprintf(szConvertedValue, "%d", value);
    ini.sections[section][key] = szConvertedValue;

    std::ofstream os((sConfigsRoot + filename).c_str());
    if(os.is_open())
    {
        ini.generate(os);
        os.flush();
        os.close();
    }
}

CLEO_Fn(READ_FLOAT_FROM_INI_FILE)
{
    inipp::Ini<char> ini;
    char filename[128], section[64], key[64];
    float result = 0.0f; int i = 0;
    cleoaddon->ReadString(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleoaddon->ReadString(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleoaddon->ReadString(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sConfigsRoot + filename).c_str());
    if(is.is_open())
    {
        ini.parse(is);
        inipp::get_value(ini.sections[section], key, result);
        is.close();
    }
    cleo->GetPointerToScriptVar(handle)->f = result;
}

CLEO_Fn(WRITE_FLOAT_TO_INI_FILE)
{
    inipp::Ini<char> ini;
    char filename[128], section[64], key[64];
    int i = 0; float value = cleo->ReadParam(handle)->f;
    cleoaddon->ReadString(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleoaddon->ReadString(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleoaddon->ReadString(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sConfigsRoot + filename).c_str());
    if(is.is_open())
    {
        ini.parse(is);
        is.close();
    }
    else ini.clear();

    sprintf(szConvertedValue, "%f", value);
    ini.sections[section][key] = szConvertedValue;

    std::ofstream os((sConfigsRoot + filename).c_str());
    if(os.is_open())
    {
        ini.generate(os);
        os.flush();
        os.close();
    }
}

char valRes[100];
CLEO_Fn(READ_STRING_FROM_INI_FILE)
{
    inipp::Ini<char> ini;
    char filename[128], section[64], key[64];
    valRes[0] = 0; int i = 0;
    cleoaddon->ReadString(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleoaddon->ReadString(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleoaddon->ReadString(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sConfigsRoot + filename).c_str());
    if(is.is_open())
    {
        ini.parse(is);
        sprintf(valRes, "%s", ini.sections[section][key].c_str());
        is.close();
    }

    if(*cleoaddon->GetScriptPC(handle) > 8)
    {
        char* dst = (char*)cleo->GetPointerToScriptVar(handle);
        memcpy(dst, valRes, 15); dst[15] = 0;
    }
    else
    {
        char* dst = (char*)cleo->ReadParam(handle)->i;
        strcpy(dst, valRes);
    }
}

CLEO_Fn(WRITE_STRING_TO_INI_FILE)
{
    inipp::Ini<char> ini;
    char filename[128], section[64], key[64], value[128] {0};
    int i = 0;
    cleoaddon->ReadString(handle, value, sizeof(value));
    cleoaddon->ReadString(handle, filename, sizeof(filename)); filename[sizeof(filename)-1] = 0;
    cleoaddon->ReadString(handle, section, sizeof(section)); section[sizeof(section)-1] = 0;
    cleoaddon->ReadString(handle, key, sizeof(key)); key[sizeof(key)-1] = 0;
    while(filename[i] != 0) // A little hack
    {
        if(filename[i] == '\\') filename[i] = '/';
        ++i;
    }
    std::ifstream is((sConfigsRoot + filename).c_str());
    if(is.is_open())
    {
        ini.parse(is);
        is.close();
    }
    else ini.clear();

    ini.sections[section][key] = value;

    std::ofstream os((sConfigsRoot + filename).c_str());
    if(os.is_open())
    {
        ini.generate(os);
        os.flush();
        os.close();
    }
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
    if(!(cleoaddon = (cleo_addon_ifs_t*)GetInterface("CLEOAddon")))
    {
        logger->Error("Cannot load a mod: CLEO's Addon interface is unknown!");
        return;
    }
    
    sConfigsRoot = aml->GetAndroidDataPath();
    sConfigsRoot += "/";

    CLEO_RegisterOpcode(0x0AF0, READ_INT_FROM_INI_FILE); // 0AF0=4,%4d% = read_int_from_ini_file %1s% section %2s% key %3s%
    CLEO_RegisterOpcode(0x0AF1, WRITE_INT_TO_INI_FILE); // 0AF1=4,write_int %1d% to_ini_file %2s% section %3s% key %4s%
    CLEO_RegisterOpcode(0x0AF2, READ_FLOAT_FROM_INI_FILE); // 0AF2=4,%4d% = read_float_from_ini_file %1s% section %2s% key %3s%
    CLEO_RegisterOpcode(0x0AF3, WRITE_FLOAT_TO_INI_FILE); // 0AF3=4,write_float %1d% to_ini_file %2s% section %3s% key %4s%
    CLEO_RegisterOpcode(0x0AF4, READ_STRING_FROM_INI_FILE); // 0AF4=4,%4d% = read_string_from_ini_file %1s% section %2s% key %3s%
    CLEO_RegisterOpcode(0x0AF5, WRITE_STRING_TO_INI_FILE); // 0AF5=4,write_string %1s% to_ini_file %2s% section %3s% key %4s%
}