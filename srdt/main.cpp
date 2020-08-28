#include "lcxmlapplication.h"

int main(int argc, char *argv[])
{
    return LCXmlApplication::instance().exec(argc, argv);
}

