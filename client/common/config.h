#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include "files.h"

#define CONFIG_FIELDS 8

typedef struct {
    /**
     * Remote server address
     */
    std::string server;
    /**
     * Remote server port
     */
    int port;
    /**
     * The language in which the villagers should response
     */
    std::string lang;
    /**
     * True if we should start AC:WW with nds-bootloader when the save edit is done
     * This require nds-bootloader to be installed at the root of your SD card, in the _nds folder as stated in their documentation
     */
    bool launcher;
    /**
     * Path to nds-bootstrap (retail)
     */
    std::string bootstrapNDS;
    /**
     * Path to nds-bootstrap ini file
     */
    std::string bootstrapINI;
    /**
     * Path to the AC:WW rom
     */
    std::string rom;
    /**
     * Path to the AC:WW save
     */
    std::string save;
} Config;

void printConfig(const Config &config) {
    consolef("config:\nserver = %s:%d\nlang = %s\nrom = %s\nsave = %s\nlauncher = %d\n", config.server.c_str(), config.port, config.lang.c_str(), config.rom.c_str(), config.save.c_str(), config.launcher);
}

Config loadConfig() {
    Config config;
    int length = 1024;
    char ccontent[length];
    if(!readFile("./ac.txt", ccontent, length)) {
        consolef("Unable to load config file, please create and setup your configuration.\n");
        dsExit(1);
    }
    std::string content(ccontent);
    size_t pos = 0;
    size_t lastPos = 0;

    std::string parts[CONFIG_FIELDS];
    int index = 0;
    while(index < CONFIG_FIELDS && (pos = content.find("\n", lastPos)) != std::string::npos) {
        parts[index] = content.substr(lastPos, pos - lastPos);
        // consolef("%d %zu %zu %s\n", index, lastPos, pos, parts[index].c_str());
        lastPos = pos + 1;
        index += 1;
    }
    if(index < CONFIG_FIELDS) {
        consolef("Malformed config file.\n");
        dsExit(1);
    }

    config.server = parts[0];
    config.port = (int)strtoll(parts[1].c_str(), NULL, 10);
    config.lang = parts[2];
    config.launcher = parts[3].compare("true") == 0;
    config.bootstrapNDS = parts[4];
    config.bootstrapINI = parts[5];
    config.rom = parts[6];
    config.save = parts[7];

    return config;
}


#endif 