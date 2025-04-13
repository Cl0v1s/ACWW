#ifndef LETTERFACTORY_H
#define LETTERFACTORY_H

#include <string>
#include <stdint.h>


#include "./letter.h"
#include "utils.h"
#include "./net.h"

Net net("127.0.0.1", 8080);

class LetterFactory {
    private:
        /**
         * Get a random paper for the answer
         */
        unsigned char GetPaper() {
            return 0x37;
        }

        /**
         * Generate answer content from player's letter
         */
        void GenerateBody(Letter &letter, Letter &answer) {
            char senderId[10]; 
            sprintf(senderId, "%04x", answer.GetSenderPlayerId());

            std::string intro = letter.GetIntroPart();
            std::string body = letter.GetBodyPart();
            std::string end = letter.GetEndPart();

            std::string reply = net.call("french", senderId, answer.GetReceiverPlayerName().c_str(), answer.GetSenderTownName().c_str(), letter.GetAttachementId(), 100, intro, body, end);
            if(reply.length() == 0) {
                consolef("Unable to generate reply\n");
                return;
            }

            for (size_t i = 0; i < reply.length() - 1; ++i) {
                if (reply[i] == '\n' && reply[i + 1] == '\n') {
                    reply.erase(i, 1);
                    --i;
                }
            }

            size_t firstNewline = reply.find('\n');
            size_t secondNewline = reply.find('\n', firstNewline + 1);
            std::string introPart = reply.substr(0, firstNewline);
            std::string bodyPart = reply.substr(firstNewline + 1, secondNewline - firstNewline - 1);
            std::string endPart = reply.substr(secondNewline + 1);

            answer.SetIntroPart(introPart);
            answer.SetBodyPart(bodyPart);
            answer.SetEndPart(endPart);
        }

        /**
         * Generate attachement from player's letter
         */
        uint16_t GetAttachement(const Letter &letter) {
            return NO_ATTACHEMENT;
        }

    public:
        /**
         * Generate answer letter from player's letter
         */
        Letter Answer(Letter &letter, char* save, int offset, LetterStruct* region) {
            Letter answer(save, offset, region);
            // back to the sender from the receiver
            answer.SetReceiverPlayerId(letter.GetSenderPlayerId());
            answer.SetReceiverPlayerName(letter.GetSenderPlayerName());
            answer.SetReceiverTownId(letter.GetSenderTownId());
            answer.SetReceiverTownName(letter.GetSenderTownName());

            answer.SetSenderPlayerId(letter.GetReceiverPlayerId());
            answer.SetSenderPlayerName(letter.GetReceiverPlayerName());
            answer.SetSenderTownId(letter.GetReceiverTownId());
            answer.SetSenderTownName(letter.GetReceiverTownName());

            this->GenerateBody(letter, answer);

            answer.SetAttachementId(this->GetAttachement(letter));
            answer.setPaperId(this->GetPaper());
            answer.SetFlags(FLAG_UNREAD);
            // answer.SetIntroFlag(INSERT_NAME_INTRO);
            answer.SetNameFlag(INSERT_NAME_INVENTORY);

            return answer;
        }
};

#endif 