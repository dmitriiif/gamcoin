// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Gamcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GAMCOIN_COMMON_SYSTEM_H
#define GAMCOIN_COMMON_SYSTEM_H

#if defined(HAVE_CONFIG_H)
#include <config/gamcoin-config.h>
#endif

#include <cstdint>
#include <string>

// Application startup time (used for uptime calculation)
int64_t GetStartupTime();

void SetupEnvironment();
[[nodiscard]] bool SetupNetworking();
#ifndef WIN32
std::string ShellEscape(const std::string& arg);
#endif
#if HAVE_SYSTEM
void runCommand(const std::string& strCommand);
#endif

/**
 * Return the number of cores available on the current system.
 * @note This does count virtual cores, such as those provided by HyperThreading.
 */
int GetNumCores();

#endif // GAMCOIN_COMMON_SYSTEM_H
