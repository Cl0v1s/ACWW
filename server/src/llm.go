package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"time"
	"unicode/utf8"
)

// Définition des types Go pour la réponse de l'API
type Message struct {
	Role    string `json:"role"`
	Content string `json:"content"`
}

type Choice struct {
	Index        int     `json:"index"`
	Message      Message `json:"message"`
	FinishReason string  `json:"finish_reason"`
}

type Usage struct {
	PromptTokens     int `json:"prompt_tokens"`
	CompletionTokens int `json:"completion_tokens"`
	TotalTokens      int `json:"total_tokens"`
}

type APIResponse struct {
	ID      string   `json:"id"`
	Object  string   `json:"object"`
	Created int64    `json:"created"`
	Model   string   `json:"model"`
	Choices []Choice `json:"choices"`
	Usage   Usage    `json:"usage"`
}

func Call(prompt string) (*APIResponse, error) {
	url := "https://api.mistral.ai/v1/chat/completions"
	apiKey := os.Getenv("API_KEY")
	data := map[string]interface{}{
		"model":      "mistral-small-latest",
		"max_tokens": utf8.RuneCountInString(prompt) + 129, // 129 is the max length of a letter in the game
		"messages": []map[string]string{
			{"role": "user", "content": prompt},
		},
	}

	jsonData, _ := json.Marshal(data)
	req, _ := http.NewRequest("POST", url, bytes.NewBuffer(jsonData))
	req.Header.Set("Content-Type", "application/json")
	req.Header.Set("Accept", "application/json")
	req.Header.Set("Authorization", "Bearer "+apiKey)

	client := &http.Client{}
	resp, err := client.Do(req)
	if err != nil {
		fmt.Println("Error:", err)
		return nil, err
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("non-200 status code: %d", resp.StatusCode)
	}

	time.Sleep(2 * time.Second) // we dont want to spam them

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}

	var response APIResponse
	err = json.Unmarshal(body, &response)
	if err != nil {
		return nil, err
	}

	return &response, nil
}
