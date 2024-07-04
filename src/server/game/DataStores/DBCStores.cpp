/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "DBCStores.h"
#include "Log.h"
#include "SharedDefines.h"
#include "SpellInfo.h"
#include "DBCfmt.h"
#include "Timer.h"

#include <map>


struct WMOAreaTableTripple
{
    WMOAreaTableTripple(int32 r, int32 a, int32 g) :  groupId(g), rootId(r), adtId(a)
    {
    }

    bool operator <(const WMOAreaTableTripple& b) const
    {
        return memcmp(this, &b, sizeof(WMOAreaTableTripple))<0;
    }

    // ordered by entropy; that way memcmp will have a minimal medium runtime
    int32 groupId;
    int32 rootId;
    int32 adtId;
};

typedef std::multimap<uint32, CharSectionsEntry const*> CharSectionsMap;
typedef std::map<uint32, std::vector<uint32>> FactionTeamMap;
typedef std::map<WMOAreaTableTripple, WMOAreaTableEntry const*> WMOAreaInfoByTripple;

DBCStorage<AchievementEntry>                   sAchievementStore(Achievementfmt);
DBCStorage<Achievement_CategoryEntry>          sAchievementCategoryStore("");
// TODO: DATA DBCStorage<Achievement_CriteriaEntry>          sAchievementCriteriaStore(AchievementCriteriafmt);
DBCStorage<AnimationDataEntry>                 sAnimationDataStore("");
DBCStorage<AnimKitEntry>                       sAnimKitStore(AnimKitfmt);
DBCStorage<AreaTableEntry>                     sAreaTableStore(AreaTablefmt);
DBCStorage<AreaTriggerEntry>                   sAreaTriggerStore(AreaTriggerfmt);
DBCStorage<AreaTriggerActionSetEntry>          sAreaTriggerActionSetStore("");
// TODO: DATA DBCStorage<AreaTriggerSphereEntry>
DBCStorage<ArmorLocationEntry>                 sArmorLocationStore(ArmorLocationfmt);
DBCStorage<AuctionHouseEntry>                  sAuctionHouseStore(AuctionHousefmt);
DBCStorage<BankBagSlotPricesEntry>             sBankBagSlotPricesStore(BankBagSlotPricesfmt);
DBCStorage<BannedAddOnsEntry>                  sBannedAddOnsStore(BannedAddOnsfmt);
DBCStorage<BarberShopStyleEntry>               sBarberShopStyleStore(BarberShopStylefmt);
DBCStorage<BattlemasterListEntry>              sBattlemasterListStore(BattlemasterListfmt);
DBCStorage<Cfg_CategoriesEntry>                sCfgCategoriesStore("");
DBCStorage<Cfg_RegionsEntry>                   sCfgRegionsStore("");
DBCStorage<CharacterLoadoutEntry>              sCharacterLoadoutStore("");
DBCStorage<CharacterLoadoutItemEntry>          sCharacterLoadoutItemStore("");
DBCStorage<CharBaseInfoEntry>                  sCharBaseInfoStore("");
DBCStorage<CharSectionsEntry>                  sCharSectionsStore(CharSectionsfmt);
CharSectionsMap                                sCharSectionMap;
DBCStorage<CharStartOutfitEntry>               sCharStartOutfitStore(CharStartOutfitfmt);
std::map<uint32, CharStartOutfitEntry const*>  sCharStartOutfitMap;
DBCStorage<CharTitlesEntry>                    sCharTitlesStore(CharTitlesfmt);
DBCStorage<ChatChannelsEntry>                  sChatChannelsStore(ChatChannelsfmt);
DBCStorage<ChrClassesEntry>                    sChrClassesStore(ChrClassesfmt);
DBCStorage<ChrClassesXPowerTypesEntry>         sChrClassesXPowerTypesStore("");
DBCStorage<ChrRacesEntry>                      sChrRacesStore(ChrRacesfmt);
DBCStorage<ChrSpecializationEntry>             sChrSpecializationStore(ChrSpecializationfmt);
ChrSpecializationByIndexArray                  sChrSpecializationByIndexStore;
DBCStorage<CinematicCameraEntry>               sCinematicCameraStore(CinematicCamerafmt);
DBCStorage<CinematicSequencesEntry>            sCinematicSequencesStore(CinematicSequencesfmt);
DBCStorage<CreatureDisplayInfoEntry>           sCreatureDisplayInfoStore(CreatureDisplayInfofmt);
DBCStorage<CreatureDisplayInfoExtraEntry>      sCreatureDisplayInfoExtraStore(CreatureDisplayInfoExtrafmt);
DBCStorage<CreatureFamilyEntry>                sCreatureFamilyStore(CreatureFamilyfmt);
DBCStorage<CreatureImmunitiesEntry>            sCreatureImmunitiesStore(CreatureImmunitiesfmt);
DBCStorage<CreatureModelDataEntry>             sCreatureModelDataStore(CreatureModelDatafmt);
DBCStorage<CreatureSpellDataEntry>             sCreatureSpellDataStore(CreatureSpellDatafmt);
DBCStorage<CreatureTypeEntry>                  sCreatureTypeStore(CreatureTypefmt);
DBCStorage<CriteriaEntry>                      sCriteriaStore(Criteriafmt);
DBCStorage<CriteriaTreeEntry>                  sCriteriaTreeStore(CriteriaTreefmt);
DBCStorage<CurrencyTypesEntry>                 sCurrencyTypesStore(CurrencyTypesfmt);
DBCStorage<DestructibleModelDataEntry>         sDestructibleModelDataStore(DestructibleModelDatafmt);
DBCStorage<DifficultyEntry>                    sDifficultyStore(DifficultyFmt);
DBCStorage<DungeonEncounterEntry>              sDungeonEncounterStore(DungeonEncounterfmt);
DBCStorage<DurabilityCostsEntry>               sDurabilityCostsStore(DurabilityCostsfmt);
DBCStorage<DurabilityQualityEntry>             sDurabilityQualityStore(DurabilityQualityfmt);
DBCStorage<EmotesEntry>                        sEmotesStore(Emotesfmt);
DBCStorage<EmotesTextEntry>                    sEmotesTextStore(EmotesTextfmt);
typedef std::tuple<uint32, uint32, uint32> EmotesTextSoundKey;
static std::map<EmotesTextSoundKey, EmotesTextSoundEntry const*> sEmotesTextSoundMap;
DBCStorage<EmotesTextSoundEntry>               sEmotesTextSoundStore(EmotesTextSoundfmt);
DBCStorage<FactionEntry>                       sFactionStore(Factionfmt);
static FactionTeamMap                          sFactionTeamMap;
DBCStorage<FactionTemplateEntry>               sFactionTemplateStore(FactionTemplatefmt);
DBCStorage<FriendshipRepReactionEntry>         sFriendshipRepReactionStore("");
DBCStorage<FriendshipReputationEntry>          sFriendshipReputationStore("");
DBCStorage<GameObjectArtKitEntry>              sGameObjectArtKitStore("");
DBCStorage<GameObjectDisplayInfoEntry>         sGameObjectDisplayInfoStore(GameObjectDisplayInfofmt);
DBCStorage<GameTablesEntry>                    sGameTablesStore("");
DBCStorage<GemPropertiesEntry>                 sGemPropertiesStore(GemPropertiesfmt);
DBCStorage<GlyphPropertiesEntry>               sGlyphPropertiesStore(GlyphPropertiesfmt);
DBCStorage<GuildColorBackgroundEntry>          sGuildColorBackgroundStore("");
DBCStorage<GuildColorBorderEntry>              sGuildColorBorderStore("");
DBCStorage<GuildColorEmblemEntry>              sGuildColorEmblemStore("");
DBCStorage<GuildPerkSpellsEntry>               sGuildPerkSpellsStore("");
DBCStorage<HolidaysEntry>                      sHolidaysStore(Holidaysfmt);
DBCStorage<ImportPriceArmorEntry>              sImportPriceArmorStore(ImportPriceArmorfmt);
DBCStorage<ImportPriceQualityEntry>            sImportPriceQualityStore(ImportPriceQualityfmt);
DBCStorage<ImportPriceShieldEntry>             sImportPriceShieldStore(ImportPriceShieldfmt);
DBCStorage<ImportPriceWeaponEntry>             sImportPriceWeaponStore(ImportPriceWeaponfmt);
DBCStorage<ItemArmorQualityEntry>              sItemArmorQualityStore(ItemArmorQualityfmt);
DBCStorage<ItemArmorShieldEntry>               sItemArmorShieldStore(ItemArmorShieldfmt);
DBCStorage<ItemArmorTotalEntry>                sItemArmorTotalStore(ItemArmorTotalfmt);
DBCStorage<ItemBagFamilyEntry>                 sItemBagFamilyStore(ItemBagFamilyfmt);
DBCStorage<ItemClassEntry>                     sItemClassStore(ItemClassfmt);
DBCStorage<ItemDamageAmmoEntry>                sItemDamageAmmoStore("");
DBCStorage<ItemDamageOneHandEntry>             sItemDamageOneHandStore(ItemDamagefmt);
DBCStorage<ItemDamageOneHandCasterEntry>       sItemDamageOneHandCasterStore(ItemDamagefmt);
// TODO: DATA ItemDamageRanged, ItemDamageThrown, ItemDamageWand
DBCStorage<ItemDamageTwoHandEntry>             sItemDamageTwoHandStore(ItemDamagefmt);
DBCStorage<ItemDamageTwoHandCasterEntry>       sItemDamageTwoHandCasterStore(ItemDamagefmt);
DBCStorage<ItemDisenchantLootEntry>            sItemDisenchantLootStore(ItemDisenchantLootfmt);
DBCStorage<ItemDisplayInfoEntry>               sItemDisplayInfoStore(ItemDisplayInfofmt);
DBCStorage<ItemLimitCategoryEntry>             sItemLimitCategoryStore(ItemLimitCategoryfmt);
DBCStorage<ItemNameDescriptionEntry>           sItemNameDescriptionStore(ItemNameDescriptionfmt);
DBCStorage<ItemPriceBaseEntry>                 sItemPriceBaseStore("");
DBCStorage<ItemRandomPropertiesEntry>          sItemRandomPropertiesStore(ItemRandomPropertiesfmt);
DBCStorage<ItemRandomSuffixEntry>              sItemRandomSuffixStore(ItemRandomSuffixfmt);
DBCStorage<ItemSetEntry>                       sItemSetStore(ItemSetfmt);
DBCStorage<ItemSpecEntry>                      sItemSpecStore(ItemSpecfmt);
DBCStorage<ItemSpecOverrideEntry>              sItemSpecOverrideStore(ItemSpecOverridefmt);
// TODO: DATA ItemUpgrade, ItemUpgradePath
DBCStorage<JournalEncounterEntry>              sJournalEncounterStore("");
DBCStorage<JournalEncounterSectionEntry>       sJournalEncounterSectionStore("");
DBCStorage<JournalInstanceEntry>               sJournalInstanceStore("");
DBCStorage<JournalTierEntry>                   sJournalTierStore("");
DBCStorage<LanguagesEntry>                     sLanguagesStore("");
DBCStorage<LanguageWordsEntry>                 sLanguageWordsStore("");
DBCStorage<LFGDungeonsEntry>                   sLFGDungeonsStore(LFGDungeonfmt);
DBCStorage<LightEntry>                         sLightStore(Lightfmt);
DBCStorage<LiquidTypeEntry>                    sLiquidTypeStore(LiquidTypefmt);
DBCStorage<LockEntry>                          sLockStore(Lockfmt);
DBCStorage<MailTemplateEntry>                  sMailTemplateStore(MailTemplatefmt);
DBCStorage<MapEntry>                           sMapStore(Mapfmt);
DBCStorage<MapDifficultyEntry>                 sMapDifficultyStore(MapDifficultyfmt); // only for loading
MapDifficultyMap                               sMapDifficultyMap;
DBCStorage<ModifierTreeEntry>                  sModifierTreeStore(ModifierTreefmt);
DBCStorage<MountCapabilityEntry>               sMountCapabilityStore(MountCapabilityfmt);
DBCStorage<MountTypeEntry>                     sMountTypeStore(MountTypefmt);
DBCStorage<MovieEntry>                         sMovieStore(Moviefmt);
DBCStorage<NameGenEntry>                       sNameGenStore(NameGenfmt);
DBCStorage<NamesProfanityEntry>                sNamesProfanityStore("");
DBCStorage<NamesReservedEntry>                 sNamesReservedStore("");
DBCStorage<NamesReservedLocaleEntry>           sNamesReservedLocaleStore("");
DBCStorage<OverrideSpellDataEntry>             sOverrideSpellDataStore(OverrideSpellDatafmt);
DBCStorage<PhaseEntry>                         sPhaseStore(Phasefmt);
DBCStorage<PhaseXPhaseGroupEntry>              sPhaseXPhaseGroupStore("");
DBCStorage<PlayerConditionEntry>               sPlayerConditionStore(PlayerConditionfmt);
DBCStorage<PowerDisplayEntry>                  sPowerDisplayStore(PowerDisplayfmt);
DBCStorage<PvPDifficultyEntry>                 sPvpDifficultyStore(PvpDifficultyfmt);
DBCStorage<QuestFactionRewardEntry>            sQuestFactionRewardStore(QuestFactionRewardfmt);
DBCStorage<QuestInfoEntry>                     sQuestInfoStore("");
DBCStorage<QuestMoneyRewardEntry>              sQuestMoneyRewardStore("");
// TODO: DATA QuestPOIBlob, QuestPOIPoint ??
DBCStorage<QuestSortEntry>                     sQuestSortStore(QuestSortfmt);
DBCStorage<QuestV2Entry>                       sQuestV2Store(QuestV2fmt);
DBCStorage<QuestXPEntry>                       sQuestXPStore(QuestXPfmt);
DBCStorage<RandPropPointsEntry>                sRandPropPointsStore(RandPropPointsfmt);
DBCStorage<ResearchBranchEntry>                sResearchBranchStore(ResearchBranchfmt);
DBCStorage<ResearchProjectEntry>               sResearchProjectStore(ResearchProjectfmt);
DBCStorage<ResearchSiteEntry>                  sResearchSiteStore(ResearchSitefmt);
// TODO: DATA ScalingStatDistribution, ScalingStatValues ??
DBCStorage<ScenarioEntry>                      sScenarioStore(Scenariofmt);
DBCStorage<ScenarioStepEntry>                  sScenarioStepStore(ScenarioStepfmt);
DBCStorage<ServerMessagesEntry>                sServerMessagesStore("");
DBCStorage<SkillLineEntry>                     sSkillLineStore(SkillLinefmt);
DBCStorage<SkillLineAbilityEntry>              sSkillLineAbilityStore(SkillLineAbilityfmt);
DBCStorage<SkillRaceClassInfoEntry>            sSkillRaceClassInfoStore(SkillRaceClassInfofmt);
SkillRaceClassInfoMap                          SkillRaceClassInfoBySkill;
// TODO: DATA FROM DB DBCStorage<SkillTiersEntry>                  sSkillTiersStore();
DBCStorage<SoundEntriesEntry>                  sSoundEntriesStore(SoundEntriesfmt);
DBCStorage<SpecializationSpellsEntry>          sSpecializationSpellsStore(SpecializationSpellsfmt);
DBCStorage<SpellEntry>                         sSpellStore(Spellfmt);
DBCStorage<SpellAuraOptionsEntry>              sSpellAuraOptionsStore(SpellAuraOptionsfmt);
DBCStorage<SpellAuraRestrictionsEntry>         sSpellAuraRestrictionsStore(SpellAuraRestrictionsfmt);
DBCStorage<SpellCastingRequirementsEntry>      sSpellCastingRequirementsStore(SpellCastingRequirementsfmt);
DBCStorage<SpellCastTimesEntry>                sSpellCastTimesStore("");
DBCStorage<SpellCategoriesEntry>               sSpellCategoriesStore(SpellCategoriesfmt);
DBCStorage<SpellCategoryEntry>                 sSpellCategoryStore(SpellCategoryfmt);
DBCStorage<SpellClassOptionsEntry>             sSpellClassOptionsStore("");
DBCStorage<SpellCooldownsEntry>                sSpellCooldownsStore(SpellCooldownsfmt);
DBCStorage<SpellDurationEntry>                 sSpellDurationStore("");
DBCStorage<SpellEffectEntry>                   sSpellEffectStore(SpellEffectfmt);
DBCStorage<SpellEffectScalingEntry>            sSpellEffectScalingStore(SpellEffectScalingfmt);
SpellEffectScalingByEffectId                   sSpellEffectScalingByEffectId;
DBCStorage<SpellEquippedItemsEntry>            sSpellEquippedItemsStore(SpellEquippedItemsfmt);
DBCStorage<SpellFocusObjectEntry>              sSpellFocusObjectStore(SpellFocusObjectfmt);
DBCStorage<SpellInterruptsEntry>               sSpellInterruptsStore(SpellInterruptsfmt);
DBCStorage<SpellItemEnchantmentEntry>          sSpellItemEnchantmentStore(SpellItemEnchantmentfmt);
DBCStorage<SpellItemEnchantmentConditionEntry> sSpellItemEnchantmentConditionStore("");
DBCStorage<SpellKeyboundOverrideEntry>         sSpellKeyboundOverrideStore("");
DBCStorage<SpellLearnSpellEntry>               sSpellLearnSpellStore("");
DBCStorage<SpellLevelsEntry>                   sSpellLevelsStore(SpellLevelsfmt);
DBCStorage<SpellMiscEntry>                     sSpellMiscStore(SpellMiscfmt);
DBCStorage<SpellPowerEntry>                    sSpellPowerStore(SpellPowerfmt);
DBCStorage<SpellProcsPerMinuteEntry>           sSpellProcsPerMinuteStore(SpellProcsPerMinutefmt);
DBCStorage<SpellProcsPerMinuteModEntry>        sSpellProcsPerMinuteModStore(SpellProcsPerMinuteModfmt);
DBCStorage<SpellRadiusEntry>                   sSpellRadiusStore(SpellRadiusfmt);
DBCStorage<SpellRangeEntry>                    sSpellRangeStore(SpellRangefmt);
// TODO: DATA ALSO DB2? DBCStorage<SpellReagentsEntry>                 sSpellReagentsStore("");
DBCStorage<SpellRuneCostEntry>                 sSpellRuneCostStore(SpellRuneCostfmt);
DBCStorage<SpellScalingEntry>                  sSpellScalingStore(SpellScalingfmt);
DBCStorage<SpellShapeshiftEntry>               sSpellShapeshiftStore(SpellShapeshiftfmt);
DBCStorage<SpellShapeshiftFormEntry>           sSpellShapeshiftFormStore(SpellShapeshiftFormfmt);
DBCStorage<SpellTargetRestrictionsEntry>       sSpellTargetRestrictionsStore(SpellTargetRestrictionsfmt);
DBCStorage<SpellTotemsEntry>                   sSpellTotemsStore(SpellTotemsfmt);
// TODO: DATA ALSO DB2? DBCStorage<SpellVisualEntry>                   sSpellVisualStore("");
// TODO: DATA ALSO DB2? DBCStorage<SpellVisualEffectNameEntry>         sSpellVisualEffectNameStore("");
// TODO: DATA ALSO DB2? DBCStorage<SpellVisualKitEntry>                sSpellVisualKitStore("");
DBCStorage<SummonPropertiesEntry>              sSummonPropertiesStore(SummonPropertiesfmt);
DBCStorage<TalentEntry>                        sTalentStore(Talentfmt);
TalentsByPosition                              sTalentByPos;
DBCStorage<TaxiNodesEntry>                     sTaxiNodesStore(TaxiNodesfmt);
DBCStorage<TaxiPathEntry>                      sTaxiPathStore(TaxiPathfmt);
DBCStorage<TaxiPathNodeEntry>                  sTaxiPathNodeStore(TaxiPathNodefmt);
DBCStorage<TotemCategoryEntry>                 sTotemCategoryStore(TotemCategoryfmt);
DBCStorage<TransportAnimationEntry>            sTransportAnimationStore(TransportAnimationfmt);
DBCStorage<TransportRotationEntry>             sTransportRotationStore(TransportRotationfmt);
DBCStorage<UnitConditionEntry>                 sUnitConditionStore("");
DBCStorage<UnitPowerBarEntry>                  sUnitPowerBarStore(UnitPowerBarfmt);
DBCStorage<VehicleEntry>                       sVehicleStore(Vehiclefmt);
DBCStorage<VehicleSeatEntry>                   sVehicleSeatStore(VehicleSeatfmt);
DBCStorage<WMOAreaTableEntry>                  sWMOAreaTableStore(WMOAreaTablefmt);
static WMOAreaInfoByTripple                    sWMOAreaInfoByTripple;
DBCStorage<WorldEffectEntry>                   sWorldEffectStore("");
DBCStorage<WorldMapAreaEntry>                  sWorldMapAreaStore(WorldMapAreafmt);
DBCStorage<WorldMapOverlayEntry>               sWorldMapOverlayStore(WorldMapOverlayfmt);
DBCStorage<WorldMapTransformsEntry>            sWorldMapTransformsStore(WorldMapTransformsfmt);
DBCStorage<WorldSafeLocsEntry>                 sWorldSafeLocsStore(WorldSafeLocsfmt);
DBCStorage<WorldStateExpressionEntry>          sWorldStateExpressionStore("");

