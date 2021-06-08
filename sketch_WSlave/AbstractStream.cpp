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
const bool AbstractStream::read()
{
    if (this->_findUntil(WS_PATH_SEPARATOR, _BUFFER_SIZE)) {
        this->_currentAction = this->_read();
        LOG(F("readAction=")); LOG(this->_currentAction); LOGLN(';');

        // ignore LF=10, CR=13, SP=32
        return this->_currentAction > ' ';
    }

    return false;
}



void AbstractStream::process()
{   
    PowerManager::hold();
    
    switch (this->_currentAction) {
        #if WS_VERBOSE & WS_VERBOSE_LENGTH
        case WS_ACTION_LENGTH:
            LOGLN(F("count"));
            this->_printLength();
            return;
        #endif

        #if WS_VERBOSE & WS_VERBOSE_LIST
        case WS_ACTION_ALL:
            LOGLN(F("showAll"));
            this->_printAll();
            return;
        #endif

        #if WS_ACL_ALLOW & WS_ACL_ALLOW_ABOUT
        case WS_ACTION_ABOUT:
            LOGLN(F("about"));
            this->_printAbout();
            return;
        #endif

        #if WS_STORAGE != WS_STORAGE_NONE
        case WS_ACTION_SAVE:
            LOGLN(F("save"));
            Relayboard.save();
            return;
        #endif

        #if WS_ACL_ALLOW & WS_ACL_ALLOW_SLEEP
        case WS_ACTION_SLEEP:
            LOGLN(F("sleep"));
            Energy.sleep();
            return;
        #endif

        #if WS_ACL_ALLOW & WS_ACL_ALLOW_RESET
        case WS_ACTION_RESET:
            LOGLN(F("reset"));
            SOFTWARE_RESET;
            return;
        #endif
    }

    this->_currentRelay = this->_parseInt();
    LOG(F("parseRelay=")); LOG(this->_currentRelay); LOGLN(';');

    if (!Relayboard.exists(this->_currentRelay)) {
        return;
    }

    #if WS_ACL_ALLOW & WS_ACL_ALLOW_LOCK
    if (WS_ACTION_READ_UNLOCK == this->_currentAction) {
        LOGLN(F("READ"));
        Relayboard.isLocked(this->_currentRelay, false);
    } else
    #endif

    if (WS_ACTION_READ != this->_currentAction) {
        const uint8_t extra = this->_parseInt();

        switch (this->_currentAction) {
            case WS_ACTION_WRITE:
                LOGLN(F("write"));
                Relayboard.setStateAt(this->_currentRelay, extra, false);
                break;
            
            #if WS_ACL_ALLOW & WS_ACL_ALLOW_LOCK  
            case WS_ACTION_WRITE_LOCK:
                LOGLN(F("WRITE"));
                Relayboard.isLocked(this->_currentRelay, true);
                Relayboard.setStateAt(this->_currentRelay, extra, true);
                break;
            #endif

            case WS_ACTION_ISNC:
                LOGLN(F("NC/NO"));
                Relayboard.isNcAt(this->_currentRelay, extra);
                break;

            case WS_ACTION_MAP:
                LOGLN(F("map"));
                Relayboard.setPinAt(this->_currentRelay, extra);
                break;
        }
    } else {
        LOGLN(F("read"));
    }

    this->_printOne(this->_currentRelay);
}



void AbstractStream::terminate() const
{
    this->_currentStream->flush();
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void AbstractStream::_printData(const uint8_t data) const
{
    this->_currentStream->print(data);
    this->_currentStream->print(WS_DATA_SEPARATOR);

    LOG(data);
    LOG(WS_DATA_SEPARATOR);
}


void AbstractStream::_printOne(const uint8_t relayId) const
{
    LOG(F("> DATA: "));
    
    this->_printData(Relayboard.getStateAt(relayId));
    this->_printData(relayId);
    this->_printData(Relayboard.isLocked(relayId));
    this->_printData(Relayboard.isNcAt(relayId));
    this->_printData(Relayboard.getPinAt(relayId));
    this->_currentStream->print(WS_LF);

    LOGLN();
}


void AbstractStream::_printAbout() const
{
    this->_currentStream->print(SCM_REPO);
    this->_currentStream->print(WS_DATA_SEPARATOR);
    this->_currentStream->print(SCM_HASH);
    this->_currentStream->print(WS_DATA_SEPARATOR);
    this->_currentStream->print(SCM_DATE);
    this->_currentStream->print(WS_DATA_SEPARATOR);
    this->_currentStream->print(SCM_CHAN);
    this->_currentStream->print(WS_DATA_SEPARATOR);
    this->_currentStream->print(WS_LF);
}


void AbstractStream::_printLength() const
{
    this->_currentStream->print(WS_RELAY_NB_MAX);
    this->_currentStream->print(WS_LF);
}


void AbstractStream::_printAll() const
{
    uint8_t i = Relayboard.optionsLength;

    while (i-->0) {
        this->_printOne(i);
    }
}


bool AbstractStream::_findUntil(const int terminator, size_t length) const
{
    do {
        if (terminator==this->_read()) {
            return true;
        }
    }
    while (--length);

    return false;
}


uint8_t AbstractStream::_parseInt() const
{
    return this->_currentStream->parseInt(SKIP_ALL);
}


const char AbstractStream::_read() const
{
    return this->_currentStream->read();
}
