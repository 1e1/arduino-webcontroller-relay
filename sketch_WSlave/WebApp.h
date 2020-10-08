
#ifndef webApp_H_
#define webApp_H_

#if WS_HTML_COMPRESSION == WS_HTML_COMPRESSION_BR
#include "webApp-generated-brotli.h"
#else
#include "webApp-generated-gzip.h"
#endif
#include "macro.h"



namespace WebApp {
    LONGBYTES(WEBPAGE_P) = WEBPAGE;
    static const size_t WEBPAGE_LENGTH = ARRAYLEN(WEBPAGE_P);
}


#endif // webApp_H_
