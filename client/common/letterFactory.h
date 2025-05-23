#ifndef LETTERFACTORY_H
#define LETTERFACTORY_H

#include <string>
#include <stdint.h>


#include "./letter.h"
#include "utils.h"
#include "./net.h"
#include "score.h"

typedef struct {
    std::wstring intro;
    std::wstring body;
    std::wstring end;
} Content;

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
        int GenerateContent(Content &content, Letter &letter, const char* lang) {
            char senderId[10]; 
            sprintf(senderId, "%04x", letter.GetReceiverPlayerId() & 0xFF); // somehow it's actually on 8bits 

            std::wstring intro = letter.GetIntroPart();
            std::wstring body = letter.GetBodyPart();
            std::wstring end = letter.GetEndPart();
            int score = calculateScore(std::string(lang), body);

            std::wstring reply = getNet()->call(lang, senderId, letter.GetSenderPlayerName(), letter.GetReceiverTownName(), letter.GetAttachementId(), score, intro, body, end);
            if(reply.length() == 0) {
                return - 1;
            }

            for (size_t i = 0; i < reply.length() - 1; ++i) {
                if (reply[i] == '\n' && reply[i + 1] == '\n') {
                    reply.erase(i, 1);
                    --i;
                }
            }

            size_t firstNewline = reply.find(L'\n');
            size_t secondNewline = reply.find(L'\n', firstNewline + 1);
            std::wstring introPart = reply.substr(0, firstNewline);
            std::wstring bodyPart = reply.substr(firstNewline + 1, secondNewline - firstNewline - 1);
            std::wstring endPart = reply.substr(secondNewline + 1);

            content.intro = introPart;
            content.body = bodyPart;
            content.end = endPart;

            return 0;
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
        Letter Answer(Letter &letter, uint8_t* save, int offset, LetterStruct* region, const char* lang) {
            Letter answer(save, offset, region);
            Content content;
            if(this->GenerateContent(content, letter, lang) == -1) {
                consolef("Unable to generate reply\n");
                return answer;
            }
            answer.SetIntroPart(content.intro);
            answer.SetBodyPart(content.body);
            answer.SetEndPart(content.end);

            // back to the sender from the receiver
            answer.SetReceiverPlayerId(letter.GetSenderPlayerId());
            answer.SetReceiverPlayerName(letter.GetSenderPlayerName());
            answer.SetReceiverTownId(letter.GetSenderTownId());
            answer.SetReceiverTownName(letter.GetSenderTownName());

            answer.SetSenderPlayerId(letter.GetReceiverPlayerId());
            answer.SetSenderPlayerName(letter.GetReceiverPlayerName());
            answer.SetSenderTownId(letter.GetReceiverTownId());
            answer.SetSenderTownName(letter.GetReceiverTownName());


            answer.SetAttachementId(this->GetAttachement(letter));
            answer.setPaperId(this->GetPaper());
            answer.SetFlags(FLAG_UNREAD);
            // answer.SetIntroFlag(INSERT_NAME_INTRO);
            answer.SetNameFlag(INSERT_NAME_INVENTORY);

            return answer;
        }
};

#endif 