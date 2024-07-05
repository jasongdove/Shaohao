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

#include "Containers.h"
#include "DBCStores.h"
#include "Log.h"
#include "MapUtils.h"
#include "Regex.h"
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
DBCStorage<FactionTemplateEntry>               sFactionTemplateStore(FactionTemplatefmt);
DBCStorage<FriendshipRepReactionEntry>         sFriendshipRepReactionStore("");
DBCStorage<FriendshipReputationEntry>          sFriendshipReputationStore("");
DBCStorage<GameObjectArtKitEntry>              sGameObjectArtKitStore("");
DBCStorage<GameObjectDisplayInfoEntry>         sGameObjectDisplayInfoStore(GameObjectDisplayInfofmt);
DBCStorage<GameTablesEntry>                    sGameTablesStore(GameTablesfmt);
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
DBCStorage<MapDifficultyEntry>                 sMapDifficultyStore(MapDifficultyfmt);
//MapDifficultyMap                               sMapDifficultyMap;
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
//SpellEffectScalingByEffectId                   sSpellEffectScalingByEffectId;
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
//TalentsByPosition                              sTalentByPos;
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

typedef ChrSpecializationEntry const* ChrSpecializationByIndexContainer[MAX_CLASSES + 1][MAX_SPECIALIZATIONS];
typedef std::unordered_map<uint32, std::vector<uint32>> FactionTeamContainer;
typedef std::array<ItemClassEntry const*, 20> ItemClassByOldEnumContainer;
typedef std::unordered_map<uint32, std::vector<ItemSpecOverrideEntry const*>> ItemSpecOverridesContainer;
typedef std::unordered_map<uint32, std::unordered_map<uint32, MapDifficultyEntry const*>> MapDifficultyContainer;
typedef std::unordered_map<uint32, std::array<std::vector<NameGenEntry const*>, 2>> NameGenContainer;
typedef std::array<std::vector<Trinity::wregex>, TOTAL_LOCALES + 1> NameValidationRegexContainer;
typedef std::unordered_map<uint32, std::vector<uint32>> PhaseGroupContainer;
typedef std::unordered_multimap<uint32, SkillRaceClassInfoEntry const*> SkillRaceClassInfoContainer;
typedef std::unordered_map<uint32, std::vector<SpecializationSpellsEntry const*>> SpecializationSpellsContainer;
typedef std::unordered_map<uint32, std::vector<SpellProcsPerMinuteModEntry const*>> SpellProcsPerMinuteModContainer;
typedef std::vector<TalentEntry const*> TalentsByPosition[MAX_CLASSES][MAX_TALENT_TIERS][MAX_TALENT_COLUMNS];
typedef std::tuple<uint16, uint8, int32> WMOAreaTableKey;
typedef std::map<WMOAreaTableKey, WMOAreaTableEntry const*> WMOAreaTableLookupContainer;

namespace
{
    struct UiMapBounds
    {
        // these coords are mixed when calculated and used... its a mess
        float Bounds[4];
        bool IsUiAssignment;
        bool IsUiLink;
    };

    ChrSpecializationByIndexContainer _chrSpecializationsByIndex;
    FactionTeamContainer _factionTeams;
    std::unordered_map<uint32, std::set<FriendshipRepReactionEntry const*, DBCManager::FriendshipRepReactionEntryComparator>> _friendshipRepReactions;
    ItemClassByOldEnumContainer _itemClassByOldEnum;
    ItemSpecOverridesContainer _itemSpecOverrides;
    std::vector<JournalTierEntry const*> _journalTiersByIndex;
    MapDifficultyContainer _mapDifficulties;
    NameGenContainer _nameGenData;
    NameValidationRegexContainer _nameValidators;
    PhaseGroupContainer _phasesByGroup;
    std::unordered_map<uint32, std::vector<SkillLineEntry const*>> _skillLinesByParentSkillLine;
    SkillRaceClassInfoContainer _skillRaceClassInfoBySkill;
    SpecializationSpellsContainer _specializationSpellsBySpec;
    std::unordered_set<uint8> _spellFamilyNames;
    SpellProcsPerMinuteModContainer _spellProcsPerMinuteMods;
    TalentsByPosition _talentsByPosition;
    std::unordered_map<std::pair<uint32, uint32>, TaxiPathEntry const*> _taxiPaths;
    std::unordered_multimap<int32, UiMapAssignmentEntry const*> _uiMapAssignmentByMap[MAX_UI_MAP_SYSTEM];
    std::unordered_multimap<int32, UiMapAssignmentEntry const*> _uiMapAssignmentByArea[MAX_UI_MAP_SYSTEM];
    std::unordered_multimap<int32, UiMapAssignmentEntry const*> _uiMapAssignmentByWmoDoodadPlacement[MAX_UI_MAP_SYSTEM];
    std::unordered_multimap<int32, UiMapAssignmentEntry const*> _uiMapAssignmentByWmoGroup[MAX_UI_MAP_SYSTEM];
    std::unordered_map<int32, UiMapBounds> _uiMapBounds;
    WMOAreaTableLookupContainer _wmoAreaTableLookup;
}

uint32 DBCFileCount = 0;

template<class T>
inline void LoadDBC(uint32& availableDbcLocales, StoreProblemList& errors, DBCStorage<T>& storage, std::string const& dbcPath, std::string const& filename, uint32 defaultLocale, uint32 extraData = 0, std::string const* customFormat = NULL, std::string const* customIndexName = NULL)
{
    // compatibility format and C++ structure sizes
    ASSERT(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) - extraData,
        "Size of '%s' set by format string (%u) not equal size of C++ structure (%u - %u = %u).",
        filename.c_str(), DBCFileLoader::GetFormatRecordSize(storage.GetFormat()), uint32(sizeof(T)), extraData, uint32(sizeof(T) - extraData));

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

            if (tableName == gt->Name)
            {
                found = true;
                storage.SetGameTableEntry(gt);
                break;
            }
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

DBCManager& DBCManager::Instance()
{
    static DBCManager instance;
    return instance;
}

