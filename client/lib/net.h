#ifndef NET_H
#define NET_H

#include <stdint.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <unistd.h>
// Linux includes 

#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#define INVALID_RESPONSE 1
#define INVALID_STATUS 2
#define INVALID_BODY 3

std::string jsonEscape(const std::string &input) {
    std::string output;
    for (char c : input) {
        switch (c) {
            case '\"': output += "\\\""; break;
            case '\\': output += "\\\\"; break;
            case '\b': output += "\\b"; break;
            case '\f': output += "\\f"; break;
            case '\n': output += "\\n"; break;
            case '\r': output += "\\r"; break;
            case '\t': output += "\\t"; break;
            default:
                if (c >= 0x20 && c <= 0x7E) {
                    output += c;
                }
        }
    }
    return output;
}

int buildBody(char* dest, const char* language, const char* senderId, const char* receiverName, const char* townName, uint16_t attachementId, uint8_t score, std::string &intro, std::string &body, std::string &end) {

    intro = jsonEscape(intro);
    body = jsonEscape(body);
    end = jsonEscape(end);
    
    return sprintf(dest, 
        "{\n"
        "  \"language\": \"%s\",\n"
        "  \"senderId\": \"%s\",\n"
        "  \"receiverName\": \"%s\",\n"
        "  \"townName\": \"%s\",\n"
        "  \"attachementId\": %u,\n"
        "  \"score\": %u,\n"
        "  \"intro\": \"%s\",\n"
        "  \"body\": \"%s\",\n"
        "  \"end\": \"%s\"\n"
        "}\0", 
        language, senderId, receiverName, townName, attachementId, score, intro.c_str(), body.c_str(), end.c_str()
    );
}


int buildRequest(char* dest, const char* addr, int port, const char* json) {
    return sprintf(dest,
        "GET /gen HTTP/1.0\r\n"
        "Host: %s:%d\r\n"
        "User-Agent: animal-crossing\r\n"
        "Accept: application/json\r\n"
        "Content-Length: %d\r\n"
        "Content-Type: application/json\r\n"
        "\r\n"
        "%s\0",
        addr, port, strlen(json), json
    );
}

int emit(int soc, const char* addr, int port, const char* language, const char* senderId, const char* receiverName, const char* townName, uint16_t attachementId, uint8_t score, std::string &intro, std::string &body, std::string &end) {
    char* raw = (char*)malloc(sizeof(char) * 1000); // big size to be sure to store everything
    buildBody(raw, language, senderId, receiverName, townName, attachementId, score, intro, body, end);
    char* json = (char*)malloc(sizeof(char) * strlen(raw) + 1); // +1 to keep the \0
    memcpy(json, raw, sizeof(char) * strlen(raw) + 1);
    buildRequest(raw, addr, port, json);
    free(json);
    char* request = (char*)malloc(sizeof(char) * strlen(raw) + 1); // +1 to keep the \0
    memcpy(request, raw, sizeof(char) * strlen(raw) + 1);
    free(raw);

    // printf("Sending:\n");
    // printf(request);
    // printf("\n");

    int bytesSent;

    size_t totalSent = 0;
    size_t requestLength = strlen(request);
    while (totalSent < requestLength) {
        bytesSent = send(soc, request + totalSent, requestLength - totalSent, 0);
        if (bytesSent < 0) {
            free(request);
            return -1;
        }
        totalSent += bytesSent;
    }

    // printf("Sent %d bytes\n", totalSent);
    free(request);
    return 0;
}

int receive(int soc, std::string &answer) {
    char buffer[1024];
    int bytesRead;
    std::string response;

    // Read the response from the server
    while ((bytesRead = recv(soc, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the buffer
        response += buffer;
    }

    printf("%s\n", response.c_str());

    if (bytesRead < 0) {
        return -1;
    }

    // Parse the HTTP response
    size_t headerEnd = response.find("\r\n\r\n");
    if (headerEnd == std::string::npos) {
        errno = INVALID_RESPONSE;
        return -1;
    }

    std::string headers = response.substr(0, headerEnd);
    std::string body = response.substr(headerEnd + 4);

    // Check the HTTP status code
    size_t statusStart = headers.find(" ") + 1;
    size_t statusEnd = headers.find(" ", statusStart);
    if (statusStart == std::string::npos || statusEnd == std::string::npos) {
        errno = INVALID_STATUS;
        return -1;
    }

    answer += body;

    int statusCode = std::stoi(headers.substr(statusStart, statusEnd - statusStart));
    if (statusCode != 200) {
        errno = statusCode;
        return -1;
    }

    return 0;
}


class Net {
    public:
        virtual std::string call(const char* language, const char* senderId, const char* receiverName, const char* townName, uint16_t attachementId, uint8_t score, std::string &intro, std::string &body, std::string &end) = 0;
};

class NetComputerImpl: public Net {
    private:
        sockaddr_in remote;
        char* addr;
        int port;

    public:
        NetComputerImpl(const char* _addr, int _port) {
            addr = (char*)malloc(sizeof(char) * strlen(_addr));
            memcpy(addr, _addr, sizeof(char) * strlen(_addr));
            remote.sin_family = AF_INET;
            remote.sin_port = htons(_port);
            int res = inet_pton(AF_INET, _addr, &remote.sin_addr);
            if(res != 1) {
                printf("addr is not a valid address.\n");
                exit(1);
            }
        }

        ~NetComputerImpl() {
            free(addr);
        }

        std::string call(const char* language, const char* senderId, const char* receiverName, const char* townName, uint16_t attachementId, uint8_t score, std::string &intro, std::string &body, std::string &end) override {
            int soc = socket(AF_INET, SOCK_STREAM, 0);
            int result = connect(soc, (struct sockaddr *)&this->remote, sizeof(this->remote));
            if(result != 0) {
                printf("%d: unable to connect to %s\n%s\n", errno, addr, strerror(errno) );
                return std::string("");
            }
            result = emit(soc, addr, port, language, senderId, receiverName, townName, attachementId, score, intro, body, end);
            if(result != 0) {
                printf("%d: unable to send data\n%s\n", errno, strerror(errno) );
                return std::string("");
            }
            
            std::string anwser("");
            result = receive(soc, anwser);
            if(result != 0) {
                printf("%d: unable to receive data\n%s\n", errno, anwser);
                return std::string("");
            }

            shutdown(soc, SHUT_RDWR);
            close(soc);
            return anwser;
        }
};

class NetDSImpl: public Net {
    public:
        std::string call(const char* language, const char* senderId, const char* receiverName, const char* townName, uint16_t attachementId, uint8_t score, std::string &intro, std::string &body, std::string &end) override {
            // Implementation for DS
            return std::string();
        }
};


#endif 