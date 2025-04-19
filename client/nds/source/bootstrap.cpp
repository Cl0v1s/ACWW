#include "../../../common/bootstrap.h"

#include <nds.h>
#include <sys/stat.h>
#include "../nds_loader_arm9.h"


void setNDSBootstrapIni(const Config &config) {
    #ifdef ARM9
        const char* iniPath = config.bootstrapINI.c_str();
        struct stat info;
        if (stat(iniPath, &info) != 0)
        {
            consolef("Unable to locate nds-bootstrap ini file.\nPlease check your config file.\n");
            dsExit(1);
            return;
        }
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
    const char* bootstrapPath = config.bootstrapNDS.c_str();
    struct stat info;
    if (stat(bootstrapPath, &info) != 0)
    {
        consolef("Unable to locate nds-bootstrap.\nPlease check your config file.\n");
        dsExit(1);
        return;
    }
    setNDSBootstrapIni(config);
    int argc = 1;
    const char* argv[1];
    argv[0] = bootstrapPath;
    int ret = runNdsFile(bootstrapPath, argc, argv);
    if(ret == 1) {
        consolef("Unable to load ROM.\nPlease check your config file.\n");
    } else if(ret == 2) {
        consolef("Unable to build command line args.\n");
    } else if(ret == 3) {
        consolef("Unable to patch dldi.\n");
    }
}