typedef std::list<std::string> StoreProblemList;

uint32 DBCFileCount = 0;
uint32 GameTableCount = 0;

template<class T>
inline void LoadDBC(uint32& availableDbcLocales, StoreProblemList& errors, DBCStorage<T>& storage, std::string const& dbcPath, std::string const& filename, uint32 defaultLocale, std::string const* customFormat = NULL, std::string const* customIndexName = NULL)
{
    // compatibility format and C++ structure sizes
    ASSERT(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T),
        "Size of '%s' set by format string (%u) not equal size of C++ structure (%u).",
        filename.c_str(), DBCFileLoader::GetFormatRecordSize(storage.GetFormat()), uint32(sizeof(T)));

    ++DBCFileCount;
    std::string dbcFilename = dbcPath + localeNames[defaultLocale] + '/' + filename;
    SqlDbc * sql = NULL;
    if (customFormat)
        sql = new SqlDbc(&filename, customFormat, customIndexName, storage.GetFormat());

    if (storage.Load(dbcFilename.c_str(), sql))
    {
        for (uint8 i = 0; i < TOTAL_LOCALES; ++i)
        {
            if (i == LOCALE_none || !(availableDbcLocales & (1 << i)))
                continue;

            std::string localizedName(dbcPath);
            localizedName.append(localeNames[i]);
            localizedName.push_back('/');
            localizedName.append(filename);

            if (!storage.LoadStringsFrom(localizedName.c_str()))
                availableDbcLocales &= ~(1<<i);             // mark as not available for speedup next checks
        }
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

    delete sql;
}

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
        // Find table definition in GameTables.db2
        for (uint32 i = 0; i < sGameTablesStore.GetNumRows(); ++i)
        {
            GameTablesEntry const* gt = sGameTablesStore.LookupEntry(i);
            if (!gt)
                continue;

            for (uint32 l = 0; l < TOTAL_LOCALES; ++l)
            {
                if (l != LOCALE_none && tableName == gt->Name->Str[l])
                {
                    found = true;
                    storage.SetGameTableEntry(gt);
                    break;
                }
            }

            if (found)
                break;
        }

        ASSERT(found, "Game table %s definition not found in GameTables.db2", tableName.c_str());
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

