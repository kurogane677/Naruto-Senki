# Kakashi

| | |
|---|---|
| Kelas | `Kakashi` (`Classes/Heroes/Characters/Kakashi.h/.cpp`) |
| Bentuk | — |
| Slot select | Training — · HardCore 3 |
| Record / COM Training / stealth | ya / ya / tidak |
| AI | role Assassin, build `default` {6, 5, 2} |
| Aset tambahan | `HeroAssets`: sheet DogWall |

## Skill
| Aksi | Tombol | Tipe | Nilai | CD | Event penting | Ditangani di |
|---|---|---|---|---|---|---|
| skill01 | skill 1 | aAttack | 1260 | 10 | `setCharge` 200 | umum (Chidori) |
| skill02 | skill 2 | Summon | – | 15 | `setMon` Paku | umum |
| skill03 | skill 3 | cBuff | – | 40 | `setBuff` 17 → Sharingan | `Kakashi::onTransformSkillStart/End` |
| skill04 | ougi 1 | Summon | – | – | `setClone` 5 → DogWall | hook klon `Kakashi` |
| skill05 | ougi 2 | aAttack | 660 | – | `setCharge` 192, `setChargeB` 48, `setMon` Shenwei, ThunderWave | umum |
| skill10 (SPC1) | – | aAttack | 560 | 10 | `setChargeB` 192 | skill 1 selama Sharingan |
| skill11 (SPC2) | – | Summon | – | 15 | `setMon` Dogs | skill 2 selama Sharingan |
| skill12 (SPC3) | – | Idle | – | – | | idle selama Sharingan |

## Perilaku di kelas
| Hook | Isi |
|---|---|
| `onTransformSkillStart()` | Sharingan: skill 1/2 = SPC1/SPC2, idle = SPC3, nilai skill 1 ditukar dengan nilai SPC1 (aslinya disimpan di `_tempAttackValue1`). Musuh hero dalam setengah layar (`kGameWidth/2`) yang tidak sedang hurt/dead keluar dari stealth (`breakStealth`). Ikon tombol skill player berganti ke `Kakashi_skill1_1.png` / `Kakashi_skill2_1.png` |
| `onTransformSkillEnd()` | animasi, nilai skill 1, dan ikon kembali |
| `swapsSkillAnimations()` | true: `HeroElement::setID` menyimpan (`retain`) `skill1Array`/`skill2Array` agar bisa dipasang kembali setelah Sharingan |
| `cloneCharacter()` | `DogWall` |
| `setUpClone()` | DogWall: role Summon, muncul `kDogWallDistance` (56 px) di depan, anchor (0,5; 0,1), bati |
| `finishClone()` | serangan DogWall `kDogWallAttack` (800) |
| `clonesExpire()` | true: DogWall hilang setelah angka `setClone` (5 detik) |

## Summon dan monster
| Unit | Dari | Kelas | Perilaku |
|---|---|---|---|
| DogWall | ougi 1 | `DogWall` (`Heroes/Summons/DogWall.h/.cpp`) | `ownAI()`: diam di tempat dan terus menyerang. **Aggro**: tiap tick, flog musuh yang hidup dalam `kAggroRange` (64 px, horizontal) menargetkan DogWall, termasuk yang sedang memukul hero/tower. `dealloc()`: saat DogWall hilang, flog yang masih menargetkannya dilepas dan mencari target sendiri lagi (tanpa pointer basi) |
| Paku | skill 2 | `Monster` biasa | jalur default `setMon` |
| Dogs | skill 2 selama Sharingan | `Monster` biasa | daftar bersama di `setMon`, `setCommand` (`findTarget`), dan `Monster::setAI` (tidak berjalan) |
| Shenwei, ThunderWave | ougi 2 | `Monster` biasa | Shenwei di daftar bersama `setMon`; ThunderWave jalur default |

## Konstanta
| Nama | Nilai | Arti |
|---|---|---|
| `kDogWallDistance` | 56 | jarak muncul DogWall |
| `kDogWallAttack` | 800 | serangan DogWall (sebelum #103: 1060) |
| `kAggroRange` | 64 | jangkauan aggro DogWall. Tidak bisa lebih jauh tanpa mengubah `Flog::setAI`, yang membuang target non-flog/non-tower di atas 64 px |

## Masih di kode bersama
- `HeroElement::setID` (2×): daftar frame bersama.

## Riwayat
#102 (kelas Kakashi, `DogWall` kelas summon pertama, hook `ownAI` dan `swapsSkillAnimations`), #103 (aggro DogWall, serangan 1060 → 800; perubahan gameplay).
