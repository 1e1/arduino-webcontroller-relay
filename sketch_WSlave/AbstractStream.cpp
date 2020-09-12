#include "AbstractStream.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




static const uint8_t _BUFFER_SIZE = 8;





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




/**
 * (...) /<action>\\w (/<pin>\d+ (/<value>\d+)? )? (...)
 * */
bool AbstractStream::read()
{
    if (this->_findUntil(WS_PATH_SEPARATOR, _BUFFER_SIZE)) {
        this->_currentAction = this->_read();
        this->_currentRelay = this->_parseInt();

        LOG("readAction="); LOG(this->_currentAction); LOGLN(';');
        LOG("parseRelay="); LOG(this->_currentRelay); LOGLN(';');

        // ignore LF=10, CR=13, SP=32
        return this->_currentAction > ' ';
    }


    return false;
}



void AbstractStream::process()
{
    const uint8_t extra = this->_parseInt();

    LOG("parseExtra="); LOG(extra); LOGLN(';');
    
    switch (this->_currentAction) {
        #if WS_MODE_VERBOSE & WS_MODE_VERBOSE_LIST
        case WS_ACTION_ALL:
            LOGLN("showAll");
            this->_printAll();
            return;
        #endif

        #if WS_DATA_STORAGE != WS_DATA_STORAGE_NONE
        case WS_ACTION_SAVE:
            LOGLN("save");
            Relay::save();
            return;
        #endif

        #if WS_ACL_ALLOW == WS_ACL_ALLOW_RESET
        case WS_ACTION_RESET:
            LOGLN("reset");
            SOFTWARE_RESET;
            return;
        #endif

        case WS_ACTION_WRITE:
            LOGLN("write");
            Relay::setStateAt(this->_currentRelay, extra);
            break;

        case WS_ACTION_NC:
        case WS_ACTION_NO:
            LOGLN("NC/NO");
            Relay::isNcAt(this->_currentRelay, this->_currentAction == WS_ACTION_NC);
            break;

        case WS_ACTION_MAP:
            LOGLN("map");
            Relay::setPinAt(this->_currentRelay, extra);
            break;


        case WS_ACTION_READ:
            LOGLN("read");
            break;
    }

    this->_printOne(this->_currentRelay);
}



void AbstractStream::terminate()
{
    this->_currentStream->flush();
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void AbstractStream::_printData(const uint8_t data)
{
    this->_currentStream->print(data);
    this->_currentStream->print(WS_DATA_SEPARATOR);
}


void AbstractStream::_printOne(const uint8_t relay)
{
    this->_printData(Relay::getStateAt(relay));
    this->_printData(relay);
    this->_printData(Relay::isNcAt(relay));
    this->_printData(Relay::getPinAt(relay));
    this->_currentStream->print(WS_LF);
}


void AbstractStream::_printAll()
{
   uint8_t i = Relay::optionsLength;
   while (i-->0) {
        if (Relay::exists(i)) {
            this->_printOne(i);
        }
   }
}


bool AbstractStream::_findUntil(const int terminator, size_t length)
{
    do {
        if (terminator==this->_read()) {
            return true;
        }
    }
    while (--length);

    return false;
}


uint8_t AbstractStream::_parseInt()
{
    return this->_currentStream->parseInt(SKIP_ALL);
}


char AbstractStream::_read()
{
    return this->_currentStream->read();
}
