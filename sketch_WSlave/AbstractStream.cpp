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
        LOG("readAction="); LOG(this->_currentAction); LOGLN(';');

        // ignore LF=10, CR=13, SP=32
        return this->_currentAction > ' ';
    }


    return false;
}



void AbstractStream::process()
{   
    switch (this->_currentAction) {
        #if WS_VERBOSE & WS_VERBOSE_LIST
        case WS_ACTION_ALL:
            LOGLN("showAll");
            this->_printAll();
            return;
        #endif

        #if WS_STORAGE != WS_STORAGE_NONE
        case WS_ACTION_SAVE:
            LOGLN("save");
            Relayboard.save();
            return;
        #endif

        #if WS_ACL_ALLOW == WS_ACL_ALLOW_SLEEP
        case WS_ACTION_SLEEP:
            LOGLN("sleep");
            Energy.sleep();
            return;
        #endif

        #if WS_ACL_ALLOW == WS_ACL_ALLOW_RESET
        case WS_ACTION_RESET:
            LOGLN("reset");
            SOFTWARE_RESET;
            return;
        #endif
    }

    this->_currentRelay = this->_parseInt();
    LOG("parseRelay="); LOG(this->_currentRelay); LOGLN(';');

    switch (this->_currentAction) {
        case WS_ACTION_WRITE:
            LOGLN("write");
            Relayboard.setStateAt(this->_currentRelay, this->_parseInt(), this->_currentAction == WS_ACTION_WRITE_LOCK);
            break;
        
        #if WS_ACL_ALLOW == WS_ACL_ALLOW_LOCK  
        case WS_ACTION_WRITE_LOCK:
            LOGLN("WRITE");
            Relayboard.isLocked(this->_currentRelay, true);
            Relayboard.setStateAt(this->_currentRelay, this->_parseInt(), this->_currentAction == WS_ACTION_WRITE_LOCK);
            break;
        #endif

        case WS_ACTION_NC:
        case WS_ACTION_NO:
            LOGLN("NC/NO");
            Relayboard.isNcAt(this->_currentRelay, this->_currentAction == WS_ACTION_NC);
            break;

        case WS_ACTION_MAP:
            LOGLN("map");
            Relayboard.setPinAt(this->_currentRelay, this->_parseInt());
            break;

        #if WS_ACL_ALLOW == WS_ACL_ALLOW_LOCK  
        case WS_ACTION_READ_UNLOCK:
            LOGLN("READ");
            Relayboard.isLocked(this->_currentRelay, false);
            break;
        #endif

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


void AbstractStream::_printOne(const uint8_t relayId)
{
    this->_printData(Relayboard.getStateAt(relayId));
    this->_printData(relayId);
    this->_printData(Relayboard.isLocked(relayId));
    this->_printData(Relayboard.isNcAt(relayId));
    this->_printData(Relayboard.getPinAt(relayId));
    this->_currentStream->print(WS_LF);
}


void AbstractStream::_printAll()
{
    uint8_t i = Relayboard.optionsLength;

    while (i-->0) {
        this->_printOne(i);
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
