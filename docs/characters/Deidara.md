# Deidara (sebagian)

> Kelas Deidara baru berisi mekanik tunggangan yang dibagi dengan Sai. Cabang Deidara lainnya masih di kode bersama sampai gilirannya di urutan hero (slot 29). Catatan ini dilengkapi di PR itu.

| | |
|---|---|
| Kelas | `Deidara` : `RidingHero` (`Classes/Heroes/Characters/Deidara.h`) |
| Bentuk | — |
| Slot select | Training 5 · HardCore 29 |
| Record / COM Training / stealth | ya / ya / **ya** (skill 3) |
| AI | role Pusher, build `default` {0, 1, 5} |
| Aset tambahan | `HeroAssets`: sheet Centipede |

## Skill
| Aksi | Tombol | Tipe | Nilai | CD | Event penting | Ditangani di |
|---|---|---|---|---|---|---|
| skill01 | skill 1 | cBuff | – | 15 | `setBuff` 17 → serangan dasar khusus | `RidingHero` |
| skill02 | skill 2 | Summon | – | 25 | `setClone` 15 → Centipede | `setClone` (kode bersama) |
| skill03 | skill 3 | stBuff | – | 25 | `setBuff` 3, `setMon` FakeDeidara (stealth) | kode bersama |
| skill04 | ougi 1 | cBuff | – | – | `setBuff` 18 → tunggangan | `RidingHero` |
| skill05 | ougi 2 | Summon | 960 | – | `setInvisible`/`reInvisible`, `setMon` DeidaraBom, `setDead` | kode bersama |
| skill10 (SPC1) | – | Summon | – | – | `setMon` ClayBird | serangan dasar selama buff 17 |

## Perilaku di kelas
Mekanik buff 17/18 dari `RidingHero`; lihat [Sai.md](Sai.md). Bagian khusus Deidara: `rideMonster()` = `"Spider"`.

## Summon dan monster
| Unit | Dari | Kelas | Perilaku |
|---|---|---|---|
| Spider | tunggangan (buff 18) | `RideMonster` | hanya mengincar hero (#69) |
| Centipede | skill 2 | `Hero` biasa (summon) | `setClone`, hilang setelah 15 detik |
| ClayBird, FakeDeidara, DeidaraBom | SPC1, skill 3, ougi 2 | `Monster` biasa | cabang khusus di `setMon` / `Monster::setAI` |

## Masih di kode bersama
- `ActionManager::setBuff`: jadwal buff 17/18 (bersama Shino, Sai).
- `ActionManager::setClone` (2×): Centipede dan daftar klon yang hilang sendiri.
- `HeroElement::setID` (4×): daftar frame bersama.
- Monster ClayBird, FakeDeidara, DeidaraBom di `setMon` dan `Monster::setAI`.

## Riwayat
#100 (kelas tipis Deidara lewat `RidingHero`).
