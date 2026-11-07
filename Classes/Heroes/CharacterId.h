#pragma once

// Every unit the game spawns, by the name its data and sprites are filed under
// (Resources/Element/<name>, setMon/setBullet/setTrap events, map towers). Only the
// team adds characters, so the list is closed: a name missing here is a bug.
#define CHARACTER_IDS(X)                                                                        \
    /* heroes and their forms */                                                                \
    X(Asuma) X(Chiyo) X(Choji) X(Deidara) X(Gaara) X(Hidan) X(Hinata) X(Hiruzen)                \
    X(ImmortalSasuke) X(Ino) X(Itachi) X(Jiraiya) X(Jugo) X(Kakashi) X(Kakuzu) X(Kankuro)       \
    X(Karin) X(Kiba) X(Kisame) X(Konan) X(Kurenai) X(Lee) X(Minato) X(Naruto) X(Neji)           \
    X(Orochimaru) X(Pain) X(RikudoNaruto) X(RockLee) X(SageJiraiya) X(SageNaruto) X(Sai)        \
    X(Sakura) X(Sasuke) X(Shikamaru) X(Shino) X(Suigetsu) X(Tenten) X(Tobi) X(Tobirama)         \
    X(Tsunade)                                                                                  \
    /* guardians */                                                                             \
    X(Han) X(Roshi)                                                                             \
    /* clones, summons and puppets */                                                           \
    X(Akamaru) X(Centipede) X(DogWall) X(Karasu) X(Kurama) X(MaskFudon) X(MaskKadon)            \
    X(MaskRaidon) X(Parents) X(Sanshouuo) X(Saso) X(Slug)                                       \
    /* monsters */                                                                              \
    X(Bikyu) X(Bug) X(BugPillar) X(BugTomado) X(ChuiDi) X(CircleMark) X(ClayBird) X(Crash)      \
    X(Crash2) X(DeidaraBom) X(Dogs) X(FakeDeidara) X(FakeItachi) X(FakeMinato) X(FakeShino)     \
    X(FakeTobirama) X(FireRain) X(FudonSRK) X(FudonSRK2) X(Hasan) X(HiraishinMark) X(InkBird)   \
    X(InkDragon) X(ItachiSusano) X(Jibaku) X(Kage) X(KageFeng) X(KageHand) X(KageHands)         \
    X(Kaiten) X(Kubi) X(Kuroari) X(LeeBom) X(Mine) X(Mouse) X(Paku) X(PaperRain) X(Qilin)       \
    X(QuanRen) X(Sabaku) X(SakuraBom) X(Sanbao) X(SandBall) X(SandHand) X(SandWave)             \
    X(SasukeSusano) X(Shark) X(Shark2) X(Shenwei) X(Shenwei2) X(Shoryu) X(SmallSlug) X(Snake)   \
    X(Spider) X(Steam) X(SuiRyuDan) X(Suiji) X(Suijin) X(TamaBomb) X(Tenmu) X(ThunderWave)      \
    X(Tiger) X(TodonPillar) X(Traps) X(Tsukuyomi) X(Tuji) X(Tuji2) X(WaterBom) X(WaterBullet)   \
    X(Wave) X(Yataikuzu) X(Yominuma)                                                            \
    /* traps */                                                                                 \
    X(Amaterasu) X(KageBom) X(Kusuri)                                                           \
    /* bullets */                                                                               \
    X(FlyKnife) X(HiraishinKunai) X(HugeSRK) X(PaperSpear) X(PaperSrk) X(Shintenshin)           \
    X(TentenSRK)                                                                                \
    /* flogs */                                                                                 \
    X(FemalePainFlog) X(IzumoFlog) X(KakashiFlog) X(KotetsuFlog) X(ObitoFlog) X(PainFlog)       \
    /* towers */                                                                                \
    X(AkatsukiCenter) X(AkatsukiTower1) X(AkatsukiTower2) X(KonohaCenter) X(KonohaTower1)       \
    X(KonohaTower2)

enum class CharacterId
{
    None,  // not set yet
#define X(name) name,
    CHARACTER_IDS(X)
#undef X
};

// CharacterId::None for null and unknown names
CharacterId characterIdFromString(const char* name);
