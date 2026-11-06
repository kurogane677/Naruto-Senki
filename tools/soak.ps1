# Autoplay soak: the regression check for refactors (#21).
#
# Plays $MatchCount seeded AI-vs-AI matches with a throwaway save folder (the player's own
# sql.db is never touched), then sums them up: crashes, win rate, match length, kills,
# deaths and levels. Matches run in fixed 1/60 s steps, as fast as the machine renders.
#
# The seed fixes the heroes and maps, but single matches still differ run to run (the
# engine runs node timers in memory-address order), so compare the averages of many
# matches, not single matches:
#
#   tools/soak.ps1 -MatchCount 20 -Out main.json                      # on main
#   tools/soak.ps1 -MatchCount 20 -Out branch.json -Baseline main.json # on the branch
param(
    [int]$Seed = 1,
    [int]$MatchCount = 20,
    [string]$Hero = "",
    [switch]$HardCore,
    [int]$Map = 0,
    [string]$Out = "soak.json",
    [string]$Baseline = "",
    [int]$TimeoutMinutes = 60,
    [string]$Config = "RelWithDebInfo"
)

$root = Split-Path $PSScriptRoot -Parent
$exe = Join-Path $root "build_x64\bin\NarutoSenki\$Config\NarutoSenki.exe"
if (-not (Test-Path $exe)) { throw "build not found: $exe" }
if ($Baseline -and -not (Test-Path $Baseline)) { throw "baseline not found: $Baseline" }

$dataDir = Join-Path ([IO.Path]::GetTempPath()) ("NarutoSenki-soak-" + [guid]::NewGuid())
New-Item -ItemType Directory $dataDir | Out-Null

$env:NS_AUTOPLAY = "1"
$env:NS_AUTOPLAY_SEED = "$Seed"
$env:NS_AUTOPLAY_MATCHES = "$MatchCount"
$env:NS_AUTOPLAY_HERO = $Hero
$env:NS_AUTOPLAY_HARDCORE = if ($HardCore) { "1" } else { "" }
$env:NS_AUTOPLAY_MAP = if ($HardCore) { "$Map" } else { "" }
$env:NS_DATA_DIR = $dataDir

$started = Get-Date
$process = Start-Process $exe -WorkingDirectory (Split-Path $exe) -PassThru
$timedOut = -not $process.WaitForExit($TimeoutMinutes * 60 * 1000)
if ($timedOut) {
    Stop-Process $process -Force
    $process.WaitForExit()
}
$seconds = [int]((Get-Date) - $started).TotalSeconds

$logPath = Join-Path $dataDir "autoplay.log"
$crashPath = Join-Path $dataDir "autoplay_crash.log"
$log = if (Test-Path $logPath) { @(Get-Content $logPath) } else { @() }
$crash = if (Test-Path $crashPath) { @(Get-Content $crashPath) } else { @() }
Remove-Item $dataDir -Recurse -Force -ErrorAction SilentlyContinue

# one line per match: "09:46 map=1 winner=player player=Tsunade | K*:Tsunade LV6 K19 D7 ... A:Jugo LV5 K8 D14 ..."
$results = foreach ($line in $log) {
    if ($line -notmatch '^(\d+):(\d+) map=(\d+) winner=(\w+)') { continue }
    $heroes = [regex]::Matches($line, ' ([KA])\*?:\w+ LV(\d+) K(-?\d+) D(\d+)')
    [pscustomobject]@{
        Seconds     = [int]$Matches[1] * 60 + [int]$Matches[2]
        KonohaWon   = $Matches[4] -eq "player"
        Kills       = ($heroes | ForEach-Object { [int]$_.Groups[3].Value } | Measure-Object -Sum).Sum
        Deaths      = ($heroes | ForEach-Object { [int]$_.Groups[4].Value } | Measure-Object -Sum).Sum
        Level       = ($heroes | ForEach-Object { [int]$_.Groups[2].Value } | Measure-Object -Average).Average
    }
}
$results = @($results)

function Average($values) { if ($values.Count) { [Math]::Round(($values | Measure-Object -Average).Average, 2) } else { 0 } }

$summary = [ordered]@{
    seed            = $Seed
    hardCore        = [bool]$HardCore
    matchesWanted   = $MatchCount
    matchesPlayed   = $results.Count
    crashed         = $crash.Count -gt 0 -or ($process.ExitCode -ne 0 -and -not $timedOut)
    timedOut        = $timedOut
    fixedStepLost   = [bool]($log | Where-Object { $_ -like "ERROR*" })
    konohaWinRate   = if ($results.Count) { [Math]::Round(@($results | Where-Object KonohaWon).Count / $results.Count, 2) } else { 0 }
    avgMatchSeconds = Average @($results.Seconds)
    avgKills        = Average @($results.Kills)
    avgDeaths       = Average @($results.Deaths)
    avgHeroLevel    = Average @($results.Level)
    wallSeconds     = $seconds
}
$summary | ConvertTo-Json | Set-Content $Out -Encoding utf8

if ($crash.Count) { "--- CRASH ---"; $crash | Select-Object -First 30 }
if ($Baseline) {
    $before = Get-Content $Baseline -Raw | ConvertFrom-Json
    foreach ($key in $summary.Keys) {
        "{0,-16} {1,10} -> {2,-10}" -f $key, $before.$key, $summary[$key]
    }
} else {
    [pscustomobject]$summary
}
if ($summary.crashed -or $summary.timedOut -or $summary.matchesPlayed -lt $MatchCount) { exit 1 }
