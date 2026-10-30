# Roadmap Migrasi Naruto Senki → Axmol (MVP dulu)

> Referensi teknis (pemetaan API, risiko, hitungan pemakaian API): [`MIGRATION_PLAN_AXMOL.md`](MIGRATION_PLAN_AXMOL.md).
> Dokumen ini adalah **urutan eksekusi**. Centang `- [x]` setiap langkah yang selesai, lalu catat temuan di bagian **Log Keputusan**.

**Target:** cocos2d-x 2.2.2 → **Axmol 2.11.5**, platform pertama **Windows x64**, toolchain VS 2026 + CMake 4.4.

---

## 1. Definisi MVP

**MVP = mode Training 3v3 bisa dimainkan sampai selesai di Windows x64 dengan Axmol.**

### Alur yang harus jalan
```
GameScene (intro → logo → push start)
  → StartMenu (menu roda)
    → Training → LoadLayer → GameLayer (peta Tiles/1.tmx)
      → menang/kalah → GameOver (reward coin ke SQLite)
        → kembali ke StartMenu
```

**Karakter MVP** mengikuti `StartMenu::onTrainingCallBack`: player **Tsunade** + pool {Lee, Shino, Kakashi, Kakuzu, Asuma}. Pool ini sudah mencakup summon (mask Kakuzu), buff, dan ougis cut-in.

### Termasuk MVP
- Joystick, normal attack, skill 1–3, ougis 1–2, item potion
- Creep tiap 15 detik, tower & core, level-up, respawn, kill feed, minimap
- `PauseLayer` (resume, surrender, toggle BGM/voice)
- Coin di SQLite, audio BGM + SFX

### Di luar MVP (cukup dikompilasi; fitur dimatikan atau di-stub)
- HardCore / `NetworkLayer`, `GearLayer`, guardian Roshi/Han, peta 2–5
- `RankingLayer`, `SkillLayer`, `CreditsLayer` (boleh ikut jika ternyata gratis)
- Mode Carnival 4v4 (`Cheats>10`)
- Semua HTTP (login, group, rank, upload)
- Android / JNI / iklan
- **Seluruh anti-tamper** (`KeyList`, `checkMD5`, `CError`, `CCDirector::end()` diam-diam)
- Hero lain dan transform Naruto/Sasuke/Lee/Jiraiya

**Kenapa "cukup dikompilasi"?** Graf include saling terkait:
- `StartMenu.h` → `NetworkLayer.h` → `RankingLayer.h` / `SkillLayer.h`
- `KTools.cpp` → `NetworkLayer.h`
- `GameLayer.h` → `GearLayer.h` / `GameOver.h`

Akibatnya semua file tetap harus bisa dikompilasi walaupun fiturnya tidak diuji di MVP.

**Flag:** `#define NS_MVP 1` di `Defines.h`. Fitur non-MVP dibungkus `#if !NS_MVP`, atau tombol menunya menampilkan pesan "coming soon".

---

## 2. Milestone

Setiap milestone punya **exit criteria** dan ditutup dengan **commit + tag git** (`m0` … `m5`, `mvp`).

### M0: Tooling siap
- [x] CMake ≥ 4.2 (`cmake --version`): 4.4.4
- [x] PowerShell 7 (`pwsh --version`): 7.6.9
- [x] VS 2026: workload **Desktop development with C++** + Windows SDK (MSVC 14.51, SDK 10.0.26100)
  - cek: `vswhere -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`
- [x] Git (2.37.3 sudah ada)
- [x] Clone Axmol **v2.11.5** ke `C:\dev\axmol`, jalankan `pwsh setup.ps1`, lalu cek env `AX_ROOT` dan perintah `axmol` (butuh **Windows Developer Mode**)
- [ ] Build HelloWorld uji di `C:\dev\HelloAx`

**Exit:** `axmol new -p net.zakume.hello -l cpp Hello` → `axmol build -p win32 -a x64` → jendela HelloWorld muncul.

### M1: Repo siap migrasi
- [x] `git init` + `.gitignore`:
  - `Debug.win32/`, `Release.win32/`, `ipch/`, `*.sdf`, `*.VC.db`, `*.suo`, `*.user`, `build/`, `.vs/`
  - plus `.gitattributes` berisi `* -text`, agar `core.autocrlf` tidak mengubah byte file
- [x] Commit "original 2.2.2 snapshot", lalu tag `legacy-2.2.2`
- [x] Konversi GBK → UTF-8 untuk `Classes/**/*.{cpp,h}` (kecuali `sqlite3*`): 18 file
  - cek `FONT_TYPE` di `Defines.h` dan string Cina lain
  - commit terpisah: "convert sources to UTF-8"
- [x] Buat branch `axmol-migration`

