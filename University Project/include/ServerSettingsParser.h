#ifndef SERVERSETTINGSPARSER_H
#define SERVERSETTINGSPARSER_H

#include "Server.h"
#include "CmdArgManager.h"

namespace kiv {

ServerSettings ParseSettingsFromCommandLine(CmdArgvManager& manager);

}

#endif