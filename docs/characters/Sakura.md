# Sakura

| | |
|---|---|
| Kelas | `Sakura` (`Classes/Heroes/Characters/Sakura.h/.cpp`) |
| Bentuk | — |
| Slot select | Training 1 · HardCore 1 |
| Record / COM Training / stealth | ya / ya / tidak |
| AI | role Support, build `default` {0, 4, 8}, `push` {4, 8, 0}, `sustain` {7, 8, 1} |
| Aset tambahan | — |

## Skill
| Aksi | Tombol | Tipe | Nilai | CD | Event penting | Ditangani di |
|---|---|---|---|---|---|---|
| skill01 | skill 1 | hBuff | – | 15 | `setBuff` 600 (heal) | `ActionManager::healBuff` (bersama Tsunade) |
| skill02 | skill 2 | aAttack | 560 | 10 | | umum |
| skill03 | skill 3 | cBuff | – | 25 | `setBuff` 17 → pukulan kuat | `Sakura::onTransformSkillStart/End` |
| skill04 | ougi 1 | aAttack | 300 | – | `setJump`, `setMon` Tuji | umum |
| skill05 | ougi 2 | aAttack | 860 | – | `setTransport`/`reTransport`, `setJump`, `setMon` SakuraBom | `Sakura::onTransportCommand` |
| skill10 (SPC1) | – | nAttack | – | 10 | | serangan dasar selama buff skill 3 |

## Perilaku di kelas
| Hook | Isi |
|---|---|
| `onTransportCommand()` | event `setTransport`: Sakura **melompat** `kLeapHeight` (64 px) ke atas di tempatnya, bukan teleport. `_originY` dicatat (0 diganti 0,1 karena 0 berarti "tidak di udara"); `reTransport` menurunkannya |
| `onTransformSkillStart()` | serangan dasar diganti animasi SPC1, nilainya +`kPunchBonus` (400) |
| `onTransformSkillEnd()` | serangan dasar kembali, nilai −400 |

## Summon dan monster
| Unit | Dari | Kelas | Perilaku |
|---|---|---|---|
| Tuji | ougi 1 | `Monster` biasa | jalur default `setMon` |
| SakuraBom | ougi 2 | `Monster` biasa | daftar bersama di `setMon` (muncul di posisi pemilik, langsung menyerang) |

## Konstanta
| Nama | Nilai | Arti |
|---|---|---|
| `kLeapHeight` | 64 | tinggi lompatan ougi 2 |
| `kPunchBonus` | 400 | tambahan serangan dasar selama buff skill 3 |

## Masih di kode bersama
- `ActionManager::healBuff`: heal skill 1, satu cabang dengan Tsunade.
- `ActionManager::findTargetEnemy` (2×): saat menjaga tower, Sakura dan Choji yang sedang memukul tower jadi target prioritas.
- `HeroElement::setID` (2×): daftar frame bersama.

## Riwayat
#93 (kelas Sakura, tabel `HeroFactory`).
