#include "AbstractStream.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




static uint8_t readLimitNbRetry = 128;





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




/**
 * (...) /<action>\\w /(<pin>\d+( /<value>\d+)?)? (...)
 * */
bool AbstractStream::read()
{
    if (this->_findUntil(PATH_SEPARATOR, BUFFER_SIZE)) {
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
        #if MODE_VERBOSE & MODE_VERBOSE_LIST
        case ACTION_ALL:
            LOGLN("showAll");
            this->_showAll();
            return;
        #endif

        #if DATA_STORAGE != DATA_STORAGE_NONE
        case ACTION_SAVE:
            LOGLN("save");
            Relay::save();
            return;
        #endif

        #if ACL_ALLOW == ACL_ALLOW_RESET
        case ACTION_RESET:
            LOGLN("reset");
            SOFTWARE_RESET;
            return;
        #endif

        case ACTION_WRITE:
            LOGLN("write");
            Relay::setStateAt(this->_currentRelay, extra);
            break;

        case ACTION_NC:
        case ACTION_NO:
            LOGLN("NC/NO");
            Relay::isNcAt(this->_currentRelay, this->_currentAction == ACTION_NC);
            break;

        case ACTION_MAP:
            LOGLN("map");
            Relay::setPinAt(this->_currentRelay, extra);
            break;


        case ACTION_READ:
            LOGLN("read");
            break;
    }

    this->_showOne(this->_currentRelay);
}



void AbstractStream::terminate()
{
    this->_findUntil(LF, -1);
    this->_currentStream->flush();
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void AbstractStream::_showOne(const uint8_t relay)
{
    this->_currentStream->print(Relay::getStateAt(relay));
    this->_currentStream->print(SEP);
    this->_currentStream->print(relay);
    this->_currentStream->print(Relay::isNcAt(relay) ? TEXT_NC : TEXT_NO);
    this->_currentStream->print(Relay::getPinAt(relay));
    this->_currentStream->print(LF);
}


void AbstractStream::_showAll()
{
   uint8_t i = NB_RELAYS;
   while (i-->0) {
        if (Relay::exists(i)) {
            this->_showOne(i);
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
    uint8_t nbRetry = readLimitNbRetry;
    int c;

    do {
        c = this->_currentStream->read();
        if (c>=0) {
            break;
        }

        delay(1);
    } while (--nbRetry);

    return c;
}
