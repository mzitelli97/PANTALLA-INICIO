#ifndef CMODEL_H
#define CMODEL_H

#include "Model.h"
#include <string>

typedef enum {IP_TO_CONNECT, MY_NAME, NONE_SELECTED}textSelected;

/*textSelected operator++(const textSelected &text);
textSelected operator--(const textSelected &text);*/

#define LOOPBACK

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
    initInfo getInfo();
    void write(char key);
    void deleteOneChar();
    bool isAnEntryEmpty();
    void selectText(textSelected selected);
    
private:
    initInfo data;
 
};

#endif /* CMODEL_H */

