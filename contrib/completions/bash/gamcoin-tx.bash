# bash programmable completion for gamcoin-tx(1)
# Copyright (c) 2016-2022 The Gamcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

_gamcoin_tx() {
    local cur prev words=() cword
    local gamcoin_tx

    # save and use original argument to invoke gamcoin-tx for -help
    # it might not be in $PATH
    gamcoin_tx="$1"

    COMPREPLY=()
    _get_comp_words_by_ref -n =: cur prev words cword

    case "$cur" in
        load=*:*)
            cur="${cur#load=*:}"
            _filedir
            return 0
            ;;
        *=*)	# prevent attempts to complete other arguments
            return 0
            ;;
    esac

    if [[ "$cword" == 1 || ( "$prev" != "-create" && "$prev" == -* ) ]]; then
        # only options (or an uncompletable hex-string) allowed
        # parse gamcoin-tx -help for options
        local helpopts
        helpopts=$($gamcoin_tx -help | sed -e '/^  -/ p' -e d )
        COMPREPLY=( $( compgen -W "$helpopts" -- "$cur" ) )
    else
        # only commands are allowed
        # parse -help for commands
        local helpcmds
        helpcmds=$($gamcoin_tx -help | sed -e '1,/Commands:/d' -e 's/=.*/=/' -e '/^  [a-z]/ p' -e d )
        COMPREPLY=( $( compgen -W "$helpcmds" -- "$cur" ) )
    fi

    # Prevent space if an argument is desired
    if [[ $COMPREPLY == *= ]]; then
        compopt -o nospace
    fi

    return 0
} &&
complete -F _gamcoin_tx gamcoin-tx

# Local variables:
# mode: shell-script
# sh-basic-offset: 4
# sh-indent-comment: t
# indent-tabs-mode: nil
# End:
# ex: ts=4 sw=4 et filetype=sh
