# Naruto (Naruto, SageNaruto, RikudoNaruto)

| | |
|---|---|
| Kelas | `Naruto` (`Classes/Heroes/Characters/Naruto.h/.cpp`), satu kelas untuk ketiga bentuk |
| Bentuk | `HeroRegistry` rantai awakening: Naruto → SageNaruto → RikudoNaruto (hanya maju) |
| Slot select | Training 0 · HardCore 0 |
| Record / COM Training / stealth | ya / ya / tidak |
| AI | role default, build `default` {3, 7, 2} |
| Aset tambahan | `HeroAssets`: sheet ketiga bentuk + Kurama; suara per bentuk |

Transformasi memakai objek yang sama dan hanya mengganti nama karakter (`setTransform` → `HeroRegistry::nextForm`). Karena itu `Naruto::form()` membaca nama saat ini: `Base`, `Sage`, atau `Rikudo`.

## Skill
| Bentuk | Aksi | Tombol | Tipe | Nilai | CD | Event penting |
|---|---|---|---|---|---|---|
| Naruto | skill01 | skill 1 | cAttack | 660 | 10 | `setCharge` 280 |
| | skill02 | skill 2 | Summon | – | 15 | `setClone` 10 → klon bayangan |
| | skill03 | skill 3 | cAttack | 660 | 25 | `setCharge` 280 |
| | skill04 | ougi 1 | oAttack | 360 | – | |
| | skill05 | ougi 2 | cBuff | – | – | `setBuff` 17 → mode Kyuubi |
| | skill09 (SPC4) | – | KnockDown | – | – | knockdown selama mode Kyuubi; berisi `setTransform` → SageNaruto |
| Sage | skill01 | skill 1 | cAttack | 760 | 10 | `setCharge` 280 |
| | skill02 | skill 2 | Summon | – | 15 | `setClone` 10 → klon bayangan |
| | skill03 | skill 3 | Summon | – | 25 | `setMon` FudonSRK |
| | skill04 | ougi 1 | cAttack | 460 | – | |
| | skill05 | ougi 2 | cBuff | – | – | `setBuff` 17 → mode Sage |
| | skill08 (SPC5) | – | KnockDown | – | – | pose awakening; berisi `setTransform` → RikudoNaruto |
| | skill10 (SPC1) | – | cAttack | 660 | 10 | skill 1 klon bayangan Sage |
| | skill12 (SPC3) | – | Summon | – | – | serangan dasar selama buff, `setMon` Bikyu |
| Rikudo | skill01 | skill 1 | cAttack | 660 | 10 | `findTarget`, `setTransport` |
| | skill02 | skill 2 | Summon | – | 15 | `setClone` 10 → klon bayangan |
| | skill03 | skill 3 | Summon | – | 25 | `setMon` FudonSRK2 |
| | skill04 | ougi 1 | aAttack | 760 | – | |
| | skill05 | ougi 2 | cBuff | – | – | `setBuff` 17 + `setClone` 9 → mode Rikudo + Kurama |
| | skill10 (SPC1) | – | aAttack | 460 | 10 | skill 1 klon bayangan Rikudo |

## Perilaku di kelas
| Hook | Isi |
|---|---|
| `expGainPercent()` | Naruto dan Sage mendapat EXP kill 125% (dipakai `UI/Game/HPBar.cpp`) |
| `onTransformSkillStart()` | **Naruto** (Kyuubi): idle/walk/serangan/knockdown dari SPC1–4, kecepatan 320, serangan dasar 960 dengan tipe dan jangkauan SPC3, guard +5000, bati, ougi terkunci. **Sage/Rikudo**: guard +5000, bati, `isPofang`, idle SPC4/walk SPC2/serangan SPC3, kecepatan 320, ougi terkunci; Rikudo juga serangan dasar 560 |
| `onTransformSkillEnd()` | kembali normal. **Naruto**: knockdown dengan animasi SPC4 sebelum animasi knockdown dipulihkan, jadi akhir mode Kyuubi selalu memicu transformasi ke Sage. **Sage LV6**: pose awakening SPC5 yang memicu transformasi ke Rikudo. Transformasi sendiri tidak punya syarat level di kode; ia mengikuti event `setTransform` di animasi |
| `cloneCharacter()` | ougi 2 Rikudo (`O2ATTACK`) → `Kurama` (Hero biasa); klon lain → nama bentuk saat ini (objek `Naruto`) |
| `setUpClone()` | Kurama: identitas, bati, kecepatan 320, `isPofang`. Klon bayangan: identitas bentuk yang sama; klon Sage/Rikudo memakai SPC1 sebagai skill 1 |
| `cloneStartsWithOwnerHP()` | klon bayangan memakai HP Naruto; Kurama HP penuh |
| `finishClone()` | Kurama: posisi HP bar 120, serangan 1060 |
| `cloneLifetime()` | Kurama selalu `kKuramaLifetime` (9 detik); klon bayangan memakai angka `setClone` (10) |
| `clonesExpire()` | semua klon Naruto hilang setelah waktunya |
| `cloneDamageSharePercent()` | sebagai klon: 15% (Naruto), 25% (Sage), 35% (Rikudo) damage yang diterima dipantulkan ke penyerang |
| `targetsHeroesOnly()` | command `findTarget` Sage/Rikudo mengabaikan creep |
| `ownAI()` | AI klon bayangan: klon Naruto mengincar creep dulu, Sage/Rikudo mengincar hero; klon Sage mundur dulu bila target terlalu dekat untuk Rasengan |

## Summon dan monster
| Unit | Dari | Kelas | Perilaku |
|---|---|---|---|
| klon bayangan | skill02 (semua bentuk) | `Naruto` | lihat `setUpClone`, `ownAI` |
| Kurama | ougi 2 Rikudo | `Hero` biasa | AI dari `SummonController` (jangkauan 156×48, mengejar hero) |
| FudonSRK / FudonSRK2 | skill03 Sage / Rikudo | `FudonSRK` (`Units/Monsters/`) | mengejar target pemilik bila Naruto menghadapnya, kalau tidak terbang lurus; meledak (animasi dead) di hero pertama; tidak kembali ke idle setelah serangan |
| Bikyu | serangan dasar Sage selama buff | `Bikyu` | muncul di depan Sage, `isPofang`, masuk daftar monster pemilik |

## Konstanta
| Nama / nilai | Arti |
|---|---|
| `kKuramaLifetime` = 9 | durasi Kurama (detik), mengabaikan angka `setClone` di XML |
| 320 / 224 | kecepatan selama buff / normal |
| 960, 560 | serangan dasar Kyuubi / Rikudo selama buff |
| 1060 | serangan Kurama |
| FudonSRK: 48, 48×32 | jarak muncul; jangkauan ledak |
| Bikyu: 16, 32 | jarak dari tepi depan pemilik; turun ke bawah |

## Masih di kode bersama
- `HeroElement::setID` (`Element.cpp`): daftar frame/animasi yang dibagi banyak hero (Naruto, Sage, Rikudo ada di beberapa daftar).

## Riwayat
#88 (kelas Naruto), #89 (rantai bentuk), #90 (`CharacterId`), #93 (tabel `HeroFactory`), #96 (FudonSRK, Bikyu), #97 (identitas klon dari skill, Kurama 9 detik).
