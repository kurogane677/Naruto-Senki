# Sai

| | |
|---|---|
| Kelas | `Sai` : `RidingHero` (`Classes/Heroes/Characters/Sai.h/.cpp`, `RidingHero.h/.cpp`) |
| Bentuk | — |
| Slot select | Training 2 · HardCore 2 |
| Record / COM Training / stealth | ya / ya / tidak |
| AI | role Ranged, build `default` {6, 5, 1} |
| Aset tambahan | — |

## Skill
| Aksi | Tombol | Tipe | Nilai | CD | Event penting | Ditangani di |
|---|---|---|---|---|---|---|
| skill01 | skill 1 | cBuff | – | 15 | `setBuff` 17 → serangan dasar khusus | `RidingHero` |
| skill02 | skill 2 | Summon | – | 20 | `setMon` Tiger | umum |
| skill03 | skill 3 | cAttack | 360 | 25 | `setMon` InkBird | daftar bersama di `setMon` |
| skill04 | ougi 1 | cBuff | – | – | `setBuff` 18 → tunggangan | `RidingHero` |
| skill05 | ougi 2 | cAttack | 960 | – | `setMon` InkDragon | kelas `InkDragon` |
| skill10 (SPC1) | – | Summon | – | – | `setMon` Snake | serangan dasar selama buff 17 |
| skill11 (SPC2) | – | Idle | – | – | | idle selama tunggangan |
| skill12 (SPC3) | – | Summon | – | – | `setMon` Mouse | walk selama tunggangan |

## Perilaku di kelas
Mekanik buff ada di base bersama `RidingHero` (juga dipakai Deidara):

| Hook | Isi |
|---|---|
| `onTransformSkillStart()` | buff 17: serangan dasar = SPC1. Buff 18 (tunggangan): semua serangan terkunci, bati, kecepatan 320, ougi terkunci, idle SPC2 / walk SPC3, HP bar di 120, lalu monster tunggangan dijatuhkan tiap 0,5 detik |
| `onTransformSkillEnd()` | buff 17: serangan dasar kembali. Buff 18: semua kembali normal (kecepatan 224, ougi terbuka, idle), monster berhenti dijatuhkan |
| `onTransformSkillCancel()` | buff berakhir lebih awal (`setActionResume`): sama seperti akhir buff, tanpa kembali ke idle; `_skillChangeBuffValue` direset |
| `spawnRideMonster()` (privat) | maksimal 5 monster tunggangan hidup sekaligus, masing-masing dihapus setelah 4 detik bila tidak sedang menyerang |

Bagian khusus Sai:
| Hook | Isi |
|---|---|
| `rideMonster()` | `"Mouse"` |
| `onRideMonsterSpawned()` | memutar `Audio/Sai/ink_mouse.mp3` |

## Summon dan monster
| Unit | Dari | Kelas | Perilaku |
|---|---|---|---|
| Mouse | tunggangan (buff 18), juga `setMon` di SPC3 | `RideMonster` | hanya mengincar hero (`targetsHeroesOnly`). Di `acceptAttack`, Mouse bisa menahan hero yang sama hanya sekali per `kMouseHoldImmunity` (1 detik) |
| InkDragon | ougi 2 | `InkDragon` | muncul 128 px di depan Sai, terbang maju 156 px dalam 2 detik |
| InkBird | skill 3 | `Monster` biasa | daftar bersama di `setMon` |
| Tiger, Snake | skill 2, SPC1 | `Monster` biasa | jalur default `setMon` |

## Konstanta
| Nama | Nilai | Arti |
|---|---|---|
| `kAttackBuff` / `kRideBuff` | 17 / 18 | nilai `setBuff` di XML |
| `kRideSpeed` / `kWalkSpeed` | 320 / 224 | kecepatan saat menunggang / normal |
| `kSpawnInterval` | 0,5 | detik antar monster tunggangan |
| `kMaxRideMonsters` | 5 | batas monster tunggangan hidup (#69) |
| `kRideMonsterLifetime` | 4 | detik sebelum monster tunggangan dihapus |

## Masih di kode bersama
- `ActionManager::setBuff`: jadwal `resumeAction` untuk buff 17/18, satu cabang dengan Shino dan Deidara.
- `ActionManager::acceptAttack`: jeda tahan Mouse (aturan di sisi korban).
- `HeroElement::setID` (4×): daftar frame bersama.

## Riwayat
#100 (kelas Sai, `RidingHero`, `RideMonster`, `InkDragon`).
