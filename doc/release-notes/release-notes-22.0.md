22.0 Release Notes
==================

Gamcoin Core version 22.0 is now available from:

  <https://gamcoincore.org/bin/gamcoin-core-22.0/>

This release includes new features, various bug fixes and performance
improvements, as well as updated translations.

Please report bugs using the issue tracker at GitHub:

  <https://github.com/gamcoin/gamcoin/issues>

To receive security and update notifications, please subscribe to:

  <https://gamcoincore.org/en/list/announcements/join/>

How to Upgrade
==============

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes in some cases), then run the
installer (on Windows) or just copy over `/Applications/Gamcoin-Qt` (on Mac)
or `gamcoind`/`gamcoin-qt` (on Linux).

Upgrading directly from a version of Gamcoin Core that has reached its EOL is
possible, but it might take some time if the data directory needs to be migrated. Old
wallet versions of Gamcoin Core are generally supported.

Compatibility
==============

Gamcoin Core is supported and extensively tested on operating systems
using the Linux kernel, macOS 10.14+, and Windows 7 and newer.  Gamcoin
Core should also work on most other Unix-like systems but is not as
frequently tested on them.  It is not recommended to use Gamcoin Core on
unsupported systems.

From Gamcoin Core 22.0 onwards, macOS versions earlier than 10.14 are no longer supported.

Notable changes
===============

