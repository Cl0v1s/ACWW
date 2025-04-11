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
		
		You received the following letter from %s, another villager:
		%s 

		You will write your %s answer in %s to this letter in 129 chars:
	`, townName, senderDescription, receiverName, content, tone, language)
}

type GenRequest struct {
	language     string `json:"language"`
	SenderId     string `json:"senderId"`     // villager id 002d
	ReceiverName string `json:"receiverName"` // receiver (player?) name
	TownName     string `json:"townName"`     // town name
	AttachmentId int16  `json:"attachmentId"` // id of the attached gift
	Score        int    `json:"score"`        // < 50 -> negative or confused reply
	Content      string `json:"content"`      // previous letter from the receiver
}

func gen(c *gin.Context) {
	var request GenRequest
	err := c.BindJSON(&request)
	if err != nil {
		fmt.Println(err)
		c.JSON(500, gin.H{"message": err})
		return
	}

	senderName, exists := GetVillagerName(request.SenderId)
	if !exists {
		c.JSON(404, gin.H{"message": "Villager " + request.SenderId + " does not exists."})
		return
	}

	senderDescription, err := LoadVillagerInfos(senderName)
	if err != nil {
		fmt.Println(err)
		c.JSON(500, gin.H{"message": err})
		return
	}

	prompt := buildPrompt(request.language, request.Score, request.ReceiverName, *senderDescription, request.TownName, request.Content)
	fmt.Println(prompt)
	response, err := Call(prompt)
	if err != nil {
		fmt.Println(err)
		c.JSON(500, gin.H{"message": err})
	} else {
		c.JSON(200, gin.H{"message": response.Choices[0].Message.Content})
	}
}

func main() {
	router := gin.Default()
	router.GET("/gen", gen)
	router.Run() // listen and serve on 0.0.0.0:8080
}
