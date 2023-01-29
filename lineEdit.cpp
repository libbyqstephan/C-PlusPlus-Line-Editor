#include "lineEdit.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if(argc > 2)
        return 0;
    else if(argc == 2)
    {
        lineEdit* lineEditor = new lineEdit(argv[1]);
        lineEditor->beginEdit();
    }
    else
    {
        lineEdit* lineEditor = new lineEdit();
        lineEditor->beginEdit();
    }
    return 0;
}