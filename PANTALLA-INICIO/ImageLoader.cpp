#include "ImageLoader.h"
#include <iostream>
#include "ProtocolFunctions.h"

string button2Str(buttonAction _button)
{
    string retVal;
    
    switch (_button)
    {
        case ZOOM_BUTTON: retVal = "Zoom"; break;
        case MUTE_BUTTON: retVal = "Mute"; break;
        case UNMUTE_BUTTON: retVal = "Unmute"; break;
        case HELP_BUTTON: retVal = "Help"; break;
        case QUIT_BUTTON: retVal = "Quit"; break;
        case PASS_BUTTON: retVal = "Pass"; break;
        case CONNECT_BUTTON: retVal = "Connect"; break;
        case CONNECTING_BUTTON: retVal = "Connecting"; break;
    }
    
    return retVal;
}

ImageLoader::ImageLoader()
{
    rules.reserve(RULES_NMBR_OF_PAGES);
}

string ImageLoader::getError()
{
	return errormsg;
}
ALLEGRO_BITMAP *ImageLoader::getGuardImage()
{
	return guard;
}
ALLEGRO_BITMAP *ImageLoader::getImageP(CardName tile)
{
	return tiles[tile];
}
ALLEGRO_BITMAP *ImageLoader::getImageP(unsigned int safeNumber)
{
	return safeNumbers[safeNumber];
}
ALLEGRO_BITMAP *ImageLoader::getImageP(CardLocation guardCard)
{
	return guardCards[guardCard];
}
ALLEGRO_BITMAP *ImageLoader::getImageP(Token token)
{
	return tokens[token];
}
ALLEGRO_BITMAP *ImageLoader::getImageP(CharacterName character, bool card)
{
    ALLEGRO_BITMAP* retVal = nullptr;
    if (card == true)
        retVal = characterCards[character];
    if (card == false)
        retVal = this->character[character];
    return retVal;
}
ALLEGRO_BITMAP *ImageLoader::getImageP(Loot loot)
{
	return loots[loot];
}
ALLEGRO_BITMAP *ImageLoader::getImageP(DiceColor color, unsigned int number)
{
    if(number != 0 && number <= DICE_MAX_NUMBER)
    {
        if(color == RED_DICE)
            return redDices[number-1];
        else
            return whiteDices[number-1];
    }
    return nullptr;
}
ALLEGRO_BITMAP * ImageLoader::getImageP(buttonAction _button)
{
    return this->button[_button];
}
ALLEGRO_BITMAP *ImageLoader::getImageBackP(CardName tile)
{
	return tileBack;
}
ALLEGRO_BITMAP *ImageLoader::getImageBackP(CardLocation guardCard)
{
	return guardBack;
}
ALLEGRO_BITMAP *ImageLoader::getImageBackP(Loot loot)
{
	return lootBack;
}
vector<ALLEGRO_BITMAP *> ImageLoader::getRules()
{
    return rules;
}

