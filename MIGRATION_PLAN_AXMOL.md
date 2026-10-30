# Rencana Migrasi: cocos2d-x 2.2.2 → Axmol 2.x

> Langkah eksekusi (MVP dulu, checklist per milestone): lihat [`MIGRATION_ROADMAP.md`](MIGRATION_ROADMAP.md).

Disusun 2026-10-31. Rencana ini menggantikan `MIGRATION_PLAN_COCOS2DX_4.md`; target diubah oleh user ke Axmol. Belum ada kode yang diubah.

## 0. Ringkasan

| | Sekarang | Target |
|---|---|---|
| Engine | cocos2d-x 2.2.2 (folder engine tidak ada di mesin) | **Axmol 2.11.x LTS** (terbaru: v2.11.5) |
| Toolchain | `.vcxproj` v141 | CMake **4.2+**, Visual Studio 2026 (didukung resmi sejak Axmol 2.10) |
| Arsitektur | Win32 x86 | **Win64 x64**, plus Android/iOS/macOS/Linux/WASM jika diperlukan |
| Bahasa | C++03-ish, encoding **GBK** | C++ modern (C++20 di Axmol 2.x; verifikasi di Fase 1), **UTF-8** |
| Namespace | `cocos2d` / `USING_NS_CC` | `ax` / `USING_NS_AX`, makro `CC_*` → `AX_*` |

Kenapa Axmol lebih cocok daripada 4.0 untuk mesin ini:
- VS 2026 didukung tanpa patch.
- Tidak butuh Python 2.7.
- Mendukung x64 dan Android modern.
- Masih aktif dirawat.

Konsekuensinya: Axmol membuang lebih banyak API lama daripada 4.0, jadi lapisan kompatibilitas sementara harus lebih tebal.

## 1. Fakta Axmol yang relevan (verifikasi saat Fase 1)
- **Header kompatibilitas**: Axmol menyediakan `cocos2d.h` berisi pemetaan namespace, tipe, dan makro cocos2d-x → Axmol. Ini membantu, tetapi pemetaan tersebut ditujukan untuk kode **3.x/4.0, bukan 2.x**.
- **`axmol-migrate`**: tool resmi untuk memperbaiki include. Tool ini **menargetkan proyek cocos2d-x 4.0**; proyek 3.x lama bisa gagal, dan 2.x hampir pasti tidak didukung langsung. Pakai setelah kode sudah berbentuk API 3.x/4.x (setelah Fase 3), bukan sebelumnya.
- **Tipe duplikat C++ standar dideprekasi/dihapus**: `CCString` → `std::string`, dan diduga `__Array`/`__Dictionary` juga. Status pasti `__String`/`__Array`/`__Dictionary`/`__NotificationCenter`/`SimpleAudioEngine` di 2.11 **harus dicek di source** (`axmol/core/`) pada Fase 1. Strategi Fase 2 bergantung pada hasilnya.
- **Audio**: `AudioEngine` (`ax::AudioEngine`). `SimpleAudioEngine` diasumsikan **tidak ada**, sehingga 125 pemakaian harus diganti.
- **Setup**: clone/unduh Axmol, jalankan `setup.ps1` (PowerShell; mengatur env `AX_ROOT` dan tool `axmol`), lalu `axmol new -p net.zakume.narutosenki -d <dir> -l cpp NarutoSenki`, dan build dengan `axmol build -p win32` atau CMake + VS 2026.

## 2. Ukuran perubahan (pemakaian API 2.x di `Classes/`)

| API 2.x | Jumlah | Pengganti di Axmol |
|---|---|---|
| `ccp(` | 1132 | `Vec2(...)` |
| `CCString` | 1014 | `std::string` / `int` / `float` |
| `CCArray` + `CCARRAY_FOREACH` | 303 + 105 | `Vector<T*>` + range-for |
| `CCSpriteFrameCache` | 172 | `SpriteFrameCache::getInstance()` |
| `schedule_selector` | 147 | `AX_SCHEDULE_SELECTOR` / `schedule(lambda, key)` |
| `CCDictionary` / `objectForKey` | 130 / 69 | `ValueMap` atau struct biasa |
| `SimpleAudioEngine` | 125 | `AudioEngine::play2d` dan wrapper `GameAudio` (lihat Fase 3) |
| `CCLabelTTF` / `CCLabelBMFont` | 111 / 66 | `Label::createWithSystemFont/TTF` / `Label::createWithBMFont` |
| `menu_selector` | 106 | `AX_CALLBACK_1` / lambda |
| `CC_SYNTHESIZE_RETAIN` + retain/release manual | 71 + 106 | `AX_SYNTHESIZE_RETAIN`, idealnya `Vector`/`RefPtr` |
| `CCHttpRequest` + `callfuncND_selector` | 66 + 44 | `network::HttpRequest` + lambda callback |
| `callfunc_selector` / `CCCallFuncND` | 47 / 16 | `CallFunc::create(lambda)` |
| `ccTouch*` / `CCTouchDelegate` | 48 / 7 | `EventListenerTouchOneByOne` |
| `CCUserDefault` | 48 | `UserDefault::getInstance()` |
| `JniHelper` | 24 | `ax::JniHelper` (Android) |
| `CCNotificationCenter` | 20 | `EventDispatcher` custom event, atau panggilan langsung |
| `CCRenderTexture` snapshot | 20 | `RenderTexture` / `utils::captureScreen` |
| `setFlipX` | ~30 | `setFlippedX` |
| `CCEditBox` / `CCScale9Sprite` | 6 / 5 | `ui::EditBox` / `ui::Scale9Sprite` |
| `keyBackClicked` | 9 | `EventListenerKeyboard` (`KEY_BACK`/`KEY_ESCAPE`) |
| TMX `objectGroupNamed`/`getObjects` | 2 | `getObjectGroup()` → `ValueVector` |
| `getFileData` | 7 | `FileUtils::getDataFromFile()` → `Data` |
| `sqlite3.c` vendored | – | tetap dipakai (Axmol tidak membundel SQLite di core) |
| tinyxml2, rapidjson | 17 / 4 | tersedia di `3rdparty/` Axmol |

