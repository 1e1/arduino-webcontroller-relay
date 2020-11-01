#include "AbstractInterfaceWebApp.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/








/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/







/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void AbstractInterfaceWebApp::_listen(Client* client)
{
  this->setStream(client);

  if (client->connected()) {
    if (client->available()) {

      client->print(F(WS_HEADER_BEGIN_F));

      #if WS_VERBOSE & WS_VERBOSE_WEBAPP
      if (this->read()) {
        client->print(F(WS_HEADER_END_ACTION_F));
        this->process();
      } else {
        #if WS_HTML_COMPRESSION == WS_HTML_COMPRESSION_BR
        client->print(F(WM_HEADER_END_HELP_BR_F));
        #else
        client->print(F(WM_HEADER_END_HELP_GZ_F));
        #endif
        for (int i = 0; i<WebApp::WEBPAGE_LENGTH; ++i) {
          client->write(pgm_read_byte_near(WebApp::WEBPAGE_P + i));
        }
      }
      #else
      if (this->read()) {
        client->print(F(WS_HEADER_END_ACTION_F));
        this->process();
      }
      #endif

      this->terminate();
    }

    client->stop();
  }
}
