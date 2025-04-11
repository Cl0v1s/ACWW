package main

import (
	"fmt"
	"os"
)

func getVillagerName(id string) (string, bool) {
	idMap := map[string]string{
		"0000": "Cyrano",
		"0001": "Antonio",
		"0002": "Pango",
		"0003": "Anabelle",
		"0004": "Teddy",
		"0005": "Pinky",
		"0006": "Curt",
		"0007": "Chow",
		"0008": "Jay",
		"0009": "Robin",
		"000a": "Anchovy",
		"000b": "Twiggy",
		"000c": "Jitters",
		"000d": "Angus",
		"000e": "Rodeo",
		"000f": "Bob",
		"0010": "Mitzi",
		"0011": "Rosie",
		"0012": "Olivia",
		"0013": "Kiki",
		"0014": "Tangy",
		"0015": "Punchy",
		"0016": "Purrl",
		"0017": "Moe",
		"0018": "Kabuki",
		"0019": "Kid Cat",
		"001a": "Monique",
		"001b": "Tabby",
		"001c": "Bluebear",
		"001d": "Maple",
		"001e": "Poncho",
		"001f": "Pudge",
		"0020": "Kody",
		"0021": "Stitches",
		"0022": "Goose",
		"0023": "Benedict",
		"0024": "Egbert",
		"0025": "Patty",
		"0026": "Tipper",
		"0027": "Alfonso",
		"0028": "Ali",
		"0029": "Goldie",
		"002a": "Butch",
		"002b": "Lucky",
		"002c": "Biskit",
		"002d": "Bones",
		"002e": "Portia",
		"002f": "Walker",
		"0030": "Daisy",
		"0031": "Bill",
		"0032": "Joey",
		"0033": "Pate",
		"0034": "Maelle",
		"0035": "Deena",
		"0036": "Pompom",
		"0037": "Mallary",
		"0038": "Freckles",
		"0039": "Derwin",
		"003a": "Drake",
		"003b": "Opal",
		"003c": "Dizzy",
		"003d": "Big Top",
		"003e": "Eloise",
		"003f": "Margie",
		"0040": "Lily",
		"0041": "Ribbot",
		"0042": "Frobert",
		"0043": "Camofrog",
		"0044": "Drift",
		"0045": "Wart Jr.",
		"0046": "Puddles",
		"0047": "Jeremiah",
		"0048": "Chevre",
		"0049": "Nan",
		"004a": "Cesar",
		"004b": "Peewee",
		"004c": "Boone",
		"004d": "Rocco",
		"004e": "Buck",
		"004f": "Victoria",
		"0050": "Savannah",
		"0051": "Elmer",
		"0052": "Roscoe",
		"0053": "Yuka",
		"0054": "Alice",
		"0055": "Melba",
		"0056": "Kitt",
		"0057": "Mathilda",
		"0058": "Bud",
		"0059": "Elvis",
		"005a": "Dora",
		"005b": "Limberg",
		"005c": "Bella",
		"005d": "Bree",
		"005e": "Samson",
		"005f": "Rod",
		"0060": "Octavian",
		"0061": "Marina",
		"0062": "Queenie",
		"0063": "Gladys",
		"0064": "Apollo",
		"0065": "Amelia",
		"0066": "Pierce",
		"0067": "Aurora",
		"0068": "Roland",
		"0069": "Cube",
		"006a": "Hopper",
		"006b": "Friga",
		"006c": "Gwen",
		"006d": "Curly",
		"006e": "Truffles",
		"006f": "Rasher",
		"0070": "Hugh",
		"0071": "Lucy",
		"0072": "Bunnie",
		"0073": "Dotty",
		"0074": "Coco",
		"0075": "Snake",
		"0076": "Gatson",
		"0077": "Gabi",
		"0078": "Pippy",
		"0079": "Tiffany",
		"007a": "Gengi",
		"007b": "Ruby",
		"007c": "Tank",
		"007d": "Rhoda",
		"007e": "Vesta",
		"007f": "Baabara",
		"0080": "Peanut",
		"0081": "Blaire",
		"0082": "Filbery",
		"0083": "Pecan",
		"0084": "Nibbles",
		"0085": "Agent S",
		"0086": "Caroline",
		"0087": "Sally",
		"0088": "Static",
		"0089": "Mint",
		"008a": "Rolf",
		"008b": "Rowan",
		"008c": "Chief",
		"008d": "Lobo",
		"008e": "Wolfgang",
		"008f": "Whitney",
		"0090": "Champ",
		"0091": "Nana",
		"0092": "Simon",
		"0093": "Tammi",
		"0094": "Monty",
		"0095": "Elise",
	}
	name, exists := idMap[id]
	return name, exists
}

func loadVillagerInfos(name string) (*string, error) {
	file := name + ".md"
	if _, err := os.Stat(file); os.IsNotExist(err) {
		prompt := fmt.Sprintf("Tell me about %s in Animal Crossing", name)
		response, err := call(prompt)
		if err != nil {
			return nil, err
		}
		infos := response.Choices[0].Message.Content
		saveVillagerInfos(name, infos)
		return &infos, nil
	}
	buffer, err := os.ReadFile(file)
	if err != nil {
		return nil, err
	}
	infos := string(buffer)
	return &infos, nil
}

func saveVillagerInfos(name string, infos string) error {
	filename := name + ".md"
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	_, errr := file.WriteString(infos)
	if errr != nil {
		return errr
	}
	return nil
}
