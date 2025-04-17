#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include <sys/stat.h>

#ifdef ARM9

#include <nds.h>
#include "../nds_loader_arm9.h"

#endif

#include "config.h"

#define BOOTSTRAP_NAMES_LENGTH 6
const char* BOOTSTRAP_ROOT = "sd:/_nds/";

const int getNDSBootstrapPath(char* name) {
    const char* NDS_NAMES[BOOTSTRAP_NAMES_LENGTH];
    NDS_NAMES[0] = "nds-bootstrap";
    NDS_NAMES[1] = "nds-bootstrap-release";
    NDS_NAMES[2] = "nds-bootstrap-hb";
    NDS_NAMES[3] = "nds-bootstrap-hb-release";
    NDS_NAMES[4] = "nds-bootstrap-retail";
    NDS_NAMES[5] = "nds-bootstrap-retail-release";

    bool done = false;
    int i = 0;
    std::string path;
    struct stat stats;
    while(done == false && i < BOOTSTRAP_NAMES_LENGTH) {
        path = std::string(BOOTSTRAP_ROOT) + std::string(NDS_NAMES[i]) + ".nds";
        if(stat(path.c_str(), &stats) != -1) {
            done = true;            
        } else {
            i += 1;
        }
    }
    if(i >= BOOTSTRAP_NAMES_LENGTH && done == false) {
        return -1;
    }
    size_t len = strlen(path.c_str());
    memccpy(name, path.c_str(), sizeof(char), len);
    name[len] = '\0';
    return 0;
}


void setNDSBootstrapIni(const Config &config) {
    #ifdef ARM9
        const char* iniPath = (std::string(BOOTSTRAP_ROOT) + "nds-bootstrap.ini").c_str();
        FILE* iniFile = fopen(iniPath, "r+");
        if (iniFile == NULL) {
            consolef("Failed to open %s\n", iniPath);
            return;
        }

        std::vector<std::string> lines;
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), iniFile)) {
            lines.push_back(buffer);
        }
        fclose(iniFile);

        iniFile = fopen(iniPath, "w");
        if (iniFile == NULL) {
            consolef("Failed to open %s for writing\n", iniPath);
            return;
        }

        for (auto &line : lines) {
            if (strncmp(line.c_str(), "NDS_PATH=", 9) == 0 || strncmp(line.c_str(), "NDS_PATH =", 10) == 0) {
                fprintf(iniFile, "NDS_PATH = %s\n", config.rom.c_str());
                consolef("NDS_PATH = %s\n", config.rom.c_str());
            } else if (strncmp(line.c_str(), "SAV_PATH=", 9) == 0 || strncmp(line.c_str(), "SAV_PATH =", 10) == 0) {
                fprintf(iniFile, "SAV_PATH = %s\n", config.save.c_str());
            } else {
                fprintf(iniFile, "%s", line.c_str());
            }
        }

        fclose(iniFile);
    #endif
}

void dsStartWW(const Config &config) {
    #ifdef ARM9
    char bootstrapPath[80];
    if(getNDSBootstrapPath(bootstrapPath) != 0) {
        consolef("Unable to locate nds-boostrap.\nEnsure it is located at the root if your sdcard, in the _nds folder.\n");
        dsExit(1);
    }
    setNDSBootstrapIni(config);
    int argc = 1;
    const char* argv[1];
    argv[0] = bootstrapPath;
    runNdsFile(bootstrapPath, argc, argv);
    #endif
}


#endif