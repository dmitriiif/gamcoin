# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libgamcoin_cli*         | RPC client functionality used by *gamcoin-cli* executable |
| *libgamcoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *libgamcoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libgamcoin_consensus*   | Stable, backwards-compatible consensus functionality used by *libgamcoin_node* and *libgamcoin_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libgamcoinconsensus*    | Shared library build of static *libgamcoin_consensus* library |
| *libgamcoin_kernel*      | Consensus engine and support library used for validation by *libgamcoin_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libgamcoinqt*           | GUI functionality used by *gamcoin-qt* and *gamcoin-gui* executables |
| *libgamcoin_ipc*         | IPC functionality used by *gamcoin-node*, *gamcoin-wallet*, *gamcoin-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libgamcoin_node*        | P2P and RPC server functionality used by *gamcoind* and *gamcoin-qt* executables. |
| *libgamcoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *libgamcoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libgamcoin_wallet*      | Wallet functionality used by *gamcoind* and *gamcoin-wallet* executables. |
| *libgamcoin_wallet_tool* | Lower-level wallet functionality used by *gamcoin-wallet* executable. |
| *libgamcoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *gamcoind* and *gamcoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libgamcoin_consensus* and *libgamcoin_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libgamcoin_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libgamcoin_node* code lives in `src/node/` in the `node::` namespace
  - *libgamcoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libgamcoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libgamcoin_util* code lives in `src/util/` in the `util::` namespace
  - *libgamcoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

gamcoin-cli[gamcoin-cli]-->libgamcoin_cli;

gamcoind[gamcoind]-->libgamcoin_node;
gamcoind[gamcoind]-->libgamcoin_wallet;

gamcoin-qt[gamcoin-qt]-->libgamcoin_node;
gamcoin-qt[gamcoin-qt]-->libgamcoinqt;
gamcoin-qt[gamcoin-qt]-->libgamcoin_wallet;

gamcoin-wallet[gamcoin-wallet]-->libgamcoin_wallet;
gamcoin-wallet[gamcoin-wallet]-->libgamcoin_wallet_tool;

libgamcoin_cli-->libgamcoin_util;
libgamcoin_cli-->libgamcoin_common;

libgamcoin_common-->libgamcoin_consensus;
libgamcoin_common-->libgamcoin_util;

libgamcoin_kernel-->libgamcoin_consensus;
libgamcoin_kernel-->libgamcoin_util;

libgamcoin_node-->libgamcoin_consensus;
libgamcoin_node-->libgamcoin_kernel;
libgamcoin_node-->libgamcoin_common;
libgamcoin_node-->libgamcoin_util;

libgamcoinqt-->libgamcoin_common;
libgamcoinqt-->libgamcoin_util;

libgamcoin_wallet-->libgamcoin_common;
libgamcoin_wallet-->libgamcoin_util;

libgamcoin_wallet_tool-->libgamcoin_wallet;
libgamcoin_wallet_tool-->libgamcoin_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class gamcoin-qt,gamcoind,gamcoin-cli,gamcoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libgamcoin_wallet* and *libgamcoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libgamcoin_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libgamcoin_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libgamcoin_common* should serve a similar function as *libgamcoin_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libgamcoin_util* and *libgamcoin_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for gamcoin-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libgamcoin_kernel* is not supposed to depend on *libgamcoin_common*, only *libgamcoin_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libgamcoin_kernel* should only depend on *libgamcoin_util* and *libgamcoin_consensus*.

- The only thing that should depend on *libgamcoin_kernel* internally should be *libgamcoin_node*. GUI and wallet libraries *libgamcoinqt* and *libgamcoin_wallet* in particular should not depend on *libgamcoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libgamcoin_consensus*, *libgamcoin_common*, and *libgamcoin_util*, instead of *libgamcoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libgamcoinqt*, *libgamcoin_node*, *libgamcoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libgamcoin_node* to *libgamcoin_kernel* as part of [The libgamcoinkernel Project #24303](https://github.com/gamcoin/gamcoin/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/gamcoin/gamcoin/issues/15732)
