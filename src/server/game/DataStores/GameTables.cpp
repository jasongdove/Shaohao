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

#include "DBCfmt.h"
#include "DBCStores.h"
#include "GameTables.h"
#include "ItemTemplate.h"
#include "Timer.h"
#include "Log.h"
#include "StringConvert.h"
#include "Util.h"
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <sstream>


// TODO: DATA UNAVAILABLE IN MOP
GameTable<GtArtifactKnowledgeMultiplierEntry>    sArtifactKnowledgeMultiplierGameTable("");
GameTable<GtArtifactLevelXPEntry>                sArtifactLevelXPGameTable("");
GameTable<GtBaseMPEntry>                         sBaseMPGameTable("");
GameTable<GtCombatRatingsMultByILvl>             sCombatRatingsMultByILvlGameTable("");
GameTable<GtHpPerStaEntry>                       sHpPerStaGameTable("");
GameTable<GtStaminaMultByILvl>                   sStaminaMultByILvlGameTable("");
GameTable<GtXpEntry>                             sXpGameTable("");



GameTable<GtBarberShopCostBaseEntry>             sBarberShopCostBaseGameTable(GtBarberShopCostBasefmt);
GameTable<GtBattlePetXPEntry>                    sBattlePetXPGameTable("");
GameTable<GtChanceToMeleeCritBaseEntry>          sChanceToMeleeCritBaseGameTable(GtChanceToMeleeCritBasefmt);
GameTable<GtChanceToMeleeCritEntry>              sChanceToMeleeCritGameTable(GtChanceToMeleeCritfmt);
GameTable<GtChanceToSpellCritBaseEntry>          sChanceToSpellCritBaseGameTable(GtChanceToSpellCritBasefmt);
GameTable<GtChanceToSpellCritEntry>              sChanceToSpellCritGameTable(GtChanceToSpellCritfmt);
GameTable<GtCombatRatingsEntry>                  sCombatRatingsGameTable(GtCombatRatingsfmt);
GameTable<GtItemSocketCostPerLevelEntry>         sItemSocketCostPerLevelGameTable(GtItemSocketCostPerLevelfmt);
GameTable<GtOCTBaseHPByClassEntry>               sOctBaseHpByClassStore(GtOCTBaseHPByClassfmt);
GameTable<GtOCTBaseMPByClassEntry>               sOctBaseMpByClassStore(GtOCTBaseMPByClassfmt);
GameTable<GtNpcManaCostScalerEntry>              sNpcManaCostScalerGameTable(GtNPCManaCostScalerfmt);
GameTable<GtRegenMPPerSptEntry>                  sRegenMPPerSptGameTable(GtRegenMPPerSptfmt);
GameTable<GtSpellScalingEntry>                   sSpellScalingGameTable(GtSpellScalingfmt);

typedef std::list<std::string> StoreProblemList;
uint32 GameTableCount = 0;

template<class T>
inline void LoadGameTable(StoreProblemList& errors, std::string const& tableName, GameTable<T>& storage, std::string const& dbcPath, std::string const& filename)
{
    // compatibility format and C++ structure sizes
    ASSERT(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T),
           "Size of '%s' set by format string (%u) not equal size of C++ structure (%u).",
           filename.c_str(), DBCFileLoader::GetFormatRecordSize(storage.GetFormat()), uint32(sizeof(T)));

    ++GameTableCount;
    std::string dbcFilename = dbcPath + filename;

    if (storage.Load(dbcFilename.c_str()))
    {
        bool found = false;
        // Find table definition in GameTables.dbc
        for (uint32 i = 0; i < sGameTablesStore.GetNumRows(); ++i)
        {
            GameTablesEntry const* gt = sGameTablesStore.LookupEntry(i);
            if (!gt)
                continue;

            if (tableName == gt->Name)
            {
                found = true;
                storage.SetGameTableEntry(gt);
                break;
            }
        }

        ASSERT(found, "Game table %s definition not found in GameTables.dbc", tableName.c_str());
    }
    else
    {
        // sort problematic dbc to (1) non compatible and (2) non-existed
        if (FILE* f = fopen(dbcFilename.c_str(), "rb"))
        {
            std::ostringstream stream;
            stream << dbcFilename << " exists, and has " << storage.GetFieldCount() << " field(s) (expected " << strlen(storage.GetFormat()) << "). Extracted file might be from wrong client version or a database-update has been forgotten.";
            std::string buf = stream.str();
            errors.push_back(buf);
            fclose(f);
        }
        else
            errors.push_back(dbcFilename);
    }
}