bool ImageLoader::initImages()
{
	if (loadCharactersCards() && loadCharactersFigures() && loadTokens() && loadLoots() && loadGuard() && loadTiles() && loadWhiteDices() && loadRedDices() && loadButton()&& loadRules())
		initOk = true;
	else
		initOk = false;

	return initOk;
}
bool ImageLoader::loadCharactersCards()
{
	bool retVal = false;
	for (unsigned int i = (unsigned int)THE_JUICER; i <= (unsigned int)THE_PETERMAN; i++)
	{
		string fullPath = ((string)IMAGE_FOLDER + (string)CHARACTER_CARD_SUBFOLDER + character2Str((CharacterName)i) + IMAGE_EXTENSION);
		ALLEGRO_BITMAP *bitmapTemp = al_load_bitmap(fullPath.c_str());
		if (bitmapTemp != nullptr)
		{
			pair<CharacterName, ALLEGRO_BITMAP *> pairTemp((CharacterName)i, bitmapTemp);
			characterCards.insert(pairTemp);
			retVal = true;
		}
		else
		{
			errormsg = "The image: " + fullPath + " could not be loaded.\n";
			break;
		}
	}
	return retVal;
}
bool ImageLoader::loadCharactersFigures()
{
	bool retVal = false;
	for (unsigned int i = (unsigned int)THE_JUICER; i <= (unsigned int)THE_PETERMAN; i++)
	{
		string fullPath = ((string)IMAGE_FOLDER + (string)CHARACTER_FIGURES_SUBFOLDER + character2Str((CharacterName)i) + IMAGE_EXTENSION);
		ALLEGRO_BITMAP *bitmapTemp = al_load_bitmap(fullPath.c_str());
		if (bitmapTemp != nullptr)
		{
			pair<CharacterName, ALLEGRO_BITMAP *> pairTemp((CharacterName)i, bitmapTemp);
			character.insert(pairTemp);
			retVal = true;
		}
		else
		{
			errormsg = "The image: " + fullPath + " could not be loaded.\n";
			break;
		}
	}
	return retVal;
}
bool ImageLoader::loadTokens()
{
	bool retVal = false;
	for (unsigned int i = (unsigned int)STEALTH_TOKEN; i <= (unsigned int)CROW_TOKEN; i++)
	{
		string fullPath = ((string)IMAGE_FOLDER + (string)TOKENS_SUBFOLDER + token2Str((Token)i) + IMAGE_EXTENSION);
		ALLEGRO_BITMAP *bitmapTemp = al_load_bitmap(fullPath.c_str());
		if (bitmapTemp != nullptr)
		{
			pair<Token, ALLEGRO_BITMAP *> pairTemp((Token)i, bitmapTemp);
			tokens.insert(pairTemp);
			retVal = true;
		}
		else
		{
			errormsg = "The image: " + fullPath + " could not be loaded.\n";
			break;
		}
	}
	return retVal;
}
bool ImageLoader::loadLoots()
{
	bool retVal = false;
	for (unsigned int i = (unsigned int)TIARA; i <= (unsigned int)GOLD_BAR; i++)
	{
		string fullPath = ((string)IMAGE_FOLDER + (string)LOOT_SUBFOLDER + loot2Str((Loot)i) + IMAGE_EXTENSION);
		ALLEGRO_BITMAP *bitmapTemp = al_load_bitmap(fullPath.c_str());
		if (bitmapTemp != nullptr)
		{
			pair<Loot, ALLEGRO_BITMAP *> pairTemp((Loot)i, bitmapTemp);
			loots.insert(pairTemp);
			retVal = true;
		}
		else
		{
			errormsg = "The image: " + fullPath + " could not be loaded.\n";
			break;
		}
	}
	if (retVal == true)
	{
		string fullPath = ((string)IMAGE_FOLDER + (string)LOOT_SUBFOLDER +(string) "Back" + IMAGE_EXTENSION);
		lootBack = al_load_bitmap(fullPath.c_str());
		if (lootBack==nullptr)
		{
			errormsg = "The image: " + fullPath + " could not be loaded.\n";
			retVal = false;
		}
	}
	return retVal;
}
bool ImageLoader::loadButton()
{
	bool retVal = false;
	for (unsigned int i = (unsigned int)ZOOM_BUTTON; i <= (unsigned int)QUIT_BUTTON; i++)
	{
		string fullPath = ((string)IMAGE_FOLDER + (string)BUTTON_SUBFOLDER + button2Str((buttonAction)i) + IMAGE_EXTENSION);
		ALLEGRO_BITMAP *bitmapTemp = al_load_bitmap(fullPath.c_str());
		if (bitmapTemp != nullptr)
		{
			pair<buttonAction, ALLEGRO_BITMAP *> pairTemp((buttonAction)i, bitmapTemp);
			button.insert(pairTemp);
			retVal = true;
		}
		else
		{
			errormsg = "The image: " + fullPath + " could not be loaded.\n";
                        retVal= false;
                        break;
		}
	}
	return retVal;
}
bool ImageLoader::loadGuard()
{
	bool retVal = false;
	CardLocation auxLocation;
	for (auxLocation.floor = 0; auxLocation.floor < BOARD_STANDARD_FLOORS; auxLocation.floor++)
	{
		for (auxLocation.row = 0; auxLocation.row < FLOOR_RAWS; auxLocation.row++)
		{
			for (auxLocation.column = 0; auxLocation.column < FLOOR_COLUMNS; auxLocation.column++)
			{
				string fullPath = ((string)IMAGE_FOLDER + (string)GUARD_SUBFOLDER + cardLocationToProtocol(auxLocation) + IMAGE_EXTENSION);
				ALLEGRO_BITMAP *bitmapTemp = al_load_bitmap(fullPath.c_str());
				if (bitmapTemp != nullptr)
				{
					pair<CardLocation, ALLEGRO_BITMAP *> pairTemp(auxLocation, bitmapTemp);
					guardCards.insert(pairTemp);
					retVal = true;
				}
				else
				{
					errormsg = "The image: " + fullPath + " could not be loaded.\n";
					retVal = false;
					break;
				}
			}
		}
	}
	if (retVal == true)
	{
		string fullPath = ((string)IMAGE_FOLDER + (string)GUARD_SUBFOLDER + (string) "Back" + IMAGE_EXTENSION);
		guardBack = al_load_bitmap(fullPath.c_str());
		if (guardBack != nullptr)
		{
			string auxPath = ((string)IMAGE_FOLDER + (string)GUARD_SUBFOLDER + (string) "Guard" + IMAGE_EXTENSION);
			guard = al_load_bitmap(auxPath.c_str());
			if (guard == nullptr)
			{
				errormsg = "The image: " + auxPath + " could not be loaded.\n";
				retVal = false;
			}
		}
		else
		{
			errormsg = "The image: " + fullPath + " could not be loaded.\n";
			retVal = false;
		}
	}
	return retVal;

}
bool ImageLoader::loadTiles()
{
	bool retVal = false;
	for (unsigned int i = (unsigned int)ATRIUM; i <= (unsigned int)WALKAWAY; i++)
	{
		string fullPath = ((string)IMAGE_FOLDER + (string)TILES_SUBFOLDER + CardNameToStr((CardName)i) + IMAGE_EXTENSION);
		ALLEGRO_BITMAP *bitmapTemp = al_load_bitmap(fullPath.c_str());
		if (bitmapTemp != nullptr)
		{
			pair<CardName, ALLEGRO_BITMAP *> pairTemp((CardName)i, bitmapTemp);
			tiles.insert(pairTemp);
			retVal = true;
		}
		else
		{
			errormsg = "The image: " + fullPath + " could not be loaded.\n";
			break;
		}
	}
	if (retVal == true)
	{
		string fullPath = ((string)IMAGE_FOLDER + (string)TILES_SUBFOLDER + (string) "Back" + IMAGE_EXTENSION);
		tileBack = al_load_bitmap(fullPath.c_str());
		if (tileBack != nullptr)
		{
			for (unsigned int j = MIN_SAFE_NUMBER; j <= MAX_SAFE_NUMBER; j++)
			{	
				char buffer[MAX_SAFE_NUMBER];
				sprintf(buffer, "%d", j);
				string auxPath = ((string)IMAGE_FOLDER + (string)TILES_SUBFOLDER + (string) buffer + IMAGE_EXTENSION);
				ALLEGRO_BITMAP *bitmapTemp = al_load_bitmap(auxPath.c_str());
				if (bitmapTemp != nullptr)
				{
					pair<unsigned int, ALLEGRO_BITMAP *> pairTemp(j, bitmapTemp);
					safeNumbers.insert(pairTemp);
				}
				else
				{
					errormsg = "The image: " + auxPath + " could not be loaded.\n";
					retVal = false;
					break;
				}
			}
		}
		else
		{
			errormsg = "The image: " + fullPath + " could not be loaded.\n";
			retVal = false;
		}
	}
	return retVal;
}
bool ImageLoader::loadRedDices()
{
    bool retVal = false;
    for(unsigned int i=0; i< DICE_MAX_NUMBER; i++)
    {
        char buffer[DICE_MAX_NUMBER];
	sprintf(buffer, "%d", i+1);
        string fullPath = ((string)IMAGE_FOLDER + (string)DICES_SUBFOLDER + (string) "Red " + (string)buffer + IMAGE_EXTENSION);
        redDices[i] = al_load_bitmap(fullPath.c_str());
        if(redDices[i] != nullptr)
            retVal=true;
        else
        {
            errormsg = "The image: " + fullPath + " could not be loaded.\n";
            retVal = false;
            break;
        }
    }
    return retVal;
}
bool ImageLoader::loadWhiteDices()
{
    bool retVal = false;
    for(unsigned int i=0; i< DICE_MAX_NUMBER; i++)
    {
        char buffer[DICE_MAX_NUMBER];
	sprintf(buffer, "%d", i+1);
        string fullPath = ((string)IMAGE_FOLDER + (string)DICES_SUBFOLDER + (string) "White " + (string)buffer + IMAGE_EXTENSION);
        whiteDices[i] = al_load_bitmap(fullPath.c_str());
        if(whiteDices[i] != nullptr)
            retVal=true;
        else
        {
            errormsg = "The image: " + fullPath + " could not be loaded.\n";
            retVal = false;
            break;
        }
    }
    return retVal;
}

