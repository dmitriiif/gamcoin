// Copyright (c) 2011-2020 The Gamcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GAMCOIN_QT_GAMCOINADDRESSVALIDATOR_H
#define GAMCOIN_QT_GAMCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class GamcoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit GamcoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Gamcoin address widget validator, checks for a valid gamcoin address.
 */
class GamcoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit GamcoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // GAMCOIN_QT_GAMCOINADDRESSVALIDATOR_H
