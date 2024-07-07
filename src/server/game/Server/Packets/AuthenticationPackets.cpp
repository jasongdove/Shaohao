/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "AuthenticationPackets.h"
#include "BigNumber.h"
#include "CharacterTemplateDataStore.h"
#include "Ed25519.h"
#include "HMAC.h"
#include "ObjectMgr.h"
#include "RSA.h"

#define PLAYABLE_CLASSES_COUNT 11
#define PLAYABLE_RACES_COUNT 15

uint8 raceExpansion[PLAYABLE_RACES_COUNT][2] =
{
        { RACE_TAUREN,            EXPANSION_CLASSIC             },
        { RACE_UNDEAD_PLAYER,     EXPANSION_CLASSIC             },
        { RACE_ORC,               EXPANSION_CLASSIC             },
        { RACE_GNOME,             EXPANSION_CLASSIC             },
        { RACE_GOBLIN,            EXPANSION_THE_BURNING_CRUSADE },
        { RACE_HUMAN,             EXPANSION_CLASSIC             },
        { RACE_TROLL,             EXPANSION_CLASSIC             },
        { RACE_PANDAREN_NEUTRAL,  EXPANSION_THE_BURNING_CRUSADE },
        { RACE_DRAENEI,           EXPANSION_THE_BURNING_CRUSADE },
        { RACE_WORGEN,            EXPANSION_THE_BURNING_CRUSADE },
        { RACE_BLOODELF,          EXPANSION_THE_BURNING_CRUSADE },
        { RACE_NIGHTELF,          EXPANSION_CLASSIC             },
        { RACE_DWARF,             EXPANSION_CLASSIC             },
        { RACE_PANDAREN_ALLIANCE, EXPANSION_THE_BURNING_CRUSADE },
        { RACE_PANDAREN_HORDE,    EXPANSION_THE_BURNING_CRUSADE },
};

uint8 classExpansion[PLAYABLE_CLASSES_COUNT][2] =
{
        { CLASS_MONK,         EXPANSION_MISTS_OF_PANDARIA      },
        { CLASS_WARRIOR,      EXPANSION_CLASSIC                },
        { CLASS_PALADIN,      EXPANSION_CLASSIC                },
        { CLASS_HUNTER,       EXPANSION_CLASSIC                },
        { CLASS_ROGUE,        EXPANSION_CLASSIC                },
        { CLASS_PRIEST,       EXPANSION_CLASSIC                },
        { CLASS_SHAMAN,       EXPANSION_CLASSIC                },
        { CLASS_MAGE,         EXPANSION_CLASSIC                },
        { CLASS_WARLOCK,      EXPANSION_CLASSIC                },
        { CLASS_DRUID,        EXPANSION_CLASSIC                },
        { CLASS_DEATH_KNIGHT, EXPANSION_WRATH_OF_THE_LICH_KING },
};

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Auth::VirtualRealmNameInfo const& virtualRealmInfo)
{
    data.WriteBit(virtualRealmInfo.IsLocal);
    data.WriteBit(virtualRealmInfo.IsInternalRealm);
    data.WriteBits(virtualRealmInfo.RealmNameActual.length(), 8);
    data.WriteBits(virtualRealmInfo.RealmNameNormalized.length(), 8);
    data.FlushBits();

    data.WriteString(virtualRealmInfo.RealmNameActual);
    data.WriteString(virtualRealmInfo.RealmNameNormalized);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Auth::VirtualRealmInfo const& virtualRealmInfo)
{
    data << uint32(virtualRealmInfo.RealmAddress);
    data << virtualRealmInfo.RealmNameInfo;

    return data;
}

bool WorldPackets::Auth::EarlyProcessClientPacket::ReadNoThrow()
{
    try
    {
        Read();
        return true;
    }
    catch (ByteBufferException const& /*ex*/)
    {
    }

    return false;
}

void WorldPackets::Auth::Ping::Read()
{
    _worldPacket >> Serial;
    _worldPacket >> Latency;
}