void LoadDBCStores(const std::string& dataPath, uint32 defaultLocale)
{
    uint32 oldMSTime = getMSTime();

    std::string dbcPath = dataPath + "dbc/";

    StoreProblemList bad_dbc_files;
    uint32 availableDbcLocales = 0xFFFFFFFF;

#define LOAD_DBC(store, file) LoadDBC(availableDbcLocales, bad_dbc_files, store, dbcPath, file, defaultLocale)

    LOAD_DBC(sAchievementStore, "Achievement.dbc");
    // TODO: DATA LOAD_DBC(sAchievementCategoryStore, "Achievement_Category.dbc");
    // TODO: DATA LOAD_DBC(sAchievementCriteriaStore, "Achievement_Criteria.dbc");
    // TODO: DATA LOAD_DBC(sAnimationDataStore, "AnimationData.dbc");
    LOAD_DBC(sAnimKitStore, "AnimKit.dbc");//20444
    LOAD_DBC(sAreaTableStore, "AreaTable.dbc");//20444
    LOAD_DBC(sAreaTriggerStore, "AreaTrigger.dbc");//20444
    // TODO: DATA LOAD_DBC(sAreaTriggerActionSetStore, "AreaTriggerActionSet.dbc");
    // TODO: DATA LOAD_DBC(sAreaTriggerSphereStore, "AreaTriggerSphere.dbc");
    LOAD_DBC(sArmorLocationStore, "ArmorLocation.dbc");//20444
    LOAD_DBC(sAuctionHouseStore, "AuctionHouse.dbc");
    LOAD_DBC(sBankBagSlotPricesStore, "BankBagSlotPrices.dbc");//20444
    LOAD_DBC(sBannedAddOnsStore, "BannedAddOns.dbc");//20444
    LOAD_DBC(sBarberShopStyleStore, "BarberShopStyle.dbc");
    LOAD_DBC(sBattlemasterListStore, "BattlemasterList.dbc");//20444
    // TODO: DATA LOAD_DBC(sCfgCategoriesStore, "Cfg_Categories.dbc");
    // TODO: DATA LOAD_DBC(sCfgRegionsStore, "Cfg_Regions.dbc");
    // TODO: DATA LOAD_DBC(sCharacterLoadoutStore, "CharacterLoadout.dbc");
    // TODO: DATA LOAD_DBC(sCharacterLoadoutItemStore, "CharacterLoadoutItem.dbc");
    // TODO: DATA LOAD_DBC(sCharBaseInfoStore, "CharBaseInfo.dbc");
    LOAD_DBC(sCharSectionsStore, "CharSections.dbc");//20444
    // sCharSectionMap
    LOAD_DBC(sCharStartOutfitStore, "CharStartOutfit.dbc");
    // sCharStartOutfitMap
    LOAD_DBC(sCharTitlesStore, "CharTitles.dbc");//20444
    LOAD_DBC(sChatChannelsStore, "ChatChannels.dbc");//20444
    LOAD_DBC(sChrClassesStore, "ChrClasses.dbc");//20444
    // TODO: DATA LOAD_DBC(sChrClassesXPowerTypesStore, "ChrClassesXPowerTypes.dbc");
    LOAD_DBC(sChrRacesStore, "ChrRaces.dbc");//20444
    LOAD_DBC(sChrSpecializationStore, "ChrSpecialization.dbc");//20444
    // sChrSpecializationByIndexStore
    LOAD_DBC(sCinematicCameraStore, "CinematicCamera.dbc");
    LOAD_DBC(sCinematicSequencesStore, "CinematicSequences.dbc");
    LOAD_DBC(sCreatureDisplayInfoStore, "CreatureDisplayInfo.dbc");
    LOAD_DBC(sCreatureDisplayInfoExtraStore, "CreatureDisplayInfoExtra.dbc");//20444
    LOAD_DBC(sCreatureFamilyStore, "CreatureFamily.dbc");//20444
    LOAD_DBC(sCreatureImmunitiesStore, "CreatureImmunities.dbc");
    LOAD_DBC(sCreatureModelDataStore, "CreatureModelData.dbc");//20444
    LOAD_DBC(sCreatureSpellDataStore, "CreatureSpellData.dbc");
    LOAD_DBC(sCreatureTypeStore, "CreatureType.dbc");
    LOAD_DBC(sCriteriaStore, "Criteria.dbc");
    LOAD_DBC(sCriteriaTreeStore, "CriteriaTree.dbc");
    LOAD_DBC(sCurrencyTypesStore, "CurrencyTypes.dbc");
    LOAD_DBC(sDestructibleModelDataStore, "DestructibleModelData.dbc");
    LOAD_DBC(sDifficultyStore, "Difficulty.dbc");//20444
    LOAD_DBC(sDungeonEncounterStore, "DungeonEncounter.dbc");//20444
    LOAD_DBC(sDurabilityCostsStore, "DurabilityCosts.dbc");//20444
    LOAD_DBC(sDurabilityQualityStore, "DurabilityQuality.dbc");
    LOAD_DBC(sEmotesStore, "Emotes.dbc");//20444
    LOAD_DBC(sEmotesTextStore, "EmotesText.dbc");//20444
    //
    //
    LOAD_DBC(sEmotesTextSoundStore, "EmotesTextSound.dbc");
    LOAD_DBC(sFactionStore, "Faction.dbc");//20444
    //
    LOAD_DBC(sFactionTemplateStore, "FactionTemplate.dbc");//20444
    // TODO: DATA LOAD_DBC(sFriendshipRepReactionStore, "FriendshipRepReaction.dbc");
    // TODO: DATA LOAD_DBC(sFriendshipReputationStore, "FriendshipReputation.dbc");
    // TODO: DATA LOAD_DBC(sGameObjectArtKitStore, "GameObjectArtKit.dbc");
    LOAD_DBC(sGameObjectDisplayInfoStore, "GameObjectDisplayInfo.dbc");//20444
    // TODO: DATA LOAD_DBC(sGameTablesStore, "GameTables.dbc");
    LOAD_DBC(sGemPropertiesStore, "GemProperties.dbc");//20444
    LOAD_DBC(sGlyphPropertiesStore, "GlyphProperties.dbc");//20444
    // TODO: DATA LOAD_DBC(sGuildColorBackgroundStore, "GuildColorBackground.dbc");//20444
    // TODO: DATA LOAD_DBC(sGuildColorBorderStore, "GuildColorBorder.dbc"); //20444
    // TODO: DATA LOAD_DBC(sGuildColorEmblemStore, "GuildColorEmblem.dbc");//20444
    // TODO: DATA LOAD_DBC(sGuildPerkSpellsStore, "GuildPerkSpells.dbc");//20444
    LOAD_DBC(sHolidaysStore, "Holidays.dbc");
    LOAD_DBC(sImportPriceArmorStore, "ImportPriceArmor.dbc");
    LOAD_DBC(sImportPriceQualityStore, "ImportPriceQuality.dbc");
    LOAD_DBC(sImportPriceShieldStore, "ImportPriceShield.dbc");
    LOAD_DBC(sImportPriceWeaponStore, "ImportPriceWeapon.dbc");
    LOAD_DBC(sItemArmorQualityStore, "ItemArmorQuality.dbc");//20444
    LOAD_DBC(sItemArmorShieldStore, "ItemArmorShield.dbc");//20444
    LOAD_DBC(sItemArmorTotalStore, "ItemArmorTotal.dbc");//20444
    LOAD_DBC(sItemBagFamilyStore, "ItemBagFamily.dbc");//20444
    LOAD_DBC(sItemClassStore, "ItemClass.dbc");
    // TODO: DATA LOAD_DBC(sItemDamageAmmoStore, "ItemDamageAmmo.dbc");//20444
    LOAD_DBC(sItemDamageOneHandStore, "ItemDamageOneHand.dbc");//20444
    LOAD_DBC(sItemDamageOneHandCasterStore, "ItemDamageOneHandCaster.dbc");//20444
    // TODO: DATA LOAD_DBC(sItemDamageRangedStore, "ItemDamageRanged.dbc");//20444
    // TODO: DATA LOAD_DBC(sItemDamageThrownStore, "ItemDamageThrown.dbc");//20444
    LOAD_DBC(sItemDamageTwoHandStore, "ItemDamageTwoHand.dbc");//20444
    LOAD_DBC(sItemDamageTwoHandCasterStore, "ItemDamageTwoHandCaster.dbc");//20444
    // TODO: DATA LOAD_DBC(sItemDamageWandStore, "ItemDamageWand.dbc");//20444
    LOAD_DBC(sItemDisenchantLootStore, "ItemDisenchantLoot.dbc");
    LOAD_DBC(sItemDisplayInfoStore, "ItemDisplayInfo.dbc");//20444
    LOAD_DBC(sItemLimitCategoryStore, "ItemLimitCategory.dbc");
    LOAD_DBC(sItemNameDescriptionStore, "ItemNameDescription.dbc");
    // TODO: DATA LOAD_DBC(sItemPriceBaseStore, "ItemPriceBase.dbc");
    LOAD_DBC(sItemRandomPropertiesStore, "ItemRandomProperties.dbc");
    LOAD_DBC(sItemRandomSuffixStore, "ItemRandomSuffix.dbc");
    LOAD_DBC(sItemSetStore, "ItemSet.dbc");//20444
    LOAD_DBC(sItemSpecStore, "ItemSpec.dbc");
    LOAD_DBC(sItemSpecOverrideStore, "ItemSpecOverride.dbc");
    //
    // TODO: DATA LOAD_DBC(sJournalEncounterStore, "JournalEncounter.dbc");
    // TODO: DATA LOAD_DBC(sJournalEncounterSectionStore, "JournalEncounterSection.dbc");
    // TODO: DATA LOAD_DBC(sJournalInstanceStore, "Journalnstance.dbc");
    // TODO: DATA LOAD_DBC(sJournalTierStore, "JournalTier.dbc");
    // TODO: DATA LOAD_DBC(sLanguagesStore, "Languages.dbc");
    // TODO: DATA LOAD_DBC(sLanguageWordsStore, "LanguageWords.dbc");
    LOAD_DBC(sLFGDungeonsStore, "LfgDungeons.dbc");//20444
    LOAD_DBC(sLightStore, "Light.dbc"); //20444
    LOAD_DBC(sLiquidTypeStore, "LiquidType.dbc");//20444
    LOAD_DBC(sLockStore, "Lock.dbc");//20444
    LOAD_DBC(sMailTemplateStore, "MailTemplate.dbc");
    LOAD_DBC(sMapStore, "Map.dbc");//20444
    LOAD_DBC(sMapDifficultyStore, "MapDifficulty.dbc");//20444
    // sMapDifficultyMap
    LOAD_DBC(sModifierTreeStore, "ModifierTree.dbc");
    LOAD_DBC(sMountCapabilityStore, "MountCapability.dbc");
    LOAD_DBC(sMountTypeStore, "MountType.dbc");
    LOAD_DBC(sMovieStore, "Movie.dbc");//20444
    LOAD_DBC(sNameGenStore, "NameGen.dbc");
    // TODO: DATA LOAD_DBC(sNamesProfanityStore, "NamesProfanity.dbc");
    // TODO: DATA LOAD_DBC(sNamesReservedStore, "NamesReserved.dbc");
    // TODO: DATA LOAD_DBC(sNamesReservedLocaleStore, "NamesReservedLocaleStore.dbc");
    LOAD_DBC(sOverrideSpellDataStore, "OverrideSpellData.dbc");
    LOAD_DBC(sPhaseStore, "Phase.dbc"); // 20444
    // TODO: DATA LOAD_DBC(sPhaseXPhaseGroupStore, "PhaseXPhaseGroup.dbc");
    LOAD_DBC(sPlayerConditionStore, "PlayerCondition.dbc");
    LOAD_DBC(sPowerDisplayStore, "PowerDisplay.dbc");//20444
    LOAD_DBC(sPvpDifficultyStore, "PvpDifficulty.dbc");//20444
    LOAD_DBC(sQuestFactionRewardStore, "QuestFactionReward.dbc");//20444
    // TODO: DATA LOAD_DBC(sQuestInfoStore, "QuestInfo.dbc");
    // TODO: DATA LOAD_DBC(sQuestMoneyRewardStore, "QuestMoneyReward.dbc");
    //
    LOAD_DBC(sQuestSortStore, "QuestSort.dbc");
    LOAD_DBC(sQuestV2Store, "QuestV2.dbc");
    LOAD_DBC(sQuestXPStore, "QuestXP.dbc");
    LOAD_DBC(sRandPropPointsStore, "RandPropPoints.dbc");
    LOAD_DBC(sResearchBranchStore, "ResearchBranch.dbc");
    LOAD_DBC(sResearchProjectStore, "ResearchProject.dbc");
    LOAD_DBC(sResearchSiteStore, "ResearchSite.dbc");
    //
    LOAD_DBC(sScenarioStore, "Scenario.dbc");
    LOAD_DBC(sScenarioStepStore, "ScenarioStep.dbc");
    // TODO: DATA LOAD_DBC(sServerMessagesStore, "ServerMessages.dbc");
    LOAD_DBC(sSkillLineStore, "SkillLine.dbc");//20444
    LOAD_DBC(sSkillLineAbilityStore, "SkillLineAbility.dbc");//20444
    LOAD_DBC(sSkillRaceClassInfoStore, "SkillRaceClassInfo.dbc");//20444
    //
    //
    LOAD_DBC(sSoundEntriesStore, "SoundEntries.dbc");
    LOAD_DBC(sSpecializationSpellsStore, "SpecializationSpells.dbc");
    LOAD_DBC(sSpellStore, "Spell.dbc"/*, &CustomSpellfmt, &CustomSpellEntryIndex*/);//20444
    LOAD_DBC(sSpellAuraOptionsStore, "SpellAuraOptions.dbc");//20444
    LOAD_DBC(sSpellAuraRestrictionsStore, "SpellAuraRestrictions.dbc");
    LOAD_DBC(sSpellCastingRequirementsStore, "SpellCastingRequirements.dbc");
    // TODO: DATA LOAD_DBC(sSpellCastTimesStore, "SpellCastTimes.dbc");
    LOAD_DBC(sSpellCategoriesStore, "SpellCategories.dbc");//20444
    LOAD_DBC(sSpellCategoryStore, "SpellCategory.dbc");//20444
    // TODO: DATA LOAD_DBC(sSpellClassOptionsStore, "SpellClassOptions.dbc");
    LOAD_DBC(sSpellCooldownsStore, "SpellCooldowns.dbc");//20444
    LOAD_DBC(sSpellDurationStore, "SpellDuration.dbc");
    LOAD_DBC(sSpellEffectStore, "SpellEffect.dbc"/*, &CustomSpellEffectfmt, &CustomSpellEffectEntryIndex*/);//20444
    LOAD_DBC(sSpellEffectScalingStore, "SpellEffectScaling.dbc");//20444
    //
    LOAD_DBC(sSpellEquippedItemsStore, "SpellEquippedItems.dbc");//20444
    LOAD_DBC(sSpellFocusObjectStore, "SpellFocusObject.dbc");//20444
    LOAD_DBC(sSpellInterruptsStore, "SpellInterrupts.dbc");//20444
    LOAD_DBC(sSpellItemEnchantmentStore, "SpellItemEnchantment.dbc");//20444
    // TODO: DATA LOAD_DBC(sSpellItemEnchantmentConditionStore, "SpellItemEnchantmentCondition.dbc");
    // TODO: DATA LOAD_DBC(sSpellKeyboundOverrideStore, "SpellKeyboundOverride.dbc");
    // TODO: DATA LOAD_DBC(sSpellLearnSpellStore, "SpellLearnSpell.dbc");
    LOAD_DBC(sSpellLevelsStore, "SpellLevels.dbc");//20444
    LOAD_DBC(sSpellMiscStore, "SpellMisc.dbc");
    LOAD_DBC(sSpellPowerStore, "SpellPower.dbc");
    LOAD_DBC(sSpellProcsPerMinuteStore, "SpellProcsPerMinute.dbc");
    LOAD_DBC(sSpellProcsPerMinuteModStore, "SpellProcsPerMinuteModStore.dbc");
    LOAD_DBC(sSpellRadiusStore, "SpellRadius.dbc");
    LOAD_DBC(sSpellRangeStore, "SpellRange.dbc");
    // TODO: DATA LOAD_DBC(sSpellReagentsStore, "SpellReagents.dbc");
    LOAD_DBC(sSpellRuneCostStore, "SpellRuneCost.dbc");
    LOAD_DBC(sSpellScalingStore, "SpellScaling.dbc");//20444
    LOAD_DBC(sSpellShapeshiftStore, "SpellShapeshift.dbc");//20444
    LOAD_DBC(sSpellShapeshiftFormStore, "SpellShapeshiftForm.dbc");//20444
    LOAD_DBC(sSpellTargetRestrictionsStore, "SpellTargetRestrictions.dbc");//20444
    LOAD_DBC(sSpellTotemsStore, "SpellTotems.dbc");
    // TODO: DATA LOAD_DBC(sSpellVisualStore, "SpellVisual.dbc");
    // TODO: DATA LOAD_DBC(sSpellVisualEffectNameStore, "SpellVisualEffectName.dbc");
    // TODO: DATA LOAD_DBC(sSpellVisualKitStore, "SpellVisualKit.dbc");
    LOAD_DBC(sSummonPropertiesStore, "SummonProperties.dbc");//20444
    LOAD_DBC(sTalentStore, "Talent.dbc");//20444
    //
    LOAD_DBC(sTaxiNodesStore, "TaxiNodes.dbc");
    LOAD_DBC(sTaxiPathStore, "TaxiPath.dbc");
    LOAD_DBC(sTaxiPathNodeStore, "TaxiPathNode.dbc");
    LOAD_DBC(sTotemCategoryStore, "TotemCategory.dbc");
    LOAD_DBC(sTransportAnimationStore, "TransportAnimation.dbc");
    LOAD_DBC(sTransportRotationStore, "TransportRotation.dbc");
    // TODO: DATA LOAD_DBC(sUnitConditionStore, "UnitCondition.dbc");
    // TODO: DATA LOAD_DBC(sUnitPowerBarStore, "UnitPowerBar.dbc");
    LOAD_DBC(sVehicleStore, "Vehicle.dbc");//20444
    LOAD_DBC(sVehicleSeatStore, "VehicleSeat.dbc");//20444
    LOAD_DBC(sWMOAreaTableStore, "WMOAreaTable.dbc");//20444
    //
    // TODO: DATA LOAD_DBC(sWorldEffectStore, "WorldEffect.dbc");
    LOAD_DBC(sWorldMapAreaStore, "WorldMapArea.dbc");//20444
    LOAD_DBC(sWorldMapOverlayStore, "WorldMapOverlay.dbc");
    LOAD_DBC(sWorldMapTransformsStore, "WorldMapTransforms.dbc");//20444
    LOAD_DBC(sWorldSafeLocsStore, "WorldSafeLocs.dbc"); // 20444
    // TODO: DATA LOAD_DBC(sWorldStateExpressionStore, "WorldStateExpression.dbc");

#undef LOAD_DBC

    for (uint32 i = 0; i < sCharSectionsStore.GetNumRows(); ++i)
        if (CharSectionsEntry const* entry = sCharSectionsStore.LookupEntry(i))
        {
            Trinity::RaceMask<uint64> raceMask = { entry->Race };
            if (entry->Race && !(raceMask & RACEMASK_ALL_PLAYABLE).IsEmpty()) //ignore Nonplayable races
                sCharSectionMap.insert({entry->GenType | (entry->Gender << 8) | (entry->Race << 16), entry});
        }

    memset(sChrSpecializationByIndexStore, 0, sizeof(sChrSpecializationByIndexStore));
    for (uint32 i = 0; i < sChrSpecializationStore.GetNumRows(); ++i)
        if (ChrSpecializationEntry const* chrSpec = sChrSpecializationStore.LookupEntry(i))
            sChrSpecializationByIndexStore[chrSpec->ClassID][chrSpec->OrderIndex] = chrSpec;

    ASSERT(MAX_DIFFICULTY >= sDifficultyStore.GetNumRows(),
        "MAX_DIFFICULTY is not large enough to contain all difficulties! (current value %d, required %d)",
        MAX_DIFFICULTY, sDifficultyStore.GetNumRows());

    for (uint32 i = 0; i < sEmotesTextSoundStore.GetNumRows(); ++i)
        if (EmotesTextSoundEntry const* entry = sEmotesTextSoundStore.LookupEntry(i))
            sEmotesTextSoundMap[EmotesTextSoundKey(entry->EmotesTextID, entry->RaceID, entry->SexID)] = entry;

    for (uint32 i = 0; i < sFactionStore.GetNumRows(); ++i)
    {
        FactionEntry const* faction = sFactionStore.LookupEntry(i);
        if (faction && faction->ParentFactionID)
        {
            std::vector<uint32> &flist = sFactionTeamMap[faction->ParentFactionID];
            flist.push_back(i);
        }
    }

    for (uint32 i = 0; i < sGameObjectDisplayInfoStore.GetNumRows(); ++i)
    {
        if (GameObjectDisplayInfoEntry const* info = sGameObjectDisplayInfoStore.LookupEntry(i))
        {
            if (info->GeoBoxMax.X < info->GeoBoxMin.X)
                std::swap(*(float*)(&info->GeoBoxMax.X), *(float*)(&info->GeoBoxMin.X));
            if (info->GeoBoxMax.Y < info->GeoBoxMin.Y)
                std::swap(*(float*)(&info->GeoBoxMax.Y), *(float*)(&info->GeoBoxMin.Y));
            if (info->GeoBoxMax.Z < info->GeoBoxMin.Z)
                std::swap(*(float*)(&info->GeoBoxMax.Z), *(float*)(&info->GeoBoxMin.Z));
        }
    }

//    for (ItemSetSpellEntry const* entry : sItemSetSpellStore)
//        sItemSetSpellsStore[entry->ItemSetID].push_back(entry);

    // fill data
    for (uint32 i = 0; i < sMapDifficultyStore.GetNumRows(); ++i)
        if (MapDifficultyEntry const* entry = sMapDifficultyStore.LookupEntry(i))
            sMapDifficultyMap[entry->MapID][entry->DifficultyID] = entry;
    sMapDifficultyMap[0][0] = sMapDifficultyMap[1][0]; //map 0 is missing from MapDifficulty.dbc use this till its ported to sql

    for (uint32 i = 0; i < sPvpDifficultyStore.GetNumRows(); ++i)
        if (PvPDifficultyEntry const* entry = sPvpDifficultyStore.LookupEntry(i))
            if (entry->BracketID > MAX_BATTLEGROUND_BRACKETS)
                ASSERT(false && "Need update MAX_BATTLEGROUND_BRACKETS by DBC data");

    for (uint32 i = 0; i < sSkillRaceClassInfoStore.GetNumRows(); ++i)
        if (SkillRaceClassInfoEntry const* entry = sSkillRaceClassInfoStore.LookupEntry(i))
            if (sSkillLineStore.LookupEntry(entry->SkillID))
                SkillRaceClassInfoBySkill.emplace(entry->SkillID, entry);

    for (uint32 j = 0; j < sSpellEffectScalingStore.GetNumRows(); j++)
    {
        SpellEffectScalingEntry const* spellEffectScaling = sSpellEffectScalingStore.LookupEntry(j);
        if (!spellEffectScaling)
            continue;

        sSpellEffectScalingByEffectId.insert(std::make_pair(spellEffectScaling->SpellEffectID, j));
    }

    for (uint32 i = 0; i < sTalentStore.GetNumRows(); ++i)
    {
        if (TalentEntry const* talentInfo = sTalentStore.LookupEntry(i))
        {
            if (talentInfo->ClassID < MAX_CLASSES && talentInfo->TierID < MAX_TALENT_TIERS && talentInfo->ColumnIndex < MAX_TALENT_COLUMNS)
                sTalentByPos[talentInfo->ClassID][talentInfo->TierID][talentInfo->ColumnIndex].push_back(talentInfo);
            else
                TC_LOG_ERROR("server.loading", "Value of class (found: %u, max allowed %u) or (found: %u, max allowed %u) tier or column (found: %u, max allowed %u) is invalid.",
                    talentInfo->ClassID, MAX_CLASSES, talentInfo->TierID, MAX_TALENT_TIERS, talentInfo->ColumnIndex, MAX_TALENT_COLUMNS);
        }
    }

    for (uint32 i = 0; i < sWMOAreaTableStore.GetNumRows(); ++i)
        if (WMOAreaTableEntry const* entry = sWMOAreaTableStore.LookupEntry(i))
            sWMOAreaInfoByTripple.insert(WMOAreaInfoByTripple::value_type(WMOAreaTableTripple(entry->WmoID, entry->NameSetID, entry->WmoGroupID), entry));

    // error checks
    if (bad_dbc_files.size() >= DBCFileCount)
    {
        TC_LOG_ERROR("misc", "Incorrect DataDir value in worldserver.conf or ALL required *.dbc files (%d) not found by path: %sdbc/%s/", DBCFileCount, dataPath.c_str(), localeNames[defaultLocale]);
        exit(1);
    }
    else if (!bad_dbc_files.empty())
    {
        std::string str;
        for (StoreProblemList::iterator i = bad_dbc_files.begin(); i != bad_dbc_files.end(); ++i)
            str += *i + "\n";

        TC_LOG_ERROR("misc", "Some required *.dbc files (%u from %d) not found or not compatible:\n%s", (uint32)bad_dbc_files.size(), DBCFileCount, str.c_str());
        exit(1);
    }

    // Check loaded DBC files proper version
    if (!sAreaTableStore.LookupEntry(7941)     ||     // last area added in 6.2.2 (20444)
        !sCharTitlesStore.LookupEntry(457)     ||     // last char title added in 6.2.2 (20444)
        !sGemPropertiesStore.LookupEntry(2544) ||     // last gem property added in 6.2.2 (20444)
        !sMapStore.LookupEntry(1497)           ||     // last map added in 6.2.2 (20444)
        !sSpellStore.LookupEntry(197204)       )      // last spell added in 6.2.2 (20444)
    {
        TC_LOG_ERROR("misc", "You have _outdated_ DBC files. Please extract correct versions from current using client.");
        exit(1);
    }

    TC_LOG_INFO("server.loading", ">> Initialized %d DBC data stores in %u ms", DBCFileCount, GetMSTimeDiffToNow(oldMSTime));
}