**Exit:** `git diff legacy-2.2.2 --stat` hanya berisi perubahan encoding, dan komentar Cina terbaca.

### M2: Kerangka proyek Axmol
- [x] `axmol new` ke folder sementara, lalu salin `CMakeLists.txt`, `proj.win32/` (`main.cpp`), dan struktur template ke root repo
  - yang disalin: `cmake/`, `CMakeLists.txt`, `.axproj`, `build.bat`, `run.bat.in`, `.clang-format`, `proj.win32/`, `proj.android/`
  - `.editorconfig` **tidak** disalin, karena memaksa LF dan trim whitespace pada kode lama
- [x] `Resources/` dipetakan sebagai content folder template; `Classes/` tetap di tempatnya
- [x] `CMakeLists.txt`:
  - source: semua `Classes/*.cpp` + `Classes/MyUtils/*.cpp` + `MyUtils/sqlite3.c` (dikompilasi sebagai C)
  - opsi compiler `/utf-8` (otomatis dari `ax_setup_app_config`)
- [x] Hapus `proj.win32/*.vcxproj`, `.sln`, `.sdf`, dan `ipch/` lama (tetap aman di tag `legacy-2.2.2`)
  - binary engine 2.2.2 (`Debug.win32`, `Release.win32`) dipindah ke `C:\dev\naruto-senki-legacy-bin\`, bukan dihapus
- [x] Cek di source Axmol status hal-hal berikut, lalu catat di Log Keputusan:
  - `__String`, `__Array`, `__Dictionary`, `__NotificationCenter`
  - `SimpleAudioEngine`
  - header compat `cocos2d.h`

**Exit:** CMake configure sukses. Build boleh gagal di kode game, tetapi tidak boleh gagal karena engine/toolchain.

### M3: Lapisan kompatibilitas (`Classes/Compat/`)
- [ ] `Compat2x.h`, di-include dari `Defines.h`:
  - alias tipe/namespace
  - `ccp`, `*_selector`, `sharedXxx()`
- [ ] `CCStringCompat` / `CCArrayCompat` / `CCDictCompat` + `CCARRAY_FOREACH`
  - **hanya** jika M2 menunjukkan kelas aslinya sudah dihapus
- [ ] `GameAudio`: API mirip `SimpleAudioEngine` di atas `ax::AudioEngine`
- [ ] Bungkus semua include `../cocos2dx/platform/android/jni/JniHelper.h` dengan `#if AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID`

**Exit:** jumlah error compile turun drastis. Catat jumlah error sebelum dan sesudah.

### M4: Kompilasi per modul
Kerjakan dari dependensi terkecil. **Setiap baris = 1 commit, dan build harus hijau di akhirnya** (pecah langkahnya jika tidak bisa).

| # | Status | Modul | Pekerjaan utama |
|---|---|---|---|
| 4.1 | [ ] | `proj.win32/main.cpp`, `AppDelegate` | `GLViewImpl`, `ResolutionPolicy::FIXED_HEIGHT` 480x320, window 960x640 |
| 4.2 | [ ] | `MyUtils/` (KTools, MD5, CCShake, CCScrewLayer, CCStrokeLabel) | `getDataFromFile`, tinyxml2, `uint32_t` di MD5, include path baru |
| 4.3 | [ ] | Input: `ActionButton`, `JoyStick`, `GearButton`, `SelectButton`, `MenuButton`, `ScrewLayer` | `CCTouchDelegate` → `EventListenerTouchOneByOne` (fixed priority -50, swallow) |
| 4.4 | [ ] | `Effect`, `HPBar`, `BGLayer` | kecil, sebagian besar sudah tertutup shim |
| 4.5 | [ ] | `ActionManager` | 16 `CCCallFuncND` → lambda (sekaligus hapus leak `callValue->retain()`), `CCNotificationCenter` → custom event, `setFlipX` → `setFlippedX`, cast pointer→`intptr_t` |
| 4.6 | [ ] | `Element`, `Characters` | sebagian besar shim; `Characters` hampir murni AI |
| 4.7 | [ ] | `GameLayer`, `HudLayer`, `PauseLayer`, `GameOver`, `LoadLayer` | TMX → `ValueVector`/`ValueMap`, snapshot `RenderTexture`, HTTP → lambda/stub |
| 4.8 | [ ] | `GameScene`, `StartMenu`, `SelectLayer` | `ui::EditBox`/`ui::Scale9Sprite`, keyboard back, matikan menu non-MVP |
| 4.9 | [ ] | Cukup dikompilasi: `NetworkLayer`, `SkillLayer`, `RankingLayer`, `GearLayer`, `CreditsLayer` | cukup compile; HTTP di-stub |
| 4.10 | [ ] | Matikan anti-tamper | `KeyList`, `checkMD5`, `checkPost`, sabotase di `checkTower`, exit `CError` di `GameOver` → `#if 0` atau hapus |

