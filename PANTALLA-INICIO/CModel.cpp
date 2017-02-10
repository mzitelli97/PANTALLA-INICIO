/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CModel.cpp
 * Author: maxi
 * 
 * Created on 7 de febrero de 2017, 17:11
 */

#include "CModel.h"
#define MAX_SIZE_IP 15
#define MAX_SIZE_NAME 15

CModel::CModel()
{
    for(int i = 0; i < NONE_SELECTED; i++)
        data.entries[i] = "";
    data.selected = NONE_SELECTED;
#ifdef LOOPBACK
    data.entries[MY_IP]="127.0.0.1";
    data.entries[OTHER_IP]= "127.0.0.1";
#endif
}

CModel::CModel(const CModel& orig) {
}

CModel::~CModel() {
}

initInfo CModel::getInfo()
{
    return data;
}

void CModel::write(char key)
{
    if(data.selected != NONE_SELECTED)
    {
        if(data.selected == MY_NAME && data.entries[data.selected].size()<MAX_SIZE_NAME)
            data.entries[data.selected] += key;
        else if(data.selected != MY_NAME && data.entries[data.selected].size()<MAX_SIZE_IP)
            if((key>='0' && key<='9') || key == '.')
                data.entries[data.selected] += key;
    }
    notifyAllObservers();
}

void CModel::deleteOneChar()
{
    if(data.selected != NONE_SELECTED && !data.entries[data.selected].empty())
        data.entries[data.selected].pop_back();
    notifyAllObservers();
}


void CModel::selectText(textSelected selected)
{
    if(selected >= MY_IP && selected <= MY_NAME)
        this->data.selected = selected;
    notifyAllObservers();
}

bool CModel::isAnEntryEmpty()
{
    bool retVal = false;
    for(int i = 0; i < NONE_SELECTED; i++)
    {
        if(data.entries[i].empty())
        {
            retVal = true;
            break;
        }
    }
    return retVal;
}


/*textSelected operator++(const textSelected& text)
{
    textSelected retVal = text;
    if(text == MY_IP) retVal = OTHER_IP;
    else if(text == OTHER_IP) retVal = MY_NAME;
    return retVal;
}

textSelected operator--(const textSelected& text)
{
    textSelected retVal = text;
    if(text == OTHER_IP) retVal = MY_IP;
    else if(text == MY_NAME) retVal = OTHER_IP;
    return retVal;
}*/