std::vector<uint32> const* GetFactionTeamList(uint32 faction)
{
    FactionTeamMap::const_iterator itr = sFactionTeamMap.find(faction);
    if (itr != sFactionTeamMap.end())
        return &itr->second;

    return NULL;
}

EmotesTextSoundEntry const* FindTextSoundEmoteFor(uint32 emote, uint32 race, uint32 gender)
{
    auto itr = sEmotesTextSoundMap.find(EmotesTextSoundKey(emote, race, gender));
    return itr != sEmotesTextSoundMap.end() ? itr->second : nullptr;
}

uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId)
{
    if (mapid != 530 && mapid != 571 && mapid != 732)   // speed for most cases
        return mapid;

    if (WorldMapAreaEntry const* wma = sWorldMapAreaStore.LookupEntry(zoneId))
        return wma->DisplayMapID >= 0 ? wma->DisplayMapID : wma->MapID;

    return mapid;
}

uint32 GetExpansionForLevel(uint32 level)
{
    if (level < 60)
        return EXPANSION_CLASSIC;
    else if (level < 70)
        return EXPANSION_THE_BURNING_CRUSADE;
    else if (level < 80)
        return EXPANSION_WRATH_OF_THE_LICH_KING;
    else if (level < 85)
        return EXPANSION_CATACLYSM;
    else if (level < 90)
        return EXPANSION_MISTS_OF_PANDARIA;
    else
        return CURRENT_EXPANSION;
}

bool IsTotemCategoryCompatibleWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId)
{
    if (requiredTotemCategoryId == 0)
        return true;
    if (itemTotemCategoryId == 0)
        return false;

    TotemCategoryEntry const* itemEntry = sTotemCategoryStore.LookupEntry(itemTotemCategoryId);
    if (!itemEntry)
        return false;
    TotemCategoryEntry const* reqEntry = sTotemCategoryStore.LookupEntry(requiredTotemCategoryId);
    if (!reqEntry)
        return false;

    if (itemEntry->TotemCategoryType != reqEntry->TotemCategoryType)
        return false;

    return (itemEntry->TotemCategoryMask & reqEntry->TotemCategoryMask) == reqEntry->TotemCategoryMask;
}

void Map2ZoneCoordinates(float& x, float& y, uint32 worldMapAreaId)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(worldMapAreaId);

    // if not listed then map coordinates (instance)
    if (!maEntry)
        return;

    x = (x - maEntry->LocTop) / ((maEntry->LocBottom - maEntry->LocTop) / 100);
    y = (y - maEntry->LocLeft) / ((maEntry->LocRight - maEntry->LocLeft) / 100);    // client y coord from top to down
    std::swap(x, y);                                         // client have map coords swapped
}

MapDifficultyEntry const* GetDefaultMapDifficulty(uint32 mapId, Difficulty* difficulty /*= nullptr*/)
{
    auto itr = sMapDifficultyMap.find(mapId);
    if (itr == sMapDifficultyMap.end())
        return nullptr;

    if (itr->second.empty())
        return nullptr;

    for (auto& p : itr->second)
    {
        DifficultyEntry const* difficultyEntry = sDifficultyStore.LookupEntry(p.first);
        if (!difficultyEntry)
            continue;

        if (difficultyEntry->Flags & DIFFICULTY_FLAG_DEFAULT)
        {
            if (difficulty)
                *difficulty = Difficulty(p.first);

            return p.second;
        }
    }

    if (difficulty)
        *difficulty = Difficulty(itr->second.begin()->first);

    return itr->second.begin()->second;
}

