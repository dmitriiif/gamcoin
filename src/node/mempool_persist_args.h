// Copyright (c) 2022 The Gamcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GAMCOIN_NODE_MEMPOOL_PERSIST_ARGS_H
#define GAMCOIN_NODE_MEMPOOL_PERSIST_ARGS_H

#include <util/fs.h>

class ArgsManager;

namespace node {

/**
 * Default for -persistmempool, indicating whether the node should attempt to
 * automatically load the mempool on start and save to disk on shutdown
 */
static constexpr bool DEFAULT_PERSIST_MEMPOOL{true};

bool ShouldPersistMempool(const ArgsManager& argsman);
fs::path MempoolPath(const ArgsManager& argsman);

} // namespace node

#endif // GAMCOIN_NODE_MEMPOOL_PERSIST_ARGS_H
