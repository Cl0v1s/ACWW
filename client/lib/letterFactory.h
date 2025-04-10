#ifndef LETTERFACTORY_H
#define LETTERFACTORY_H

#include <string>
#include <stdint.h>


#include "./letter.h"
#include "utils.h"

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
        void GenerateBody(Letter &letter, Letter & answer) {
            answer.SetIntroPart(std::string("Pour "));
            answer.SetBodyPart(std::string("Covfefe"));
            answer.SetEndPart(std::string("Ton voisin"));
        }

        /**
         * Generate attachement from player's letter
         */
        uint16_t GetAttachement(const Letter &letter) {
            return 0x0000;
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

            answer.SetSenderPlayerId(0x2D);
            answer.SetSenderPlayerName(std::string("Nonos"));
            answer.SetSenderTownId(letter.GetReceiverTownId());
            answer.SetSenderTownName(letter.GetReceiverTownName());

            this->GenerateBody(letter, answer);

            answer.SetAttachementId(this->GetAttachement(letter));
            answer.setPaperId(this->GetPaper());
            answer.SetFlags(FLAG_CREATED);
            answer.SetIntroIndex(0);

            printf("Answer:\n");
            print(answer);
            return answer;
        }
};

#endif 