MapDifficultyEntry const* GetMapDifficultyData(uint32 mapId, Difficulty difficulty)
{
    auto itr = sMapDifficultyMap.find(mapId);
    if (itr == sMapDifficultyMap.end())
        return nullptr;

    auto diffItr = itr->second.find(difficulty);
    if (diffItr == itr->second.end())
        return nullptr;

    return diffItr->second;
}

MapDifficultyEntry const* GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &difficulty)
{
    DifficultyEntry const* diffEntry = sDifficultyStore.LookupEntry(difficulty);
    if (!diffEntry)
        return GetDefaultMapDifficulty(mapId, &difficulty);

    uint32 tmpDiff = difficulty;
    MapDifficultyEntry const* mapDiff = GetMapDifficultyData(mapId, Difficulty(tmpDiff));
    while (!mapDiff)
    {
        tmpDiff = diffEntry->FallbackDifficultyID;
        diffEntry = sDifficultyStore.LookupEntry(tmpDiff);
        if (!diffEntry)
            return GetDefaultMapDifficulty(mapId, &difficulty);

        // pull new data
        mapDiff = GetMapDifficultyData(mapId, Difficulty(tmpDiff)); // we are 10 normal or 25 normal
    }

    difficulty = Difficulty(tmpDiff);
    return mapDiff;
}

