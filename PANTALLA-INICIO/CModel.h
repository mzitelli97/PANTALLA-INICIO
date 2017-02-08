/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CModel.h
 * Author: maxi
 *
 * Created on 7 de febrero de 2017, 17:11
 */

#ifndef CMODEL_H
#define CMODEL_H

#include "Model.h"
#include "View.h"
#include <string>

typedef enum {MY_IP = 0, OTHER_IP, MY_NAME, NONE_SELECTED}textSelected;

/*textSelected operator++(const textSelected &text);
textSelected operator--(const textSelected &text);*/

typedef struct
{
    std::string entries[NONE_SELECTED];
    textSelected selected;
}initInfo;

class CModel : public Model{
public:
    CModel();
    CModel(const CModel& orig);
    virtual ~CModel();
    void attachView(View * view);
    initInfo getInfo();
    void write(char key);
    void deleteOneChar();
    void selectText(textSelected selected);
    
private:
    View * view;
    initInfo data;
    

};

#endif /* CMODEL_H */