uint32 DBCManager::LoadStores(const std::string& dataPath, uint32 defaultLocale)
{
    uint32 oldMSTime = getMSTime();

    std::string dbcPath = dataPath + "dbc/";

    StoreProblemList bad_dbc_files;
    uint32 availableDbcLocales = 0xFFFFFFFF;

// Shaohao: it seems better to make the MOP data look like TC master data than to change
// tons of code outside of DB2/DBC that will complicate future merges
// Specifically, LOAD_DBC_EXTRA adjusts the validation for members that have been added to
// match TC master and are not part of the MOP DBC files
#define LOAD_DBC_EXTRA(store, file, extraData) LoadDBC(availableDbcLocales, bad_dbc_files, store, dbcPath, file, defaultLocale, extraData)

#define LOAD_DBC(store, file) LoadDBC(availableDbcLocales, bad_dbc_files, store, dbcPath, file, defaultLocale)

//    LOAD_DBC(sAchievementStore, "Achievement.dbc");
//    // TODO: DATA LOAD_DBC(sAchievementCategoryStore, "Achievement_Category.dbc");
//    // TODO: DATA LOAD_DBC(sAchievementCriteriaStore, "Achievement_Criteria.dbc");
//    // TODO: DATA LOAD_DBC(sAnimationDataStore, "AnimationData.dbc");
//    LOAD_DBC(sAnimKitStore, "AnimKit.dbc");//20444
LOAD_DBC_EXTRA(sAreaTableStore, "AreaTable.dbc", AreaTableEntry::ExtraData);//20444
//    LOAD_DBC(sAreaTriggerStore, "AreaTrigger.dbc");//20444
//    // TODO: DATA LOAD_DBC(sAreaTriggerActionSetStore, "AreaTriggerActionSet.dbc");
//    // TODO: DATA LOAD_DBC(sAreaTriggerSphereStore, "AreaTriggerSphere.dbc");
//    LOAD_DBC(sArmorLocationStore, "ArmorLocation.dbc");//20444
LOAD_DBC(sAuctionHouseStore, "AuctionHouse.dbc");
//    LOAD_DBC(sBankBagSlotPricesStore, "BankBagSlotPrices.dbc");//20444
//    LOAD_DBC(sBannedAddOnsStore, "BannedAddOns.dbc");//20444
//    LOAD_DBC(sBarberShopStyleStore, "BarberShopStyle.dbc");
//    LOAD_DBC(sBattlemasterListStore, "BattlemasterList.dbc");//20444
//    // TODO: DATA LOAD_DBC(sCfgCategoriesStore, "Cfg_Categories.dbc");
//    // TODO: DATA LOAD_DBC(sCfgRegionsStore, "Cfg_Regions.dbc");
//    // TODO: DATA LOAD_DBC(sCharacterLoadoutStore, "CharacterLoadout.dbc");
//    // TODO: DATA LOAD_DBC(sCharacterLoadoutItemStore, "CharacterLoadoutItem.dbc");
//    // TODO: DATA LOAD_DBC(sCharBaseInfoStore, "CharBaseInfo.dbc");
//    LOAD_DBC(sCharSectionsStore, "CharSections.dbc");//20444
//    // sCharSectionMap
//    LOAD_DBC(sCharStartOutfitStore, "CharStartOutfit.dbc");
//    // sCharStartOutfitMap
//    LOAD_DBC(sCharTitlesStore, "CharTitles.dbc");//20444
//    LOAD_DBC(sChatChannelsStore, "ChatChannels.dbc");//20444
//    LOAD_DBC(sChrClassesStore, "ChrClasses.dbc");//20444
//    // TODO: DATA LOAD_DBC(sChrClassesXPowerTypesStore, "ChrClassesXPowerTypes.dbc");
//    LOAD_DBC(sChrRacesStore, "ChrRaces.dbc");//20444
//    LOAD_DBC(sChrSpecializationStore, "ChrSpecialization.dbc");//20444
//    // sChrSpecializationByIndexStore
//    LOAD_DBC(sCinematicCameraStore, "CinematicCamera.dbc");
//    LOAD_DBC(sCinematicSequencesStore, "CinematicSequences.dbc");
LOAD_DBC(sCreatureDisplayInfoStore, "CreatureDisplayInfo.dbc");
//    LOAD_DBC(sCreatureDisplayInfoExtraStore, "CreatureDisplayInfoExtra.dbc");//20444
LOAD_DBC(sCreatureFamilyStore, "CreatureFamily.dbc");//20444
//    LOAD_DBC(sCreatureImmunitiesStore, "CreatureImmunities.dbc");
//    LOAD_DBC(sCreatureModelDataStore, "CreatureModelData.dbc");//20444
//    LOAD_DBC(sCreatureSpellDataStore, "CreatureSpellData.dbc");
LOAD_DBC(sCreatureTypeStore, "CreatureType.dbc");
//    LOAD_DBC(sCriteriaStore, "Criteria.dbc");
//    LOAD_DBC(sCriteriaTreeStore, "CriteriaTree.dbc");
//    LOAD_DBC(sCurrencyTypesStore, "CurrencyTypes.dbc");
//    LOAD_DBC(sDestructibleModelDataStore, "DestructibleModelData.dbc");
LOAD_DBC(sDifficultyStore, "Difficulty.dbc");//20444
//    LOAD_DBC(sDungeonEncounterStore, "DungeonEncounter.dbc");//20444
//    LOAD_DBC(sDurabilityCostsStore, "DurabilityCosts.dbc");//20444
//    LOAD_DBC(sDurabilityQualityStore, "DurabilityQuality.dbc");
//    LOAD_DBC(sEmotesStore, "Emotes.dbc");//20444
//    LOAD_DBC(sEmotesTextStore, "EmotesText.dbc");//20444
//    //
//    //
//    LOAD_DBC(sEmotesTextSoundStore, "EmotesTextSound.dbc");
LOAD_DBC(sFactionStore, "Faction.dbc");//20444
LOAD_DBC(sFactionTemplateStore, "FactionTemplate.dbc");//20444
//    // TODO: DATA LOAD_DBC(sFriendshipRepReactionStore, "FriendshipRepReaction.dbc");
//    // TODO: DATA LOAD_DBC(sFriendshipReputationStore, "FriendshipReputation.dbc");
//    // TODO: DATA LOAD_DBC(sGameObjectArtKitStore, "GameObjectArtKit.dbc");
//    LOAD_DBC(sGameObjectDisplayInfoStore, "GameObjectDisplayInfo.dbc");//20444
LOAD_DBC(sGameTablesStore, "GameTables.dbc");
//    LOAD_DBC(sGemPropertiesStore, "GemProperties.dbc");//20444
//    LOAD_DBC(sGlyphPropertiesStore, "GlyphProperties.dbc");//20444
//    // TODO: DATA LOAD_DBC(sGuildColorBackgroundStore, "GuildColorBackground.dbc");//20444
//    // TODO: DATA LOAD_DBC(sGuildColorBorderStore, "GuildColorBorder.dbc"); //20444
//    // TODO: DATA LOAD_DBC(sGuildColorEmblemStore, "GuildColorEmblem.dbc");//20444
//    // TODO: DATA LOAD_DBC(sGuildPerkSpellsStore, "GuildPerkSpells.dbc");//20444
//    LOAD_DBC(sHolidaysStore, "Holidays.dbc");
//    LOAD_DBC(sImportPriceArmorStore, "ImportPriceArmor.dbc");
//    LOAD_DBC(sImportPriceQualityStore, "ImportPriceQuality.dbc");
//    LOAD_DBC(sImportPriceShieldStore, "ImportPriceShield.dbc");
//    LOAD_DBC(sImportPriceWeaponStore, "ImportPriceWeapon.dbc");
//    LOAD_DBC(sItemArmorQualityStore, "ItemArmorQuality.dbc");//20444
//    LOAD_DBC(sItemArmorShieldStore, "ItemArmorShield.dbc");//20444
//    LOAD_DBC(sItemArmorTotalStore, "ItemArmorTotal.dbc");//20444
//    LOAD_DBC(sItemBagFamilyStore, "ItemBagFamily.dbc");//20444
//    LOAD_DBC(sItemClassStore, "ItemClass.dbc");
//    // TODO: DATA LOAD_DBC(sItemDamageAmmoStore, "ItemDamageAmmo.dbc");//20444
//    LOAD_DBC(sItemDamageOneHandStore, "ItemDamageOneHand.dbc");//20444
//    LOAD_DBC(sItemDamageOneHandCasterStore, "ItemDamageOneHandCaster.dbc");//20444
//    // TODO: DATA LOAD_DBC(sItemDamageRangedStore, "ItemDamageRanged.dbc");//20444
//    // TODO: DATA LOAD_DBC(sItemDamageThrownStore, "ItemDamageThrown.dbc");//20444
//    LOAD_DBC(sItemDamageTwoHandStore, "ItemDamageTwoHand.dbc");//20444
//    LOAD_DBC(sItemDamageTwoHandCasterStore, "ItemDamageTwoHandCaster.dbc");//20444
//    // TODO: DATA LOAD_DBC(sItemDamageWandStore, "ItemDamageWand.dbc");//20444
//    LOAD_DBC(sItemDisenchantLootStore, "ItemDisenchantLoot.dbc");
//    LOAD_DBC(sItemDisplayInfoStore, "ItemDisplayInfo.dbc");//20444
//    LOAD_DBC(sItemLimitCategoryStore, "ItemLimitCategory.dbc");
//    LOAD_DBC(sItemNameDescriptionStore, "ItemNameDescription.dbc");
//    // TODO: DATA LOAD_DBC(sItemPriceBaseStore, "ItemPriceBase.dbc");
//    LOAD_DBC(sItemRandomPropertiesStore, "ItemRandomProperties.dbc");
//    LOAD_DBC(sItemRandomSuffixStore, "ItemRandomSuffix.dbc");
//    LOAD_DBC(sItemSetStore, "ItemSet.dbc");//20444
//    LOAD_DBC(sItemSpecStore, "ItemSpec.dbc");
//    LOAD_DBC(sItemSpecOverrideStore, "ItemSpecOverride.dbc");
//    //
//    // TODO: DATA LOAD_DBC(sJournalEncounterStore, "JournalEncounter.dbc");
//    // TODO: DATA LOAD_DBC(sJournalEncounterSectionStore, "JournalEncounterSection.dbc");
//    // TODO: DATA LOAD_DBC(sJournalInstanceStore, "Journalnstance.dbc");
//    // TODO: DATA LOAD_DBC(sJournalTierStore, "JournalTier.dbc");
//    // TODO: DATA LOAD_DBC(sLanguagesStore, "Languages.dbc");
//    // TODO: DATA LOAD_DBC(sLanguageWordsStore, "LanguageWords.dbc");
//    LOAD_DBC(sLFGDungeonsStore, "LfgDungeons.dbc");//20444
//    LOAD_DBC(sLightStore, "Light.dbc"); //20444
//    LOAD_DBC(sLiquidTypeStore, "LiquidType.dbc");//20444
//    LOAD_DBC(sLockStore, "Lock.dbc");//20444
//    LOAD_DBC(sMailTemplateStore, "MailTemplate.dbc");
LOAD_DBC_EXTRA(sMapStore, "Map.dbc", MapEntry::ExtraData);//20444
//    LOAD_DBC(sMapDifficultyStore, "MapDifficulty.dbc");//20444
//    // sMapDifficultyMap
//    LOAD_DBC(sModifierTreeStore, "ModifierTree.dbc");
//    LOAD_DBC(sMountCapabilityStore, "MountCapability.dbc");
//    LOAD_DBC(sMountTypeStore, "MountType.dbc");
//    LOAD_DBC(sMovieStore, "Movie.dbc");//20444
//    LOAD_DBC(sNameGenStore, "NameGen.dbc");
//    // TODO: DATA LOAD_DBC(sNamesProfanityStore, "NamesProfanity.dbc");
//    // TODO: DATA LOAD_DBC(sNamesReservedStore, "NamesReserved.dbc");
//    // TODO: DATA LOAD_DBC(sNamesReservedLocaleStore, "NamesReservedLocaleStore.dbc");
//    LOAD_DBC(sOverrideSpellDataStore, "OverrideSpellData.dbc");
//    LOAD_DBC(sPhaseStore, "Phase.dbc"); // 20444
//    // TODO: DATA LOAD_DBC(sPhaseXPhaseGroupStore, "PhaseXPhaseGroup.dbc");
//    LOAD_DBC(sPlayerConditionStore, "PlayerCondition.dbc");
//    LOAD_DBC(sPowerDisplayStore, "PowerDisplay.dbc");//20444
//    LOAD_DBC(sPvpDifficultyStore, "PvpDifficulty.dbc");//20444
//    LOAD_DBC(sQuestFactionRewardStore, "QuestFactionReward.dbc");//20444
//    // TODO: DATA LOAD_DBC(sQuestInfoStore, "QuestInfo.dbc");
//    // TODO: DATA LOAD_DBC(sQuestMoneyRewardStore, "QuestMoneyReward.dbc");
//    //
//    LOAD_DBC(sQuestSortStore, "QuestSort.dbc");
//    LOAD_DBC(sQuestV2Store, "QuestV2.dbc");
//    LOAD_DBC(sQuestXPStore, "QuestXP.dbc");
//    LOAD_DBC(sRandPropPointsStore, "RandPropPoints.dbc");
//    LOAD_DBC(sResearchBranchStore, "ResearchBranch.dbc");
//    LOAD_DBC(sResearchProjectStore, "ResearchProject.dbc");
//    LOAD_DBC(sResearchSiteStore, "ResearchSite.dbc");
//    //
//    LOAD_DBC(sScenarioStore, "Scenario.dbc");
//    LOAD_DBC(sScenarioStepStore, "ScenarioStep.dbc");
//    // TODO: DATA LOAD_DBC(sServerMessagesStore, "ServerMessages.dbc");
//    LOAD_DBC(sSkillLineStore, "SkillLine.dbc");//20444
//    LOAD_DBC(sSkillLineAbilityStore, "SkillLineAbility.dbc");//20444
//    LOAD_DBC(sSkillRaceClassInfoStore, "SkillRaceClassInfo.dbc");//20444
//    //
//    //
//    LOAD_DBC(sSoundEntriesStore, "SoundEntries.dbc");
//    LOAD_DBC(sSpecializationSpellsStore, "SpecializationSpells.dbc");
LOAD_DBC(sSpellStore, "Spell.dbc"/*, &CustomSpellfmt, &CustomSpellEntryIndex*/);//20444
//    LOAD_DBC(sSpellAuraOptionsStore, "SpellAuraOptions.dbc");//20444
//    LOAD_DBC(sSpellAuraRestrictionsStore, "SpellAuraRestrictions.dbc");
//    LOAD_DBC(sSpellCastingRequirementsStore, "SpellCastingRequirements.dbc");
//    // TODO: DATA LOAD_DBC(sSpellCastTimesStore, "SpellCastTimes.dbc");
//    LOAD_DBC(sSpellCategoriesStore, "SpellCategories.dbc");//20444
//    LOAD_DBC(sSpellCategoryStore, "SpellCategory.dbc");//20444
//    // TODO: DATA LOAD_DBC(sSpellClassOptionsStore, "SpellClassOptions.dbc");
//    LOAD_DBC(sSpellCooldownsStore, "SpellCooldowns.dbc");//20444
//    LOAD_DBC(sSpellDurationStore, "SpellDuration.dbc");
LOAD_DBC_EXTRA(sSpellEffectStore, "SpellEffect.dbc", SpellEffectEntry::ExtraData/*, &CustomSpellEffectfmt, &CustomSpellEffectEntryIndex*/);//20444
//    LOAD_DBC(sSpellEffectScalingStore, "SpellEffectScaling.dbc");//20444
//    //
//    LOAD_DBC(sSpellEquippedItemsStore, "SpellEquippedItems.dbc");//20444
//    LOAD_DBC(sSpellFocusObjectStore, "SpellFocusObject.dbc");//20444
//    LOAD_DBC(sSpellInterruptsStore, "SpellInterrupts.dbc");//20444
//    LOAD_DBC(sSpellItemEnchantmentStore, "SpellItemEnchantment.dbc");//20444
//    // TODO: DATA LOAD_DBC(sSpellItemEnchantmentConditionStore, "SpellItemEnchantmentCondition.dbc");
//    // TODO: DATA LOAD_DBC(sSpellKeyboundOverrideStore, "SpellKeyboundOverride.dbc");
//    // TODO: DATA LOAD_DBC(sSpellLearnSpellStore, "SpellLearnSpell.dbc");
LOAD_DBC(sSpellLevelsStore, "SpellLevels.dbc");//20444
//    LOAD_DBC(sSpellMiscStore, "SpellMisc.dbc");
//    LOAD_DBC(sSpellPowerStore, "SpellPower.dbc");
//    LOAD_DBC(sSpellProcsPerMinuteStore, "SpellProcsPerMinute.dbc");
//    LOAD_DBC(sSpellProcsPerMinuteModStore, "SpellProcsPerMinuteModStore.dbc");
//    LOAD_DBC(sSpellRadiusStore, "SpellRadius.dbc");
//    LOAD_DBC(sSpellRangeStore, "SpellRange.dbc");
//    // TODO: DATA LOAD_DBC(sSpellReagentsStore, "SpellReagents.dbc");
//    LOAD_DBC(sSpellRuneCostStore, "SpellRuneCost.dbc");
//    LOAD_DBC(sSpellScalingStore, "SpellScaling.dbc");//20444
//    LOAD_DBC(sSpellShapeshiftStore, "SpellShapeshift.dbc");//20444
//    LOAD_DBC(sSpellShapeshiftFormStore, "SpellShapeshiftForm.dbc");//20444
//    LOAD_DBC(sSpellTargetRestrictionsStore, "SpellTargetRestrictions.dbc");//20444
//    LOAD_DBC(sSpellTotemsStore, "SpellTotems.dbc");
//    // TODO: DATA LOAD_DBC(sSpellVisualStore, "SpellVisual.dbc");
//    // TODO: DATA LOAD_DBC(sSpellVisualEffectNameStore, "SpellVisualEffectName.dbc");
//    // TODO: DATA LOAD_DBC(sSpellVisualKitStore, "SpellVisualKit.dbc");
//    LOAD_DBC(sSummonPropertiesStore, "SummonProperties.dbc");//20444
//    LOAD_DBC(sTalentStore, "Talent.dbc");//20444
//    //
//    LOAD_DBC(sTaxiNodesStore, "TaxiNodes.dbc");
//    LOAD_DBC(sTaxiPathStore, "TaxiPath.dbc");
//    LOAD_DBC(sTaxiPathNodeStore, "TaxiPathNode.dbc");
//    LOAD_DBC(sTotemCategoryStore, "TotemCategory.dbc");
//    LOAD_DBC(sTransportAnimationStore, "TransportAnimation.dbc");
//    LOAD_DBC(sTransportRotationStore, "TransportRotation.dbc");
//    // TODO: DATA LOAD_DBC(sUnitConditionStore, "UnitCondition.dbc");
//    // TODO: DATA LOAD_DBC(sUnitPowerBarStore, "UnitPowerBar.dbc");
//    LOAD_DBC(sVehicleStore, "Vehicle.dbc");//20444
//    LOAD_DBC(sVehicleSeatStore, "VehicleSeat.dbc");//20444
//    LOAD_DBC(sWMOAreaTableStore, "WMOAreaTable.dbc");//20444
//    //
//    // TODO: DATA LOAD_DBC(sWorldEffectStore, "WorldEffect.dbc");
//    LOAD_DBC(sWorldMapAreaStore, "WorldMapArea.dbc");//20444
//    LOAD_DBC(sWorldMapOverlayStore, "WorldMapOverlay.dbc");
//    LOAD_DBC(sWorldMapTransformsStore, "WorldMapTransforms.dbc");//20444
//    LOAD_DBC(sWorldSafeLocsStore, "WorldSafeLocs.dbc"); // 20444
//    // TODO: DATA LOAD_DBC(sWorldStateExpressionStore, "WorldStateExpression.dbc");

#undef LOAD_DBC
#undef LOAD_DBC_EXTRA

    for (uint32 i = 0; i < sCharSectionsStore.GetNumRows(); ++i)
        if (CharSectionsEntry const* entry = sCharSectionsStore.LookupEntry(i))
        {
            Trinity::RaceMask<uint64> raceMask = { entry->Race };
            if (entry->Race && !(raceMask & RACEMASK_ALL_PLAYABLE).IsEmpty()) //ignore Nonplayable races
                sCharSectionMap.insert({entry->GenType | (entry->Gender << 8) | (entry->Race << 16), entry});
        }

            ASSERT(MAX_DIFFICULTY >= sDifficultyStore.GetNumRows(),
                   "MAX_DIFFICULTY is not large enough to contain all difficulties! (current value %d, required %d)",
                   MAX_DIFFICULTY, sDifficultyStore.GetNumRows());

    for (uint32 i = 0; i < sEmotesTextSoundStore.GetNumRows(); ++i)
        if (EmotesTextSoundEntry const* entry = sEmotesTextSoundStore.LookupEntry(i))
            sEmotesTextSoundMap[EmotesTextSoundKey(entry->EmotesTextID, entry->RaceID, entry->SexID)] = entry;

    for (FactionEntry const* faction : sFactionStore)
        if (faction->ParentFactionID)
            _factionTeams[faction->ParentFactionID].push_back(faction->ID);

    for (FriendshipRepReactionEntry const* friendshipRepReaction : sFriendshipRepReactionStore)
        _friendshipRepReactions[friendshipRepReaction->FriendshipRepID].insert(friendshipRepReaction);

    for (GameObjectDisplayInfoEntry const* gameObjectDisplayInfo : sGameObjectDisplayInfoStore)
    {
        if (gameObjectDisplayInfo->GeoBoxMax.X < gameObjectDisplayInfo->GeoBoxMin.X)
            std::swap(const_cast<GameObjectDisplayInfoEntry*>(gameObjectDisplayInfo)->GeoBoxMax.X, const_cast<GameObjectDisplayInfoEntry*>(gameObjectDisplayInfo)->GeoBoxMin.X);
        if (gameObjectDisplayInfo->GeoBoxMax.Y < gameObjectDisplayInfo->GeoBoxMin.Y)
            std::swap(const_cast<GameObjectDisplayInfoEntry*>(gameObjectDisplayInfo)->GeoBoxMax.Y, const_cast<GameObjectDisplayInfoEntry*>(gameObjectDisplayInfo)->GeoBoxMin.Y);
        if (gameObjectDisplayInfo->GeoBoxMax.Z < gameObjectDisplayInfo->GeoBoxMin.Z)
            std::swap(const_cast<GameObjectDisplayInfoEntry*>(gameObjectDisplayInfo)->GeoBoxMax.Z, const_cast<GameObjectDisplayInfoEntry*>(gameObjectDisplayInfo)->GeoBoxMin.Z);
    }

    for (ItemClassEntry const* itemClass : sItemClassStore)
    {
        ASSERT(itemClass->ClassID < int32(_itemClassByOldEnum.size()));
        ASSERT(!_itemClassByOldEnum[itemClass->ClassID]);
        _itemClassByOldEnum[itemClass->ClassID] = itemClass;
    }

    for (BattlemasterListEntry const* battlemaster : sBattlemasterListStore)
    {
        if (battlemaster->MaxLevel < battlemaster->MinLevel)
        {
            TC_LOG_ERROR("db2.hotfix.battlemaster_list", "Battlemaster ({}) contains bad values for MinLevel ({}) and MaxLevel ({}). Swapping values.", battlemaster->ID, battlemaster->MinLevel, battlemaster->MaxLevel);
            std::swap(const_cast<BattlemasterListEntry*>(battlemaster)->MaxLevel, const_cast<BattlemasterListEntry*>(battlemaster)->MinLevel);
        }
        if (battlemaster->MaxPlayers < battlemaster->MinPlayers)
        {
            TC_LOG_ERROR("db2.hotfix.battlemaster_list", "Battlemaster ({}) contains bad values for MinPlayers ({}) and MaxPlayers ({}). Swapping values.", battlemaster->ID, battlemaster->MinPlayers, battlemaster->MaxPlayers);
            int8 minPlayers = battlemaster->MinPlayers;
            const_cast<BattlemasterListEntry*>(battlemaster)->MinPlayers = battlemaster->MaxPlayers;
            const_cast<BattlemasterListEntry*>(battlemaster)->MaxPlayers = minPlayers;
        }
    }

    std::unordered_map<uint32, uint32> parentRaces;
    for (ChrRacesEntry const* chrRace : sChrRacesStore)
        if (chrRace->UnalteredVisualRaceID)
            parentRaces[chrRace->UnalteredVisualRaceID] = chrRace->ID;

    // TODO: DATA i don't think MOP has this
//    for (ChrRaceXChrModelEntry const* raceModel : sChrRaceXChrModelStore)
//    {
//        if (ChrModelEntry const* model = sChrModelStore.LookupEntry(raceModel->ChrModelID))
//        {
//            _chrModelsByRaceAndGender[{ uint8(raceModel->ChrRacesID), uint8(raceModel->Sex) }] = model;
//
//            if (std::vector<ChrCustomizationOptionEntry const*> const* customizationOptionsForModel = Trinity::Containers::MapGetValuePtr(customizationOptionsByModel, model->ID))
//            {
//                std::vector<ChrCustomizationOptionEntry const*>& raceOptions = _chrCustomizationOptionsByRaceAndGender[{ uint8(raceModel->ChrRacesID), uint8(raceModel->Sex) }];
//                raceOptions.insert(raceOptions.end(), customizationOptionsForModel->begin(), customizationOptionsForModel->end());
//
//                if (uint32 const* parentRace = Trinity::Containers::MapGetValuePtr(parentRaces, raceModel->ChrRacesID))
//                {
//                    std::vector<ChrCustomizationOptionEntry const*>& parentRaceOptions = _chrCustomizationOptionsByRaceAndGender[{ uint8(*parentRace), uint8(raceModel->Sex) }];
//                    parentRaceOptions.insert(parentRaceOptions.end(), customizationOptionsForModel->begin(), customizationOptionsForModel->end());
//                }
//            }
//
//            // link shapeshift displays to race/gender/form
//            for (std::pair<uint32 const, std::pair<uint32, uint8>> const& shapeshiftOptionsForModel : Trinity::Containers::MapEqualRange(shapeshiftFormByModel, model->ID))
//            {
//                ShapeshiftFormModelData& data = _chrCustomizationChoicesForShapeshifts[{ uint8(raceModel->ChrRacesID), uint8(raceModel->Sex), shapeshiftOptionsForModel.second.second }];
//                data.OptionID = shapeshiftOptionsForModel.second.first;
//                data.Choices = Trinity::Containers::MapGetValuePtr(_chrCustomizationChoicesByOption, shapeshiftOptionsForModel.second.first);
//                if (data.Choices)
//                {
//                    data.Displays.resize(data.Choices->size());
//                    for (std::size_t i = 0; i < data.Choices->size(); ++i)
//                        data.Displays[i] = Trinity::Containers::MapGetValuePtr(displayInfoByCustomizationChoice, (*data.Choices)[i]->ID);
//                }
//            }
//        }
//    }

    memset(_chrSpecializationsByIndex, 0, sizeof(_chrSpecializationsByIndex));
    for (ChrSpecializationEntry const* chrSpec : sChrSpecializationStore)
    {
        ASSERT(chrSpec->ClassID < MAX_CLASSES);
        ASSERT(chrSpec->OrderIndex < MAX_SPECIALIZATIONS);

        uint32 storageIndex = chrSpec->ClassID;
        if (chrSpec->GetFlags().HasFlag(ChrSpecializationFlag::PetOverrideSpec))
        {
            ASSERT(!chrSpec->ClassID);
            storageIndex = PET_SPEC_OVERRIDE_CLASS_INDEX;
        }

        _chrSpecializationsByIndex[storageIndex][chrSpec->OrderIndex] = chrSpec;
    }

    // TODO: DATA fix this
//    {
//        std::set<ChrClassesXPowerTypesEntry const*, ChrClassesXPowerTypesEntryComparator> powers;
//        for (ChrClassesXPowerTypesEntry const* power : sChrClassesXPowerTypesStore)
//            powers.insert(power);
//
//        for (std::size_t i = 0; i < _powersByClass.size(); ++i)
//            _powersByClass[i].fill(MAX_POWERS);
//
//        for (ChrClassesXPowerTypesEntry const* power : powers)
//        {
//            uint32 index = 0;
//            for (uint32 j = 0; j < MAX_POWERS; ++j)
//                if (_powersByClass[power->ClassID][j] != MAX_POWERS)
//                    ++index;
//
//                    ASSERT(power->ClassID < MAX_CLASSES);
//                    ASSERT(power->PowerType < MAX_POWERS);
//            _powersByClass[power->ClassID][power->PowerType] = index;
//        }
//    }

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
//    for (uint32 i = 0; i < sMapDifficultyStore.GetNumRows(); ++i)
//        if (MapDifficultyEntry const* entry = sMapDifficultyStore.LookupEntry(i))
//            sMapDifficultyMap[entry->MapID][entry->DifficultyID] = entry;
//    sMapDifficultyMap[0][0] = sMapDifficultyMap[1][0]; //map 0 is missing from MapDifficulty.dbc use this till its ported to sql

    for (ItemSpecOverrideEntry const* itemSpecOverride : sItemSpecOverrideStore)
        _itemSpecOverrides[itemSpecOverride->ItemID].push_back(itemSpecOverride);

    for (JournalTierEntry const* journalTier : sJournalTierStore)
        _journalTiersByIndex.push_back(journalTier);

    for (NameGenEntry const* nameGen : sNameGenStore)
        _nameGenData[nameGen->RaceID][nameGen->Sex].push_back(nameGen);

    for (NamesProfanityEntry const* namesProfanity : sNamesProfanityStore)
    {
        ASSERT(namesProfanity->Language < TOTAL_LOCALES || namesProfanity->Language == -1);
        std::wstring name;
        bool conversionResult = Utf8toWStr(namesProfanity->Name, name);
        ASSERT(conversionResult);
        if (namesProfanity->Language != -1)
            _nameValidators[namesProfanity->Language].emplace_back(name, Trinity::regex::perl | Trinity::regex::icase | Trinity::regex::optimize);
        else
        {
            for (uint32 i = 0; i < TOTAL_LOCALES; ++i)
            {
                if (i == LOCALE_none)
                    continue;

                _nameValidators[i].emplace_back(name, Trinity::regex::perl | Trinity::regex::icase | Trinity::regex::optimize);
            }
        }
    }

    for (NamesReservedEntry const* namesReserved : sNamesReservedStore)
    {
        std::wstring name;
        bool conversionResult = Utf8toWStr(namesReserved->Name, name);
        ASSERT(conversionResult);
        _nameValidators[TOTAL_LOCALES].emplace_back(name, Trinity::regex::perl | Trinity::regex::icase | Trinity::regex::optimize);
    }

    for (NamesReservedLocaleEntry const* namesReserved : sNamesReservedLocaleStore)
    {
        ASSERT(!(namesReserved->LocaleMask & ~((1u << TOTAL_LOCALES) - 1)));
        std::wstring name;
        bool conversionResult = Utf8toWStr(namesReserved->Name, name);
        ASSERT(conversionResult);
        for (uint32 i = 0; i < TOTAL_LOCALES; ++i)
        {
            if (i == LOCALE_none)
                continue;

            if (namesReserved->LocaleMask & (1 << i))
                _nameValidators[i].emplace_back(name, Trinity::regex::perl | Trinity::regex::icase | Trinity::regex::optimize);
        }
    }

    for (PhaseXPhaseGroupEntry const* group : sPhaseXPhaseGroupStore)
        if (PhaseEntry const* phase = sPhaseStore.LookupEntry(group->PhaseID))
            _phasesByGroup[group->PhaseGroupID].push_back(phase->ID);

    for (uint32 i = 0; i < sPvpDifficultyStore.GetNumRows(); ++i)
        if (PvPDifficultyEntry const* entry = sPvpDifficultyStore.LookupEntry(i))
            if (entry->BracketID > MAX_BATTLEGROUND_BRACKETS)
                ASSERT(false && "Need update MAX_BATTLEGROUND_BRACKETS by DBC data");

    for (SkillLineEntry const* skill : sSkillLineStore)
        if (skill->ParentSkillLineID)
            _skillLinesByParentSkillLine[skill->ParentSkillLineID].push_back(skill);

    for (SkillRaceClassInfoEntry const* entry : sSkillRaceClassInfoStore)
        if (sSkillLineStore.LookupEntry(entry->SkillID))
            _skillRaceClassInfoBySkill.insert(SkillRaceClassInfoContainer::value_type(entry->SkillID, entry));

    for (SpecializationSpellsEntry const* specSpells : sSpecializationSpellsStore)
        _specializationSpellsBySpec[specSpells->SpecID].push_back(specSpells);

//    for (uint32 j = 0; j < sSpellEffectScalingStore.GetNumRows(); j++)
//    {
//        SpellEffectScalingEntry const* spellEffectScaling = sSpellEffectScalingStore.LookupEntry(j);
//        if (!spellEffectScaling)
//            continue;
//
//        sSpellEffectScalingByEffectId.insert(std::make_pair(spellEffectScaling->SpellEffectID, j));
//    }

//    for (uint32 i = 0; i < sTalentStore.GetNumRows(); ++i)
//    {
//        if (TalentEntry const* talentInfo = sTalentStore.LookupEntry(i))
//        {
//            if (talentInfo->ClassID < MAX_CLASSES && talentInfo->TierID < MAX_TALENT_TIERS && talentInfo->ColumnIndex < MAX_TALENT_COLUMNS)
//                sTalentByPos[talentInfo->ClassID][talentInfo->TierID][talentInfo->ColumnIndex].push_back(talentInfo);
//            else
//                TC_LOG_ERROR("server.loading", "Value of class (found: %u, max allowed %u) or (found: %u, max allowed %u) tier or column (found: %u, max allowed %u) is invalid.",
//                    talentInfo->ClassID, MAX_CLASSES, talentInfo->TierID, MAX_TALENT_TIERS, talentInfo->ColumnIndex, MAX_TALENT_COLUMNS);
//        }
//    }

    for (SpellClassOptionsEntry const* classOption : sSpellClassOptionsStore)
        _spellFamilyNames.insert(classOption->SpellClassSet);

    for (SpellProcsPerMinuteModEntry const* ppmMod : sSpellProcsPerMinuteModStore)
        _spellProcsPerMinuteMods[ppmMod->SpellProcsPerMinuteID].push_back(ppmMod);

    for (TalentEntry const* talentInfo : sTalentStore)
    {
        ASSERT(talentInfo->ClassID < MAX_CLASSES);
        ASSERT(talentInfo->TierID < MAX_TALENT_TIERS, "MAX_TALENT_TIERS must be at least %u", talentInfo->TierID + 1);
        ASSERT(talentInfo->ColumnIndex < MAX_TALENT_COLUMNS, "MAX_TALENT_COLUMNS must be at least %u", talentInfo->ColumnIndex + 1);

        _talentsByPosition[talentInfo->ClassID][talentInfo->TierID][talentInfo->ColumnIndex].push_back(talentInfo);
    }

    for (TaxiPathEntry const* entry : sTaxiPathStore)
        _taxiPaths[{ entry->FromTaxiNode, entry->ToTaxiNode }] = entry;

    uint32 pathCount = sTaxiPathStore.GetNumRows();

    // Calculate path nodes count
    std::vector<uint32> pathLength;
    pathLength.resize(pathCount);                           // 0 and some other indexes not used
    for (TaxiPathNodeEntry const* entry : sTaxiPathNodeStore)
        if (pathLength[entry->PathID] < entry->NodeIndex + 1u)
            pathLength[entry->PathID] = entry->NodeIndex + 1u;

    // Set path length
    sTaxiPathNodesByPath.resize(pathCount);                 // 0 and some other indexes not used
    for (uint32 i = 0; i < sTaxiPathNodesByPath.size(); ++i)
        sTaxiPathNodesByPath[i].resize(pathLength[i]);

    // fill data
    for (TaxiPathNodeEntry const* entry : sTaxiPathNodeStore)
        sTaxiPathNodesByPath[entry->PathID][entry->NodeIndex] = entry;

    for (uint32 i = 0; i < sWMOAreaTableStore.GetNumRows(); ++i)
        if (WMOAreaTableEntry const* entry = sWMOAreaTableStore.LookupEntry(i))
            sWMOAreaInfoByTripple.insert(WMOAreaInfoByTripple::value_type(WMOAreaTableTripple(entry->WmoID, entry->NameSetID, entry->WmoGroupID), entry));

    // error checks
    if (bad_dbc_files.size() >= DBCFileCount)
    {
        TC_LOG_ERROR("misc", "Incorrect DataDir value in worldserver.conf or ALL required *.dbc files ({}) not found by path: {}dbc/{}/", DBCFileCount, dataPath.c_str(), localeNames[defaultLocale]);
        exit(1);
    }
    else if (!bad_dbc_files.empty())
    {
        std::string str;
        for (StoreProblemList::iterator i = bad_dbc_files.begin(); i != bad_dbc_files.end(); ++i)
            str += *i + "\n";

        TC_LOG_ERROR("misc", "Some required *.dbc files ({} from {}) not found or not compatible:\n{}", (uint32)bad_dbc_files.size(), DBCFileCount, str.c_str());
        exit(1);
    }

    // TODO: DATA re-enable these checks, and adjust for MOP
    // Check loaded DBC files proper version
//    if (!sAreaTableStore.LookupEntry(7941)     ||     // last area added in 6.2.2 (20444)
//        !sCharTitlesStore.LookupEntry(457)     ||     // last char title added in 6.2.2 (20444)
//        !sGemPropertiesStore.LookupEntry(2544) ||     // last gem property added in 6.2.2 (20444)
//        !sMapStore.LookupEntry(1497)           ||     // last map added in 6.2.2 (20444)
//        !sSpellStore.LookupEntry(197204)       )      // last spell added in 6.2.2 (20444)
//    {
//        TC_LOG_ERROR("misc", "You have _outdated_ DBC files. Please extract correct versions from current using client.");
//        exit(1);
//    }

    // TODO: DATA MOP doesn't have ui map?
//    std::unordered_multimap<int32, UiMapAssignmentEntry const*> uiMapAssignmentByUiMap;
//    for (UiMapAssignmentEntry const* uiMapAssignment : sUiMapAssignmentStore)
//    {
//        uiMapAssignmentByUiMap.emplace(uiMapAssignment->UiMapID, uiMapAssignment);
//        if (UiMapEntry const* uiMap = sUiMapStore.LookupEntry(uiMapAssignment->UiMapID))
//        {
//                    ASSERT(uiMap->System < MAX_UI_MAP_SYSTEM, "MAX_UI_MAP_SYSTEM must be at least %u", uiMap->System + 1);
//            if (uiMapAssignment->MapID >= 0)
//                _uiMapAssignmentByMap[uiMap->System].emplace(uiMapAssignment->MapID, uiMapAssignment);
//            if (uiMapAssignment->AreaID)
//                _uiMapAssignmentByArea[uiMap->System].emplace(uiMapAssignment->AreaID, uiMapAssignment);
//            if (uiMapAssignment->WmoDoodadPlacementID)
//                _uiMapAssignmentByWmoDoodadPlacement[uiMap->System].emplace(uiMapAssignment->WmoDoodadPlacementID, uiMapAssignment);
//            if (uiMapAssignment->WmoGroupID)
//                _uiMapAssignmentByWmoGroup[uiMap->System].emplace(uiMapAssignment->WmoGroupID, uiMapAssignment);
//        }
//    }
//
//    std::unordered_map<std::pair<int32, uint32>, UiMapLinkEntry const*> uiMapLinks;
//    for (UiMapLinkEntry const* uiMapLink : sUiMapLinkStore)
//        uiMapLinks[std::make_pair(uiMapLink->ParentUiMapID, uint32(uiMapLink->ChildUiMapID))] = uiMapLink;
//
//    for (UiMapEntry const* uiMap : sUiMapStore)
//    {
//        UiMapBounds& bounds = _uiMapBounds[uiMap->ID];
//        memset(&bounds, 0, sizeof(bounds));
//        if (UiMapEntry const* parentUiMap = sUiMapStore.LookupEntry(uiMap->ParentUiMapID))
//        {
//            if (parentUiMap->GetFlags().HasFlag(UiMapFlag::NoWorldPositions))
//                continue;
//
//            UiMapAssignmentEntry const* uiMapAssignment = nullptr;
//            UiMapAssignmentEntry const* parentUiMapAssignment = nullptr;
//            for (std::pair<int32 const, UiMapAssignmentEntry const*> const& uiMapAssignmentForMap : Trinity::Containers::MapEqualRange(uiMapAssignmentByUiMap, uiMap->ID))
//            {
//                if (uiMapAssignmentForMap.second->MapID >= 0 &&
//                    uiMapAssignmentForMap.second->Region[1].X - uiMapAssignmentForMap.second->Region[0].X > 0 &&
//                    uiMapAssignmentForMap.second->Region[1].Y - uiMapAssignmentForMap.second->Region[0].Y > 0)
//                {
//                    uiMapAssignment = uiMapAssignmentForMap.second;
//                    break;
//                }
//            }
//
//            if (!uiMapAssignment)
//                continue;
//
//            for (std::pair<int32 const, UiMapAssignmentEntry const*> const& uiMapAssignmentForMap : Trinity::Containers::MapEqualRange(uiMapAssignmentByUiMap, uiMap->ParentUiMapID))
//            {
//                if (uiMapAssignmentForMap.second->MapID == uiMapAssignment->MapID &&
//                    uiMapAssignmentForMap.second->Region[1].X - uiMapAssignmentForMap.second->Region[0].X > 0 &&
//                    uiMapAssignmentForMap.second->Region[1].Y - uiMapAssignmentForMap.second->Region[0].Y > 0)
//                {
//                    parentUiMapAssignment = uiMapAssignmentForMap.second;
//                    break;
//                }
//            }
//
//            if (!parentUiMapAssignment)
//                continue;
//
//            float parentXsize = parentUiMapAssignment->Region[1].X - parentUiMapAssignment->Region[0].X;
//            float parentYsize = parentUiMapAssignment->Region[1].Y - parentUiMapAssignment->Region[0].Y;
//            float bound0scale = (uiMapAssignment->Region[1].X - parentUiMapAssignment->Region[0].X) / parentXsize;
//            float bound0 = ((1.0f - bound0scale) * parentUiMapAssignment->UiMax.Y) + (bound0scale * parentUiMapAssignment->UiMin.Y);
//            float bound2scale = (uiMapAssignment->Region[0].X - parentUiMapAssignment->Region[0].X) / parentXsize;
//            float bound2 = ((1.0f - bound2scale) * parentUiMapAssignment->UiMax.Y) + (bound2scale * parentUiMapAssignment->UiMin.Y);
//            float bound1scale = (uiMapAssignment->Region[1].Y - parentUiMapAssignment->Region[0].Y) / parentYsize;
//            float bound1 = ((1.0f - bound1scale) * parentUiMapAssignment->UiMax.X) + (bound1scale * parentUiMapAssignment->UiMin.X);
//            float bound3scale = (uiMapAssignment->Region[0].Y - parentUiMapAssignment->Region[0].Y) / parentYsize;
//            float bound3 = ((1.0f - bound3scale) * parentUiMapAssignment->UiMax.X) + (bound3scale * parentUiMapAssignment->UiMin.X);
//            if ((bound3 - bound1) > 0.0f || (bound2 - bound0) > 0.0f)
//            {
//                bounds.Bounds[0] = bound0;
//                bounds.Bounds[1] = bound1;
//                bounds.Bounds[2] = bound2;
//                bounds.Bounds[3] = bound3;
//                bounds.IsUiAssignment = true;
//            }
//        }
//
//        if (UiMapLinkEntry const* uiMapLink = Trinity::Containers::MapGetValuePtr(uiMapLinks, std::make_pair(uiMap->ParentUiMapID, uiMap->ID)))
//        {
//            bounds.IsUiAssignment = false;
//            bounds.IsUiLink = true;
//            bounds.Bounds[0] = uiMapLink->UiMin.Y;
//            bounds.Bounds[1] = uiMapLink->UiMin.X;
//            bounds.Bounds[2] = uiMapLink->UiMax.Y;
//            bounds.Bounds[3] = uiMapLink->UiMax.X;
//        }
//    }

    // Initialize global taxinodes mask
    // reinitialize internal storage for globals after loading TaxiNodes.db2
    sTaxiNodesMask = {};
    sHordeTaxiNodesMask = {};
    sAllianceTaxiNodesMask = {};
    sOldContinentsNodesMask = {};
    // include existed nodes that have at least single not spell base (scripted) path
    for (TaxiNodesEntry const* node : sTaxiNodesStore)
    {
        if (!node->IsPartOfTaxiNetwork())
            continue;

        // valid taxi network node
        uint32 field = uint32((node->ID - 1) / (sizeof(TaxiMask::value_type) * 8));
        TaxiMask::value_type submask = TaxiMask::value_type(1 << ((node->ID - 1) % (sizeof(TaxiMask::value_type) * 8)));

        sTaxiNodesMask[field] |= submask;
        if (node->GetFlags().HasFlag(TaxiNodeFlags::ShowOnHordeMap))
            sHordeTaxiNodesMask[field] |= submask;
        if (node->GetFlags().HasFlag(TaxiNodeFlags::ShowOnAllianceMap))
            sAllianceTaxiNodesMask[field] |= submask;

        int32 uiMapId = -1;
        if (!GetUiMapPosition(node->Pos.X, node->Pos.Y, node->Pos.Z, node->ContinentID, 0, 0, 0, UI_MAP_SYSTEM_ADVENTURE, false, &uiMapId))
            GetUiMapPosition(node->Pos.X, node->Pos.Y, node->Pos.Z, node->ContinentID, 0, 0, 0, UI_MAP_SYSTEM_TAXI, false, &uiMapId);

        if (uiMapId == 985 || uiMapId == 986)
            sOldContinentsNodesMask[field] |= submask;
    }

    for (WMOAreaTableEntry const* entry : sWMOAreaTableStore)
        _wmoAreaTableLookup[WMOAreaTableKey(entry->WmoID, entry->NameSetID, entry->WmoGroupID)] = entry;

    TC_LOG_INFO("server.loading", ">> Initialized {} DBC data stores in {} ms", DBCFileCount, GetMSTimeDiffToNow(oldMSTime));

    return availableDbcLocales;
}