PvPDifficultyEntry const* GetBattlegroundBracketByLevel(uint32 mapid, uint32 level)
{
    PvPDifficultyEntry const* maxEntry = NULL;              // used for level > max listed level case
    for (uint32 i = 0; i < sPvpDifficultyStore.GetNumRows(); ++i)
    {
        if (PvPDifficultyEntry const* entry = sPvpDifficultyStore.LookupEntry(i))
        {
            // skip unrelated and too-high brackets
            if (entry->MapID != mapid || entry->MinLevel > level)
                continue;

            // exactly fit
            if (entry->MaxLevel >= level)
                return entry;

            // remember for possible out-of-range case (search higher from existed)
            if (!maxEntry || maxEntry->MaxLevel < entry->MaxLevel)
                maxEntry = entry;
        }
    }

    return maxEntry;
}

PvPDifficultyEntry const* GetBattlegroundBracketById(uint32 mapid, BattlegroundBracketId id)
{
    for (uint32 i = 0; i < sPvpDifficultyStore.GetNumRows(); ++i)
        if (PvPDifficultyEntry const* entry = sPvpDifficultyStore.LookupEntry(i))
            if (entry->MapID == mapid && entry->GetBracketId() == id)
                return entry;

    return NULL;
}

CharSectionsEntry const* GetCharSectionEntry(uint8 race, CharSectionType genType, uint8 gender, uint8 type, uint8 color)
{
    std::pair<CharSectionsMap::const_iterator, CharSectionsMap::const_iterator> eqr = sCharSectionMap.equal_range(uint32(genType) | uint32(gender << 8) | uint32(race << 16));
    for (CharSectionsMap::const_iterator itr = eqr.first; itr != eqr.second; ++itr)
    {
        if (itr->second->Type == type && itr->second->Color == color)
            return itr->second;
    }

    return NULL;
}

