// Copyright (c) 2014-2020 The Gamcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GAMCOIN_CHAINPARAMSBASE_H
#define GAMCOIN_CHAINPARAMSBASE_H

#include <util/chaintype.h>

#include <memory>
#include <string>

class ArgsManager;

/**
 * CBaseChainParams defines the base parameters (shared between gamcoin-cli and gamcoind)
 * of a given instance of the Gamcoin system.
 */
class CBaseChainParams
{
public:
    const std::string& DataDir() const { return strDataDir; }
    uint16_t RPCPort() const { return m_rpc_port; }
    uint16_t OnionServiceTargetPort() const { return m_onion_service_target_port; }

    CBaseChainParams() = delete;
    CBaseChainParams(const std::string& data_dir, uint16_t rpc_port, uint16_t onion_service_target_port)
        : m_rpc_port(rpc_port), m_onion_service_target_port(onion_service_target_port), strDataDir(data_dir) {}

private:
    const uint16_t m_rpc_port;
    const uint16_t m_onion_service_target_port;
    std::string strDataDir;
};

/**
 * Creates and returns a std::unique_ptr<CBaseChainParams> of the chosen chain.
 */
std::unique_ptr<CBaseChainParams> CreateBaseChainParams(const ChainType chain);

/**
 *Set the arguments for chainparams
 */
void SetupChainParamsBaseOptions(ArgsManager& argsman);

/**
 * Return the currently selected parameters. This won't change after app
 * startup, except for unit tests.
 */
const CBaseChainParams& BaseParams();

/** Sets the params returned by Params() to those for the given chain. */
void SelectBaseParams(const ChainType chain);

#endif // GAMCOIN_CHAINPARAMSBASE_H