**Exit:** `axmol build -p win32 -a x64` sukses dengan 0 error (warning dicatat).

### M5: MVP berjalan (runtime)
Uji secara berurutan dan perbaiki temuan sebelum lanjut ke langkah berikutnya.
- [ ] Intro → logo → push start → menu roda bisa diputar
- [ ] `sql.db` terbentuk di writable path (`%LOCALAPPDATA%\NarutoSenki`)
- [ ] Training: loading selesai, 6 hero muncul di posisi dari TMX, kamera mengikuti player
- [ ] Joystick 8 arah, attack, skill 1–3 + cooldown, item potion
- [ ] Level-up → ougis terbuka → cut-in (dunia di-pause) → resume
- [ ] Creep tiap 15 detik → tower hancur → creep naik level → core hancur → GameOver
- [ ] Coin reward tersimpan; kembali ke menu, lalu main lagi (cek crash/leak saat scene diulang)
- [ ] Pause: resume, surrender, toggle BGM/voice
- [ ] Kakuzu memanggil mask; hero yang punya summon mati lalu respawn
  - bug crash `Element.cpp` L766 kemungkinan muncul di sini → **perbaiki di langkah ini**

**Exit:** 3 match berturut-turut tanpa crash, lalu tag `mvp`.

### M6: Pasca-MVP
Masing-masing menjadi milestone kecil tersendiri, dikerjakan sesuai urutan prioritas:
1. Seluruh hero + transform di Training dan Carnival 4v4
2. HardCore: layar pilih `NetworkLayer`, peta 2–5, `GearLayer`, guardian
3. `SkillLayer`, `RankingLayer` (lokal `rank.json`), Credits
4. Perbaikan bug dari report:
   - HP underflow
   - stun hilang karena pembagian integer
   - `abs()` hilang di cek jarak
   - ougis tanpa cek chakra
   - dll.
5. Hapus shim:
   - `CCString` → tipe numerik, `CCArray` → `Vector<>`, `ccp` → `Vec2`
   - `GameAudio` → `AudioEngine` langsung
   - jalankan `axmol-migrate`
6. Android: `axmol build -p android`, port Activity Java seperlunya

---

## 3. Aturan kerja
- Satu commit per langkah kecil, satu tag per milestone. **Jangan mencampur** konversi encoding, migrasi API, dan perbaikan bug dalam satu commit.
- **Jangan memperbaiki bug gameplay sebelum MVP berjalan**, kecuali bug itu membuat jalur MVP crash. Samakan perilakunya dulu, perbaiki belakangan.
- Build harus hijau di akhir setiap langkah M4.
- Catat setiap keputusan atau temuan engine di Log Keputusan di bawah.

---

## 4. Log Keputusan
| Tanggal | Keputusan / Temuan |
|---|---|
| 2026-10-31 | Target engine Axmol 2.11.5 (bukan cocos2d-x 4.0); platform pertama Windows x64 |
| 2026-10-31 | MVP = Training 3v3; anti-tamper dimatikan; HTTP di-stub |
| 2026-10-31 | M0: CMake 4.4.4, pwsh 7.6.9, workload C++ VS 2026 (MSVC 14.51, SDK 10.0.26100), Axmol v2.11.5 di `C:\dev\axmol`. `setup.ps1` butuh Windows Developer Mode |
| 2026-10-31 | M1: `.gitattributes` `* -text`, karena `core.autocrlf=true` di level sistem akan mengubah byte resource yang dicek MD5 |
| 2026-10-31 | M1: 18 file GBK → UTF-8 tanpa BOM, round-trip terverifikasi, jumlah baris tetap. `RankingLayer.cpp` sudah UTF-8 BOM sejak awal. Byte `0x14` tersembunyi di `StartMenu.cpp` tetap utuh. **Wajib compile dengan `/utf-8`** (M2) |
| 2026-10-31 | M2: Axmol 2.11.5 **sudah menghapus** `__String`, `__Array`, `__Dictionary`, `__NotificationCenter`, dan `SimpleAudioEngine`. Shim buatan sendiri di M3 wajib. `core/cocos2d.h` hanya berisi alias kecil (`namespace cocos2d = ax`, `Ref` = `Object`, `CC_SAFE_*`, `CCLOG`, `CC_CALLBACK_*`). C++20; `/utf-8` otomatis |
| 2026-10-31 | M2: configure CMake berhasil dengan generator "Visual Studio 18 2026" x64 ke `build_x64/`. 28 source dari `Classes/` (termasuk `sqlite3.c`), resource dari `Resources/` |
| 2026-10-31 | M2: binary engine 2.2.2 (`Debug.win32`/`Release.win32`, satu-satunya salinan) dipindah ke `C:\dev\naruto-senki-legacy-bin\` |
