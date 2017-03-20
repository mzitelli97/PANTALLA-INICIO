#ifndef STRNMBRFIELDTOARRAY_H
#define STRNMBRFIELDTOARRAY_H
#include <iostream>
#include <string>
#include "BurgleBrosModel.h"

using namespace std;

#define PROTOCOL_LOCATION_LENGTH 4  //Por ejemplo A1F1, todas tienen 4 caracteres.

std::string cardLocationToProtocol(CardLocation &cardLocation);		//(0,0,0) (floor,row,column) = A1F1 (protocol)
CardLocation protocolToCardLocation(std::string location);
void strNmbrFieldToArray(string &field, unsigned char *buffer, unsigned int *length);
void arrayToStrNmbrField(string *field, unsigned char *buffer, unsigned int length);

#endif /* STRNMBRFIELDTOARRAY_H */