bool ImageLoader::loadRules() 
{
    bool retVal = false;
    for(unsigned int i=1; i <= RULES_NMBR_OF_PAGES; i++)
    {
        char buffer[DICE_MAX_NUMBER];
	sprintf(buffer, "%d", i);
        string fullPath = ((string)IMAGE_FOLDER + (string)RULES_SUBFOLDER + (string) "Rules" + (string)buffer + (string)".jpg");
        ALLEGRO_BITMAP *temp= al_load_bitmap(fullPath.c_str());
        if(temp!=nullptr)
        {
            rules.push_back(temp);
            retVal=true;
        }
        else
        {
            errormsg = "The image: " + fullPath + " could not be loaded.\n";
            retVal = false;
            break;
        }
    }
    return retVal;
}

void ImageLoader::destroyImages() {

    for(auto &tile : tiles)
            if(tile.second != nullptr ) al_destroy_bitmap(tile.second);
    for(auto &safeNumber : safeNumbers)
        if(safeNumber.second != nullptr) al_destroy_bitmap(safeNumber.second);
    for(auto &guardCard: guardCards)
        if(guardCard.second != nullptr) al_destroy_bitmap(guardCard.second);
    for(auto &loot : loots)
        if(loot.second != nullptr) al_destroy_bitmap(loot.second);
    for(auto &buton : button)
        if(buton.second != nullptr) al_destroy_bitmap(buton.second);
    for(auto &token : tokens)
        if(token.second != nullptr) al_destroy_bitmap(token.second);
    for(auto &characterCard : characterCards)
        if(characterCard.second != nullptr) al_destroy_bitmap(characterCard.second);
    for(auto &characte : character)
        if(characte.second != nullptr) al_destroy_bitmap(characte.second);
    for(auto &rul : rules)
        if(rul != nullptr) al_destroy_bitmap(rul);
    
    if(tileBack != nullptr ) al_destroy_bitmap(tileBack);
    if(guardBack != nullptr) al_destroy_bitmap(guardBack);
    if(guard != nullptr ) al_destroy_bitmap(guard);
    if(lootBack != nullptr) al_destroy_bitmap(lootBack);
    
    for(int i =0; i< DICE_MAX_NUMBER; i++)
    {
        if(whiteDices[i] != nullptr) al_destroy_bitmap(whiteDices[i]);
        if(redDices[i] != nullptr) al_destroy_bitmap(redDices[i]);
    }
}


ImageLoader::~ImageLoader()
{
    destroyImages();
}
