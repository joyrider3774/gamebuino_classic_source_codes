# Gamebuino Classic — source code archive

The [Gamebuino Classic](https://gamebuino.com/) was a small hobbyist
open-source handheld built around an ATmega328, an 84x48 Nokia 5110 LCD,
and an Arduino-based library. Its community produced several dozen real
homebrew games between roughly 2012 and 2018, indexed on the official
community wiki at `legacy.gamebuino.com/wiki/index.php?title=Games`.

**That wiki, and most of the game pages/forum threads/personal sites it
linked to, no longer exist on the live web.** This repository is a plain,
historical archive of every Gamebuino Classic game (and a few non-game
tools) that could actually be located, so the source code doesn't
disappear along with the sites that used to host it.

Nothing here is original work — every subfolder is someone else's real
game or tool, reproduced (or pointed to) as faithfully as it could be
recovered. See each entry's own `LICENSE`/header comments for its actual
terms; nothing in this repository is relicensed, and several entries below
have **no license specified at all** by their own original author, which
is noted explicitly rather than assumed permissive.

## How this archive is built

Every game lives under [`games/`](games/) and every non-game tool under
[`tools/`](tools/), one subfolder each, named after its original
repository or release. Two different storage mechanisms are used,
deliberately:

- **Git submodule**, pinned to a specific commit — used for every entry
  whose original repository was still live and clonable at the time this
  archive was built. A submodule is only a *pointer* (author, URL, exact
  commit SHA recorded in [`.gitmodules`](.gitmodules)) — if the upstream
  repository ever disappears, the pointer alone won't recover the code.
  Pinning to an exact commit at least guarantees this archive always
  points at the exact version it was built from, not whatever the
  repository happens to contain later.
- **Plain, committed files** — used for every entry recovered by some
  other means: a manually downloaded `.zip` from a forum/Mediafire/
  Dropbox/personal site, or a repository whose *original* home is already
  gone (e.g. a dead Bitbucket link, recovered instead from a copy
  supplied directly by someone who still had one). There is no live
  source left to point a submodule at, so the actual files are committed
  here directly — this is the case that actually satisfies "so they
  don't get lost."

Each table below has a "Source" column linking either straight to the
still-live upstream repository (submodule entries) or to the local folder
in this archive (manually-recovered entries, since that folder is the
only place the content still exists).

One entry (`bub`) is a special case worth flagging explicitly, since a
naive submodule-everything pass would have gotten it wrong: its
originally-listed GitHub repository really did exist, but contained
nothing but a `README.md` and no real source at all. The real source was
recovered separately (a direct `.zip` from the author's own site) and is
stored here as plain committed files, not a submodule of the empty repo.
(The author has since moved active development to
[gitlab.com/smogheap/bub](https://gitlab.com/smogheap/bub), which does
now carry real, substantial source of its own — found during a later
author-repo sweep. Not switched to a submodule of that GitLab repo since
the plain files already archived here were independently recovered and
verified; noted here in case a future session wants to reconcile the two
or track that GitLab repo's own further development.)

`games/Gamebuino-Classic/` is the *official Gamebuino Classic library
itself* (not a game) — archived here too since it's the actual API every
other entry in this repository is written against, and it's just as
exposed to disappearing as everything else. Gamebuino's own bundled
`2.Intermediate/Pong` example (credited below as "Pong Solo") lives inside
it, not in its own top-level folder.

## Games found on the original community wiki

The wiki listed roughly 75 games. The table below covers every one whose
real source code could actually be found and archived.

| Game | Author | License | Source |
|---|---|---|---|
| 101 Starships | Zoglu | None specified | [games/101Starships](games/101Starships) (Manual download - zoglu.net) |
| A to K | Carlos Mari | CC-BY 4.0 (+ a non-standard no-selling note) | [games/A2K](games/A2K) (Manual download - carloslabs.com, later 404'd) |
| Agaruino | ogbaba | GPLv3 | [games/Agaruino](https://github.com/ogbaba/Agaruino) (Submodule - an agar.io-style clone; also independently found via a later broader author/repo search, not just the wiki) |
| armageddon | wuuff | GPLv3 | [games/armageddon](https://github.com/wuuff/armageddon) (Submodule) |
| Artillery | Frakasss | None specified | [games/Artillery](https://github.com/Frakasss/Artillery) (Submodule) |
| Asterocks | Yoda Zhang | None specified | [games/yoda-asterocks](games/yoda-asterocks) (Manual download - yodasvideoarcade.com bundle) |
| AsteroidRipper | ripper121 | None specified | [games/asteroid-ripper121](games/asteroid-ripper121) (Manual download - user-supplied, no wiki link at all) |
| B-Rally | scmar | MIT | [games/B-Rally](https://github.com/scmar/B-Rally) (Submodule) |
| Bang! Bang! | RackhamLeNoir | GPLv3 | [games/gamebuino-bangbang](https://github.com/RackhamLeNoir/gamebuino-bangbang) (Submodule) |
| BigBlackBox | STUDIOCRAFTapps | Custom (free to modify, do not sell this game) | [games/BigBlackBox](https://github.com/akkera102/08_gamebuino) (Submodule - recovered via a mirror repo, the wiki's own download link was dead) |
| Blob Attack | LudumDareDevelopment | None specified | [games/Blob-Attack](https://github.com/LudumDareDevelopment/Blob-Attack) (Submodule) |
| blockdude | Sorunome | None specified | [games/blockdude-gamebuino](https://github.com/Sorunome/blockdude-gamebuino) (Submodule) |
| BlocksBuino | frthery | None specified | [games/BlocksBuino](https://github.com/frthery/BlocksBuino) (Submodule) |
| Bomber | Clement83 | None specified | [games/Bomber](https://github.com/Clement83/Bomber) (Submodule) |
| Breakout Ripper | ripper121 | None specified | [games/breakout-ripper121](games/breakout-ripper121) (Manual download - user-supplied, no wiki link at all) |
| Bub | smogheap | GPLv3 | [games/bub](games/bub) (Manual download - the original repo was empty, see note above) |
| Castle Defence | kh9282 | None specified | [games/CastleDefence](https://github.com/kh9282/CastleDefence) (Submodule) |
| conduit | adekto | MIT | [games/conduit](https://github.com/adekto/conduit) (Submodule) |
| Copter | Clement83 | None specified | [games/Copter](https://github.com/Clement83/Copter) (Submodule) |
| CopterStrike | Frakasss | None specified | [games/CopterStrike](https://github.com/Frakasss/CopterStrike) (Submodule) |
| Crabator | Rodot | None specified | [games/Crabator](https://github.com/Rodot/Crabator) (Submodule) |
| CrazyCar | Baptiste Pouget | GPLv3 | [games/CRAZYCAR-Gamebuino](https://github.com/baptistepouget/CRAZYCAR-Gamebuino) (Submodule) |
| CrazyTown | Clement Quintard | None specified | [games/CrazyTown](https://github.com/Clement83/CrazyTown) (Submodule) |
| DeathMaze | msevilgenius | None specified | [games/DeathMaze](games/DeathMaze) (Manual download - user-supplied, forum thread never archived) |
| Descent into Hell | etienne72230 | None specified | [games/DescentIntoHeel](https://github.com/etienne72230/DescentIntoHeel) (Submodule) |
| Digger | scmar | None specified | [games/Digger](https://github.com/scmar/Digger) (Submodule) |
| FireBuino! | LADBSoft | LGPLv3 | [games/makerbuino-firebuino](https://github.com/ladbsoft/makerbuino-firebuino) (Submodule - targets the related MakerBuino board) |
| Flappy Birdo | Forklift5 | None specified | [games/FlappyBirdo](https://github.com/Forklift5/FlappyBirdo) (Submodule) |
| Gamebuino Catcher | qubist | None specified | [games/Gamebuino-Catcher](https://github.com/qubist/Gamebuino-Catcher) (Submodule) |
| Gamebuino2048 | Josiah Winslow | None specified | [games/Gamebuino2048](games/Gamebuino2048) (Manual download - Mediafire) |
| GlaciGlaca | Clement83 | None specified | [games/GlaciGlaca](https://github.com/Clement83/GlaciGlaca) (Submodule) |
| Gruniozerca | Arkadiusz Kaminski (arhneu) | Unlicense | [games/gruniozerca-gamebuino](https://github.com/arhneu/gruniozerca-gamebuino) (Submodule) |
| Invaders | Yoda Zhang | None specified | [games/yoda-invaders](games/yoda-invaders) (Manual download - yodasvideoarcade.com bundle) |
| Jezzball | RackhamLeNoir | GPLv3 | [games/gamebuino-jezzball](https://github.com/RackhamLeNoir/gamebuino-jezzball) (Submodule) |
| Kill Race | Yoda Zhang | None specified | [games/yoda-killrace](games/yoda-killrace) (Manual download - yodasvideoarcade.com bundle) |
| Lander | Yoda Zhang | None specified | [games/yoda-lander](games/yoda-lander) (Manual download - yodasvideoarcade.com bundle) |
| Lights Out AD | 94k | WTFPL | [games/LightsOutAD](games/LightsOutAD) (Manual download - user-supplied, the wiki's own `git://` link was dead) |
| Maruino | ajsb113 | None specified | [games/Maruino](games/Maruino) (Manual download - Dropbox) |
| Maze | Andy O'Neill | MIT | [games/gamebuino-maze](https://github.com/aoneill01/gamebuino-maze) (Submodule) |
| microHexagon | valdenthoranar | None specified | [games/microhexagon](https://bitbucket.org/valdenthoranar/microhexagon) (Submodule) |
| Minesweeper | dirksteindorf | None specified | [games/Gamebuino-Minesweeper](https://github.com/dirksteindorf/Gamebuino-Minesweeper) (Submodule) |
| Paqman | Yoda Zhang | None specified | [games/yoda-paqman](games/yoda-paqman) (Manual download - yodasvideoarcade.com bundle) |
| Pong Solo | Aurelien Rodot | LGPLv3 | [games/Gamebuino-Classic/examples/2.Intermediate/Pong](https://github.com/Gamebuino/Gamebuino-Classic) (Submodule - bundled in the official library) |
| Punkt | Andy O'Neill | MIT | [games/gamebuino-punkt](https://github.com/aoneill01/gamebuino-punkt) (Submodule) |
| Senet | Maximilian Timmerkamp (DelphiMarkus) | Apache 2.0 | [games/senet](games/senet) (Manual download - the original Bitbucket repo requires auth to clone) |
| shipwrek | yawn-g | None specified | [games/shipwrek](https://github.com/yawn-g/shipwrek) (Submodule) |
| ShootBuino | frthery | None specified | [games/ShootBuino](https://github.com/frthery/ShootBuino) (Submodule) |
| Shufflepuck Cafe | AWOT83 | GPLv3 | [games/Gamebuino-Shufflepuck_cafe](https://github.com/Awot83/Gamebuino-Shufflepuck_cafe) (Submodule) |
| Simonbuino | Jerom (Forklift5) | None specified | [games/Simonbuino](https://github.com/Forklift5/Simonbuino) (Submodule) |
| Skibuino | Mike Del Pozzo | GPLv3 | [games/skibuino](https://github.com/delpozzo/skibuino) (Submodule - also officially targets the related MAKERbuino board) |
| Smash-and-Crash | Skyrunner65 | None specified | [games/Smash-and-Crash](https://github.com/Skyrunner65/Smash-and-Crash) (Submodule) |
| Snake ABC | frthery | None specified | [games/SnakeAbcBuino](https://github.com/frthery/SnakeAbcBuino) (Submodule) |
| Sokobuino | martinsustek | None specified | [games/sokobuino](games/sokobuino) (Manual download - user-supplied, the author's GitHub has no matching repo) |
| Solitaire | Andy O'Neill | MIT | [games/gamebuino-solitaire](https://github.com/aoneill01/gamebuino-solitaire) (Submodule) |
| Spin Spin Spinbuino! | Zoglu | None specified | [games/SpinSpinSpinbuino](games/SpinSpinSpinbuino) (Manual download - zoglu.net) |
| StickFighter | Clement83 (art by Quirby64) | None specified | [games/StickFighter](https://github.com/Clement83/StickFighter) (Submodule) |
| Super Crate Buino | Aurelien Rodot | None specified | [games/Super-Crate-Buino](https://github.com/Rodot/Super-Crate-Buino) (Submodule) |
| Super Space Shooter | msevilgenius | None specified | [games/Gamebuino-SuperSpaceShooter](https://github.com/msevilgenius/Gamebuino-SuperSpaceShooter) (Submodule) |
| T-Rex Quest | Awot83 | GPLv3 | [games/Gamebuino-TREX-QUEST](https://github.com/Awot83/Gamebuino-TREX-QUEST) (Submodule - recovered via the author's GitHub, the wiki's own forum link was dead) |
| Taquin | RackhamLeNoir | GPLv3 | [games/gamebuino-taquin](https://github.com/RackhamLeNoir/gamebuino-taquin) (Submodule) |
| Tetrino | j0ff | MIT | [games/tetrino](https://github.com/j0ff/tetrino) (Submodule) |
| Thunder Shoot | Awot83 | GPLv3 | [games/Gamebuino-Thunder-Shoot](https://github.com/Awot83/Gamebuino-Thunder-Shoot) (Submodule - found via the author's GitHub, not the wiki's own dead link) |
| Tron | Clement83 | None specified | [games/Tron](https://github.com/Clement83/Tron) (Submodule) |
| UFO Race | Rodot | None specified | [games/UFO-Race](https://github.com/Rodot/UFO-Race) (Submodule) |
| Video Poker | Mike Del Pozzo | GPLv3 | [games/videopoker-gamebuino](https://github.com/delpozzo/videopoker-gamebuino) (Submodule - also officially targets the related MAKERbuino board) |
| World's Hardest Game | Sorunome | None specified | [games/Worlds-Hardest-Game-Gamebuino](https://github.com/Sorunome/Worlds-Hardest-Game-Gamebuino) (Submodule) |
| ZombiEscape | Frakasss | None specified | [games/ZombiEscape](https://github.com/Frakasss/ZombiEscape) (Submodule) |

**67 real wiki-listed games found and archived**, out of ~75 known
entries — see "Games we have not found source code for" below for the
rest.

## Non-game tools

Real, recoverable non-game source — editors, compilations, simulators,
converters, demos, and utilities. Most were real wiki entries; several
more were found later via broader author/repo sweeps not listed on the
wiki at all (noted per-row below). None of these are games, so they live
in [`tools/`](tools/), not `games/`:

| Tool | Author | License | Source |
|---|---|---|---|
| chip-8-gamebuino | wuuff | GPLv3 | [tools/chip-8-gamebuino](https://github.com/wuuff/chip-8-gamebuino) (Submodule - a real CHIP-8 interpreter for Gamebuino Classic; not on the wiki, found via an author/repo sweep) |
| GambiPaint | Trist06Git | None specified | [tools/GambiPaint](https://github.com/Trist06Git/GambiPaint) (Submodule - a drawing/paint tool) |
| Gamebuino BEER-CALC | Awot83 | GPLv3 | [tools/Gamebuino-BEER-CALC](https://github.com/Awot83/Gamebuino-BEER-CALC) (Submodule - a beer-brewing calculator; not on the wiki, found via an author/repo sweep) |
| Gamebuino Classic Games Compilation | Gamebuino (official) | GPLv3 | [tools/Gamebuino-Classic-Games-Compilation](https://github.com/Gamebuino/Gamebuino-Classic-Games-Compilation) (Submodule - the official SD-card compilation; ships only compiled `.HEX`/`.INF` files, no real source of its own) |
| Gamebuino Drawer | qubist | None specified | [tools/Gamebuino-Drawer](https://github.com/qubist/Gamebuino-Drawer) (Submodule - a real, functional pixel-art drawing tool with a menu system; only Save/Export are unfinished `//FIXME` stubs, the core drawing loop works; found via a full author-repo-list sweep) |
| Gamebuino HIRAGANA Japanese Learning tool | Awot83 | GPLv3 | [tools/Gamebuino-HIRAGANA-Japanese-Learning-tool](https://github.com/Awot83/Gamebuino-HIRAGANA-Japanese-Learning-tool) (Submodule - a Japanese hiragana-reading learning tool; not on the wiki, found via an author/repo sweep) |
| Gamebuino MusicNote | Awot83 | GPLv3 | [tools/Gamebuino-MusicNote](https://github.com/Awot83/Gamebuino-MusicNote) (Submodule - a music-note learning/ear-training tool; not on the wiki, found via an author/repo sweep) |
| Gamebuino Thunderbolt Calc | Awot83 | GPLv3 | [tools/Gamebuino-Thunderbolt_Calc](https://github.com/Awot83/Gamebuino-Thunderbolt_Calc) (Submodule - a lightning-strike-distance calculator; not on the wiki, found via an author/repo sweep) |
| gamebuinoEducation | CalebDueck | Custom "Educational Use License" — **non-commercial use only**, redistribution/mirroring restricted, not a standard OSS license (read the repo's own `LICENSE.md` before reusing anything) | [tools/gamebuinoEducation](https://github.com/CalebDueck/gamebuinoEducation) (Submodule - a genuinely substantial 13-lesson Gamebuino Classic programming course with real working starter/example code per lesson; not on the wiki, found via an author/repo sweep) |
| ghost | Clement83 | None specified | [tools/ghost](https://github.com/Clement83/ghost) (Submodule - a "ghost detector" novelty gadget using the real ambient-light sensor as a dowsing-needle gauge; a real, complete, non-stub demo, not a game with a win/lose state; found via a full author-repo-list sweep) |
| HelloGamebuino | SloeComputers (originally AnotherJohnH) | Unlicense | [tools/HelloGamebuino](https://github.com/SloeComputers/HelloGamebuino) (Submodule - a CMake/Makefile-based dev environment for building real Gamebuino Classic `.HEX` binaries from the command line, without the Arduino IDE; not on the wiki, found via a fresh topic search) |
| Metalog | ogbaba | GPLv3 | [tools/Metalog](https://github.com/ogbaba/Metalog) (Submodule - a logic-circuit simulator) |
| Pyditn's Dice Thrower | pyditn2 | None specified | [tools/Pyditns-Dice-Thrower-Gamebuino](https://github.com/pyditn2/Pyditns-Dice-Thrower-Gamebuino) (Submodule - a real dice-rolling utility; not on the wiki, found via an author/repo sweep) |
| Radio | scmar | MIT | [tools/Radio](https://github.com/scmar/Radio) (Submodule - a real FM radio receiver app using an RDA5807 chip, with RDS support; found via a full author-repo-list sweep) |
| sd_map_test | wuuff | None specified | [tools/sd_map_test](https://github.com/wuuff/sd_map_test) (Submodule - an interactive SD-card-streamed scrolling-map tech demo, not a game in its own right; not on the wiki, found via an author/repo sweep) |
| yoda-fxsynth | Yoda Zhang | None specified | [tools/yoda-fxsynth](tools/yoda-fxsynth) (Manual download - a sound-effect synth/editor tool, not a game) |

## Games found outside the wiki search

Not listed on the community wiki at all — found later via broader
searches (GitHub topic/keyword search for `gamebuino classic`, and
checking each already-known author's own other repositories for more
Gamebuino Classic work they never submitted to the wiki). Every
candidate was checked one by one against `<Gamebuino-Meta.h>`-based
projects for the newer, unrelated Gamebuino META board, and excluded
when it matched that instead:

| Game | Author | License | Source |
|---|---|---|---|
| Aimbuino | Baptiste Pouget (hosted under ogbaba's account) | GPLv3 | [games/Aimbuino](https://github.com/ogbaba/Aimbuino) (Submodule - "Bsktuino", an aiming/basketball game; a real 2016 school project, "ISN Project 2016, Lorgues", per its own README - explains the shared hosting) |
| another2048 | grafMakulaDer2te | None specified | [games/another2048](https://github.com/grafMakulaDer2te/another2048) (Submodule - a genuinely different 2048 codebase from the already-archived `Gamebuino2048`; the repo also carries a separate `meta_version` branch for the unrelated META board — the default `main` branch archived here is the real Classic one; found via the live `gamebuino.com/creations` site) |
| Community RPG | Sorunome | None specified | [games/gamebuino-community-rpg](https://github.com/Sorunome/gamebuino-community-rpg) (Submodule - a genuinely substantial class-based RPG with real EEPROM + SD-card save data; found via an author/repo sweep) |
| cruiser | specht | None specified | [games/cruiser](https://github.com/specht/cruiser) (Submodule - a genuine 3D shooter, the most structurally unusual find in this archive; ships its own from-scratch reimplementation of the Gamebuino API rather than the real library) |
| Dark Shmup | Clement83 | None specified | [games/DarkShmup](https://github.com/Clement83/DarkShmup) (Submodule - a real shoot-em-up) |
| Dark Tower | Marcus Hutchings | GPLv3 | [games/DarkTower](https://github.com/marcushutchings/DarkTower) (Submodule - a real text-based adventure game, found via the live `gamebuino.com/creations` site rather than the old wiki or GitHub search) |
| Duel | Clement83 | None specified | [games/Duel](https://github.com/Clement83/Duel) (Submodule - a sword-duel game with genuine two-cartridge `Wire.h` master/slave multiplayer) |
| Elventure | wuuff | GPLv3 | [games/Elventure](https://github.com/wuuff/elventure-gamebuino) (Submodule - a real, class-based Zelda-like dungeon crawler) |
| Fifteen | Tnxec2 | None specified | [games/fifteen](https://github.com/Tnxec2/fifteen) (Submodule - a classic sliding 15-puzzle with real EEPROM save; a genuinely different codebase from the already-archived Taquin, same concept, different author; found by checking this author's own full repo list directly rather than trusting a paginated listing view, which had been silently dropping entries) |
| Firemen | Vicking69 | GPLv2 | [games/firemen](https://github.com/Vicking69/firemen) (Submodule) |
| Footlol | Baptiste Pouget (hosted under ogbaba's account) | GPLv3 | [games/FOOTLOL-Gamebuino](https://github.com/ogbaba/FOOTLOL-Gamebuino) (Submodule - a small football game) |
| Frogger (MAKERbuino port) | Andy Jackson | None specified | [games/Frogger_MAKERbuino](games/Frogger_MAKERbuino) (Manual extraction - one real subfolder of `andyhighnumber/Attiny-Arduino-Games`, a live but mostly-unrelated ATtiny85 compilation repo; extracted as plain files rather than submoduled whole, same treatment as `BigBlackBox`'s own compilation-repo source) |
| Gamebuino (StijnCaerts) | Stijn Caerts | MIT | [games/StijnCaerts-Gamebuino](https://github.com/StijnCaerts/Gamebuino) (Submodule - bundles a real local 2-player Pong and a Snake; the Snake needs a real external `ivanseidel/LinkedList` Arduino library) |
| Gamebuino-Classic-Snake-5110 | Lady Awesome & MakerSquirrel | GPLv3 (per the repo's own `LICENSE`) — though the source file's own header comment separately claims CC-BY-SA; a real, unresolved conflict between the two, not resolved here | [games/Gamebuino-Classic-Snake-5110](https://github.com/makerSquirrel/Gamebuino-Classic-Snake-5110) (Submodule - a genuinely different codebase from Snake Classic below, confirmed by direct diff, not a duplicate) |
| Gemgem | Tnxec2 | None specified | [games/gemgem-gamebuino](https://github.com/Tnxec2/gemgem-gamebuino) (Submodule - a Bejeweled-style match-3 game with real EEPROM save/pause; found via an author/repo sweep) |
| Master Kebab | ogbaba | GPLv3 | [games/MasterKebab](https://github.com/ogbaba/RMKebab) (Submodule - a kebab-shop management game with a real EEPROM save; the author's own README warns "this game may not work on emulators") |
| Minesweeper (Tnxec2's fork) | Tnxec2 | None specified | [games/minesw-gameguino](https://github.com/Tnxec2/minesw-gameguino) (Submodule - a real, confirmed fork of the already-archived `dirksteindorf/Gamebuino-Minesweeper` with its own gameplay changes: maximized board size, a medium difficulty, restart-on-loss; a genuinely different codebase state, not a duplicate; found via an author/repo sweep) |
| Mole Control | Markus Klingler (grafMakulaDer2te) | None specified | [games/mole-control](https://github.com/grafMakulaDer2te/mole-control) (Submodule - a real, complete Whack-a-Mole game with scoring, levels, and lives; found via a full author-repo-list sweep) |
| MotoCross | Clement83 | None specified | [games/MotoCross](https://github.com/Clement83/motoCross) (Submodule - a racing game) |
| My RPG | Frakasss | None specified | [games/MyRPG](https://github.com/Frakasss/MyRPG) (Submodule - a small RPG) |
| No Name Platform Game | Frakasss | None specified | [games/NoNamePlatformGame](https://github.com/Frakasss/NoNamePlatformGame) (Submodule - a real platformer despite the placeholder-looking name) |
| PAK-MAN (MAKERbuino port) | Andy Jackson | None specified | [games/PAK-MAN_MAKERbuino](games/PAK-MAN_MAKERbuino) (Manual extraction - the other real Gamebuino/MAKERbuino subfolder of the same `andyhighnumber/Attiny-Arduino-Games` compilation repo as Frogger above; a Pac-Man clone ported from the AttinyArcade) |
| Parachute | Jicehel | None specified | [games/Parachute_Gamebuino](https://github.com/jicehel/Parachute_Gamebuino) (Submodule - a Game & Watch "Parachute" remake) |
| Petit Monstre | Clement83 | None specified | [games/PetitMonstre](https://github.com/Clement83/petitMonstre) (Submodule - "Futuromon", a real Pokémon-style monster-catching/battling game) |
| Pinball | Clement83 | None specified | [games/PinBall](https://github.com/Clement83/pinBall) (Submodule) |
| Pirates | Clement83 (art by Jerom) | None specified | [games/Pirates](https://github.com/Clement83/Pirates) (Submodule - "SwordsPi", sword-fighting vs. a real AI opponent) |
| Pong Local Multiplayer | qubist | None specified | [games/Gamebuino-PongLocalMultiplayer](https://github.com/qubist/Gamebuino-PongLocalMultiplayer) (Submodule - a genuine local hot-seat 2-player Pong) |
| Pong Revisited | yawn-g | None specified | [games/pong-2017](https://github.com/yawn-g/pong-2017) (Submodule - real single-player Pong vs. a ball-tracking AI, plus a power-up/"tricks" system) |
| Ralph | Clement83 | None specified | [games/Ralph](https://github.com/Clement83/ralph) (Submodule - a Wreck-It-Ralph-themed platformer) |
| Robot | Frakasss | None specified | [games/Robot](https://github.com/Frakasss/Robot) (Submodule - a real shooting/action game) |
| Save Princesse | Clement83 | None specified | [games/SavePrincesse](https://github.com/Clement83/SavePrincesse) (Submodule - a tiny knight-vs-monster teaching game) |
| Snake Classic | Ripper121 (original), Tnxec2 (fork) | None specified | [games/snake-gamebuino-classic](https://github.com/Tnxec2/snake-gamebuino-classic) (Submodule - confirmed via source diff to be a direct fork of Ripper121's own, otherwise-unrecoverable, wiki-listed Snake) |
| Star Honor | wuuff (a port of Wenceslao Villanueva Jr's original Arduboy game) | MIT | [games/StarHonor](https://github.com/wuuff/StarHonor-gamebuino) (Submodule - a roguelike space adventure with a real EEPROM save) |
| Strike-Down | Skyrunner65 (renamed to SkylarHylar) | None specified | [games/Strike-Down](https://github.com/SkylarHylar/Strike-Down) (Submodule - "Aerial-Assault", a real Joust clone; same author already credited for Smash-and-Crash, whose GitHub account was later renamed - confirmed via redirect, not a different person; found via a full author-repo-list sweep) |
| Tetrino (Tnxec2's fork) | j0ff (original), Tnxec2 (fork) | None specified | [games/Tnxec2-tetrino](https://github.com/Tnxec2/tetrino) (Submodule - a real, confirmed fork of the already-archived `j0ff/tetrino`: same `.ino` source but a genuinely different `gb_platform.cpp` swapping the B/C button mapping, plus its own `.INF` SD-card icon the original never shipped; found by checking this author's own full repo list, per the same discipline as the Fifteen entry above) |
| Under the Tower | wuuff | GPLv3 | [games/UnderTheTower](https://github.com/wuuff/under-the-tower) (Submodule - a real turn-based RPG across 10 randomized dungeons) |
| Xonix | Tnxec2 | None specified | [games/xonix-gamebuino](https://github.com/Tnxec2/xonix-gamebuino) (Submodule - a real Xonix clone; found via an author/repo sweep) |

**37 games found this way**, on top of the 67 real wiki-listed ones
above.

## Games we have not found source code for

Every one of these was a real wiki entry, checked more than once via a
different route (the wiki's own dead link, the author's GitHub/current
gamebuino.com profile, a broad general web search) before being given up
on. Worth a fresh attempt if a future visitor to this repo has a lead any
of these searches missed — a personal archive, an old email attachment, a
still-running mirror this project's own search tooling didn't find.

| Game | Author | Why it's still missing |
|---|---|---|
| EPIC MINI GAMES! | MicroGames | The author's own current gamebuino.com page states directly that download was never enabled |
| THE SHIP | Macgyvers1 (Vincent aro) | Facebook-hosted download only, now behind a login wall; the credited author's GitHub account doesn't exist |
| ROBOTRON | Macgyvers1 (Vincent aro) | Same as THE SHIP |
| Balls | Macgyvers1 (Vincent aro) | Same as THE SHIP |
| NightDriver | svenofnine | Personal-site zip now 404s; the rest of the site is still live |
| Stack Challenge | BlackDragon | Resolves to a `copy.com` cloud-storage link — that service shut down in 2016 |
| Shot Zero | Linky439 | Original host dead; the one alternate forum mirror's raw HTML (both of its only two Wayback Machine snapshots, plus the live page) all confirm the download link was never actually present, not merely mis-rendered; the wiki's own original shortlink is dead with zero Wayback coverage; the author's later itch.io work never re-released it — every remaining lever now checked, not just re-confirmed |
| Mandeluino *(not a game — a Mandelbrot-set viewer)* | svenofnine | A real copy was recovered (user-supplied `.7z`), but it contains only a compiled `mandelbuino.hex`/`mandelbuino.INF`, no real source — removed from `tools/` on that basis rather than kept as a binary-only entry |

## License

This repository is an archival index, not a single work — it carries no
overall license of its own. Every subfolder is the real, original work of
its own credited author, under whatever license (or lack of one) that
author actually chose; check each entry's own `LICENSE` file or source
header before reusing anything from it.
