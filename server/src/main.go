package main

import (
	"fmt"

	"github.com/gin-gonic/gin"
)

func buildPrompt(score int, receiverName string, senderDescription string, townName string, content string) string {
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
		You will write your %s answer to this letter in 129 chars:
	`, townName, senderDescription, receiverName, content, tone)
}

type GenRequest struct {
	SenderId      string `json:"senderId"`      // villager id
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
	prompt := buildPrompt(request.Score, request.ReceiverName, "", request.TownName, request.Content)
	response, error := call(prompt)
	if error != nil {
		c.JSON(500, gin.H{"error": error})
	} else {
		c.JSON(200, gin.H{"content": response.Choices[0].Message.Content})
	}
}

func main() {
	router := gin.Default()
	router.GET("/gen", gen)
	router.Run() // listen and serve on 0.0.0.0:8080
}