P2P and network changes
-----------------------
- Added support for running Gamcoin Core as an
  [I2P (Invisible Internet Project)](https://en.wikipedia.org/wiki/I2P) service
  and connect to such services. See [i2p.md](https://github.com/gamcoin/gamcoin/blob/22.x/doc/i2p.md) for details. (#20685)
- This release removes support for Tor version 2 hidden services in favor of Tor
  v3 only, as the Tor network [dropped support for Tor
  v2](https://blog.torproject.org/v2-deprecation-timeline) with the release of
  Tor version 0.4.6.  Henceforth, Gamcoin Core ignores Tor v2 addresses; it
  neither rumors them over the network to other peers, nor stores them in memory
  or to `peers.dat`.  (#22050)

- Added NAT-PMP port mapping support via
  [`libnatpmp`](https://miniupnp.tuxfamily.org/libnatpmp.html). (#18077)

New and Updated RPCs
--------------------

- Due to [BIP 350](https://github.com/gamcoin/bips/blob/master/bip-0350.mediawiki)
  being implemented, behavior for all RPCs that accept addresses is changed when
  a native witness version 1 (or higher) is passed. These now require a Bech32m
  encoding instead of a Bech32 one, and Bech32m encoding will be used for such
  addresses in RPC output as well. No version 1 addresses should be created
  for mainnet until consensus rules are adopted that give them meaning
  (as will happen through [BIP 341](https://github.com/gamcoin/bips/blob/master/bip-0341.mediawiki)).
  Once that happens, Bech32m is expected to be used for them, so this shouldn't
  affect any production systems, but may be observed on other networks where such
  addresses already have meaning (like signet). (#20861)

- The `getpeerinfo` RPC returns two new boolean fields, `bip152_hb_to` and
  `bip152_hb_from`, that respectively indicate whether we selected a peer to be
  in compact blocks high-bandwidth mode or whether a peer selected us as a
  compact blocks high-bandwidth peer. High-bandwidth peers send new block
  announcements via a `cmpctblock` message rather than the usual inv/headers
  announcements. See BIP 152 for more details. (#19776)

- `getpeerinfo` no longer returns the following fields: `addnode`, `banscore`,
  and `whitelisted`, which were previously deprecated in 0.21. Instead of
  `addnode`, the `connection_type` field returns manual. Instead of
  `whitelisted`, the `permissions` field indicates if the peer has special
  privileges. The `banscore` field has simply been removed. (#20755)

- The following RPCs:  `gettxout`, `getrawtransaction`, `decoderawtransaction`,
  `decodescript`, `gettransaction`, and REST endpoints: `/rest/tx`,
  `/rest/getutxos`, `/rest/block` deprecated the following fields (which are no
  longer returned in the responses by default): `addresses`, `reqSigs`.
  The `-deprecatedrpc=addresses` flag must be passed for these fields to be
  included in the RPC response. This flag/option will be available only for this major release, after which
  the deprecation will be removed entirely. Note that these fields are attributes of
  the `scriptPubKey` object returned in the RPC response. However, in the response
  of `decodescript` these fields are top-level attributes, and included again as attributes
  of the `scriptPubKey` object. (#20286)

- When creating a hex-encoded gamcoin transaction using the `gamcoin-tx` utility
  with the `-json` option set, the following fields: `addresses`, `reqSigs` are no longer
  returned in the tx output of the response. (#20286)

- The `listbanned` RPC now returns two new numeric fields: `ban_duration` and `time_remaining`.
  Respectively, these new fields indicate the duration of a ban and the time remaining until a ban expires,
  both in seconds. Additionally, the `ban_created` field is repositioned to come before `banned_until`. (#21602)

- The `setban` RPC can ban onion addresses again. This fixes a regression introduced in version 0.21.0. (#20852)

- The `getnodeaddresses` RPC now returns a "network" field indicating the
  network type (ipv4, ipv6, onion, or i2p) for each address.  (#21594)

- `getnodeaddresses` now also accepts a "network" argument (ipv4, ipv6, onion,
  or i2p) to return only addresses of the specified network.  (#21843)

- The `testmempoolaccept` RPC now accepts multiple transactions (still experimental at the moment,
  API may be unstable). This is intended for testing transaction packages with dependency
  relationships; it is not recommended for batch-validating independent transactions. In addition to
  mempool policy, package policies apply: the list cannot contain more than 25 transactions or have a
  total size exceeding 101K virtual bytes, and cannot conflict with (spend the same inputs as) each other or
  the mempool, even if it would be a valid BIP125 replace-by-fee. There are some known limitations to
  the accuracy of the test accept: it's possible for `testmempoolaccept` to return "allowed"=True for a
  group of transactions, but "too-long-mempool-chain" if they are actually submitted. (#20833)

- `addmultisigaddress` and `createmultisig` now support up to 20 keys for
  Segwit addresses. (#20867)

Changes to Wallet or GUI related RPCs can be found in the GUI or Wallet section below.

Build System
------------

- Release binaries are now produced using the new `guix`-based build system.
  The [/doc/release-process.md](/doc/release-process.md) document has been updated accordingly.

Files
-----

- The list of banned hosts and networks (via `setban` RPC) is now saved on disk
  in JSON format in `banlist.json` instead of `banlist.dat`. `banlist.dat` is
  only read on startup if `banlist.json` is not present. Changes are only written to the new
  `banlist.json`. A future version of Gamcoin Core may completely ignore
  `banlist.dat`. (#20966)

New settings
------------

- The `-natpmp` option has been added to use NAT-PMP to map the listening port.
  If both UPnP and NAT-PMP are enabled, a successful allocation from UPnP
  prevails over one from NAT-PMP. (#18077)

Updated settings
----------------

Changes to Wallet or GUI related settings can be found in the GUI or Wallet section below.

- Passing an invalid `-rpcauth` argument now cause gamcoind to fail to start.  (#20461)

Tools and Utilities
-------------------

- A new CLI `-addrinfo` command returns the number of addresses known to the
  node per network type (including Tor v2 versus v3) and total. This can be
  useful to see if the node knows enough addresses in a network to use options
  like `-onlynet=<network>` or to upgrade to this release of Gamcoin Core 22.0
  that supports Tor v3 only.  (#21595)

- A new `-rpcwaittimeout` argument to `gamcoin-cli` sets the timeout
  in seconds to use with `-rpcwait`. If the timeout expires,
  `gamcoin-cli` will report a failure. (#21056)

Wallet
------

- External signers such as hardware wallets can now be used through the new RPC methods `enumeratesigners` and `displayaddress`. Support is also added to the `send` RPC call. This feature is experimental. See [external-signer.md](https://github.com/gamcoin/gamcoin/blob/22.x/doc/external-signer.md) for details. (#16546)

- A new `listdescriptors` RPC is available to inspect the contents of descriptor-enabled wallets.
  The RPC returns public versions of all imported descriptors, including their timestamp and flags.
  For ranged descriptors, it also returns the range boundaries and the next index to generate addresses from. (#20226)

- The `bumpfee` RPC is not available with wallets that have private keys
  disabled. `psbtbumpfee` can be used instead. (#20891)

- The `fundrawtransaction`, `send` and `walletcreatefundedpsbt` RPCs now support an `include_unsafe` option
  that when `true` allows using unsafe inputs to fund the transaction.
  Note that the resulting transaction may become invalid if one of the unsafe inputs disappears.
  If that happens, the transaction must be funded with different inputs and republished. (#21359)

- We now support up to 20 keys in `multi()` and `sortedmulti()` descriptors
  under `wsh()`. (#20867)

- Taproot descriptors can be imported into the wallet only after activation has occurred on the network (e.g. mainnet, testnet, signet) in use. See [descriptors.md](https://github.com/gamcoin/gamcoin/blob/22.x/doc/descriptors.md) for supported descriptors.

GUI changes
-----------

- External signers such as hardware wallets can now be used. These require an external tool such as [HWI](https://github.com/gamcoin-core/HWI) to be installed and configured under Options -> Wallet. When creating a new wallet a new option "External signer" will appear in the dialog. If the device is detected, its name is suggested as the wallet name. The watch-only keys are then automatically imported. Receive addresses can be verified on the device. The send dialog will automatically use the connected device. This feature is experimental and the UI may freeze for a few seconds when performing these actions.

Low-level changes
=================

RPC
---

- The RPC server can process a limited number of simultaneous RPC requests.
  Previously, if this limit was exceeded, the RPC server would respond with
  [status code 500 (`HTTP_INTERNAL_SERVER_ERROR`)](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes#5xx_server_errors).
  Now it returns status code 503 (`HTTP_SERVICE_UNAVAILABLE`). (#18335)

- Error codes have been updated to be more accurate for the following error cases (#18466):
  - `signmessage` now returns RPC_INVALID_ADDRESS_OR_KEY (-5) if the
    passed address is invalid. Previously returned RPC_TYPE_ERROR (-3).
  - `verifymessage` now returns RPC_INVALID_ADDRESS_OR_KEY (-5) if the
    passed address is invalid. Previously returned RPC_TYPE_ERROR (-3).
  - `verifymessage` now returns RPC_TYPE_ERROR (-3) if the passed signature
    is malformed. Previously returned RPC_INVALID_ADDRESS_OR_KEY (-5).

Tests
-----

22.0 change log
===============

A detailed list of changes in this version follows. To keep the list to a manageable length, small refactors and typo fixes are not included, and similar changes are sometimes condensed into one line.

### Consensus
- gamcoin/gamcoin#19438 Introduce deploymentstatus (ajtowns)
- gamcoin/gamcoin#20207 Follow-up extra comments on taproot code and tests (sipa)
- gamcoin/gamcoin#21330 Deal with missing data in signature hashes more consistently (sipa)

### Policy
- gamcoin/gamcoin#18766 Disable fee estimation in blocksonly mode (by removing the fee estimates global) (darosior)
- gamcoin/gamcoin#20497 Add `MAX_STANDARD_SCRIPTSIG_SIZE` to policy (sanket1729)
- gamcoin/gamcoin#20611 Move `TX_MAX_STANDARD_VERSION` to policy (MarcoFalke)

### Mining
- gamcoin/gamcoin#19937, gamcoin/gamcoin#20923 Signet mining utility (ajtowns)

### Block and transaction handling
- gamcoin/gamcoin#14501 Fix possible data race when committing block files (luke-jr)
- gamcoin/gamcoin#15946 Allow maintaining the blockfilterindex when using prune (jonasschnelli)
- gamcoin/gamcoin#18710 Add local thread pool to CCheckQueue (hebasto)
- gamcoin/gamcoin#19521 Coinstats Index (fjahr)
- gamcoin/gamcoin#19806 UTXO snapshot activation (jamesob)
- gamcoin/gamcoin#19905 Remove dead CheckForkWarningConditionsOnNewFork (MarcoFalke)
- gamcoin/gamcoin#19935 Move SaltedHashers to separate file and add some new ones (achow101)
- gamcoin/gamcoin#20054 Remove confusing and useless "unexpected version" warning (MarcoFalke)
- gamcoin/gamcoin#20519 Handle rename failure in `DumpMempool(…)` by using the `RenameOver(…)` return value (practicalswift)
- gamcoin/gamcoin#20749, gamcoin/gamcoin#20750, gamcoin/gamcoin#21055, gamcoin/gamcoin#21270, gamcoin/gamcoin#21525, gamcoin/gamcoin#21391, gamcoin/gamcoin#21767, gamcoin/gamcoin#21866 Prune `g_chainman` usage (dongcarl)
- gamcoin/gamcoin#20833 rpc/validation: enable packages through testmempoolaccept (glozow)
- gamcoin/gamcoin#20834 Locks and docs in ATMP and CheckInputsFromMempoolAndCache (glozow)
- gamcoin/gamcoin#20854 Remove unnecessary try-block (amitiuttarwar)
- gamcoin/gamcoin#20868 Remove redundant check on pindex (jarolrod)
- gamcoin/gamcoin#20921 Don't try to invalidate genesis block in CChainState::InvalidateBlock (theStack)
- gamcoin/gamcoin#20972 Locks: Annotate CTxMemPool::check to require `cs_main` (dongcarl)
- gamcoin/gamcoin#21009 Remove RewindBlockIndex logic (dhruv)
- gamcoin/gamcoin#21025 Guard chainman chainstates with `cs_main` (dongcarl)
- gamcoin/gamcoin#21202 Two small clang lock annotation improvements (amitiuttarwar)
- gamcoin/gamcoin#21523 Run VerifyDB on all chainstates (jamesob)
- gamcoin/gamcoin#21573 Update libsecp256k1 subtree to latest master (sipa)
- gamcoin/gamcoin#21582, gamcoin/gamcoin#21584, gamcoin/gamcoin#21585 Fix assumeutxo crashes (MarcoFalke)
- gamcoin/gamcoin#21681 Fix ActivateSnapshot to use hardcoded nChainTx (jamesob)
- gamcoin/gamcoin#21796 index: Avoid async shutdown on init error (MarcoFalke)
- gamcoin/gamcoin#21946 Document and test lack of inherited signaling in RBF policy (ariard)
- gamcoin/gamcoin#22084 Package testmempoolaccept followups (glozow)
- gamcoin/gamcoin#22102 Remove `Warning:` from warning message printed for unknown new rules (prayank23)
- gamcoin/gamcoin#22112 Force port 0 in I2P (vasild)
- gamcoin/gamcoin#22135 CRegTestParams: Use `args` instead of `gArgs` (kiminuo)
- gamcoin/gamcoin#22146 Reject invalid coin height and output index when loading assumeutxo (MarcoFalke)
- gamcoin/gamcoin#22253 Distinguish between same tx and same-nonwitness-data tx in mempool (glozow)
- gamcoin/gamcoin#22261 Two small fixes to node broadcast logic (jnewbery)
- gamcoin/gamcoin#22415 Make `m_mempool` optional in CChainState (jamesob)
- gamcoin/gamcoin#22499 Update assumed chain params (sriramdvt)
- gamcoin/gamcoin#22589 net, doc: update I2P hardcoded seeds and docs for 22.0 (jonatack)

### P2P protocol and network code
- gamcoin/gamcoin#18077 Add NAT-PMP port forwarding support (hebasto)
- gamcoin/gamcoin#18722 addrman: improve performance by using more suitable containers (vasild)
- gamcoin/gamcoin#18819 Replace `cs_feeFilter` with simple std::atomic (MarcoFalke)
- gamcoin/gamcoin#19203 Add regression fuzz harness for CVE-2017-18350. Add FuzzedSocket (practicalswift)
- gamcoin/gamcoin#19288 fuzz: Add fuzzing harness for TorController (practicalswift)
- gamcoin/gamcoin#19415 Make DNS lookup mockable, add fuzzing harness (practicalswift)
- gamcoin/gamcoin#19509 Per-Peer Message Capture (troygiorshev)
- gamcoin/gamcoin#19763 Don't try to relay to the address' originator (vasild)
- gamcoin/gamcoin#19771 Replace enum CConnMan::NumConnections with enum class ConnectionDirection (luke-jr)
- gamcoin/gamcoin#19776 net, rpc: expose high bandwidth mode state via getpeerinfo (theStack)
- gamcoin/gamcoin#19832 Put disconnecting logs into BCLog::NET category (hebasto)
- gamcoin/gamcoin#19858 Periodically make block-relay connections and sync headers (sdaftuar)
- gamcoin/gamcoin#19884 No delay in adding fixed seeds if -dnsseed=0 and peers.dat is empty (dhruv)
- gamcoin/gamcoin#20079 Treat handshake misbehavior like unknown message (MarcoFalke)
- gamcoin/gamcoin#20138 Assume that SetCommonVersion is called at most once per peer (MarcoFalke)
- gamcoin/gamcoin#20162 p2p: declare Announcement::m_state as uint8_t, add getter/setter (jonatack)
- gamcoin/gamcoin#20197 Protect onions in AttemptToEvictConnection(), add eviction protection test coverage (jonatack)
- gamcoin/gamcoin#20210 assert `CNode::m_inbound_onion` is inbound in ctor, add getter, unit tests (jonatack)
- gamcoin/gamcoin#20228 addrman: Make addrman a top-level component (jnewbery)
- gamcoin/gamcoin#20234 Don't bind on 0.0.0.0 if binds are restricted to Tor (vasild)
- gamcoin/gamcoin#20477 Add unit testing of node eviction logic (practicalswift)
- gamcoin/gamcoin#20516 Well-defined CAddress disk serialization, and addrv2 anchors.dat (sipa)
- gamcoin/gamcoin#20557 addrman: Fix new table bucketing during unserialization (jnewbery)
- gamcoin/gamcoin#20561 Periodically clear `m_addr_known` (sdaftuar)
- gamcoin/gamcoin#20599 net processing: Tolerate sendheaders and sendcmpct messages before verack (jnewbery)
- gamcoin/gamcoin#20616 Check CJDNS address is valid (lontivero)
- gamcoin/gamcoin#20617 Remove `m_is_manual_connection` from CNodeState (ariard)
- gamcoin/gamcoin#20624 net processing: Remove nStartingHeight check from block relay (jnewbery)
- gamcoin/gamcoin#20651 Make p2p recv buffer timeout 20 minutes for all peers (jnewbery)
- gamcoin/gamcoin#20661 Only select from addrv2-capable peers for torv3 address relay (sipa)
- gamcoin/gamcoin#20685 Add I2P support using I2P SAM (vasild)
- gamcoin/gamcoin#20690 Clean up logging of outbound connection type (sdaftuar)
- gamcoin/gamcoin#20721 Move ping data to `net_processing` (jnewbery)
- gamcoin/gamcoin#20724 Cleanup of -debug=net log messages (ajtowns)
- gamcoin/gamcoin#20747 net processing: Remove dropmessagestest (jnewbery)
- gamcoin/gamcoin#20764 cli -netinfo peer connections dashboard updates 🎄 ✨ (jonatack)
- gamcoin/gamcoin#20788 add RAII socket and use it instead of bare SOCKET (vasild)
- gamcoin/gamcoin#20791 remove unused legacyWhitelisted in AcceptConnection() (jonatack)
- gamcoin/gamcoin#20816 Move RecordBytesSent() call out of `cs_vSend` lock (jnewbery)
- gamcoin/gamcoin#20845 Log to net debug in MaybeDiscourageAndDisconnect except for noban and manual peers (MarcoFalke)
- gamcoin/gamcoin#20864 Move SocketSendData lock annotation to header (MarcoFalke)
- gamcoin/gamcoin#20965 net, rpc:  return `NET_UNROUTABLE` as `not_publicly_routable`, automate helps (jonatack)
- gamcoin/gamcoin#20966 banman: save the banlist in a JSON format on disk (vasild)
- gamcoin/gamcoin#21015 Make all of `net_processing` (and some of net) use std::chrono types (dhruv)
- gamcoin/gamcoin#21029 gamcoin-cli: Correct docs (no "generatenewaddress" exists) (luke-jr)
- gamcoin/gamcoin#21148 Split orphan handling from `net_processing` into txorphanage (ajtowns)
- gamcoin/gamcoin#21162 Net Processing: Move RelayTransaction() into PeerManager (jnewbery)
- gamcoin/gamcoin#21167 make `CNode::m_inbound_onion` public, initialize explicitly (jonatack)
- gamcoin/gamcoin#21186 net/net processing: Move addr data into `net_processing` (jnewbery)
- gamcoin/gamcoin#21187 Net processing: Only call PushAddress() from `net_processing` (jnewbery)
- gamcoin/gamcoin#21198 Address outstanding review comments from PR20721 (jnewbery)
- gamcoin/gamcoin#21222 log: Clarify log message when file does not exist (MarcoFalke)
- gamcoin/gamcoin#21235 Clarify disconnect log message in ProcessGetBlockData, remove send bool (MarcoFalke)
- gamcoin/gamcoin#21236 Net processing: Extract `addr` send functionality into MaybeSendAddr() (jnewbery)
- gamcoin/gamcoin#21261 update inbound eviction protection for multiple networks, add I2P peers (jonatack)
- gamcoin/gamcoin#21328 net, refactor: pass uint16 CService::port as uint16 (jonatack)
- gamcoin/gamcoin#21387 Refactor sock to add I2P fuzz and unit tests (vasild)
- gamcoin/gamcoin#21395 Net processing: Remove unused CNodeState.address member (jnewbery)
- gamcoin/gamcoin#21407 i2p: limit the size of incoming messages (vasild)
- gamcoin/gamcoin#21506 p2p, refactor: make NetPermissionFlags an enum class (jonatack)
- gamcoin/gamcoin#21509 Don't send FEEFILTER in blocksonly mode (mzumsande)
- gamcoin/gamcoin#21560 Add Tor v3 hardcoded seeds (laanwj)
- gamcoin/gamcoin#21563 Restrict period when `cs_vNodes` mutex is locked (hebasto)
- gamcoin/gamcoin#21564 Avoid calling getnameinfo when formatting IPv4 addresses in CNetAddr::ToStringIP (practicalswift)
- gamcoin/gamcoin#21631 i2p: always check the return value of Sock::Wait() (vasild)
- gamcoin/gamcoin#21644 p2p, bugfix: use NetPermissions::HasFlag() in CConnman::Bind() (jonatack)
- gamcoin/gamcoin#21659 flag relevant Sock methods with [[nodiscard]] (vasild)
- gamcoin/gamcoin#21750 remove unnecessary check of `CNode::cs_vSend` (vasild)
- gamcoin/gamcoin#21756 Avoid calling `getnameinfo` when formatting IPv6 addresses in `CNetAddr::ToStringIP` (practicalswift)
- gamcoin/gamcoin#21775 Limit `m_block_inv_mutex` (MarcoFalke)
- gamcoin/gamcoin#21825 Add I2P hardcoded seeds (jonatack)
- gamcoin/gamcoin#21843 p2p, rpc: enable GetAddr, GetAddresses, and getnodeaddresses by network (jonatack)
- gamcoin/gamcoin#21845 net processing: Don't require locking `cs_main` before calling RelayTransactions() (jnewbery)
- gamcoin/gamcoin#21872 Sanitize message type for logging (laanwj)
- gamcoin/gamcoin#21914 Use stronger AddLocal() for our I2P address (vasild)
- gamcoin/gamcoin#21985 Return IPv6 scope id in `CNetAddr::ToStringIP()` (laanwj)
- gamcoin/gamcoin#21992 Remove -feefilter option (amadeuszpawlik)
- gamcoin/gamcoin#21996 Pass strings to NetPermissions::TryParse functions by const ref (jonatack)
- gamcoin/gamcoin#22013 ignore block-relay-only peers when skipping DNS seed (ajtowns)
- gamcoin/gamcoin#22050 Remove tor v2 support (jonatack)
- gamcoin/gamcoin#22096 AddrFetch - don't disconnect on self-announcements (mzumsande)
- gamcoin/gamcoin#22141 net processing: Remove hash and fValidatedHeaders from QueuedBlock (jnewbery)
- gamcoin/gamcoin#22144 Randomize message processing peer order (sipa)
- gamcoin/gamcoin#22147 Protect last outbound HB compact block peer (sdaftuar)
- gamcoin/gamcoin#22179 Torv2 removal followups (vasild)
- gamcoin/gamcoin#22211 Relay I2P addresses even if not reachable (by us) (vasild)
- gamcoin/gamcoin#22284 Performance improvements to ProtectEvictionCandidatesByRatio() (jonatack)
- gamcoin/gamcoin#22387 Rate limit the processing of rumoured addresses (sipa)
- gamcoin/gamcoin#22455 addrman: detect on-disk corrupted nNew and nTried during unserialization (vasild)

### Wallet
- gamcoin/gamcoin#15710 Catch `ios_base::failure` specifically (Bushstar)
- gamcoin/gamcoin#16546 External signer support - Wallet Box edition (Sjors)
- gamcoin/gamcoin#17331 Use effective values throughout coin selection (achow101)
- gamcoin/gamcoin#18418 Increase `OUTPUT_GROUP_MAX_ENTRIES` to 100 (fjahr)
- gamcoin/gamcoin#18842 Mark replaced tx to not be in the mempool anymore (MarcoFalke)
- gamcoin/gamcoin#19136 Add `parent_desc` to `getaddressinfo` (achow101)
- gamcoin/gamcoin#19137 wallettool: Add dump and createfromdump commands (achow101)
- gamcoin/gamcoin#19651 `importdescriptor`s update existing (S3RK)
- gamcoin/gamcoin#20040 Refactor OutputGroups to handle fees and spending eligibility on grouping (achow101)
- gamcoin/gamcoin#20202 Make BDB support optional (achow101)
- gamcoin/gamcoin#20226, gamcoin/gamcoin#21277, - gamcoin/gamcoin#21063 Add `listdescriptors` command (S3RK)
- gamcoin/gamcoin#20267 Disable and fix tests for when BDB is not compiled (achow101)
- gamcoin/gamcoin#20275 List all wallets in non-SQLite and non-BDB builds (ryanofsky)
- gamcoin/gamcoin#20365 wallettool: Add parameter to create descriptors wallet (S3RK)
- gamcoin/gamcoin#20403 `upgradewallet` fixes, improvements, test coverage (jonatack)
- gamcoin/gamcoin#20448 `unloadwallet`: Allow specifying `wallet_name` param matching RPC endpoint wallet (luke-jr)
- gamcoin/gamcoin#20536 Error with "Transaction too large" if the funded tx will end up being too large after signing (achow101)
- gamcoin/gamcoin#20687 Add missing check for -descriptors wallet tool option (MarcoFalke)
- gamcoin/gamcoin#20952 Add BerkeleyDB version sanity check at init time (laanwj)
- gamcoin/gamcoin#21127 Load flags before everything else (Sjors)
- gamcoin/gamcoin#21141 Add new format string placeholders for walletnotify (maayank)
- gamcoin/gamcoin#21238 A few descriptor improvements to prepare for Taproot support (sipa)
- gamcoin/gamcoin#21302 `createwallet` examples for descriptor wallets (S3RK)
- gamcoin/gamcoin#21329 descriptor wallet: Cache last hardened xpub and use in normalized descriptors (achow101)
- gamcoin/gamcoin#21365 Basic Taproot signing support for descriptor wallets (sipa)
- gamcoin/gamcoin#21417 Misc external signer improvement and HWI 2 support (Sjors)
- gamcoin/gamcoin#21467 Move external signer out of wallet module (Sjors)
- gamcoin/gamcoin#21572 Fix wrong wallet RPC context set after #21366 (ryanofsky)
- gamcoin/gamcoin#21574 Drop JSONRPCRequest constructors after #21366 (ryanofsky)
- gamcoin/gamcoin#21666 Miscellaneous external signer changes (fanquake)
- gamcoin/gamcoin#21759 Document coin selection code (glozow)
- gamcoin/gamcoin#21786 Ensure sat/vB feerates are in range (mantissa of 3) (jonatack)
- gamcoin/gamcoin#21944 Fix issues when `walletdir` is root directory (prayank23)
- gamcoin/gamcoin#22042 Replace size/weight estimate tuple with struct for named fields (instagibbs)
- gamcoin/gamcoin#22051 Basic Taproot derivation support for descriptors (sipa)
- gamcoin/gamcoin#22154 Add OutputType::BECH32M and related wallet support for fetching bech32m addresses (achow101)
- gamcoin/gamcoin#22156 Allow tr() import only when Taproot is active (achow101)
- gamcoin/gamcoin#22166 Add support for inferring tr() descriptors (sipa)
- gamcoin/gamcoin#22173 Do not load external signers wallets when unsupported (achow101)
- gamcoin/gamcoin#22308 Add missing BlockUntilSyncedToCurrentChain (MarcoFalke)
- gamcoin/gamcoin#22334 Do not spam about non-existent spk managers (S3RK)
- gamcoin/gamcoin#22379 Erase spkmans rather than setting to nullptr (achow101)
- gamcoin/gamcoin#22421 Make IsSegWitOutput return true for taproot outputs (sipa)
- gamcoin/gamcoin#22461 Change ScriptPubKeyMan::Upgrade default to True (achow101)
- gamcoin/gamcoin#22492 Reorder locks in dumpwallet to avoid lock order assertion (achow101)
- gamcoin/gamcoin#22686 Use GetSelectionAmount in ApproximateBestSubset (achow101)

### RPC and other APIs
- gamcoin/gamcoin#18335, gamcoin/gamcoin#21484 cli: Print useful error if gamcoind rpc work queue exceeded (LarryRuane)
- gamcoin/gamcoin#18466 Fix invalid parameter error codes for `{sign,verify}message` RPCs (theStack)
- gamcoin/gamcoin#18772 Calculate fees in `getblock` using BlockUndo data (robot-visions)
- gamcoin/gamcoin#19033 http: Release work queue after event base finish (promag)
- gamcoin/gamcoin#19055 Add MuHash3072 implementation (fjahr)
- gamcoin/gamcoin#19145 Add `hash_type` MUHASH for gettxoutsetinfo (fjahr)
- gamcoin/gamcoin#19847 Avoid duplicate set lookup in `gettxoutproof` (promag)
- gamcoin/gamcoin#20286 Deprecate `addresses` and `reqSigs` from RPC outputs (mjdietzx)
- gamcoin/gamcoin#20459 Fail to return undocumented return values (MarcoFalke)
- gamcoin/gamcoin#20461 Validate `-rpcauth` arguments (promag)
- gamcoin/gamcoin#20556 Properly document return values (`submitblock`, `gettxout`, `getblocktemplate`, `scantxoutset`) (MarcoFalke)
- gamcoin/gamcoin#20755 Remove deprecated fields from `getpeerinfo` (amitiuttarwar)
- gamcoin/gamcoin#20832 Better error messages for invalid addresses (eilx2)
- gamcoin/gamcoin#20867 Support up to 20 keys for multisig under Segwit context (darosior)
- gamcoin/gamcoin#20877 cli: `-netinfo` user help and argument parsing improvements (jonatack)
- gamcoin/gamcoin#20891 Remove deprecated bumpfee behavior (achow101)
- gamcoin/gamcoin#20916 Return wtxid from `testmempoolaccept` (MarcoFalke)
- gamcoin/gamcoin#20917 Add missing signet mentions in network name lists (theStack)
- gamcoin/gamcoin#20941 Document `RPC_TRANSACTION_ALREADY_IN_CHAIN` exception (jarolrod)
- gamcoin/gamcoin#20944 Return total fee in `getmempoolinfo` (MarcoFalke)
- gamcoin/gamcoin#20964 Add specific error code for "wallet already loaded" (laanwj)
- gamcoin/gamcoin#21053 Document {previous,next}blockhash as optional (theStack)
- gamcoin/gamcoin#21056 Add a `-rpcwaittimeout` parameter to limit time spent waiting (cdecker)
- gamcoin/gamcoin#21192 cli: Treat high detail levels as maximum in `-netinfo` (laanwj)
- gamcoin/gamcoin#21311 Document optional fields for `getchaintxstats` result (theStack)
- gamcoin/gamcoin#21359 `include_unsafe` option for fundrawtransaction (t-bast)
- gamcoin/gamcoin#21426 Remove `scantxoutset` EXPERIMENTAL warning (jonatack)
- gamcoin/gamcoin#21544 Missing doc updates for bumpfee psbt update (MarcoFalke)
- gamcoin/gamcoin#21594 Add `network` field to `getnodeaddresses` (jonatack)
- gamcoin/gamcoin#21595, gamcoin/gamcoin#21753 cli: Create `-addrinfo` (jonatack)
- gamcoin/gamcoin#21602 Add additional ban time fields to `listbanned` (jarolrod)
- gamcoin/gamcoin#21679 Keep default argument value in correct type (promag)
- gamcoin/gamcoin#21718 Improve error message for `getblock` invalid datatype (klementtan)
- gamcoin/gamcoin#21913 RPCHelpMan fixes (kallewoof)
- gamcoin/gamcoin#22021 `bumpfee`/`psbtbumpfee` fixes and updates (jonatack)
- gamcoin/gamcoin#22043 `addpeeraddress` test coverage, code simplify/constness (jonatack)
- gamcoin/gamcoin#22327 cli: Avoid truncating `-rpcwaittimeout` (MarcoFalke)

### GUI
- gamcoin/gamcoin#18948 Call setParent() in the parent's context (hebasto)
- gamcoin/gamcoin#20482 Add depends qt fix for ARM macs (jonasschnelli)
- gamcoin/gamcoin#21836 scripted-diff: Replace three dots with ellipsis in the ui strings (hebasto)
- gamcoin/gamcoin#21935 Enable external signer support for GUI builds (Sjors)
- gamcoin/gamcoin#22133 Make QWindowsVistaStylePlugin available again (regression) (hebasto)
- gamcoin-core/gui#4 UI external signer support (e.g. hardware wallet) (Sjors)
- gamcoin-core/gui#13 Hide peer detail view if multiple are selected (promag)
- gamcoin-core/gui#18 Add peertablesortproxy module (hebasto)
- gamcoin-core/gui#21 Improve pruning tooltip (fluffypony, GamcoinErrorLog)
- gamcoin-core/gui#72 Log static plugins meta data and used style (hebasto)
- gamcoin-core/gui#79 Embed monospaced font (hebasto)
- gamcoin-core/gui#85 Remove unused "What's This" button in dialogs on Windows OS (hebasto)
- gamcoin-core/gui#115 Replace "Hide tray icon" option with positive "Show tray icon" one (hebasto)
- gamcoin-core/gui#118 Remove BDB version from the Information tab (hebasto)
- gamcoin-core/gui#121 Early subscribe core signals in transaction table model (promag)
- gamcoin-core/gui#123 Do not accept command while executing another one (hebasto)
- gamcoin-core/gui#125 Enable changing the autoprune block space size in intro dialog (luke-jr)
- gamcoin-core/gui#138 Unlock encrypted wallet "OK" button bugfix (mjdietzx)
- gamcoin-core/gui#139 doc: Improve gui/src/qt README.md (jarolrod)
- gamcoin-core/gui#154 Support macOS Dark mode (goums, Uplab)
- gamcoin-core/gui#162 Add network to peers window and peer details (jonatack)
- gamcoin-core/gui#163, gamcoin-core/gui#180 Peer details: replace Direction with Connection Type (jonatack)
- gamcoin-core/gui#164 Handle peer addition/removal in a right way (hebasto)
- gamcoin-core/gui#165 Save QSplitter state in QSettings (hebasto)
- gamcoin-core/gui#173 Follow Qt docs when implementing rowCount and columnCount (hebasto)
- gamcoin-core/gui#179 Add Type column to peers window, update peer details name/tooltip (jonatack)
- gamcoin-core/gui#186 Add information to "Confirm fee bump" window (prayank23)
- gamcoin-core/gui#189 Drop workaround for QTBUG-42503 which was fixed in Qt 5.5.0 (prusnak)
- gamcoin-core/gui#194 Save/restore RPCConsole geometry only for window (hebasto)
- gamcoin-core/gui#202 Fix right panel toggle in peers tab (RandyMcMillan)
- gamcoin-core/gui#203 Display plain "Inbound" in peer details (jonatack)
- gamcoin-core/gui#204 Drop buggy TableViewLastColumnResizingFixer class (hebasto)
- gamcoin-core/gui#205, gamcoin-core/gui#229 Save/restore TransactionView and recentRequestsView tables column sizes (hebasto)
- gamcoin-core/gui#206 Display fRelayTxes and `bip152_highbandwidth_{to, from}` in peer details (jonatack)
- gamcoin-core/gui#213 Add Copy Address Action to Payment Requests (jarolrod)
- gamcoin-core/gui#214 Disable requests context menu actions when appropriate (jarolrod)
- gamcoin-core/gui#217 Make warning label look clickable (jarolrod)
- gamcoin-core/gui#219 Prevent the main window popup menu (hebasto)
- gamcoin-core/gui#220 Do not translate file extensions (hebasto)
- gamcoin-core/gui#221 RPCConsole translatable string fixes and improvements (jonatack)
- gamcoin-core/gui#226 Add "Last Block" and "Last Tx" rows to peer details area (jonatack)
- gamcoin-core/gui#233 qt test: Don't bind to regtest port (achow101)
- gamcoin-core/gui#243 Fix issue when disabling the auto-enabled blank wallet checkbox (jarolrod)
- gamcoin-core/gui#246 Revert "qt: Use "fusion" style on macOS Big Sur with old Qt" (hebasto)
- gamcoin-core/gui#248 For values of "Bytes transferred" and "Bytes/s" with 1000-based prefix names use 1000-based divisor instead of 1024-based (wodry)
- gamcoin-core/gui#251 Improve URI/file handling message (hebasto)
- gamcoin-core/gui#256 Save/restore column sizes of the tables in the Peers tab (hebasto)
- gamcoin-core/gui#260 Handle exceptions isntead of crash (hebasto)
- gamcoin-core/gui#263 Revamp context menus (hebasto)
- gamcoin-core/gui#271 Don't clear console prompt when font resizing (jarolrod)
- gamcoin-core/gui#275 Support runtime appearance adjustment on macOS (hebasto)
- gamcoin-core/gui#276 Elide long strings in their middle in the Peers tab (hebasto)
- gamcoin-core/gui#281 Set shortcuts for console's resize buttons (jarolrod)
- gamcoin-core/gui#293 Enable wordWrap for Services (RandyMcMillan)
- gamcoin-core/gui#296 Do not use QObject::tr plural syntax for numbers with a unit symbol (hebasto)
- gamcoin-core/gui#297 Avoid unnecessary translations (hebasto)
- gamcoin-core/gui#298 Peertableview alternating row colors (RandyMcMillan)
- gamcoin-core/gui#300 Remove progress bar on modal overlay (brunoerg)
- gamcoin-core/gui#309 Add access to the Peers tab from the network icon (hebasto)
- gamcoin-core/gui#311 Peers Window rename 'Peer id' to 'Peer' (jarolrod)
- gamcoin-core/gui#313 Optimize string concatenation by default (hebasto)
- gamcoin-core/gui#325 Align numbers in the "Peer Id" column to the right (hebasto)
- gamcoin-core/gui#329 Make console buttons look clickable (jarolrod)
- gamcoin-core/gui#330 Allow prompt icon to be colorized (jarolrod)
- gamcoin-core/gui#331 Make RPC console welcome message translation-friendly (hebasto)
- gamcoin-core/gui#332 Replace disambiguation strings with translator comments (hebasto)
- gamcoin-core/gui#335 test: Use QSignalSpy instead of QEventLoop (jarolrod)
- gamcoin-core/gui#343 Improve the GUI responsiveness when progress dialogs are used (hebasto)
- gamcoin-core/gui#361 Fix GUI segfault caused by gamcoin/gamcoin#22216 (ryanofsky)
- gamcoin-core/gui#362 Add keyboard shortcuts to context menus (luke-jr)
- gamcoin-core/gui#366 Dark Mode fixes/portability (luke-jr)
- gamcoin-core/gui#375 Emit dataChanged signal to dynamically re-sort Peers table (hebasto)
- gamcoin-core/gui#393 Fix regression in "Encrypt Wallet" menu item (hebasto)
- gamcoin-core/gui#396 Ensure external signer option remains disabled without signers (achow101)
- gamcoin-core/gui#406 Handle new added plurals in `gamcoin_en.ts` (hebasto)

### Build system
- gamcoin/gamcoin#17227 Add Android packaging support (icota)
- gamcoin/gamcoin#17920 guix: Build support for macOS (dongcarl)
- gamcoin/gamcoin#18298 Fix Qt processing of configure script for depends with DEBUG=1 (hebasto)
- gamcoin/gamcoin#19160 multiprocess: Add basic spawn and IPC support (ryanofsky)
- gamcoin/gamcoin#19504 Bump minimum python version to 3.6 (ajtowns)
- gamcoin/gamcoin#19522 fix building libconsensus with reduced exports for Darwin targets (fanquake)
- gamcoin/gamcoin#19683 Pin clang search paths for darwin host (dongcarl)
- gamcoin/gamcoin#19764 Split boost into build/host packages + bump + cleanup (dongcarl)
- gamcoin/gamcoin#19817 libtapi 1100.0.11 (fanquake)
- gamcoin/gamcoin#19846 enable unused member function diagnostic (Zero-1729)
- gamcoin/gamcoin#19867 Document and cleanup Qt hacks (fanquake)
- gamcoin/gamcoin#20046 Set `CMAKE_INSTALL_RPATH` for native packages (ryanofsky)
- gamcoin/gamcoin#20223 Drop the leading 0 from the version number (achow101)
- gamcoin/gamcoin#20333 Remove `native_biplist` dependency (fanquake)
- gamcoin/gamcoin#20353 configure: Support -fdebug-prefix-map and -fmacro-prefix-map (ajtowns)
- gamcoin/gamcoin#20359 Various config.site.in improvements and linting (dongcarl)
- gamcoin/gamcoin#20413 Require C++17 compiler (MarcoFalke)
- gamcoin/gamcoin#20419 Set minimum supported macOS to 10.14 (fanquake)
- gamcoin/gamcoin#20421 miniupnpc 2.2.2 (fanquake)
- gamcoin/gamcoin#20422 Mac deployment unification (fanquake)
- gamcoin/gamcoin#20424 Update univalue subtree (MarcoFalke)
- gamcoin/gamcoin#20449 Fix Windows installer build (achow101)
- gamcoin/gamcoin#20468 Warn when generating man pages for binaries built from a dirty branch (tylerchambers)
- gamcoin/gamcoin#20469 Avoid secp256k1.h include from system (dergoegge)
- gamcoin/gamcoin#20470 Replace genisoimage with xorriso (dongcarl)
- gamcoin/gamcoin#20471 Use C++17 in depends (fanquake)
- gamcoin/gamcoin#20496 Drop unneeded macOS framework dependencies (hebasto)
- gamcoin/gamcoin#20520 Do not force Precompiled Headers (PCH) for building Qt on Linux (hebasto)
- gamcoin/gamcoin#20549 Support make src/gamcoin-node and src/gamcoin-gui (promag)
- gamcoin/gamcoin#20565 Ensure PIC build for bdb on Android (BlockMechanic)
- gamcoin/gamcoin#20594 Fix getauxval calls in randomenv.cpp (jonasschnelli)
- gamcoin/gamcoin#20603 Update crc32c subtree (MarcoFalke)
- gamcoin/gamcoin#20609 configure: output notice that test binary is disabled by fuzzing (apoelstra)
- gamcoin/gamcoin#20619 guix: Quality of life improvements (dongcarl)
- gamcoin/gamcoin#20629 Improve id string robustness (dongcarl)
- gamcoin/gamcoin#20641 Use Qt top-level build facilities (hebasto)
- gamcoin/gamcoin#20650 Drop workaround for a fixed bug in Qt build system (hebasto)
- gamcoin/gamcoin#20673 Use more legible qmake commands in qt package (hebasto)
- gamcoin/gamcoin#20684 Define .INTERMEDIATE target once only (hebasto)
- gamcoin/gamcoin#20720 more robustly check for fcf-protection support (fanquake)
- gamcoin/gamcoin#20734 Make platform-specific targets available for proper platform builds only (hebasto)
- gamcoin/gamcoin#20936 build fuzz tests by default (danben)
- gamcoin/gamcoin#20937 guix: Make nsis reproducible by respecting SOURCE-DATE-EPOCH (dongcarl)
- gamcoin/gamcoin#20938 fix linking against -latomic when building for riscv (fanquake)
- gamcoin/gamcoin#20939 fix `RELOC_SECTION` security check for gamcoin-util (fanquake)
- gamcoin/gamcoin#20963 gitian-linux: Build binaries for 64-bit POWER (continued) (laanwj)
- gamcoin/gamcoin#21036 gitian: Bump descriptors to focal for 22.0 (fanquake)
- gamcoin/gamcoin#21045 Adds switch to enable/disable randomized base address in MSVC builds (EthanHeilman)
- gamcoin/gamcoin#21065 make macOS HOST in download-osx generic (fanquake)
- gamcoin/gamcoin#21078 guix: only download sources for hosts being built (fanquake)
- gamcoin/gamcoin#21116 Disable --disable-fuzz-binary for gitian/guix builds (hebasto)
- gamcoin/gamcoin#21182 remove mostly pointless `BOOST_PROCESS` macro (fanquake)
- gamcoin/gamcoin#21205 actually fail when Boost is missing (fanquake)
- gamcoin/gamcoin#21209 use newer source for libnatpmp (fanquake)
- gamcoin/gamcoin#21226 Fix fuzz binary compilation under windows (danben)
- gamcoin/gamcoin#21231 Add /opt/homebrew to path to look for boost libraries (fyquah)
- gamcoin/gamcoin#21239 guix: Add codesignature attachment support for osx+win (dongcarl)
- gamcoin/gamcoin#21250 Make `HAVE_O_CLOEXEC` available outside LevelDB (bugfix) (theStack)
- gamcoin/gamcoin#21272 guix: Passthrough `SDK_PATH` into container (dongcarl)
- gamcoin/gamcoin#21274 assumptions:  Assume C++17 (fanquake)
- gamcoin/gamcoin#21286 Bump minimum Qt version to 5.9.5 (hebasto)
- gamcoin/gamcoin#21298 guix: Bump time-machine, glibc, and linux-headers (dongcarl)
- gamcoin/gamcoin#21304 guix: Add guix-clean script + establish gc-root for container profiles (dongcarl)
- gamcoin/gamcoin#21320 fix libnatpmp macos cross compile (fanquake)
- gamcoin/gamcoin#21321 guix: Add curl to required tool list (hebasto)
- gamcoin/gamcoin#21333 set Unicode true for NSIS installer (fanquake)
- gamcoin/gamcoin#21339 Make `AM_CONDITIONAL([ENABLE_EXTERNAL_SIGNER])` unconditional (hebasto)
- gamcoin/gamcoin#21349 Fix fuzz-cuckoocache cross-compiling with DEBUG=1 (hebasto)
- gamcoin/gamcoin#21354 build, doc: Drop no longer required packages from macOS cross-compiling dependencies (hebasto)
- gamcoin/gamcoin#21363 build, qt: Improve Qt static plugins/libs check code (hebasto)
- gamcoin/gamcoin#21375 guix: Misc feedback-based fixes + hier restructuring (dongcarl)
- gamcoin/gamcoin#21376 Qt 5.12.10 (fanquake)
- gamcoin/gamcoin#21382 Clean remnants of QTBUG-34748 fix (hebasto)
- gamcoin/gamcoin#21400 Fix regression introduced in #21363 (hebasto)
- gamcoin/gamcoin#21403 set --build when configuring packages in depends (fanquake)
- gamcoin/gamcoin#21421 don't try and use -fstack-clash-protection on Windows (fanquake)
- gamcoin/gamcoin#21423 Cleanups and follow ups after bumping Qt to 5.12.10 (hebasto)
- gamcoin/gamcoin#21427 Fix `id_string` invocations (dongcarl)
- gamcoin/gamcoin#21430 Add -Werror=implicit-fallthrough compile flag (hebasto)
- gamcoin/gamcoin#21457 Split libtapi and clang out of `native_cctools` (fanquake)
- gamcoin/gamcoin#21462 guix: Add guix-{attest,verify} scripts (dongcarl)
- gamcoin/gamcoin#21495 build, qt: Fix static builds on macOS Big Sur (hebasto)
- gamcoin/gamcoin#21497 Do not opt-in unused CoreWLAN stuff in depends for macOS (hebasto)
- gamcoin/gamcoin#21543 Enable safe warnings for msvc builds (hebasto)
- gamcoin/gamcoin#21565 Make `gamcoin_qt.m4` more generic (fanquake)
- gamcoin/gamcoin#21610 remove -Wdeprecated-register from NOWARN flags (fanquake)
- gamcoin/gamcoin#21613 enable -Wdocumentation (fanquake)
- gamcoin/gamcoin#21629 Fix configuring when building depends with `NO_BDB=1` (fanquake)
- gamcoin/gamcoin#21654 build, qt: Make Qt rcc output always deterministic (hebasto)
- gamcoin/gamcoin#21655 build, qt: No longer need to set `QT_RCC_TEST=1` for determinism (hebasto)
- gamcoin/gamcoin#21658 fix make deploy for arm64-darwin (sgulls)
- gamcoin/gamcoin#21694 Use XLIFF file to provide more context to Transifex translators (hebasto)
- gamcoin/gamcoin#21708, gamcoin/gamcoin#21593 Drop pointless sed commands (hebasto)
- gamcoin/gamcoin#21731 Update msvc build to use Qt5.12.10 binaries (sipsorcery)
- gamcoin/gamcoin#21733 Re-add command to install vcpkg (dplusplus1024)
- gamcoin/gamcoin#21793 Use `-isysroot` over `--sysroot` on macOS (fanquake)
- gamcoin/gamcoin#21869 Add missing `-D_LIBCPP_DEBUG=1` to debug flags (MarcoFalke)
- gamcoin/gamcoin#21889 macho: check for control flow instrumentation (fanquake)
- gamcoin/gamcoin#21920 Improve macro for testing -latomic requirement (MarcoFalke)
- gamcoin/gamcoin#21991 libevent 2.1.12-stable (fanquake)
- gamcoin/gamcoin#22054 Bump Qt version to 5.12.11 (hebasto)
- gamcoin/gamcoin#22063 Use Qt archive of the same version as the compiled binaries (hebasto)
- gamcoin/gamcoin#22070 Don't use cf-protection when targeting arm-apple-darwin (fanquake)
- gamcoin/gamcoin#22071 Latest config.guess and config.sub (fanquake)
- gamcoin/gamcoin#22075 guix: Misc leftover usability improvements (dongcarl)
- gamcoin/gamcoin#22123 Fix qt.mk for mac arm64 (promag)
- gamcoin/gamcoin#22174 build, qt: Fix libraries linking order for Linux hosts (hebasto)
- gamcoin/gamcoin#22182 guix: Overhaul how guix-{attest,verify} works and hierarchy (dongcarl)
- gamcoin/gamcoin#22186 build, qt: Fix compiling qt package in depends with GCC 11 (hebasto)
- gamcoin/gamcoin#22199 macdeploy: minor fixups and simplifications (fanquake)
- gamcoin/gamcoin#22230 Fix MSVC linker /SubSystem option for gamcoin-qt.exe (hebasto)
- gamcoin/gamcoin#22234 Mark print-% target as phony (dgoncharov)
- gamcoin/gamcoin#22238 improve detection of eBPF support (fanquake)
- gamcoin/gamcoin#22258 Disable deprecated-copy warning only when external warnings are enabled (MarcoFalke)
- gamcoin/gamcoin#22320 set minimum required Boost to 1.64.0 (fanquake)
- gamcoin/gamcoin#22348 Fix cross build for Windows with Boost Process (hebasto)
- gamcoin/gamcoin#22365 guix: Avoid relying on newer symbols by rebasing our cross toolchains on older glibcs (dongcarl)
- gamcoin/gamcoin#22381 guix: Test security-check sanity before performing them (with macOS) (fanquake)
- gamcoin/gamcoin#22405 Remove --enable-glibc-back-compat from Guix build (fanquake)
- gamcoin/gamcoin#22406 Remove --enable-determinism configure option (fanquake)
- gamcoin/gamcoin#22410 Avoid GCC 7.1 ABI change warning in guix build (sipa)
- gamcoin/gamcoin#22436 use aarch64 Clang if cross-compiling for darwin on aarch64 (fanquake)
- gamcoin/gamcoin#22465 guix: Pin kernel-header version, time-machine to upstream 1.3.0 commit (dongcarl)
- gamcoin/gamcoin#22511 guix: Silence `getent(1)` invocation, doc fixups (dongcarl)
- gamcoin/gamcoin#22531 guix: Fixes to guix-{attest,verify} (achow101)
- gamcoin/gamcoin#22642 release: Release with separate sha256sums and sig files (dongcarl)
- gamcoin/gamcoin#22685 clientversion: No suffix `#if CLIENT_VERSION_IS_RELEASE` (dongcarl)
- gamcoin/gamcoin#22713 Fix build with Boost 1.77.0 (sizeofvoid)

### Tests and QA
- gamcoin/gamcoin#14604 Add test and refactor `feature_block.py` (sanket1729)
- gamcoin/gamcoin#17556 Change `feature_config_args.py` not to rely on strange regtest=0 behavior (ryanofsky)
- gamcoin/gamcoin#18795 wallet issue with orphaned rewards (domob1812)
- gamcoin/gamcoin#18847 compressor: Use a prevector in CompressScript serialization (jb55)
- gamcoin/gamcoin#19259 fuzz: Add fuzzing harness for LoadMempool(…) and DumpMempool(…) (practicalswift)
- gamcoin/gamcoin#19315 Allow outbound & block-relay-only connections in functional tests. (amitiuttarwar)
- gamcoin/gamcoin#19698 Apply strict verification flags for transaction tests and assert backwards compatibility (glozow)
- gamcoin/gamcoin#19801 Check for all possible `OP_CLTV` fail reasons in `feature_cltv.py` (BIP 65) (theStack)
- gamcoin/gamcoin#19893 Remove or explain syncwithvalidationinterfacequeue (MarcoFalke)
- gamcoin/gamcoin#19972 fuzz: Add fuzzing harness for node eviction logic (practicalswift)
- gamcoin/gamcoin#19982 Fix inconsistent lock order in `wallet_tests/CreateWallet` (hebasto)
- gamcoin/gamcoin#20000 Fix creation of "std::string"s with \0s (vasild)
- gamcoin/gamcoin#20047 Use `wait_for_{block,header}` helpers in `p2p_fingerprint.py` (theStack)
- gamcoin/gamcoin#20171 Add functional test `test_txid_inv_delay` (ariard)
- gamcoin/gamcoin#20189 Switch to BIP341's suggested scheme for outputs without script (sipa)
- gamcoin/gamcoin#20248 Fix length of R check in `key_signature_tests` (dgpv)
- gamcoin/gamcoin#20276, gamcoin/gamcoin#20385, gamcoin/gamcoin#20688, gamcoin/gamcoin#20692 Run various mempool tests even with wallet disabled (mjdietzx)
- gamcoin/gamcoin#20323 Create or use existing properly initialized NodeContexts (dongcarl)
- gamcoin/gamcoin#20354 Add `feature_taproot.py --previous_release` (MarcoFalke)
- gamcoin/gamcoin#20370 fuzz: Version handshake (MarcoFalke)
- gamcoin/gamcoin#20377 fuzz: Fill various small fuzzing gaps (practicalswift)
- gamcoin/gamcoin#20425 fuzz: Make CAddrMan fuzzing harness deterministic (practicalswift)
- gamcoin/gamcoin#20430 Sanitizers: Add suppression for unsigned-integer-overflow in libstdc++ (jonasschnelli)
- gamcoin/gamcoin#20437 fuzz: Avoid time-based "non-determinism" in fuzzing harnesses by using mocked GetTime() (practicalswift)
- gamcoin/gamcoin#20458 Add `is_bdb_compiled` helper (Sjors)
- gamcoin/gamcoin#20466 Fix intermittent `p2p_fingerprint` issue (MarcoFalke)
- gamcoin/gamcoin#20472 Add testing of ParseInt/ParseUInt edge cases with leading +/-/0:s (practicalswift)
- gamcoin/gamcoin#20507 sync: print proper lock order location when double lock is detected (vasild)
- gamcoin/gamcoin#20522 Fix sync issue in `disconnect_p2ps` (amitiuttarwar)
- gamcoin/gamcoin#20524 Move `MIN_VERSION_SUPPORTED` to p2p.py (jnewbery)
- gamcoin/gamcoin#20540 Fix `wallet_multiwallet` issue on windows (MarcoFalke)
- gamcoin/gamcoin#20560 fuzz: Link all targets once (MarcoFalke)
- gamcoin/gamcoin#20567 Add option to git-subtree-check to do full check, add help (laanwj)
- gamcoin/gamcoin#20569 Fix intermittent `wallet_multiwallet` issue with `got_loading_error` (MarcoFalke)
- gamcoin/gamcoin#20613 Use Popen.wait instead of RPC in `assert_start_raises_init_error` (MarcoFalke)
- gamcoin/gamcoin#20663 fuzz: Hide `script_assets_test_minimizer` (MarcoFalke)
- gamcoin/gamcoin#20674 fuzz: Call SendMessages after ProcessMessage to increase coverage (MarcoFalke)
- gamcoin/gamcoin#20683 Fix restart node race (MarcoFalke)
- gamcoin/gamcoin#20686 fuzz: replace CNode code with fuzz/util.h::ConsumeNode() (jonatack)
- gamcoin/gamcoin#20733 Inline non-member functions with body in fuzzing headers (pstratem)
- gamcoin/gamcoin#20737 Add missing assignment in `mempool_resurrect.py` (MarcoFalke)
- gamcoin/gamcoin#20745 Correct `epoll_ctl` data race suppression (hebasto)
- gamcoin/gamcoin#20748 Add race:SendZmqMessage tsan suppression (MarcoFalke)
- gamcoin/gamcoin#20760 Set correct nValue for multi-op-return policy check (MarcoFalke)
- gamcoin/gamcoin#20761 fuzz: Check that `NULL_DATA` is unspendable (MarcoFalke)
- gamcoin/gamcoin#20765 fuzz: Check that certain script TxoutType are nonstandard (mjdietzx)
- gamcoin/gamcoin#20772 fuzz: Bolster ExtractDestination(s) checks (mjdietzx)
- gamcoin/gamcoin#20789 fuzz: Rework strong and weak net enum fuzzing (MarcoFalke)
- gamcoin/gamcoin#20828 fuzz: Introduce CallOneOf helper to replace switch-case (MarcoFalke)
- gamcoin/gamcoin#20839 fuzz: Avoid extraneous copy of input data, using Span<> (MarcoFalke)
- gamcoin/gamcoin#20844 Add sanitizer suppressions for AMD EPYC CPUs (MarcoFalke)
- gamcoin/gamcoin#20857 Update documentation in `feature_csv_activation.py` (PiRK)
- gamcoin/gamcoin#20876 Replace getmempoolentry with testmempoolaccept in MiniWallet (MarcoFalke)
- gamcoin/gamcoin#20881 fuzz: net permission flags in net processing (MarcoFalke)
- gamcoin/gamcoin#20882 fuzz: Add missing muhash registration (MarcoFalke)
- gamcoin/gamcoin#20908 fuzz: Use mocktime in `process_message*` fuzz targets (MarcoFalke)
- gamcoin/gamcoin#20915 fuzz: Fail if message type is not fuzzed (MarcoFalke)
- gamcoin/gamcoin#20946 fuzz: Consolidate fuzzing TestingSetup initialization (dongcarl)
- gamcoin/gamcoin#20954 Declare `nodes` type `in test_framework.py` (kiminuo)
- gamcoin/gamcoin#20955 Fix `get_previous_releases.py` for aarch64 (MarcoFalke)
- gamcoin/gamcoin#20969 check that getblockfilter RPC fails without block filter index (theStack)
- gamcoin/gamcoin#20971 Work around libFuzzer deadlock (MarcoFalke)
- gamcoin/gamcoin#20993 Store subversion (user agent) as string in `msg_version` (theStack)
- gamcoin/gamcoin#20995 fuzz: Avoid initializing version to less than `MIN_PEER_PROTO_VERSION` (MarcoFalke)
- gamcoin/gamcoin#20998 Fix BlockToJsonVerbose benchmark (martinus)
- gamcoin/gamcoin#21003 Move MakeNoLogFileContext to `libtest_util`, and use it in bench (MarcoFalke)
- gamcoin/gamcoin#21008 Fix zmq test flakiness, improve speed (theStack)
- gamcoin/gamcoin#21023 fuzz: Disable shuffle when merge=1 (MarcoFalke)
- gamcoin/gamcoin#21037 fuzz: Avoid designated initialization (C++20) in fuzz tests (practicalswift)
- gamcoin/gamcoin#21042 doc, test: Improve `setup_clean_chain` documentation (fjahr)
- gamcoin/gamcoin#21080 fuzz: Configure check for main function (take 2) (MarcoFalke)
- gamcoin/gamcoin#21084 Fix timeout decrease in `feature_assumevalid` (brunoerg)
- gamcoin/gamcoin#21096 Re-add dead code detection (flack)
- gamcoin/gamcoin#21100 Remove unused function `xor_bytes` (theStack)
- gamcoin/gamcoin#21115 Fix Windows cross build (hebasto)
- gamcoin/gamcoin#21117 Remove `assert_blockchain_height` (MarcoFalke)
- gamcoin/gamcoin#21121 Small unit test improvements, including helper to make mempool transaction (amitiuttarwar)
- gamcoin/gamcoin#21124 Remove unnecessary assignment in bdb (brunoerg)
- gamcoin/gamcoin#21125 Change `BOOST_CHECK` to `BOOST_CHECK_EQUAL` for paths (kiminuo)
- gamcoin/gamcoin#21142, gamcoin/gamcoin#21512 fuzz: Add `tx_pool` fuzz target (MarcoFalke)
- gamcoin/gamcoin#21165 Use mocktime in `test_seed_peers` (dhruv)
- gamcoin/gamcoin#21169 fuzz: Add RPC interface fuzzing. Increase fuzzing coverage from 65% to 70% (practicalswift)
- gamcoin/gamcoin#21170 bench: Add benchmark to write json into a string (martinus)
- gamcoin/gamcoin#21178 Run `mempool_reorg.py` even with wallet disabled (DariusParvin)
- gamcoin/gamcoin#21185 fuzz: Remove expensive and redundant muhash from crypto fuzz target (MarcoFalke)
- gamcoin/gamcoin#21200 Speed up `rpc_blockchain.py` by removing miniwallet.generate() (MarcoFalke)
- gamcoin/gamcoin#21211 Move `P2WSH_OP_TRUE` to shared test library (MarcoFalke)
- gamcoin/gamcoin#21228 Avoid comparision of integers with different signs (jonasschnelli)
- gamcoin/gamcoin#21230 Fix `NODE_NETWORK_LIMITED_MIN_BLOCKS` disconnection (MarcoFalke)
- gamcoin/gamcoin#21252 Add missing wait for sync to `feature_blockfilterindex_prune` (MarcoFalke)
- gamcoin/gamcoin#21254 Avoid connecting to real network when running tests (MarcoFalke)
- gamcoin/gamcoin#21264 fuzz: Two scripted diff renames (MarcoFalke)
- gamcoin/gamcoin#21280 Bug fix in `transaction_tests` (glozow)
- gamcoin/gamcoin#21293 Replace accidentally placed bit-OR with logical-OR (hebasto)
- gamcoin/gamcoin#21297 `feature_blockfilterindex_prune.py` improvements (jonatack)
- gamcoin/gamcoin#21310 zmq test: fix sync-up by matching notification to generated block (theStack)
- gamcoin/gamcoin#21334 Additional BIP9 tests (Sjors)
- gamcoin/gamcoin#21338 Add functional test for anchors.dat (brunoerg)
- gamcoin/gamcoin#21345 Bring `p2p_leak.py` up to date (mzumsande)
- gamcoin/gamcoin#21357 Unconditionally check for fRelay field in test framework (jarolrod)
- gamcoin/gamcoin#21358 fuzz: Add missing include (`test/util/setup_common.h`) (MarcoFalke)
- gamcoin/gamcoin#21371 fuzz: fix gcc Woverloaded-virtual build warnings (jonatack)
- gamcoin/gamcoin#21373 Generate fewer blocks in `feature_nulldummy` to fix timeouts, speed up (jonatack)
- gamcoin/gamcoin#21390 Test improvements for UTXO set hash tests (fjahr)
- gamcoin/gamcoin#21410 increase `rpc_timeout` for fundrawtx `test_transaction_too_large` (jonatack)
- gamcoin/gamcoin#21411 add logging, reduce blocks, move `sync_all` in `wallet_` groups (jonatack)
- gamcoin/gamcoin#21438 Add ParseUInt8() test coverage (jonatack)
- gamcoin/gamcoin#21443 fuzz: Implement `fuzzed_dns_lookup_function` as a lambda (practicalswift)
- gamcoin/gamcoin#21445 cirrus: Use SSD cluster for speedup (MarcoFalke)
- gamcoin/gamcoin#21477 Add test for CNetAddr::ToString IPv6 address formatting (RFC 5952) (practicalswift)
- gamcoin/gamcoin#21487 fuzz: Use ConsumeWeakEnum in addrman for service flags (MarcoFalke)
- gamcoin/gamcoin#21488 Add ParseUInt16() unit test and fuzz coverage (jonatack)
- gamcoin/gamcoin#21491 test: remove duplicate assertions in util_tests (jonatack)
- gamcoin/gamcoin#21522 fuzz: Use PickValue where possible (MarcoFalke)
- gamcoin/gamcoin#21531 remove qt byteswap compattests (fanquake)
- gamcoin/gamcoin#21557 small cleanup in RPCNestedTests tests (fanquake)
- gamcoin/gamcoin#21586 Add missing suppression for signed-integer-overflow:txmempool.cpp (MarcoFalke)
- gamcoin/gamcoin#21592 Remove option to make TestChain100Setup non-deterministic (MarcoFalke)
- gamcoin/gamcoin#21597 Document `race:validation_chainstatemanager_tests` suppression (MarcoFalke)
- gamcoin/gamcoin#21599 Replace file level integer overflow suppression with function level suppression (practicalswift)
- gamcoin/gamcoin#21604 Document why no symbol names can be used for suppressions (MarcoFalke)
- gamcoin/gamcoin#21606 fuzz: Extend psbt fuzz target a bit (MarcoFalke)
- gamcoin/gamcoin#21617 fuzz: Fix uninitialized read in i2p test (MarcoFalke)
- gamcoin/gamcoin#21630 fuzz: split FuzzedSock interface and implementation (vasild)
- gamcoin/gamcoin#21634 Skip SQLite fsyncs while testing (achow101)
- gamcoin/gamcoin#21669 Remove spurious double lock tsan suppressions by bumping to clang-12 (MarcoFalke)
- gamcoin/gamcoin#21676 Use mocktime to avoid intermittent failure in `rpc_tests` (MarcoFalke)
- gamcoin/gamcoin#21677 fuzz: Avoid use of low file descriptor ids (which may be in use) in FuzzedSock (practicalswift)
- gamcoin/gamcoin#21678 Fix TestPotentialDeadLockDetected suppression (hebasto)
- gamcoin/gamcoin#21689 Remove intermittently failing and not very meaningful `BOOST_CHECK` in `cnetaddr_basic` (practicalswift)
- gamcoin/gamcoin#21691 Check that no versionbits are re-used (MarcoFalke)
- gamcoin/gamcoin#21707 Extend functional tests for addr relay (mzumsande)
- gamcoin/gamcoin#21712 Test default `include_mempool` value of gettxout (promag)
- gamcoin/gamcoin#21738 Use clang-12 for ASAN, Add missing suppression (MarcoFalke)
- gamcoin/gamcoin#21740 add new python linter to check file names and permissions (windsok)
- gamcoin/gamcoin#21749 Bump shellcheck version (hebasto)
- gamcoin/gamcoin#21754 Run `feature_cltv` with MiniWallet (MarcoFalke)
- gamcoin/gamcoin#21762 Speed up `mempool_spend_coinbase.py` (MarcoFalke)
- gamcoin/gamcoin#21773 fuzz: Ensure prevout is consensus-valid (MarcoFalke)
- gamcoin/gamcoin#21777 Fix `feature_notifications.py` intermittent issue (MarcoFalke)
- gamcoin/gamcoin#21785 Fix intermittent issue in `p2p_addr_relay.py` (MarcoFalke)
- gamcoin/gamcoin#21787 Fix off-by-ones in `rpc_fundrawtransaction` assertions (jonatack)
- gamcoin/gamcoin#21792 Fix intermittent issue in `p2p_segwit.py` (MarcoFalke)
- gamcoin/gamcoin#21795 fuzz: Terminate immediately if a fuzzing harness tries to perform a DNS lookup (belt and suspenders) (practicalswift)
- gamcoin/gamcoin#21798 fuzz: Create a block template in `tx_pool` targets (MarcoFalke)
- gamcoin/gamcoin#21804 Speed up `p2p_segwit.py` (jnewbery)
- gamcoin/gamcoin#21810 fuzz: Various RPC fuzzer follow-ups (practicalswift)
- gamcoin/gamcoin#21814 Fix `feature_config_args.py` intermittent issue (MarcoFalke)
- gamcoin/gamcoin#21821 Add missing test for empty P2WSH redeem (MarcoFalke)
- gamcoin/gamcoin#21822 Resolve bug in `interface_gamcoin_cli.py` (klementtan)
- gamcoin/gamcoin#21846 fuzz: Add `-fsanitize=integer` suppression needed for RPC fuzzer (`generateblock`) (practicalswift)
- gamcoin/gamcoin#21849 fuzz: Limit toxic test globals to their respective scope (MarcoFalke)
- gamcoin/gamcoin#21867 use MiniWallet for `p2p_blocksonly.py` (theStack)
- gamcoin/gamcoin#21873 minor fixes & improvements for files linter test (windsok)
- gamcoin/gamcoin#21874 fuzz: Add `WRITE_ALL_FUZZ_TARGETS_AND_ABORT` (MarcoFalke)
- gamcoin/gamcoin#21884 fuzz: Remove unused --enable-danger-fuzz-link-all option (MarcoFalke)
- gamcoin/gamcoin#21890 fuzz: Limit ParseISO8601DateTime fuzzing to 32-bit (MarcoFalke)
- gamcoin/gamcoin#21891 fuzz: Remove strprintf test cases that are known to fail (MarcoFalke)
- gamcoin/gamcoin#21892 fuzz: Avoid excessively large min fee rate in `tx_pool` (MarcoFalke)
- gamcoin/gamcoin#21895 Add TSA annotations to the WorkQueue class members (hebasto)
- gamcoin/gamcoin#21900 use MiniWallet for `feature_csv_activation.py` (theStack)
- gamcoin/gamcoin#21909 fuzz: Limit max insertions in timedata fuzz test (MarcoFalke)
- gamcoin/gamcoin#21922 fuzz: Avoid timeout in EncodeBase58 (MarcoFalke)
- gamcoin/gamcoin#21927 fuzz: Run const CScript member functions only once (MarcoFalke)
- gamcoin/gamcoin#21929 fuzz: Remove incorrect float round-trip serialization test (MarcoFalke)
- gamcoin/gamcoin#21936 fuzz: Terminate immediately if a fuzzing harness tries to create a TCP socket (belt and suspenders) (practicalswift)
- gamcoin/gamcoin#21941 fuzz: Call const member functions in addrman fuzz test only once (MarcoFalke)
- gamcoin/gamcoin#21945 add P2PK support to MiniWallet (theStack)
- gamcoin/gamcoin#21948 Fix off-by-one in mockscheduler test RPC (MarcoFalke)
- gamcoin/gamcoin#21953 fuzz: Add `utxo_snapshot` target (MarcoFalke)
- gamcoin/gamcoin#21970 fuzz: Add missing CheckTransaction before CheckTxInputs (MarcoFalke)
- gamcoin/gamcoin#21989 Use `COINBASE_MATURITY` in functional tests (kiminuo)
- gamcoin/gamcoin#22003 Add thread safety annotations (ajtowns)
- gamcoin/gamcoin#22004 fuzz: Speed up transaction fuzz target (MarcoFalke)
- gamcoin/gamcoin#22005 fuzz: Speed up banman fuzz target (MarcoFalke)
- gamcoin/gamcoin#22029 [fuzz] Improve transport deserialization fuzz test coverage (dhruv)
- gamcoin/gamcoin#22048 MiniWallet: introduce enum type for output mode (theStack)
- gamcoin/gamcoin#22057 use MiniWallet (P2PK mode) for `feature_dersig.py` (theStack)
- gamcoin/gamcoin#22065 Mark `CheckTxInputs` `[[nodiscard]]`. Avoid UUM in fuzzing harness `coins_view` (practicalswift)
- gamcoin/gamcoin#22069 fuzz: don't try and use fopencookie() when building for Android (fanquake)
- gamcoin/gamcoin#22082 update nanobench from release 4.0.0 to 4.3.4 (martinus)
- gamcoin/gamcoin#22086 remove BasicTestingSetup from unit tests that don't need it (fanquake)
- gamcoin/gamcoin#22089 MiniWallet: fix fee calculation for P2PK and check tx vsize (theStack)
- gamcoin/gamcoin#21107, gamcoin/gamcoin#22092 Convert documentation into type annotations (fanquake)
- gamcoin/gamcoin#22095 Additional BIP32 test vector for hardened derivation with leading zeros (kristapsk)
- gamcoin/gamcoin#22103 Fix IPv6 check on BSD systems (n-thumann)
- gamcoin/gamcoin#22118 check anchors.dat when node starts for the first time (brunoerg)
- gamcoin/gamcoin#22120 `p2p_invalid_block`: Check that a block rejected due to too-new tim… (willcl-ark)
- gamcoin/gamcoin#22153 Fix `p2p_leak.py` intermittent failure (mzumsande)
- gamcoin/gamcoin#22169 p2p, rpc, fuzz: various tiny follow-ups (jonatack)
- gamcoin/gamcoin#22176 Correct outstanding -Werror=sign-compare errors (Empact)
- gamcoin/gamcoin#22180 fuzz: Increase branch coverage of the float fuzz target (MarcoFalke)
- gamcoin/gamcoin#22187 Add `sync_blocks` in `wallet_orphanedreward.py` (domob1812)
- gamcoin/gamcoin#22201 Fix TestShell to allow running in Jupyter Notebook (josibake)
- gamcoin/gamcoin#22202 Add temporary coinstats suppressions (MarcoFalke)
- gamcoin/gamcoin#22203 Use ConnmanTestMsg from test lib in `denialofservice_tests` (MarcoFalke)
- gamcoin/gamcoin#22210 Use MiniWallet in `test_no_inherited_signaling` RBF test (MarcoFalke)
- gamcoin/gamcoin#22224 Update msvc and appveyor builds to use Qt5.12.11 binaries (sipsorcery)
- gamcoin/gamcoin#22249 Kill process group to avoid dangling processes when using `--failfast` (S3RK)
- gamcoin/gamcoin#22267 fuzz: Speed up crypto fuzz target (MarcoFalke)
- gamcoin/gamcoin#22270 Add gamcoin-util tests (+refactors) (MarcoFalke)
- gamcoin/gamcoin#22271 fuzz: Assert roundtrip equality for `CPubKey` (theStack)
- gamcoin/gamcoin#22279 fuzz: add missing ECCVerifyHandle to `base_encode_decode` (apoelstra)
- gamcoin/gamcoin#22292 bench, doc: benchmarking updates and fixups (jonatack)
- gamcoin/gamcoin#22306 Improvements to `p2p_addr_relay.py` (amitiuttarwar)
- gamcoin/gamcoin#22310 Add functional test for replacement relay fee check (ariard)
- gamcoin/gamcoin#22311 Add missing syncwithvalidationinterfacequeue in `p2p_blockfilters` (MarcoFalke)
- gamcoin/gamcoin#22313 Add missing `sync_all` to `feature_coinstatsindex` (MarcoFalke)
- gamcoin/gamcoin#22322 fuzz: Check banman roundtrip (MarcoFalke)
- gamcoin/gamcoin#22363 Use `script_util` helpers for creating P2{PKH,SH,WPKH,WSH} scripts (theStack)
- gamcoin/gamcoin#22399 fuzz: Rework CTxDestination fuzzing (MarcoFalke)
- gamcoin/gamcoin#22408 add tests for `bad-txns-prevout-null` reject reason (theStack)
- gamcoin/gamcoin#22445 fuzz: Move implementations of non-template fuzz helpers from util.h to util.cpp (sriramdvt)
- gamcoin/gamcoin#22446 Fix `wallet_listdescriptors.py` if bdb is not compiled (hebasto)
- gamcoin/gamcoin#22447 Whitelist `rpc_rawtransaction` peers to speed up tests (jonatack)
- gamcoin/gamcoin#22742 Use proper target in `do_fund_send` (S3RK)

### Miscellaneous
- gamcoin/gamcoin#19337 sync: Detect double lock from the same thread (vasild)
- gamcoin/gamcoin#19809 log: Prefix log messages with function name and source code location if -logsourcelocations is set (practicalswift)
- gamcoin/gamcoin#19866 eBPF Linux tracepoints (jb55)
- gamcoin/gamcoin#20024 init: Fix incorrect warning "Reducing -maxconnections from N to N-1, because of system limitations" (practicalswift)
- gamcoin/gamcoin#20145 contrib: Add getcoins.py script to get coins from (signet) faucet (kallewoof)
- gamcoin/gamcoin#20255 util: Add assume() identity function (MarcoFalke)
- gamcoin/gamcoin#20288 script, doc: Contrib/seeds updates (jonatack)
- gamcoin/gamcoin#20358 src/randomenv.cpp: Fix build on uclibc (ffontaine)
- gamcoin/gamcoin#20406 util: Avoid invalid integer negation in formatmoney and valuefromamount (practicalswift)
- gamcoin/gamcoin#20434 contrib: Parse elf directly for symbol and security checks (laanwj)
- gamcoin/gamcoin#20451 lint: Run mypy over contrib/devtools (fanquake)
- gamcoin/gamcoin#20476 contrib: Add test for elf symbol-check (laanwj)
- gamcoin/gamcoin#20530 lint: Update cppcheck linter to c++17 and improve explicit usage (fjahr)
- gamcoin/gamcoin#20589 log: Clarify that failure to read/write `fee_estimates.dat` is non-fatal (MarcoFalke)
- gamcoin/gamcoin#20602 util: Allow use of c++14 chrono literals (MarcoFalke)
- gamcoin/gamcoin#20605 init: Signal-safe instant shutdown (laanwj)
- gamcoin/gamcoin#20608 contrib: Add symbol check test for PE binaries (fanquake)
- gamcoin/gamcoin#20689 contrib: Replace binary verification script verify.sh with python rewrite (theStack)
- gamcoin/gamcoin#20715 util: Add argsmanager::getcommand() and use it in gamcoin-wallet (MarcoFalke)
- gamcoin/gamcoin#20735 script: Remove outdated extract-osx-sdk.sh (hebasto)
- gamcoin/gamcoin#20817 lint: Update list of spelling linter false positives, bump to codespell 2.0.0 (theStack)
- gamcoin/gamcoin#20884 script: Improve robustness of gamcoind.service on startup (hebasto)
- gamcoin/gamcoin#20906 contrib: Embed c++11 patch in `install_db4.sh` (gruve-p)
- gamcoin/gamcoin#21004 contrib: Fix docker args conditional in gitian-build (setpill)
- gamcoin/gamcoin#21007 gamcoind: Add -daemonwait option to wait for initialization (laanwj)
- gamcoin/gamcoin#21041 log: Move "Pre-allocating up to position 0x[…] in […].dat" log message to debug category (practicalswift)
- gamcoin/gamcoin#21059 Drop boost/preprocessor dependencies (hebasto)
- gamcoin/gamcoin#21087 guix: Passthrough `BASE_CACHE` into container (dongcarl)
- gamcoin/gamcoin#21088 guix: Jump forwards in time-machine and adapt (dongcarl)
- gamcoin/gamcoin#21089 guix: Add support for powerpc64{,le} (dongcarl)
- gamcoin/gamcoin#21110 util: Remove boost `posix_time` usage from `gettime*` (fanquake)
- gamcoin/gamcoin#21111 Improve OpenRC initscript (parazyd)
- gamcoin/gamcoin#21123 code style: Add EditorConfig file (kiminuo)
- gamcoin/gamcoin#21173 util: Faster hexstr => 13% faster blocktojson (martinus)
- gamcoin/gamcoin#21221 tools: Allow argument/parameter bin packing in clang-format (jnewbery)
- gamcoin/gamcoin#21244 Move GetDataDir to ArgsManager (kiminuo)
- gamcoin/gamcoin#21255 contrib: Run test-symbol-check for risc-v (fanquake)
- gamcoin/gamcoin#21271 guix: Explicitly set umask in build container (dongcarl)
- gamcoin/gamcoin#21300 script: Add explanatory comment to tc.sh (dscotese)
- gamcoin/gamcoin#21317 util: Make assume() usable as unary expression (MarcoFalke)
- gamcoin/gamcoin#21336 Make .gitignore ignore src/test/fuzz/fuzz.exe (hebasto)
- gamcoin/gamcoin#21337 guix: Update darwin native packages dependencies (hebasto)
- gamcoin/gamcoin#21405 compat: remove memcpy -> memmove backwards compatibility alias (fanquake)
- gamcoin/gamcoin#21418 contrib: Make systemd invoke dependencies only when ready (laanwj)
- gamcoin/gamcoin#21447 Always add -daemonwait to known command line arguments (hebasto)
- gamcoin/gamcoin#21471 bugfix: Fix `bech32_encode` calls in `gen_key_io_test_vectors.py` (sipa)
- gamcoin/gamcoin#21615 script: Add trusted key for hebasto (hebasto)
- gamcoin/gamcoin#21664 contrib: Use lief for macos and windows symbol & security checks (fanquake)
- gamcoin/gamcoin#21695 contrib: Remove no longer used contrib/gamcoin-qt.pro (hebasto)
- gamcoin/gamcoin#21711 guix: Add full installation and usage documentation (dongcarl)
- gamcoin/gamcoin#21799 guix: Use `gcc-8` across the board (dongcarl)
- gamcoin/gamcoin#21802 Avoid UB in util/asmap (advance a dereferenceable iterator outside its valid range) (MarcoFalke)
- gamcoin/gamcoin#21823 script: Update reviewers (jonatack)
- gamcoin/gamcoin#21850 Remove `GetDataDir(net_specific)` function (kiminuo)
- gamcoin/gamcoin#21871 scripts: Add checks for minimum required os versions (fanquake)
- gamcoin/gamcoin#21966 Remove double serialization; use software encoder for fee estimation (sipa)
- gamcoin/gamcoin#22060 contrib: Add torv3 seed nodes for testnet, drop v2 ones (laanwj)
- gamcoin/gamcoin#22244 devtools: Correctly extract symbol versions in symbol-check (laanwj)
- gamcoin/gamcoin#22533 guix/build: Remove vestigial SKIPATTEST.TAG (dongcarl)
- gamcoin/gamcoin#22643 guix-verify: Non-zero exit code when anything fails (dongcarl)
- gamcoin/gamcoin#22654 guix: Don't include directory name in SHA256SUMS (achow101)

### Documentation
- gamcoin/gamcoin#15451 clarify getdata limit after #14897 (HashUnlimited)
- gamcoin/gamcoin#15545 Explain why CheckBlock() is called before AcceptBlock (Sjors)
- gamcoin/gamcoin#17350 Add developer documentation to isminetype (HAOYUatHZ)
- gamcoin/gamcoin#17934 Use `CONFIG_SITE` variable instead of --prefix option (hebasto)
- gamcoin/gamcoin#18030 Coin::IsSpent() can also mean never existed (Sjors)
- gamcoin/gamcoin#18096 IsFinalTx comment about nSequence & `OP_CLTV` (nothingmuch)
- gamcoin/gamcoin#18568 Clarify developer notes about constant naming (ryanofsky)
- gamcoin/gamcoin#19961 doc: tor.md updates (jonatack)
- gamcoin/gamcoin#19968 Clarify CRollingBloomFilter size estimate (robot-dreams)
- gamcoin/gamcoin#20200 Rename CODEOWNERS to REVIEWERS (adamjonas)
- gamcoin/gamcoin#20329 docs/descriptors.md: Remove hardened marker in the path after xpub (dgpv)
- gamcoin/gamcoin#20380 Add instructions on how to fuzz the P2P layer using Honggfuzz NetDriver (practicalswift)
- gamcoin/gamcoin#20414 Remove generated manual pages from master branch (laanwj)
- gamcoin/gamcoin#20473 Document current boost dependency as 1.71.0 (laanwj)
- gamcoin/gamcoin#20512 Add bash as an OpenBSD dependency (emilengler)
- gamcoin/gamcoin#20568 Use FeeModes doc helper in estimatesmartfee (MarcoFalke)
- gamcoin/gamcoin#20577 libconsensus: add missing error code description, fix NGamcoin link (theStack)
- gamcoin/gamcoin#20587 Tidy up Tor doc (more stringent) (wodry)
- gamcoin/gamcoin#20592 Update wtxidrelay documentation per BIP339 (jonatack)
- gamcoin/gamcoin#20601 Update for FreeBSD 12.2, add GUI Build Instructions (jarolrod)
- gamcoin/gamcoin#20635 fix misleading comment about call to non-existing function (pox)
- gamcoin/gamcoin#20646 Refer to BIPs 339/155 in feature negotiation (jonatack)
- gamcoin/gamcoin#20653 Move addr relay comment in net to correct place (MarcoFalke)
- gamcoin/gamcoin#20677 Remove shouty enums in `net_processing` comments (sdaftuar)
- gamcoin/gamcoin#20741 Update 'Secure string handling' (prayank23)
- gamcoin/gamcoin#20757 tor.md and -onlynet help updates (jonatack)
- gamcoin/gamcoin#20829 Add -netinfo help (jonatack)
- gamcoin/gamcoin#20830 Update developer notes with signet (jonatack)
- gamcoin/gamcoin#20890 Add explicit macdeployqtplus dependencies install step (hebasto)
- gamcoin/gamcoin#20913 Add manual page generation for gamcoin-util (laanwj)
- gamcoin/gamcoin#20985 Add xorriso to macOS depends packages (fanquake)
- gamcoin/gamcoin#20986 Update developer notes to discourage very long lines (jnewbery)
- gamcoin/gamcoin#20987 Add instructions for generating RPC docs (ben-kaufman)
- gamcoin/gamcoin#21026 Document use of make-tag script to make tags (laanwj)
- gamcoin/gamcoin#21028 doc/bips: Add BIPs 43, 44, 49, and 84 (luke-jr)
- gamcoin/gamcoin#21049 Add release notes for listdescriptors RPC (S3RK)
- gamcoin/gamcoin#21060 More precise -debug and -debugexclude doc (wodry)
- gamcoin/gamcoin#21077 Clarify -timeout and -peertimeout config options (glozow)
- gamcoin/gamcoin#21105 Correctly identify script type (niftynei)
- gamcoin/gamcoin#21163 Guix is shipped in Debian and Ubuntu (MarcoFalke)
- gamcoin/gamcoin#21210 Rework internal and external links (MarcoFalke)
- gamcoin/gamcoin#21246 Correction for VerifyTaprootCommitment comments (roconnor-blockstream)
- gamcoin/gamcoin#21263 Clarify that squashing should happen before review (MarcoFalke)
- gamcoin/gamcoin#21323 guix, doc: Update default HOSTS value (hebasto)
- gamcoin/gamcoin#21324 Update build instructions for Fedora (hebasto)
- gamcoin/gamcoin#21343 Revamp macOS build doc (jarolrod)
- gamcoin/gamcoin#21346 install qt5 when building on macOS (fanquake)
- gamcoin/gamcoin#21384 doc: add signet to gamcoin.conf documentation (jonatack)
- gamcoin/gamcoin#21394 Improve comment about protected peers (amitiuttarwar)
- gamcoin/gamcoin#21398 Update fuzzing docs for afl-clang-lto (MarcoFalke)
- gamcoin/gamcoin#21444 net, doc: Doxygen updates and fixes in netbase.{h,cpp} (jonatack)
- gamcoin/gamcoin#21481 Tell howto install clang-format on Debian/Ubuntu (wodry)
- gamcoin/gamcoin#21567 Fix various misleading comments (glozow)
- gamcoin/gamcoin#21661 Fix name of script guix-build (Emzy)
- gamcoin/gamcoin#21672 Remove boostrap info from `GUIX_COMMON_FLAGS` doc (fanquake)
- gamcoin/gamcoin#21688 Note on SDK for macOS depends cross-compile (jarolrod)
- gamcoin/gamcoin#21709 Update reduce-memory.md and gamcoin.conf -maxconnections info (jonatack)
- gamcoin/gamcoin#21710 update helps for addnode rpc and -addnode/-maxconnections config options (jonatack)
- gamcoin/gamcoin#21752 Clarify that feerates are per virtual size (MarcoFalke)
- gamcoin/gamcoin#21811 Remove Visual Studio 2017 reference from readme (sipsorcery)
- gamcoin/gamcoin#21818 Fixup -coinstatsindex help, update gamcoin.conf and files.md (jonatack)
- gamcoin/gamcoin#21856 add OSS-Fuzz section to fuzzing.md doc (adamjonas)
- gamcoin/gamcoin#21912 Remove mention of priority estimation (MarcoFalke)
- gamcoin/gamcoin#21925 Update bips.md for 0.21.1 (MarcoFalke)
- gamcoin/gamcoin#21942 improve make with parallel jobs description (klementtan)
- gamcoin/gamcoin#21947 Fix OSS-Fuzz links (MarcoFalke)
- gamcoin/gamcoin#21988 note that brew installed qt is not supported (jarolrod)
- gamcoin/gamcoin#22056 describe in fuzzing.md how to reproduce a CI crash (jonatack)
- gamcoin/gamcoin#22080 add maxuploadtarget to gamcoin.conf example (jarolrod)
- gamcoin/gamcoin#22088 Improve note on choosing posix mingw32 (jarolrod)
- gamcoin/gamcoin#22109 Fix external links (IRC, …) (MarcoFalke)
- gamcoin/gamcoin#22121 Various validation doc fixups (MarcoFalke)
- gamcoin/gamcoin#22172 Update tor.md, release notes with removal of tor v2 support (jonatack)
- gamcoin/gamcoin#22204 Remove obsolete `okSafeMode` RPC guideline from developer notes (theStack)
- gamcoin/gamcoin#22208 Update `REVIEWERS` (practicalswift)
- gamcoin/gamcoin#22250 add basic I2P documentation (vasild)
- gamcoin/gamcoin#22296 Final merge of release notes snippets, mv to wiki (MarcoFalke)
- gamcoin/gamcoin#22335 recommend `--disable-external-signer` in OpenBSD build guide (theStack)
- gamcoin/gamcoin#22339 Document minimum required libc++ version (hebasto)
- gamcoin/gamcoin#22349 Repository IRC updates (jonatack)
- gamcoin/gamcoin#22360 Remove unused section from release process (MarcoFalke)
- gamcoin/gamcoin#22369 Add steps for Transifex to release process (jonatack)
- gamcoin/gamcoin#22393 Added info to gamcoin.conf doc (bliotti)
- gamcoin/gamcoin#22402 Install Rosetta on M1-macOS for qt in depends (hebasto)
- gamcoin/gamcoin#22432 Fix incorrect `testmempoolaccept` doc (glozow)
- gamcoin/gamcoin#22648 doc, test: improve i2p/tor docs and i2p reachable unit tests (jonatack)

Credits
=======

Thanks to everyone who directly contributed to this release:

- Aaron Clauson
- Adam Jonas
- amadeuszpawlik
- Amiti Uttarwar
- Andrew Chow
- Andrew Poelstra
- Anthony Towns
- Antoine Poinsot
- Antoine Riard
- apawlik
- apitko
- Ben Carman
- Ben Woosley
- benk10
- Bezdrighin
- Block Mechanic
- Brian Liotti
- Bruno Garcia
- Carl Dong
- Christian Decker
- coinforensics
- Cory Fields
- Dan Benjamin
- Daniel Kraft
- Darius Parvin
- Dhruv Mehta
- Dmitry Goncharov
- Dmitry Petukhov
- dplusplus1024
- dscotese
- Duncan Dean
- Elle Mouton
- Elliott Jin
- Emil Engler
- Ethan Heilman
- eugene
- Evan Klitzke
- Fabian Jahr
- Fabrice Fontaine
- fanquake
- fdov
- flack
- Fotis Koutoupas
- Fu Yong Quah
- fyquah
- glozow
- Gregory Sanders
- Guido Vranken
- Gunar C. Gessner
- h
- HAOYUatHZ
- Hennadii Stepanov
- Igor Cota
- Ikko Ashimine
- Ivan Metlushko
- jackielove4u
- James O'Beirne
- Jarol Rodriguez
- Joel Klabo
- John Newbery
- Jon Atack
- Jonas Schnelli
- João Barbosa
- Josiah Baker
- Karl-Johan Alm
- Kiminuo
- Klement Tan
- Kristaps Kaupe
- Larry Ruane
- lisa neigut
- Lucas Ontivero
- Luke Dashjr
- Maayan Keshet
- MarcoFalke
- Martin Ankerl
- Martin Zumsande
- Michael Dietz
- Michael Polzer
- Michael Tidwell
- Niklas Gögge
- nthumann
- Oliver Gugger
- parazyd
- Patrick Strateman
- Pavol Rusnak
- Peter Bushnell
- Pierre K
- Pieter Wuille
- PiRK
- pox
- practicalswift
- Prayank
- R E Broadley
- Rafael Sadowski
- randymcmillan
- Raul Siles
- Riccardo Spagni
- Russell O'Connor
- Russell Yanofsky
- S3RK
- saibato
- Samuel Dobson
- sanket1729
- Sawyer Billings
- Sebastian Falbesoner
- setpill
- sgulls
- sinetek
- Sjors Provoost
- Sriram
- Stephan Oeste
- Suhas Daftuar
- Sylvain Goumy
- t-bast
- Troy Giorshev
- Tushar Singla
- Tyler Chambers
- Uplab
- Vasil Dimov
- W. J. van der Laan
- willcl-ark
- William Bright
- William Casarin
- windsok
- wodry
- Yerzhan Mazhkenov
- Yuval Kogman
- Zero

As well as to everyone that helped with translations on
[Transifex](https://www.transifex.com/gamcoin/gamcoin/).
