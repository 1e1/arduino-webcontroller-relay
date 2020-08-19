#include "AbstractStream.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/









/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




/**
 * (...) /<action>\\w /(<pin>\d+( /<value>\d+)?)? (...)
 * */
bool AbstractStream::read()
{
    if (this->_findUntil(PATH_SEPARATOR, BUFFER_SIZE)) {
        this->_currentAction = this->_currentStream->read();
        this->_currentRelay = this->_currentStream->parseInt(SKIP_ALL);

        return true;
    }


    return false;
}



void AbstractStream::process()
{
    const uint8_t extra = this->_currentStream->parseInt(SKIP_ALL);
    
    switch (this->_currentAction) {
        #if MODE_VERBOSE & MODE_VERBOSE_LIST
        case ACTION_ALL:
            this->_showAll();
            return;
        #endif

        #if STORAGE != STORAGE_NONE
        case ACTION_SAVE:
            Relay::save();
            return;
        #endif

        case ACTION_WRITE:
            Relay::setStateAt(this->_currentRelay, extra);
            break;

        case ACTION_NC:
        case ACTION_NO:
            Relay::isNcAt(this->_currentRelay, this->_currentAction == ACTION_NC);
            break;

        case ACTION_MAP:
            Relay::setPinAt(this->_currentRelay, extra);
            break;


        case ACTION_READ:;
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
    this->_currentStream->print(relay);
    this->_currentStream->print(SEP);
    this->_currentStream->print(Relay::getStateAt(relay));
    this->_currentStream->print(Relay::isNcAt(relay) ? TEXT_NC : TEXT_NO);
    this->_currentStream->println(Relay::getPinAt(relay));
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
        delay(1);
    }
    while (terminator!=this->_currentStream->read() && --length);

    return (bool) length;
}