## 3. Risiko
1. **CMake 4.2+** belum ada di PATH. Install terpisah, atau biarkan `setup.ps1` mengunduhnya. **PowerShell 7 (pwsh)** juga mungkin diperlukan oleh tool `axmol`.
2. **Lompatan versi besar (2.x → Axmol)**: tidak ada tool otomatis, jadi `axmol-migrate` baru berguna setelah Fase 3.
3. **Perilaku renderer berbeda**: snapshot `RenderTexture` (Pause/Gear/GameOver), z-order `-positionY`, blending sprite batch. Butuh uji visual.
4. **Pindah ke x64**: ada cast pointer ke int (`(void*)int`, `(bool)date` di ActionManager) dan `unsigned long` di MD5 (8 byte di Linux/Android ARM64). Wajib diperbaiki dengan `intptr_t`/`uint32_t`.
5. **Kode anti-tamper/sabotase** (lihat report memory). Kode ini harus dimatikan/dihapus, karena kalau tidak, MD5 resource dan JNI package check akan memicu `Director::end()` diam-diam.
6. **Belum ada version control**. `git init` wajib sebelum mulai.
7. **Android**: Activity Java `net.zakume.game.NarutoSenki` (dialog exit, WebView news, `getCurrentName`) dan SDK iklan lama harus di-port atau dibuang.

## 4. Fase

### Fase 0: Persiapan (± 0,5 hari)
- `git init`, `.gitignore` (`Debug.win32/`, `Release.win32/`, `ipch/`, `*.sdf`, `*.VC.db`, `*.suo`, `*.user`, `build*/`), lalu commit snapshot asli.
- Konversi `Classes/**/*.{cpp,h}` dari GBK ke UTF-8, kecuali `sqlite3.*`. Cek `FONT_TYPE` (微软雅黑) dan string Cina lainnya.

### Fase 1: Engine & proyek kosong (± 1 hari), sekaligus gate
- Install CMake ≥ 4.2 (dan pwsh jika perlu). Ambil Axmol **v2.11.5**, lalu jalankan `setup.ps1`.
- `axmol new` dengan bahasa cpp, lalu build dan jalankan HelloWorld di **Win64 dengan VS 2026**.
- Pindahkan `Resources/` dan `Classes/` ke struktur proyek baru (`Content/` atau `Resources/` sesuai template). Daftarkan semua source di `CMakeLists.txt`, termasuk `MyUtils/sqlite3.c`.
- Cek di source Axmol: keberadaan `__String`/`__Array`/`__Dictionary`/`__NotificationCenter`/`SimpleAudioEngine`, isi `cocos2d.h` compat, dan standar C++ yang dipakai.

### Fase 2: Lapisan kompatibilitas sementara `Compat2x.h` (± 1 hari)
Header ini disertakan dari `Defines.h` agar mayoritas kode bisa dikompilasi dulu:
- Alias namespace dan tipe (`CCSprite`→`Sprite`, `CCNode`→`Node`, `CCObject`→`Ref`, dst.).
- Makro `ccp`, `schedule_selector`, `callfunc_selector`, `callfuncN_selector`, `menu_selector`, serta `sharedXxx()` → `getInstance()`.
- Jika `__String`/`__Array`/`__Dictionary` **sudah dihapus** di Axmol: buat **shim kecil milik proyek** (`CCStringCompat`, `CCArrayCompat`, `CCDictCompat` berbasis `Ref` + `std::string`/`Vector<Ref*>`/`std::unordered_map`) yang meniru API yang dipakai (`create`, `createWithFormat`, `getCString`, `intValue`, `addObject`, `objectAtIndex`, `count`, `removeObject`, `CCARRAY_FOREACH`, `valueForKey`, `setObject`, `objectForKey`). Ini jauh lebih murah daripada menulis ulang 1.400+ pemakaian sekaligus.
- Wrapper `GameAudio` dengan API mirip `SimpleAudioEngine` (`playEffect`, `playBackgroundMusic`, `stopAllEffects`, `isBackgroundMusicPlaying`, …) di atas `AudioEngine`, supaya 125 call site cukup diganti nama kelasnya.

