#ifndef NET_H
#define NET_H

#include "./utils.h"

#include <stdint.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <locale>
#include <codecvt>

#if defined(ARM9) 

#include <nds.h>

#elif defined(__3DS)

#include <3ds.h>

#endif

#define INVALID_RESPONSE 1
#define INVALID_STATUS 2
#define INVALID_BODY 3




static inline std::string jsonEscape(const std::string &input) {
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
                output += c;
        }
    }
    return output;
}

static inline int buildBody(char* dest, const char* language, const char* senderId, const std::wstring &receiverName, const std::wstring &townName, uint16_t attachementId, int score, const std::wstring &intro, const std::wstring &body, const std::wstring &end) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string utf8Intro = jsonEscape(converter.to_bytes(intro));
    std::string utf8Body = jsonEscape(converter.to_bytes(body));
    std::string utf8End = jsonEscape(converter.to_bytes(end));
    std::string utf8TownName = jsonEscape(converter.to_bytes(townName));
    std::string utf8ReceiverName = jsonEscape(converter.to_bytes(receiverName));
    
    return sprintf(dest, 
        "{\n"
        "  \"language\": \"%s\",\n"
        "  \"senderId\": \"%s\",\n"
        "  \"receiverName\": \"%s\",\n"
        "  \"townName\": \"%s\",\n"
        "  \"attachementId\": %u,\n"
        "  \"score\": %d,\n"
        "  \"intro\": \"%s\",\n"
        "  \"body\": \"%s\",\n"
        "  \"end\": \"%s\"\n"
        "}", 
        language, senderId, utf8ReceiverName.c_str(), utf8TownName.c_str(), attachementId, score, utf8Intro.c_str(), utf8Body.c_str(), utf8End.c_str()
    );
}


static inline int buildRequest(char* dest, const char* addr, int port, const char* json) {
    return sprintf(dest,
        "GET /gen HTTP/1.0\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "Content-Length: %d\r\n"
        "Content-Type: application/json; charset=UTF-8\r\n"
        "\r\n"
        "%s",
        addr, strlen(json), json
    );
}

static inline int emit(int soc, const char* addr, int port, const char* language, const char* senderId, const std::wstring &receiverName, const std::wstring &townName, uint16_t attachementId, int score, std::wstring &intro, std::wstring &body, std::wstring &end) {
    char* raw = (char*)malloc(sizeof(char) * 1000); // big size to be sure to store everything
    buildBody(raw, language, senderId, receiverName, townName, attachementId, score, intro, body, end);
    char* json = (char*)malloc(sizeof(char) * strlen(raw) + 1); // +1 to keep the \0
    memcpy(json, raw, sizeof(char) * strlen(raw) + 1);
    buildRequest(raw, addr, port, json);
    free(json);
    char* request = (char*)malloc(sizeof(char) * strlen(raw) + 1); // +1 to keep the \0
    memcpy(request, raw, sizeof(char) * strlen(raw) + 1);
    free(raw);

    // consolef("Sending:\n");
    // consolef(request);
    // consolef("\n");

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

    // consolef("Sent %d bytes\n", totalSent);
    free(request);
    return 0;
}

static inline int receive(int soc, std::wstring &answer) {
    char buffer[1024];
    int bytesRead;
    std::string response;

    // Read the response from the server
    while ((bytesRead = recv(soc, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the buffer
        response += buffer;
        // consolef("%s\n", response.c_str());
    }
    if (bytesRead < 0 && errno == EWOULDBLOCK) {
        consolef("Timeout reached while waiting for server response.\n");
        return -1;
    }

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

    // received body is in extended ASCII so we pad each byte with anoth byte to make it utf16
    for(unsigned int i = 0; i < body.length(); i++) {
        wchar_t c = 0x0000 | body[i];
        answer += c;
    }

    int statusCode = std::stoi(headers.substr(statusStart, statusEnd - statusStart));
    if (statusCode != 200) {
        errno = statusCode;
        return -1;
    }

    return 0;
}


class Net {
    protected:
        sockaddr_in remote;
        char* addr;
        int port;
    public:
        std::wstring call(const char* language, const char* senderId, const std::wstring &receiverName, const std::wstring &townName, uint16_t attachementId, int score, std::wstring &intro, std::wstring &body, std::wstring &end) {
                int soc = socket(AF_INET, SOCK_STREAM, 0);
                int result = connect(soc, (struct sockaddr *)&this->remote, sizeof(this->remote));
                if(result != 0) {
                    consolef("%d: unable to connect to %s\n%s\n", errno, addr, strerror(errno) );
                    return std::wstring(L"");
                }
                result = emit(soc, addr, port, language, senderId, receiverName, townName, attachementId, score, intro, body, end);
                if(result != 0) {
                    consolef("%d: unable to send data\n%s\n", errno, strerror(errno) );
                    return std::wstring(L"");
                }
                
                std::wstring anwser(L"");
                result = receive(soc, anwser);
                if(result != 0) {
                    consolef("%d: unable to receive data\n%s\n", errno, anwser.c_str());
                    return std::wstring(L"");
                }
    
                #ifndef ARM9
                    close(soc);
                #else 
                    shutdown(soc, SHUT_RDWR);
                    closesocket(soc);
                #endif
                return anwser;
        }
        virtual ~Net() {};
};

void initNet(const char* server, int port);

Net* getNet();

void closeNet();



#endif 