void LoadGameTables(std::string const& dataPath, uint32 defaultLocale)
{
    uint32 oldMSTime = getMSTime();

    std::string gtPath = dataPath + "dbc/" + localeNames[defaultLocale] + '/';

    StoreProblemList bad_gt_files;

#define LOAD_GT(tableName, store, file) LoadGameTable(bad_gt_files, tableName, store, gtPath, file)

//    LOAD_GT(sArtifactKnowledgeMultiplierGameTable, "ArtifactKnowledgeMultiplier.txt");
//    LOAD_GT(sArtifactLevelXPGameTable, "ArtifactLevelXP.txt");
//    LOAD_GT(sBarberShopCostBaseGameTable, "BarberShopCostBase.txt");
//    LOAD_GT(sBaseMPGameTable, "BaseMp.txt");
//    LOAD_GT(sBattlePetXPGameTable, "BattlePetXP.txt");
//    LOAD_GT(sCombatRatingsGameTable, "CombatRatings.txt");
//    LOAD_GT(sCombatRatingsMultByILvlGameTable, "CombatRatingsMultByILvl.txt");
//    LOAD_GT(sItemSocketCostPerLevelGameTable, "ItemSocketCostPerLevel.txt");
//    LOAD_GT(sHpPerStaGameTable, "HpPerSta.txt");
//    LOAD_GT(sNpcManaCostScalerGameTable, "NPCManaCostScaler.txt");
//    LOAD_GT(sSpellScalingGameTable, "SpellScaling.txt");
//    LOAD_GT(sStaminaMultByILvlGameTable, "StaminaMultByILvl.txt");
//    LOAD_GT(sXpGameTable, "xp.txt");
//
//    GameTable<GtBarberShopCostBaseEntry>             sBarberShopCostBaseGameTable(GtBarberShopCostBasefmt);
//    GameTable<GtBattlePetXPEntry>                    sBattlePetXPGameTable("");
//    GameTable<GtChanceToMeleeCritBaseEntry>          sChanceToMeleeCritBaseGameTable(GtChanceToMeleeCritBasefmt);
//    GameTable<GtChanceToMeleeCritEntry>              sChanceToMeleeCritGameTable(GtChanceToMeleeCritfmt);
//    GameTable<GtChanceToSpellCritBaseEntry>          sChanceToSpellCritBaseGameTable(GtChanceToSpellCritBasefmt);
//    GameTable<GtChanceToSpellCritEntry>              sChanceToSpellCritGameTable(GtChanceToSpellCritfmt);
//    GameTable<GtCombatRatingsEntry>                  sCombatRatingsGameTable(GtCombatRatingsfmt);
//    GameTable<GtItemSocketCostPerLevelEntry>         sItemSocketCostPerLevelGameTable(GtItemSocketCostPerLevelfmt);
//    GameTable<GtNpcManaCostScalerEntry>              sNpcManaCostScalerGameTable(GtNPCManaCostScalerfmt);
//    GameTable<GtRegenMPPerSptEntry>                  sRegenMPPerSptGameTable(GtRegenMPPerSptfmt);
//    GameTable<GtSpellScalingEntry>                   sSpellScalingGameTable(GtSpellScalingfmt);
    LOAD_GT("BarberShopCostBase", sBarberShopCostBaseGameTable, "gtBarberShopCostBase.dbc");
    LOAD_GT("OCTBaseHPByClass", sOctBaseHpByClassStore, "gtOCTBaseHPByClass.dbc");
    LOAD_GT("OCTBaseMPByClass", sOctBaseMpByClassStore, "gtOCTBaseMPByClass.dbc");

#undef LOAD_GT

    // error checks
    if (bad_gt_files.size() >= GameTableCount)
    {
        TC_LOG_ERROR("misc", "Incorrect DataDir value in worldserver.conf or ALL required *.dbc GameTable files ({}) not found by path: {}dbc/{}/", GameTableCount, dataPath.c_str(), localeNames[defaultLocale]);
        exit(1);
    }
    else if (!bad_gt_files.empty())
    {
        std::string str;
        for (auto i = bad_gt_files.begin(); i != bad_gt_files.end(); ++i)
            str += *i + "\n";

        TC_LOG_ERROR("misc", "Some required *.dbc GameTable files ({} from {}) not found or not compatible:\n{}", (uint32)bad_gt_files.size(), GameTableCount, str.c_str());
        exit(1);
    }

    TC_LOG_INFO("server.loading", ">> Initialized {} GameTables in {} ms", GameTableCount, GetMSTimeDiffToNow(oldMSTime));
}

template<class T>
float GetIlvlStatMultiplier(T const* row, InventoryType invType)
{
    switch (invType)
    {
        case INVTYPE_NECK:
        case INVTYPE_FINGER:
            return row->JewelryMultiplier;
            break;
        case INVTYPE_TRINKET:
            return row->TrinketMultiplier;
            break;
        case INVTYPE_WEAPON:
        case INVTYPE_SHIELD:
        case INVTYPE_RANGED:
        case INVTYPE_2HWEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
        case INVTYPE_HOLDABLE:
        case INVTYPE_RANGEDRIGHT:
            return row->WeaponMultiplier;
            break;
        default:
            return row->ArmorMultiplier;
            break;
    }
}

template float GetIlvlStatMultiplier(GtCombatRatingsMultByILvl const* row, InventoryType invType);
template float GetIlvlStatMultiplier(GtStaminaMultByILvl const* row, InventoryType invType);
