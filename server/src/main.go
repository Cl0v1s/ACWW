package main

import (
	"fmt"

	"github.com/gin-gonic/gin"
)

func buildPrompt(language string, score int, receiverName string, senderDescription string, townName string, content string) string {
	var tone string
	if score > 50 {
		tone = "nice or positive"
	} else {
		tone = "confused or negative"
	}

	return fmt.Sprintf(`
		You are a villager living in a town named %s in Animal Crossing.
		Here is a description of your character: 
		%s
		You received a letter from %s, another villager:
		%s 
		You will write your %s answer in %s to this letter in 129 chars:
	`, townName, senderDescription, receiverName, content, tone, language)
}

type GenRequest struct {
	language      string `json:"language"`
	SenderId      string `json:"senderId"`      // villager id 002d
	ReceiverName  string `json:"receiverName"`  // receiver (player?) name
	TownName      string `json:"townName"`      // town name
	AttachementId int16  `json:"attachementId"` // id of the attached gift
	Score         int    `json:"score"`         // < 50 -> negative or confused reply
	Content       string `json:"body"`          // previous letter from the receiver
}

func gen(c *gin.Context) {
	var request GenRequest
	err := c.BindJSON(&request)
	if err != nil {
		return
	}

	senderName, exists := getVillagerName(request.SenderId)
	if !exists {
		c.JSON(404, gin.H{"message": "Villager " + request.SenderId + " does not exists."})
		return
	}

	senderDescription, err := loadVillagerInfos(senderName)
	if err != nil {
		c.JSON(500, gin.H{"message": err})
		return
	}

	prompt := buildPrompt(request.language, request.Score, request.ReceiverName, *senderDescription, request.TownName, request.Content)
	response, error := call(prompt)
	if error != nil {
		c.JSON(500, gin.H{"message": error})
	} else {
		c.JSON(200, gin.H{"message": response.Choices[0].Message.Content})
	}
}

func main() {
	router := gin.Default()
	router.GET("/gen", gen)
	router.Run() // listen and serve on 0.0.0.0:8080
}