SkillRaceClassInfoEntry const* GetSkillRaceClassInfo(uint32 skill, uint8 race, uint8 class_)
{
    for (auto&& [_, skllRaceClassInfo] : Trinity::Containers::MapEqualRange(SkillRaceClassInfoBySkill, skill))
    {
        if (!skllRaceClassInfo->RaceMask.IsEmpty() && !(skllRaceClassInfo->RaceMask.HasRace(race)))
            continue;
        if (skllRaceClassInfo->ClassMask && !(skllRaceClassInfo->ClassMask & (1 << (class_ - 1))))
            continue;

        return skllRaceClassInfo;
    }

    return nullptr;
}

void DeterminaAlternateMapPosition(uint32 mapId, float x, float y, float z, uint32* newMapId /*= nullptr*/, DBCPosition2D* newPos /*= nullptr*/)
{
    ASSERT(newMapId || newPos);
    WorldMapTransformsEntry const* transformation = nullptr;
    for (WorldMapTransformsEntry const* transform : sWorldMapTransformsStore)
    {
        if (transform->MapID != mapId)
            continue;

        if (transform->RegionMin.X > x || transform->RegionMax.X < x)
            continue;
        if (transform->RegionMin.Y > y || transform->RegionMax.Y < y)
            continue;
        if (transform->RegionMin.Z > z || transform->RegionMax.Z < z)
            continue;

        transformation = transform;
        break;
    }

    if (!transformation)
    {
        if (newMapId)
            *newMapId = mapId;

        if (newPos)
        {
            newPos->X = x;
            newPos->Y = y;
        }
        return;
    }

    if (newMapId)
        *newMapId = transformation->NewMapID;

    if (!newPos)
        return;

    if (transformation->RegionScale > 0.0f && transformation->RegionScale < 1.0f)
    {
        x = (x - transformation->RegionMin.X) * transformation->RegionScale + transformation->RegionMin.X;
        y = (y - transformation->RegionMin.Y) * transformation->RegionScale + transformation->RegionMin.Y;
    }

    newPos->X = x + transformation->RegionOffset.X;
    newPos->Y = y + transformation->RegionOffset.Y;
}