std::vector<uint32> const* DBCManager::GetFactionTeamList(uint32 faction) const
{
    return Trinity::Containers::MapGetValuePtr(_factionTeams, faction);
}

ItemClassEntry const* DBCManager::GetItemClassByOldEnum(uint32 itemClass) const
{
    return _itemClassByOldEnum[itemClass];
}

TaxiPathEntry const* DBCManager::GetTaxiPath(uint32 from, uint32 to) const
{
    return Trinity::Containers::MapGetValuePtr(_taxiPaths, { from, to });
}

std::string DBCManager::GetNameGenEntry(uint8 race, uint8 gender) const
{
    ASSERT(gender < GENDER_NONE);
    auto ritr = _nameGenData.find(race);
    if (ritr == _nameGenData.end())
        return "";

    if (ritr->second[gender].empty())
        return "";

    return Trinity::Containers::SelectRandomContainerElement(ritr->second[gender])->Name;
}

MapDifficultyEntry const* DBCManager::GetDefaultMapDifficulty(uint32 mapId, Difficulty* difficulty /*= nullptr*/) const
{
    auto itr = _mapDifficulties.find(mapId);
    if (itr == _mapDifficulties.end())
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

MapDifficultyEntry const* DBCManager::GetMapDifficultyData(uint32 mapId, Difficulty difficulty) const
{
    auto itr = _mapDifficulties.find(mapId);
    if (itr == _mapDifficulties.end())
        return nullptr;

    auto diffItr = itr->second.find(difficulty);
    if (diffItr == itr->second.end())
        return nullptr;

    return diffItr->second;
}

MapDifficultyEntry const* DBCManager::GetDownscaledMapDifficultyData(uint32 mapId, Difficulty& difficulty) const
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

bool DBCManager::IsInArea(uint32 objectAreaId, uint32 areaId)
{
    do
    {
        if (objectAreaId == areaId)
            return true;

        AreaTableEntry const* objectArea = sAreaTableStore.LookupEntry(objectAreaId);
        if (!objectArea)
            break;

        objectAreaId = objectArea->ParentAreaID;
    } while (objectAreaId);

    return false;
}

struct UiMapAssignmentStatus
{
    UiMapAssignmentEntry const* UiMapAssignment = nullptr;
    // distances if inside
    struct
    {
        float DistanceToRegionCenterSquared = std::numeric_limits<float>::max();
        float DistanceToRegionBottom = std::numeric_limits<float>::max();
    } Inside;

    // distances if outside
    struct
    {
        float DistanceToRegionEdgeSquared = std::numeric_limits<float>::max();
        float DistanceToRegionTop = std::numeric_limits<float>::max();
        float DistanceToRegionBottom = std::numeric_limits<float>::max();
    } Outside;

    int8 MapPriority = 3;
    int8 AreaPriority = -1;
    int8 WmoPriority = 3;

    bool IsInside() const
    {
        return Outside.DistanceToRegionEdgeSquared < std::numeric_limits<float>::epsilon() &&
               std::abs(Outside.DistanceToRegionTop) < std::numeric_limits<float>::epsilon() &&
               std::abs(Outside.DistanceToRegionBottom) < std::numeric_limits<float>::epsilon();
    }
};

static bool CheckUiMapAssignmentStatus(float x, float y, float z, int32 mapId, int32 areaId, int32 wmoDoodadPlacementId, int32 wmoGroupId,
                                       UiMapAssignmentEntry const* uiMapAssignment, UiMapAssignmentStatus* status)
{
    status->UiMapAssignment = uiMapAssignment;
    // x,y not in region
    if (x < uiMapAssignment->Region[0].X || x > uiMapAssignment->Region[1].X || y < uiMapAssignment->Region[0].Y || y > uiMapAssignment->Region[1].Y)
    {
        float xDiff, yDiff;
        if (x >= uiMapAssignment->Region[0].X)
        {
            xDiff = 0.0f;
            if (x > uiMapAssignment->Region[1].X)
                xDiff = x - uiMapAssignment->Region[0].X;
        }
        else
            xDiff = uiMapAssignment->Region[0].X - x;

        if (y >= uiMapAssignment->Region[0].Y)
        {
            yDiff = 0.0f;
            if (y > uiMapAssignment->Region[1].Y)
                yDiff = y - uiMapAssignment->Region[0].Y;
        }
        else
            yDiff = uiMapAssignment->Region[0].Y - y;

        status->Outside.DistanceToRegionEdgeSquared = xDiff * xDiff + yDiff * yDiff;
    }
    else
    {
        status->Inside.DistanceToRegionCenterSquared =
                (x - (uiMapAssignment->Region[0].X + uiMapAssignment->Region[1].X) * 0.5f) * (x - (uiMapAssignment->Region[0].X + uiMapAssignment->Region[1].X) * 0.5f)
                + (y - (uiMapAssignment->Region[0].Y + uiMapAssignment->Region[1].Y) * 0.5f) * (y - (uiMapAssignment->Region[0].Y + uiMapAssignment->Region[1].Y) * 0.5f);
        status->Outside.DistanceToRegionEdgeSquared = 0.0f;
    }

    // z not in region
    if (z < uiMapAssignment->Region[0].Z || z > uiMapAssignment->Region[1].Z)
    {
        if (z < uiMapAssignment->Region[1].Z)
        {
            if (z < uiMapAssignment->Region[0].Z)
                status->Outside.DistanceToRegionBottom = std::min(uiMapAssignment->Region[0].Z - z, 10000.0f);
        }
        else
            status->Outside.DistanceToRegionTop = std::min(z - uiMapAssignment->Region[1].Z, 10000.0f);
    }
    else
    {
        status->Outside.DistanceToRegionTop = 0.0f;
        status->Outside.DistanceToRegionBottom = 0.0f;
        status->Inside.DistanceToRegionBottom = std::min(uiMapAssignment->Region[0].Z - z, 10000.0f);
    }

    if (areaId && uiMapAssignment->AreaID)
    {
        int8 areaPriority = 0;
        while (areaId != uiMapAssignment->AreaID)
        {
            if (AreaTableEntry const* areaEntry = sAreaTableStore.LookupEntry(areaId))
            {
                areaId = areaEntry->ParentAreaID;
                ++areaPriority;
            }
            else
                return false;
        }

        status->AreaPriority = areaPriority;
    }

    if (mapId >= 0 && uiMapAssignment->MapID >= 0)
    {
        if (mapId != uiMapAssignment->MapID)
        {
            if (MapEntry const* mapEntry = sMapStore.LookupEntry(mapId))
            {
                if (mapEntry->ParentMapID == uiMapAssignment->MapID)
                    status->MapPriority = 1;
                else if (mapEntry->CosmeticParentMapID == uiMapAssignment->MapID)
                    status->MapPriority = 2;
                else
                    return false;
            }
            else
                return false;
        }
        else
            status->MapPriority = 0;
    }

    if (wmoGroupId || wmoDoodadPlacementId)
    {
        if (uiMapAssignment->WmoGroupID || uiMapAssignment->WmoDoodadPlacementID)
        {
            bool hasDoodadPlacement = false;
            if (wmoDoodadPlacementId && uiMapAssignment->WmoDoodadPlacementID)
            {
                if (wmoDoodadPlacementId != uiMapAssignment->WmoDoodadPlacementID)
                    return false;

                hasDoodadPlacement = true;
            }

            if (wmoGroupId && uiMapAssignment->WmoGroupID)
            {
                if (wmoGroupId != uiMapAssignment->WmoGroupID)
                    return false;

                if (hasDoodadPlacement)
                    status->WmoPriority = 0;
                else
                    status->WmoPriority = 2;
            }
            else if (hasDoodadPlacement)
                status->WmoPriority = 1;
        }
    }

    return true;
}

static bool operator<(UiMapAssignmentStatus const& left, UiMapAssignmentStatus const& right)
{
    bool leftInside = left.IsInside();
    bool rightInside = right.IsInside();
    if (leftInside != rightInside)
        return leftInside;

    if (left.UiMapAssignment && right.UiMapAssignment &&
        left.UiMapAssignment->UiMapID == right.UiMapAssignment->UiMapID &&
        left.UiMapAssignment->OrderIndex != right.UiMapAssignment->OrderIndex)
        return left.UiMapAssignment->OrderIndex < right.UiMapAssignment->OrderIndex;

    if (left.WmoPriority != right.WmoPriority)
        return left.WmoPriority < right.WmoPriority;

    if (left.AreaPriority != right.AreaPriority)
        return left.AreaPriority < right.AreaPriority;

    if (left.MapPriority != right.MapPriority)
        return left.MapPriority < right.MapPriority;

    if (leftInside)
    {
        if (left.Inside.DistanceToRegionBottom != right.Inside.DistanceToRegionBottom)
            return left.Inside.DistanceToRegionBottom < right.Inside.DistanceToRegionBottom;

        float leftUiSizeX = left.UiMapAssignment ? (left.UiMapAssignment->UiMax.X - left.UiMapAssignment->UiMin.X) : 0.0f;
        float rightUiSizeX = right.UiMapAssignment ? (right.UiMapAssignment->UiMax.X - right.UiMapAssignment->UiMin.X) : 0.0f;

        if (leftUiSizeX > std::numeric_limits<float>::epsilon() && rightUiSizeX > std::numeric_limits<float>::epsilon())
        {
            float leftScale = (left.UiMapAssignment->Region[1].X - left.UiMapAssignment->Region[0].X) / leftUiSizeX;
            float rightScale = (right.UiMapAssignment->Region[1].X - right.UiMapAssignment->Region[0].X) / rightUiSizeX;
            if (leftScale != rightScale)
                return leftScale < rightScale;
        }

        if (left.Inside.DistanceToRegionCenterSquared != right.Inside.DistanceToRegionCenterSquared)
            return left.Inside.DistanceToRegionCenterSquared < right.Inside.DistanceToRegionCenterSquared;
    }
    else
    {
        if (left.Outside.DistanceToRegionTop != right.Outside.DistanceToRegionTop)
            return left.Outside.DistanceToRegionTop < right.Outside.DistanceToRegionTop;

        if (left.Outside.DistanceToRegionBottom != right.Outside.DistanceToRegionBottom)
            return left.Outside.DistanceToRegionBottom < right.Outside.DistanceToRegionBottom;

        if (left.Outside.DistanceToRegionEdgeSquared != right.Outside.DistanceToRegionEdgeSquared)
            return left.Outside.DistanceToRegionEdgeSquared < right.Outside.DistanceToRegionEdgeSquared;
    }

    return true;
}

static UiMapAssignmentEntry const* FindNearestMapAssignment(float x, float y, float z, int32 mapId, int32 areaId, int32 wmoDoodadPlacementId, int32 wmoGroupId, UiMapSystem system)
{
    UiMapAssignmentStatus nearestMapAssignment;
    auto iterateUiMapAssignments = [&](std::unordered_multimap<int32, UiMapAssignmentEntry const*> const& assignments, int32 id)
    {
        for (auto assignment : Trinity::Containers::MapEqualRange(assignments, id))
        {
            UiMapAssignmentStatus status;
            if (CheckUiMapAssignmentStatus(x, y, z, mapId, areaId, wmoDoodadPlacementId, wmoGroupId, assignment.second, &status))
                if (status < nearestMapAssignment)
                    nearestMapAssignment = status;
        }
    };

    iterateUiMapAssignments(_uiMapAssignmentByWmoGroup[system], wmoGroupId);
    iterateUiMapAssignments(_uiMapAssignmentByWmoDoodadPlacement[system], wmoDoodadPlacementId);

    AreaTableEntry const* areaEntry = sAreaTableStore.LookupEntry(areaId);
    while (areaEntry)
    {
        iterateUiMapAssignments(_uiMapAssignmentByArea[system], areaEntry->ID);
        areaEntry = sAreaTableStore.LookupEntry(areaEntry->ParentAreaID);
    }

    if (MapEntry const* mapEntry = sMapStore.LookupEntry(mapId))
    {
        iterateUiMapAssignments(_uiMapAssignmentByMap[system], mapEntry->ID);
        if (mapEntry->ParentMapID >= 0)
            iterateUiMapAssignments(_uiMapAssignmentByMap[system], mapEntry->ParentMapID);
        if (mapEntry->CosmeticParentMapID >= 0)
            iterateUiMapAssignments(_uiMapAssignmentByMap[system], mapEntry->CosmeticParentMapID);
    }

    return nearestMapAssignment.UiMapAssignment;
}

static DBCPosition2D CalculateGlobalUiMapPosition(int32 uiMapID, DBCPosition2D uiPosition)
{
    UiMapEntry const* uiMap = sUiMapStore.LookupEntry(uiMapID);
    while (uiMap)
    {
        if (uiMap->Type <= UI_MAP_TYPE_CONTINENT)
            break;

        UiMapBounds const* bounds = Trinity::Containers::MapGetValuePtr(_uiMapBounds, uiMap->ID);
        if (!bounds || !bounds->IsUiAssignment)
            break;

        uiPosition.X = ((1.0 - uiPosition.X) * bounds->Bounds[1]) + (bounds->Bounds[3] * uiPosition.X);
        uiPosition.Y = ((1.0 - uiPosition.Y) * bounds->Bounds[0]) + (bounds->Bounds[2] * uiPosition.Y);

        uiMap = sUiMapStore.LookupEntry(uiMap->ParentUiMapID);
    }

    return uiPosition;
}

bool DBCManager::GetUiMapPosition(float x, float y, float z, int32 mapId, int32 areaId, int32 wmoDoodadPlacementId, int32 wmoGroupId, UiMapSystem system, bool local,
                                  int32* uiMapId /*= nullptr*/, DBCPosition2D* newPos /*= nullptr*/)
{
    if (uiMapId)
        *uiMapId = -1;

    if (newPos)
    {
        newPos->X = 0.0f;
        newPos->Y = 0.0f;
    }

    UiMapAssignmentEntry const* uiMapAssignment = FindNearestMapAssignment(x, y, z, mapId, areaId, wmoDoodadPlacementId, wmoGroupId, system);
    if (!uiMapAssignment)
        return false;

    if (uiMapId)
        *uiMapId = uiMapAssignment->UiMapID;

    DBCPosition2D relativePosition{ 0.5f, 0.5f };
    DBCPosition2D regionSize{ uiMapAssignment->Region[1].X - uiMapAssignment->Region[0].X, uiMapAssignment->Region[1].Y - uiMapAssignment->Region[0].Y };
    if (regionSize.X > 0.0f)
        relativePosition.X = (x - uiMapAssignment->Region[0].X) / regionSize.X;
    if (regionSize.Y > 0.0f)
        relativePosition.Y = (y - uiMapAssignment->Region[0].Y) / regionSize.Y;

    DBCPosition2D uiPosition
            {
                    // x and y are swapped
                    ((1.0f - (1.0f - relativePosition.Y)) * uiMapAssignment->UiMin.X) + ((1.0f - relativePosition.Y) * uiMapAssignment->UiMax.X),
                    ((1.0f - (1.0f - relativePosition.X)) * uiMapAssignment->UiMin.Y) + ((1.0f - relativePosition.X) * uiMapAssignment->UiMax.Y)
            };

    if (!local)
        uiPosition = CalculateGlobalUiMapPosition(uiMapAssignment->UiMapID, uiPosition);

    if (newPos)
        *newPos = uiPosition;

    return true;
}

bool DBCManager::Zone2MapCoordinates(uint32 areaId, float& x, float& y) const
{
    AreaTableEntry const* areaEntry = sAreaTableStore.LookupEntry(areaId);
    if (!areaEntry)
        return false;

    for (auto assignment : Trinity::Containers::MapEqualRange(_uiMapAssignmentByArea[UI_MAP_SYSTEM_WORLD], areaId))
    {
        if (assignment.second->MapID >= 0 && assignment.second->MapID != areaEntry->ContinentID)
            continue;

        float tmpY = (y - assignment.second->UiMax.Y) / (assignment.second->UiMin.Y - assignment.second->UiMax.Y);
        float tmpX = (x - assignment.second->UiMax.X) / (assignment.second->UiMin.X - assignment.second->UiMax.X);
        x = assignment.second->Region[0].X + tmpY * (assignment.second->Region[1].X - assignment.second->Region[0].X);
        y = assignment.second->Region[0].Y + tmpX * (assignment.second->Region[1].Y - assignment.second->Region[0].Y);

        return true;
    }

    return false;
}

void DBCManager::Map2ZoneCoordinates(uint32 areaId, float& x, float& y) const
{
    DBCPosition2D zoneCoords;
    if (!GetUiMapPosition(x, y, 0.0f, -1, areaId, 0, 0, UI_MAP_SYSTEM_WORLD, true, nullptr, &zoneCoords))
        return;

    x = zoneCoords.Y * 100.0f;
    y = zoneCoords.X * 100.0f;
}

ChrSpecializationEntry const* DBCManager::GetDefaultChrSpecializationForClass(uint32 class_) const
{
    return GetChrSpecializationByIndex(class_, INITIAL_SPECIALIZATION_INDEX);
}

char const* DBCManager::GetChrRaceName(uint8 race, LocaleConstant locale /*= DEFAULT_LOCALE*/)
{
    ChrRacesEntry const* raceEntry = sChrRacesStore.LookupEntry(race);
    if (!raceEntry)
        return "";

    if (raceEntry->Name[locale][0] != '\0')
        return raceEntry->Name[locale];

    return raceEntry->Name[DEFAULT_LOCALE];
}

char const* DBCManager::GetClassName(uint8 class_, LocaleConstant locale /*= DEFAULT_LOCALE*/)
{
    ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(class_);
    if (!classEntry)
        return "";

    if (classEntry->Name[locale][0] != '\0')
        return classEntry->Name[locale];

    return classEntry->Name[DEFAULT_LOCALE];
}

char const* DBCManager::GetCreatureFamilyPetName(uint32 petfamily, LocaleConstant locale)
{
    if (!petfamily)
        return nullptr;

    CreatureFamilyEntry const* petFamily = sCreatureFamilyStore.LookupEntry(petfamily);
    if (!petFamily)
        return nullptr;

    return petFamily->Name(locale)[0] != '\0' ? petFamily->Name(locale) : nullptr;
}

ChrSpecializationEntry const* DBCManager::GetChrSpecializationByIndex(uint32 class_, uint32 index) const
{
    return _chrSpecializationsByIndex[class_][index];
}

SkillRaceClassInfoEntry const* DBCManager::GetSkillRaceClassInfo(uint32 skill, uint8 race, uint8 class_) const
{
    for (auto&& [_, skllRaceClassInfo] : Trinity::Containers::MapEqualRange(_skillRaceClassInfoBySkill, skill))
    {
        if (!skllRaceClassInfo->RaceMask.IsEmpty() && !(skllRaceClassInfo->RaceMask.HasRace(race)))
            continue;
        if (skllRaceClassInfo->ClassMask && !(skllRaceClassInfo->ClassMask & (1 << (class_ - 1))))
            continue;

        return skllRaceClassInfo;
    }

    return nullptr;
}

std::vector<SkillRaceClassInfoEntry const*> DBCManager::GetSkillRaceClassInfo(uint32 skill) const
{
    std::vector<SkillRaceClassInfoEntry const*> result;
    for (auto const& [_, skillRaceClassInfo] : Trinity::Containers::MapEqualRange(_skillRaceClassInfoBySkill, skill))
        result.push_back(skillRaceClassInfo);

    return result;
}

uint32 DBCManager::GetEmptyAnimStateID() const
{
    return sAnimationDataStore.GetNumRows();
}

DBCManager::FriendshipRepReactionSet const* DBCManager::GetFriendshipRepReactions(uint32 friendshipRepID) const
{
    return Trinity::Containers::MapGetValuePtr(_friendshipRepReactions, friendshipRepID);
}

std::vector<ItemSpecOverrideEntry const*> const* DBCManager::GetItemSpecOverrides(uint32 itemId) const
{
    return Trinity::Containers::MapGetValuePtr(_itemSpecOverrides, itemId);
}

JournalTierEntry const* DBCManager::GetJournalTier(uint32 index) const
{
    if (index < _journalTiersByIndex.size())
        return _journalTiersByIndex[index];
    return nullptr;
}

LFGDungeonsEntry const* DBCManager::GetLfgDungeon(uint32 mapId, Difficulty difficulty)
{
    for (LFGDungeonsEntry const* dungeon : sLFGDungeonsStore)
        if (dungeon->MapID == int32(mapId) && Difficulty(dungeon->DifficultyID) == difficulty)
            return dungeon;

    return nullptr;
}

uint32 DBCManager::GetLiquidFlags(uint32 liquidType)
{
    if (LiquidTypeEntry const* liq = sLiquidTypeStore.LookupEntry(liquidType))
        return 1 << liq->SoundBank;

    return 0;
}

ResponseCodes DBCManager::ValidateName(std::wstring const& name, LocaleConstant locale) const
{
    for (Trinity::wregex const& regex : _nameValidators[locale])
        if (Trinity::regex_search(name, regex))
            return CHAR_NAME_PROFANE;

    // regexes at TOTAL_LOCALES are loaded from NamesReserved which is not locale specific
    for (Trinity::wregex const& regex : _nameValidators[TOTAL_LOCALES])
        if (Trinity::regex_search(name, regex))
            return CHAR_NAME_RESERVED;

    return CHAR_NAME_SUCCESS;
}

uint32 DBCManager::GetQuestUniqueBitFlag(uint32 questId)
{
    QuestV2Entry const* v2 = sQuestV2Store.LookupEntry(questId);
    if (!v2)
        return 0;

    return v2->UniqueBitFlag;
}

std::vector<uint32> const* DBCManager::GetPhasesForGroup(uint32 group) const
{
    return Trinity::Containers::MapGetValuePtr(_phasesByGroup, group);
}

std::vector<SkillLineEntry const*> const* DBCManager::GetSkillLinesForParentSkill(uint32 parentSkillId) const
{
    return Trinity::Containers::MapGetValuePtr(_skillLinesByParentSkillLine, parentSkillId);
}

std::vector<SpecializationSpellsEntry const*> const* DBCManager::GetSpecializationSpells(uint32 specId) const
{
    return Trinity::Containers::MapGetValuePtr(_specializationSpellsBySpec, specId);
}

bool DBCManager::IsValidSpellFamilyName(SpellFamilyNames family)
{
    return _spellFamilyNames.count(family) > 0;
}

std::vector<SpellProcsPerMinuteModEntry const*> DBCManager::GetSpellProcsPerMinuteMods(uint32 spellprocsPerMinuteId) const
{
    auto itr = _spellProcsPerMinuteMods.find(spellprocsPerMinuteId);
    if (itr != _spellProcsPerMinuteMods.end())
        return itr->second;

    return std::vector<SpellProcsPerMinuteModEntry const*>();
}

std::vector<TalentEntry const*> const& DBCManager::GetTalentsByPosition(uint32 class_, uint32 tier, uint32 column) const
{
    return _talentsByPosition[class_][tier][column];
}

bool DBCManager::IsTotemCategoryCompatibleWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId, bool requireAllTotems /*= true*/)
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

    int32 sharedMask = itemEntry->TotemCategoryMask & reqEntry->TotemCategoryMask;
    return requireAllTotems ? sharedMask == reqEntry->TotemCategoryMask : sharedMask != 0;
}

WMOAreaTableEntry const* DBCManager::GetWMOAreaTable(int32 rootId, int32 adtId, int32 groupId) const
{
    return Trinity::Containers::MapGetValuePtr(_wmoAreaTableLookup, WMOAreaTableKey(int16(rootId), int8(adtId), groupId));
}

TaxiMask::TaxiMask()
{
    if (sTaxiNodesStore.GetNumRows())
    {
        _data.resize(((sTaxiNodesStore.GetNumRows() - 1) / (sizeof(value_type) * 64) + 1) * 8, 0);
                ASSERT((_data.size() % 8) == 0, "TaxiMask size must be aligned to a multiple of uint64");
    }
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

bool DBCManager::FriendshipRepReactionEntryComparator::Compare(FriendshipRepReactionEntry const* left, FriendshipRepReactionEntry const* right)
{
    return left->ReactionThreshold < right->ReactionThreshold;
}
