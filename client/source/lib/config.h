#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include "./files.h"

#define CONFIG_FIELDS 4

typedef struct {
    std::string server;
    int port;
    std::string lang;
    std::string path;
} Config;

void printConfig(const Config &config) {
    consolef("config: %s:%d %s %s\n", config.server.c_str(), config.port, config.lang.c_str(), config.path.c_str());
}

Config loadConfig() {
    Config config;
    char* ccontent;
    if(!readFile("./ac.config", &ccontent)) {
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
    config.path = parts[3];

    return config;
}


#endif 