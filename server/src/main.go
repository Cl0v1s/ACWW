package main

import (
	"fmt"
	"strings"
	"unicode/utf8"

	"github.com/gin-gonic/gin"
	"golang.org/x/text/encoding/charmap"
)

func buildPrompt(language string, score int, receiverName string, senderDescription string, townName string, content string) string {
	var tone string
	if score > 50 {
		tone = "nice or positive"
	} else {
		tone = "confused or negative"
	}

	return fmt.Sprintf(`
		As a villager residing in the town of %s in Animal Crossing, you are known for the following traits:
		%s

		You have received a letter from %s, another villager, which reads:
		%s 

		Compose a %s reply in %s to this letter. Only answer with the letter. Use the following structure:
		- Greetings,<line break>
		- Body adhering to a 100-character limit<line break>
		- Closing
	`, townName, senderDescription, receiverName, content, tone, language)
}

type GenRequest struct {
	Language     string `json:"language"`
	SenderId     string `json:"senderId"`     // villager id 002d
	ReceiverName string `json:"receiverName"` // receiver (player?) name
	TownName     string `json:"townName"`     // town name
	AttachmentId int16  `json:"attachmentId"` // id of the attached gift
	Score        int    `json:"score"`        // < 50 -> negative or confused reply
	Intro        string `json:"intro"`        // previous letter from the receiver
	Body         string `json:"body"`         // previous letter from the receiver
	End          string `json:"end"`          // previous letter from the receiver
}

func gen(c *gin.Context) {
	var request GenRequest
	err := c.BindJSON(&request)
	if err != nil {
		fmt.Println(err)
		c.String(500, err.Error())
		return
	}

	senderName, exists := GetVillagerName(request.SenderId)
	if !exists {
		c.String(404, "Villager "+request.SenderId+" does not exists.")
		return
	}

	senderDescription, err := LoadVillagerInfos(senderName)
	if err != nil {
		fmt.Println(err)
		c.String(500, err.Error())
		return
	}

	prompt := buildPrompt(request.Language, request.Score, request.ReceiverName, *senderDescription, request.TownName, request.Intro+"\n"+request.Body+"\n"+request.End)
	fmt.Println(prompt)
	response, err := Call(prompt)
	if err != nil {
		fmt.Println(err)
		c.String(500, err.Error())
	} else {
		reply := response.Choices[0].Message.Content

		// we cant trust llms regarding response lengt
		parts := strings.Split(reply, "\n\n")
		body := parts[1]
		for utf8.RuneCountInString(body) > 100 {
			bodyparts := strings.Split(body, ".")
			bodyparts = bodyparts[:len(bodyparts)-1]
			body = strings.Join(bodyparts, ".")
		}
		parts[1] = body

		reply = strings.Join(parts, "\n\n")

		encoder := charmap.ISO8859_1.NewEncoder()
		iso8859Text, err := encoder.String(reply)
		if err != nil {
			fmt.Println(err)
			c.String(500, err.Error())
			return
		}
		c.Header("Content-Type", "text/plain; charset=ISO-8859-1")
		c.String(200, iso8859Text)
	}
}

func main() {
	router := gin.Default()
	router.GET("/gen", gen)
	router.Run() // listen and serve on 0.0.0.0:8080
}