### Fase 3: Migrasi API yang tidak bisa di-shim (± 5–8 hari)
Urutan dari dependensi paling sedikit:
1. `main.cpp`/`AppDelegate`: `GLViewImpl`, `setDesignResolutionSize(480,320, ResolutionPolicy::FIXED_HEIGHT)`.
2. `MyUtils/` (KTools, MD5, CCScrewLayer, CCShake, CCStrokeLabel): `getDataFromFile`, tinyxml2, tipe ukuran tetap untuk MD5.
3. Input: ActionButton, JoyStick, GearButton, SelectButton, MenuButton, ScrewLayer → `EventListenerTouchOneByOne` + fixed priority (-50) + swallow.
4. Callback berparameter: 16 `CCCallFuncND` di `ActionManager::createAnimation` menjadi lambda yang menangkap data frame. Ini sekaligus menghapus leak `callValue->retain()`. 44 callback HTTP juga menjadi lambda.
5. HTTP → `ax::network::HttpClient`. Hampir semua endpoint sudah di-stub; migrasikan yang hidup saja.
6. `CCNotificationCenter` (`acceptAttack`, `updateMap`) → custom event, atau langsung iterasi array unit di GameLayer (sekaligus menghapus O(n²) broadcast).
7. TMX (GameLayer `initHeros`/`initTower`) → `ValueVector`/`ValueMap`.
8. Label/UI: `Label`, `ui::EditBox`, `ui::Scale9Sprite`. Sediakan TTF untuk teks Cina agar tidak bergantung pada font sistem.
9. Snapshot scene (Pause/Gear/GameOver) → uji `RenderTexture`; fallback ke `utils::captureScreen`.
10. Sisanya: `setFlippedX`, `pause()`/`resume()`, keyboard listener untuk back, pointer-ke-int cast → `intptr_t`.
11. File besar terakhir: `ActionManager.cpp` (~8k), `Characters.cpp` (~9k, mayoritas logika AI), `StartMenu.cpp` (~3k).
- Setelah kompilasi berhasil, jalankan `axmol-migrate` untuk merapikan include dan rename `CC_*`→`AX_*`.

### Fase 4: Build & uji Win64 (± 2–3 hari)
Checklist manual:
- [ ] Intro → logo (10x klik = Carnival) → push start → menu roda
- [ ] `sql.db` dibuat di writable path, coin terbaca/tersimpan
- [ ] Training 3v3: joystick, attack, skill1-3, ougis (cut-in + pause), creep tiap 15s, tower, GameOver + reward
- [ ] HardCore: peta acak 1-5, gear shop, guardian Roshi/Han
- [ ] Transform (Naruto/Sasuke/Lee/Jiraiya), summon (Kankuro/Chiyo/Kakuzu), mind control
- [ ] Pause/resume/surrender, toggle BGM/voice; audio `.ogg` + `.mp3`
- [ ] Ranking (`rank.json`), SkillLayer, Credits

### Fase 5: Bersih-bersih (opsional, ± 3–5 hari)
- Hapus `Compat2x.h` dan shim: stat jadi tipe numerik (sekaligus memperbaiki HP underflow), `Vector<ActionManager*>`, `Vec2`.
- Perbaiki bug crash dari report (Element L766, RankingLayer, null `_master`/`_slayer`, ownership retain).

### Fase 6: Android (opsional, ± 2–3 hari)
- `axmol build -p android`; port Activity Java seperlunya, buang SDK iklan lama.

## 5. Estimasi
- **Win64 bisa dimainkan (Fase 0–4)**: ± **10–14 hari kerja**. Lebih lama daripada rencana 4.0 karena shim `__String`/`__Array` dan audio mungkin harus dibuat sendiri, tetapi tanpa risiko toolchain.
- **+ bersih-bersih**: +3–5 hari.
- **+ Android**: +2–3 hari.

## 6. Langkah pertama
1. `git init` + commit snapshot.
2. Konversi GBK → UTF-8.
3. Install CMake 4.2+, ambil Axmol v2.11.5, `setup.ps1`, lalu build HelloWorld Win64 dengan VS 2026.

Referensi: rilis Axmol di github.com/axmolengine/axmol/releases dan migration guide di github.com/axmolengine/axmol/wiki/Cocos2d‐x-migration-guide.