const WorldPacket* WorldPackets::Auth::Pong::Write()
{
    _worldPacket << uint32(Serial);
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Auth::AuthChallenge::Write()
{
    _worldPacket << uint16(0);
    _worldPacket.append(Challenge.data(), Challenge.size());
    _worldPacket << uint8(1);
    _worldPacket.append(AuthSeed);
    return &_worldPacket;
}

void WorldPackets::Auth::AuthSession::Read()
{
    uint32 addonDataSize;

    _worldPacket.read_skip<uint32>();   // 0-3
    _worldPacket.read_skip<uint32>();   // 4-7
    _worldPacket >> Digest[18];         // 8
    _worldPacket >> Digest[14];         // 9
    _worldPacket >> Digest[3];          // 10
    _worldPacket >> Digest[4];          // 11
    _worldPacket >> Digest[0];          // 12
    _worldPacket.read_skip<uint32>();   // 13-16
    _worldPacket >> Digest[11];         // 17
    _worldPacket >> ClientSeed;         // 18-21
    _worldPacket >> Digest[19];
    _worldPacket.read_skip<uint8>();
    _worldPacket.read_skip<uint8>();
    _worldPacket >> Digest[2];
    _worldPacket >> Digest[9];
    _worldPacket >> Digest[12];
    _worldPacket.read_skip<uint64>();
    _worldPacket.read_skip<uint32>();
    _worldPacket >> Digest[16];
    _worldPacket >> Digest[5];
    _worldPacket >> Digest[6];
    _worldPacket >> Digest[8];
    _worldPacket >> ClientBuild;
    _worldPacket >> Digest[17];
    _worldPacket >> Digest[7];
    _worldPacket >> Digest[13];
    _worldPacket >> Digest[15];
    _worldPacket >> Digest[1];
    _worldPacket >> Digest[10];
    _worldPacket >> addonDataSize;

    if (addonDataSize)
    {
        AddonInfo.resize(addonDataSize);
        _worldPacket.read(AddonInfo.contents(), addonDataSize);
    }

    _worldPacket.ReadBit();
    uint32 accountNameLength = _worldPacket.ReadBits(11);

    Account = _worldPacket.ReadString(accountNameLength);
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Auth::AuthWaitInfo const& waitInfo)
{
    data << uint32(waitInfo.WaitCount);
    data << uint32(waitInfo.WaitTime);
    data << uint32(waitInfo.AllowedFactionGroupForCharacterCreate);
    data.WriteBit(waitInfo.HasFCM);
    data.WriteBit(waitInfo.CanCreateOnlyIfExisting);
    data.FlushBits();

    return data;
}

WorldPacket const* WorldPackets::Auth::AuthResponse::Write()
{
    std::map<uint32, std::string> realmNamesToSend;

    _worldPacket.WriteBit(SuccessInfo.has_value());

    if (SuccessInfo)
    {
        _worldPacket.WriteBits(SuccessInfo->VirtualRealms.size(), 21);

        for (VirtualRealmInfo const &virtualRealm: SuccessInfo->VirtualRealms)
        {
            _worldPacket.WriteBits(virtualRealm.RealmNameInfo.RealmNameActual.length(), 8);
            _worldPacket.WriteBits(virtualRealm.RealmNameInfo.RealmNameNormalized.length(), 8);
            _worldPacket.WriteBit(virtualRealm.RealmNameInfo.IsLocal);
        }

        _worldPacket.WriteBits(PLAYABLE_CLASSES_COUNT, 23); //
        _worldPacket.WriteBits(0, 21);
        _worldPacket.WriteBit(0);
        _worldPacket.WriteBit(0);
        _worldPacket.WriteBit(0);
        _worldPacket.WriteBit(0);
        _worldPacket.WriteBits(PLAYABLE_RACES_COUNT, 23);
        _worldPacket.WriteBit(0);
    }

    _worldPacket.WriteBit(WaitInfo.has_value());

    if (WaitInfo.has_value())
        _worldPacket.WriteBit(1);

    _worldPacket.FlushBits();

    if (WaitInfo.has_value())
        _worldPacket << WaitInfo->WaitCount;

    if (SuccessInfo)
    {
        for (VirtualRealmInfo const &virtualRealm: SuccessInfo->VirtualRealms)
        {
            _worldPacket << uint32(virtualRealm.RealmAddress);
            _worldPacket.WriteString(virtualRealm.RealmNameInfo.RealmNameActual);
            _worldPacket.WriteString(virtualRealm.RealmNameInfo.RealmNameNormalized);
        }

        for (auto &r: raceExpansion)
        {
            _worldPacket << uint8(r[1]);
            _worldPacket << uint8(r[0]);
        }

        for (auto &c: classExpansion)
        {
            _worldPacket << uint8(c[1]);
            _worldPacket << uint8(c[0]);
        }

        _worldPacket << uint32(0);
        _worldPacket << uint8(SuccessInfo->ActiveExpansionLevel);
        _worldPacket << uint32(0);
        _worldPacket << uint32(0); // unk time in ms
        _worldPacket << uint8(SuccessInfo->AccountExpansionLevel);
        _worldPacket << uint32(SuccessInfo->CurrencyID);
        _worldPacket << uint32(0);
        _worldPacket << uint32(0);
    }

    _worldPacket << uint8(Result);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Auth::WaitQueueUpdate::Write()
{
    _worldPacket << WaitInfo;

    return &_worldPacket;
}

namespace
{
std::string const RSAPrivateKey = R"(-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA7rPc1NPDtFRRzmZbyzK48PeSU8YZ8gyFL4omqXpFn2DE683q
f41Z2FeyYHsJTJtouMft7x6ADeZrN1tTkOsYEw1/Q2SD2pjmrMIwooKlxsvH+4af
n6kCagNJxTj7wMhVzMDOJZG+hc/R0TfOzIPS6jCAB3uAn51EVCIpvoba20jFqfkT
NpUjdvEO3IQNlAISqJfzOxTuqm+YBSdOH6Ngpana2BffM8viE1SLGLDKubuIZAbf
dabXYQC7sFoOetR3CE0V4hCDsASqnot3qQaJXQhdD7gua8HLZM9uXNtPWGUIUfsN
SBpvtj0fC93+Gx3wv7Ana/WOvMdAAf+nC4DWXwIDAQABAoIBACKa5q/gB2Y0Nyvi
APrDXrZoXclRVd+WWxSaRaKaPE+vuryovI9DUbwgcpa0H5QAj70CFwdsd4oMVozO
6519x56zfTiq8MaXFhIDkQNuR1Q7pMFdMfT2jogJ8/7olO7M3EtzxC8EIwfJKhTX
r15M2h3jbBwplmsNZKOB1GVvrXjOm1KtOZ4CTTM0WrPaLVDT9ax8pykjmFw16vGP
j/R5Dky9VpabtfZOu/AEW259XDEiQgTrB4Eg+S4GJjHqAzPZBmMy/xhlDK4oMXef
qXScfD4w0RxuuCFr6lxLPZz0S35BK1kIWmIkuv+9eQuI4Hr1CyVwch4fkfvrp84x
8tvAFnkCgYEA87NZaG9a8/Mob6GgY4BVLHJVOSzzFdNyMA+4LfSbtzgON2RSZyeD
0JpDowwXssw5XOyUUctj2cLLdlMCpDfdzk4F/PEakloDJWpason3lmur0/5Oq3T9
3+fnNUl4d3UOs1jcJ1yGQ/BfrTyRTcEoZx8Mu9mJ4ituVkKuLeG5vX0CgYEA+r/w
QBJS6kDyQPj1k/SMClUhWhyADwDod03hHTQHc9BleJyjXmVy+/pWhN7aELhjgLbf
o/Gm3aKJjCxS4qBmqUKwAvGoSVux1Bo2ZjcfF7sX9BXBOlFTG+bPVCZUoaksTyXN
g7GsA1frKkWWkgQuOeK3o/p9IZoBl93vEgcTGgsCgYEAv5ucCIjFMllUybCCsrkM
Ps4GQ9YbqmV9ulwhq8BPTlc8lkDCqWhgM3uXAnNXjrUTxQQd+dG4yFZoMrhBs2xZ
cQPXoXDQO5GaN6jPduETUamGiD/DCvwJQCrNlxAVL5dR36FWN3x/9JriHwsoE8Jz
SeEX2frIdpM/RYNX/6sipuECgYEA+rwFRDxOdvm8hGWuQ2WMxyQ7Nn07PEV/LxVM
HkSRkyh23vVakyDEqty3uSOSUJfgv6ud07TnU8ac3fLQatdT8LrDgB4fVkN/fYU8
kldaGwO1vxgl4OfDQCo7dXzisciViwtVBvQZ+jnm6J0vJBFUHAPt9+WZTIlQQIjm
71LtseMCgYBSAhs6lshtz+ujR3fmc4QqJVGqeXvEBPAVm6yYoKYRLwVs/rFv3WLN
LOwwBQ6lz7P9RqYYB5wVlaRvEhb9+lCve/xVcxMeZ5GkOBPxVygYV9l/wNdE25Nz
OHYtKG3GK3GEcFDwZU2LPHq21EroUAdtRfbrJ4KW2yc8igtXKxTBYw==
-----END RSA PRIVATE KEY-----
)";

std::array<uint8, 32> constexpr EnterEncryptedModePrivateKey =
{
    0x08, 0xBD, 0xC7, 0xA3, 0xCC, 0xC3, 0x4F, 0x3F,
    0x6A, 0x0B, 0xFF, 0xCF, 0x31, 0xC1, 0xB6, 0x97,
    0x69, 0x1E, 0x72, 0x9A, 0x0A, 0xAB, 0x2C, 0x77,
    0xC3, 0x6F, 0x8A, 0xE7, 0x5A, 0x9A, 0xA7, 0xC9
};

std::unique_ptr<Trinity::Crypto::RsaSignature> ConnectToRSA;
std::unique_ptr<Trinity::Crypto::Ed25519> EnterEncryptedModeSigner;
}

bool WorldPackets::Auth::ConnectTo::InitializeEncryption()
{
    std::unique_ptr<Trinity::Crypto::RsaSignature> rsa = std::make_unique<Trinity::Crypto::RsaSignature>();
    if (!rsa->LoadKeyFromString(RSAPrivateKey))
        return false;

    ConnectToRSA = std::move(rsa);
    return true;
}

void WorldPackets::Auth::ConnectTo::ShutdownEncryption()
{
    ConnectToRSA.reset();
}

WorldPackets::Auth::ConnectTo::ConnectTo() : ServerPacket(SMSG_CONNECT_TO, 256 + 1 + 16 + 2 + 4 + 1 + 8)
{
}

WorldPacket const* WorldPackets::Auth::ConnectTo::Write()
{
    ByteBuffer whereBuffer;
    whereBuffer << uint8(Payload.Where.Type);
    switch (Payload.Where.Type)
    {
        case IPv4:
            whereBuffer.append(Payload.Where.Address.V4.data(), Payload.Where.Address.V4.size());
            break;
        case IPv6:
            whereBuffer.append(Payload.Where.Address.V6.data(), Payload.Where.Address.V6.size());
            break;
        case NamedSocket:
            whereBuffer << Payload.Where.Address.Name.data();
            break;
        default:
            break;
    }

    ByteBuffer signBuffer;
    signBuffer.append(whereBuffer);
    signBuffer << uint32(Payload.Where.Type);
    signBuffer << uint16(Payload.Port);

    Trinity::Crypto::RsaSignature rsa(*ConnectToRSA);
    Trinity::Crypto::RsaSignature::SHA256 digestGenerator;
    std::vector<uint8> signature;
    rsa.Sign(signBuffer.contents(), signBuffer.size(), digestGenerator, signature);

    _worldPacket.append(signature.data(), signature.size());
    _worldPacket.append(whereBuffer);
    _worldPacket << uint16(Payload.Port);
    _worldPacket << uint32(Serial);
    _worldPacket << uint8(Con);
    _worldPacket << uint64(Key);

    return &_worldPacket;
}

void WorldPackets::Auth::AuthContinuedSession::Read()
{
    _worldPacket >> DosResponse;
    _worldPacket >> Key;
    _worldPacket.read(LocalChallenge.data(), LocalChallenge.size());
    _worldPacket.read(Digest.data(), Digest.size());
}

void WorldPackets::Auth::ConnectToFailed::Read()
{
    Serial = _worldPacket.read<ConnectToSerial>();
    _worldPacket >> Con;
}

bool WorldPackets::Auth::EnterEncryptedMode::InitializeEncryption()
{
    std::unique_ptr<Trinity::Crypto::Ed25519> ed25519 = std::make_unique<Trinity::Crypto::Ed25519>();
    if (!ed25519->LoadFromByteArray(EnterEncryptedModePrivateKey))
        return false;

    EnterEncryptedModeSigner = std::move(ed25519);
    return true;
}

void WorldPackets::Auth::EnterEncryptedMode::ShutdownEncryption()
{
    EnterEncryptedModeSigner.reset();
}

std::array<uint8, 16> constexpr EnableEncryptionSeed = { 0x90, 0x9C, 0xD0, 0x50, 0x5A, 0x2C, 0x14, 0xDD, 0x5C, 0x2C, 0xC0, 0x64, 0x14, 0xF3, 0xFE, 0xC9 };
std::array<uint8, 16> constexpr EnableEncryptionContext = { 0xA7, 0x1F, 0xB6, 0x9B, 0xC9, 0x7C, 0xDD, 0x96, 0xE9, 0xBB, 0xB8, 0x21, 0x39, 0x8D, 0x5A, 0xD4 };

WorldPacket const* WorldPackets::Auth::EnterEncryptedMode::Write()
{
    std::array<uint8, 32> toSign = Trinity::Crypto::HMAC_SHA256::GetDigestOf(EncryptionKey,
        std::array<uint8, 1>{uint8(Enabled ? 1 : 0)},
        EnableEncryptionSeed);

    Trinity::Crypto::Ed25519 ed25519(*EnterEncryptedModeSigner);
    std::vector<uint8> signature;

    ed25519.SignWithContext(toSign, { EnableEncryptionContext.begin(), EnableEncryptionContext.end() }, signature);

    _worldPacket.append(signature.data(), signature.size());
    _worldPacket.WriteBit(Enabled);
    _worldPacket.FlushBits();

    return &_worldPacket;
}
