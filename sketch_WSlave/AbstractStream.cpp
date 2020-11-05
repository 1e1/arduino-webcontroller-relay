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
    PowerManager::hold();
    
    switch (this->_currentAction) {
        #if WS_VERBOSE & WS_VERBOSE_LIST
        case WS_ACTION_ALL:
            LOGLN("showAll");
            this->_printAll();
            return;
        #endif

        #if WS_ACL_ALLOW == WS_ACL_ALLOW_ABOUT
        case WS_ACTION_ABOUT:
            LOGLN("about");
            this->_printAbout();
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

    if (!Relayboard.exists(this->_currentRelay)) {
        return;
    }

    #if WS_ACL_ALLOW == WS_ACL_ALLOW_LOCK
    if (WS_ACTION_READ_UNLOCK == this->_currentAction) {
        LOGLN("READ");
        Relayboard.isLocked(this->_currentRelay, false);
    } else
    #endif

    if (WS_ACTION_READ != this->_currentAction) {
        const uint8_t extra = this->_parseInt();

        switch (this->_currentAction) {
            case WS_ACTION_WRITE:
                LOGLN("write");
                Relayboard.setStateAt(this->_currentRelay, extra, false);
                break;
            
            #if WS_ACL_ALLOW == WS_ACL_ALLOW_LOCK  
            case WS_ACTION_WRITE_LOCK:
                LOGLN("WRITE");
                Relayboard.isLocked(this->_currentRelay, true);
                Relayboard.setStateAt(this->_currentRelay, extra, true);
                break;
            #endif

            case WS_ACTION_ISNC:
                LOGLN("NC/NO");
                Relayboard.isNcAt(this->_currentRelay, extra);
                break;

            case WS_ACTION_MAP:
                LOGLN("map");
                Relayboard.setPinAt(this->_currentRelay, extra);
                break;
        }
    } else {
        LOGLN("read");
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


void AbstractStream::_printAbout()
{
    this->_currentStream->print(SCM_HASH);
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
