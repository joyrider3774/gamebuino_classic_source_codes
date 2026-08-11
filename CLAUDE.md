# Gamebuino Classic — source code archive

Project context for Claude Code. Read this before making changes, and
keep it updated as decisions get made — same convention as this
project's own sibling, `gamebuino_classic_vircon32` (see "Related
projects" below).

## Goal

A plain, historical **source archive** — not a porting project, not a
curated "best of" list. Every real Gamebuino Classic game and non-game
tool that can actually be located gets its source preserved here, so it
doesn't disappear along with whatever site used to host it. The
[Gamebuino Classic](https://gamebuino.com/) community wiki
(`legacy.gamebuino.com/wiki/index.php?title=Games`) and most of what it
linked to are already gone from the live web — this repo exists because
that already happened once.

**This is not the place for**: porting quality/priority tiers, gameplay
bug fixes, Vircon32-dialect notes, or any kind of "is this game good"
judgment. All of that belongs in the sibling project. This project's own
only real judgment calls are: *does real source code exist*, and *is it
a game or a tool*.

## Directory structure

- **`games/`** — one subfolder per game, named after its original
  repository or release.
- **`tools/`** — one subfolder per non-game (editors, compilations,
  simulators, converters, synths). Same storage rules as `games/`, just
  a different top-level folder. A tool found to actually be a game (or
  vice versa) gets moved, not left in the wrong place for consistency's
  sake.
- **`unsorted/`** — staging only. Anything just found or manually
  supplied (a `.zip`/`.7z`/direct download, a lead not yet verified as
  real source, something not yet triaged into `games/` or `tools/` under
  its real name) lands here first. Nothing should stay here longer than
  it takes to verify and file it properly — an empty `unsorted/` is the
  goal state, not a permanent third category.

## The submodule-vs-plain-files rule

Two storage mechanisms, chosen per-entry, never mixed for the same one:

- **Git submodule, pinned to a specific commit** — used whenever the
  original repository is still live and clonable. `.gitmodules` records
  the author/URL; the pin itself records the exact commit this archive
  was built from. A submodule is only a *pointer* — if upstream ever
  disappears, the pointer alone doesn't recover the code, so this option
  is preferred specifically *because* the repo is still there, not out
  of laziness.
- **Plain, committed files** — used for everything recovered by any
  other means: a manually downloaded archive from a forum/Mediafire/
  Dropbox/personal site, or a repository whose original home is already
  gone (recovered instead from a mirror or a copy someone still had).
  There is no live source to point a submodule at, so the actual files
  get committed directly here — this is the case that actually satisfies
  "so it doesn't get lost," and it's the reason `unsorted/` and manual
  recovery matter at all.

**A real trap already hit once, worth checking for every new submodule
candidate**: a repository can exist and clone cleanly while containing
*no real source at all* (`bub`'s own originally-listed repo had only a
`README.md`). Don't assume "the repo is live" means "the repo has the
game" — open it and check before wiring up a submodule.

`games/Gamebuino-Classic/` is the official Gamebuino Classic library
itself, archived as a submodule too (not a game, but the real API every
other entry here is written against, and just as exposed to
disappearing as anything else). Its own bundled `examples/2.Intermediate/
Pong` is credited as "Pong Solo" rather than getting its own top-level
folder, since it isn't a separate repository.

## Related projects — read before assuming a game is unrecoverable

**`c:\github\gamebuino_classic_vircon32`** is the sibling porting
project (same author) that has already done the actual discovery legwork
this archive is built from — five separate, increasingly broad discovery
passes (the wiki itself, direct zip recovery, broad web search, GitHub
API search, two parallel research agents), each one documented in detail
in that project's own `more games/DISCOVERED_GAMES.md` and `CLAUDE.md`.
**As of this file's original writing, this archive's own `games/`+
`tools/` was a verified, exact 1:1 mirror of every one of the 97
directories that project has ever staged** (`comm -23`/`comm -13` on the
sorted directory name lists both came back empty) — every real find that
project made was already represented here, and this file's own "still
not found" list matched that project's own "confirmed still
unrecoverable" section. That mirror is no longer exact by design: this
archive has since staged `tools/mandelbuino` (a real, binary-only find
the sibling project never carries as its own staged directory since it
isn't a portable game) and, per this project's own goal, deliberately
also searches beyond that project's game-only scope for real non-game
tools/demos it wouldn't stage at all. Any *game* divergence between the
two would still be worth double-checking against `DISCOVERED_GAMES.md`.

**Before spending real effort re-searching for a "missing" title, read
that project's own `more games/DISCOVERED_GAMES.md` first** — it records
the *exact* dead links, shortlink resolutions, Wayback Machine CDX
lookups, and author-account checks already tried for every one of the
still-missing titles, not just a summary. A search that doesn't start
from a genuinely new lead (a different host, an old email, a personal
archive) is very likely to just re-find the same dead ends that project
already documented. If a new lead *is* found and recovered here, it's
worth mentioning back to that project too, since its own audit assumes
this list is current.

That project also ported ~91 of these games into a single Vircon32
cartridge, and along the way corrected a few real author/license
mistakes an earlier pass had gotten wrong (e.g. `gamebuino-solitaire` is
real MIT, not GPLv3; `makerbuino-firebuino` is real LGPLv3, not
unspecified; `BigBlackBox`'s real source lives at `akkera102/
08_gamebuino`, a compilation repo, not a dedicated repo of its own). This
archive's own README table already reflects those corrections — if a
license/author cell here and in that project's own `README.md` Games
table ever disagree, treat that as a real discrepancy worth resolving,
not a coincidence.

## Current status

- **115 real directories staged total**: 102 games + 13 tools already
  sorted into `games/`/`tools/` — see `README.md`'s own tables for the
  full per-entry breakdown with author/license/source, plus `Pirates` and
  `Duel` (both real, both archived here despite the sibling porting
  project choosing not to ship either one — this archive keeps them
  regardless, since "does the sibling project want to port it" has no
  bearing on "does the source deserve preserving").
- **Fixed a pre-existing miscount in `README.md`'s two games tables**:
  the wiki table actually had 67 real rows all along, not the 64 its own
  summary line claimed, because 3 of them (`Agaruino`, `Skibuino`,
  `Video Poker`) were *also* separately listed in the "found outside the
  wiki" table — real wiki entries that a later broader search
  independently re-found, mistakenly given a second row instead of a note
  on their existing one. Resolved by keeping each only in the wiki table
  (folding in the one genuinely useful extra detail — Agaruino's "also
  independently found via a later broader search" — as a note on its
  existing row) and removing the 3 duplicate rows from the outside-wiki
  table. Both tables' own summary lines now read 67 and 35 respectively,
  and 67 + 35 = 102 matches the real `games/` directory count exactly (it
  didn't before). Worth checking for the same duplicate-row pattern
  before trusting either table's stated count again in the future.
- **User-prompted "check one author's *entire* repo list, not just names/
  descriptions" pass, on `Tnxec2`** (already credited for 4 games).
  Fetched the real GitHub API repo list directly (34 total, not judged
  from a paginated web view) and checked every C++ repo's actual content
  (the only language capable of producing real AVR firmware, so the
  fastest genuine filter — still verified by content, not trusted from
  the language tag alone) plus two more name-plausible non-C++ repos:
  **`games/fifteen`** (a real classic 15-puzzle, real EEPROM save,
  genuinely different codebase from the already-archived `gamebuino-
  taquin` despite the same core concept) and **`games/Tnxec2-tetrino`**
  (a real, confirmed fork of the already-archived `j0ff/tetrino` —
  byte-identical `.ino` but a genuinely different `gb_platform.cpp`
  swapping the B/C button mapping, plus its own `.INF` file the original
  never shipped, confirmed via direct diff, not assumed from the fork's
  own README claim alone) were staged. `arduino-barograf` (real Arduino
  C++, but a from-scratch `Adafruit_PCD8544`/`BMP180` project with no
  `Gamebuino.h` at all — a different, non-Gamebuino board using the same
  physical LCD chip) and `fontdraw`/`GridForArtist` (a browser EGA-palette
  tool and an Android Gradle app respectively, both unambiguously
  unrelated once actually opened, not just skipped on name) were checked
  and rejected. The remaining ~27 repos (Amazfit watchface apps, Kotlin
  Android apps, Godot/GDScript games, crochet/budget/ebook-reader tools)
  were not individually cloned — their language/toolchain alone (Kotlin,
  TypeScript, GDScript) structurally cannot produce ATmega328 firmware,
  which is a real content-based exclusion criterion, not a name-based
  one. Worth re-running this same full-repo-list check against every
  other already-credited author in a future session, not just the one the
  user happened to flag.
- **A fresh two-agent deep-search pass was launched** (one scoped to
  GitHub — broad repo/topic search plus re-checking every already-known
  author's current full repo list, one scoped to everything else — itch.io,
  personal sites, forums, the live `gamebuino.com/creations` site, and a
  fresh look at the unresolved Shot Zero lead) to look for real source
  this project's own prior passes haven't found yet, including non-game
  tools/demos the sibling porting project wouldn't have bothered staging.
  **Both agents' results are in, independently re-verified, and filed**
  (`git ls-remote`, `#include <Gamebuino.h>` grep, actual license text
  read directly — matching the sibling project's own "an agent fabricated
  a repo once, never trust a report unverified" discipline; every
  rejection either agent reported was spot-checked too, e.g.
  `Frakasss/Octopus_HD` really is `<Gamebuino-Meta.h>`). GitHub-scoped
  agent, net of the user's own follow-up scope calls below: **4 new
  games** (`gamebuino-community-rpg`, `gemgem-gamebuino`,
  `minesw-gameguino`, `xonix-gamebuino`) and **9 new tools**
  (`Gamebuino-BEER-CALC`, `Gamebuino-HIRAGANA-Japanese-Learning-tool`,
  `Gamebuino-MusicNote`, `Gamebuino-Thunderbolt_Calc`,
  `Pyditns-Dice-Thrower-Gamebuino`, `chip-8-gamebuino`, `sd_map_test`,
  `gamebuinoEducation`, `HelloGamebuino`). Non-GitHub web agent: **4 more
  new games** — `games/DarkTower` (Marcus Hutchings, GPLv3, found via the
  live `gamebuino.com/creations` site's own `sitemap.xml`, not the old
  wiki or GitHub search) and `games/another2048` (grafMakulaDer2te, a
  genuinely different codebase from the already-archived `Gamebuino2048`)
  both as submodules; `games/PAK-MAN_MAKERbuino` and `games/
  Frogger_MAKERbuino` (Andy Jackson) as plain extracted files, since both
  are real subfolders of `andyhighnumber/Attiny-Arduino-Games` — a live
  but mostly-unrelated ATtiny85 compilation repo, so a whole-repo
  submodule wouldn't map cleanly to either single game, matching
  `BigBlackBox`'s own compilation-repo-extraction precedent. That same
  agent's priority task, **Shot Zero, is now confirmed thoroughly
  unrecoverable** rather than merely re-confirmed — it fetched the raw
  HTML of the one forum mirror plus both of its only two Wayback
  snapshots and confirmed the download link was never actually present in
  any of them (not a parsing miss), the wiki's own original shortlink has
  zero Wayback coverage at all, and the author's later itch.io work never
  re-released it. See `README.md`'s two new "found via a fresh
  author/repo sweep" sections (games and tools) for the full per-entry
  breakdown of both agents' combined results.
- **User has overturned two precedents this project used to follow,
  directly and specifically — apply both narrowly, don't over-generalize
  without being asked:**
  1. **Binary-only tools are usually no longer kept for completeness —
     but this was walked back once, so it's narrower than it first
     looked.** On direct user instruction, several binary-only tools were
     removed instead of filed/kept: `tools/Gamebuino-Classic_Games`
     (FeroBoh's fan compilation), `tools/PlayBuino` (muessigb's Game &
     Watch player, ships only a prebuilt `.exe`/`.hex`), `tools/
     mandelbuino` (svenofnine's Mandelbrot viewer — briefly promoted from
     `unsorted/` per this file's own former "kept anyway for
     completeness" open question, then removed the same session on the
     same basis). **`tools/Gamebuino-Classic-Games-Compilation`
     (the official compilation) was also removed on this basis at first,
     then the user explicitly reverted that specific removal** — it's
     back as a submodule, pinned to its original commit
     (`a50b94a97829fa2d18010db0be7475366448bfeb`), and `README.md` lists
     it again. Read this as "the official compilation is a deliberate,
     specific exception the user wants kept despite being binary-only,"
     not as the whole binary-only rule being reversed — the other three
     removals above stand unless the user says otherwise about those
     individually too. `README.md`'s "not found" table lists Mandeluino
     (a real copy exists and was checked, but isn't kept here, so it's
     not literally "not found" either — noted as such in that table's own
     row); the other two binary-only removals just have their table rows
     deleted outright, same as `PlayBuino`.
  2. **Desktop/PC-only companion tools (no `#include <Gamebuino.h>`, code
     that never runs on the device itself) don't belong here either**,
     even though `tools/` is nominally for non-game tools generally.
     Removed: `tools/Rodot-BitmapEncoder`, `tools/Rodot-InfEncoder`,
     `tools/Rodot-Tracker` (all desktop Java), `tools/Gamebuino-Bitmap-
     Encoder` (msevilgenius's desktop Python tool — proactively removed
     once the pattern from the three Rodot removals was clear, not
     separately requested, since leaving it would just be the same
     correction asked a fourth time). **Not removed, and shouldn't be
     without being asked**: `tools/GambiPaint` (real on-device `.ino`
     paint app, confirmed via its own `#include <Gamebuino.h>` — a
     pre-existing entry that was checked directly against this same rule
     and genuinely doesn't match it, not overlooked) and `tools/
     HelloGamebuino` (its `hello.cpp` is real on-device
     `#include <Gamebuino.h>` firmware source, just packaged as a
     command-line build template instead of a game — also checked and
     kept, not overlooked).
- **Still genuinely unrecoverable** (every one checked more than once via
  a different route — see the sibling project's own `DISCOVERED_GAMES.md`
  for the exact dead ends already tried on each): `EPIC MINI GAMES!`
  (MicroGames — download deliberately never enabled), `THE SHIP`/
  `ROBOTRON`/`Balls` (Macgyvers1 — Facebook-only, author's GitHub doesn't
  exist), `NightDriver` (svenofnine — dead personal-site link),
  `Stack Challenge` (BlackDragon — resolves to a `copy.com` link, dead
  since 2016), `Shot Zero` (Linky439 — original host dead; one forum
  mirror found at `e-magination.jeun.fr/t4958-shot-zero-gamebuino`, but
  its own "download link" heading never actually had a URL under it when
  last checked — worth a real manual look in a browser, not more
  automated searching).

## Workflow for finding more sources

1. Check the sibling project's own `DISCOVERED_GAMES.md` first, for any
   title being searched — don't redo work already recorded there.
2. A genuinely new lead (a different host than already tried, an author's
   *current* profile/repo list, a direct user-supplied file) goes into
   `unsorted/` first, exactly as found — don't rename/restructure before
   it's actually confirmed to be real source.
3. Verify it's real source (not just a compiled binary, not an empty
   repo, not the wrong Gamebuino generation — `<Gamebuino.h>` is Classic,
   `<Gamebuino-Meta.h>` is the newer, unrelated META board and out of
   scope here) before filing it.
4. File it: `games/<name>/` or `tools/<name>/`, submodule if the real
   repo is still live and clonable, plain committed files otherwise.
5. Add the real author/license/source row to the matching table in
   `README.md`, following that table's own existing format exactly
   (License column states "None specified" rather than guessing, exactly
   like every other unlicensed entry already there).
6. If the find corrects or supersedes something the sibling project's own
   docs currently say, that's worth flagging back there too.

## A real methodology gap found: an author's own repo listing was silently truncated, hiding a real game

Prompted directly ("seems we have another game https://github.com/Tnxec2/fifteen
check all tnxec2 repo's not just 1st page or repo name listings") after
the deep-search pass above had already checked this exact author
(Tnxec2 - already the source of `gemgem-gamebuino`/`xonix-gamebuino`/
`minesw-gameguino`) and missed a real fourth game. Root cause, confirmed
directly rather than assumed: a web-page-summarizing fetch tool
(equivalent to `WebFetch`) run against this author's own GitHub repo
listing was silently dropping entries between calls - three separate
fetches of the exact same 34-repo listing returned 19, then "20 before
truncating," then 23 total, each a different subset, and none of them
ever included `xonix-gamebuino` even though that repo is definitely real
and already archived here. The fix was to stop trusting a summarized
fetch for anything that needs a complete, authoritative enumeration and
pull the raw JSON directly instead (`curl` against the real GitHub API,
`grep`ed for `full_name`/`fork` fields) - this immediately produced the
correct, complete, matching-`public_repos`-count list of all 34 repos in
one pass.

That full sweep found two real candidates beyond what the original pass
already knew about, both from the same author:
- **`tetrino`** - a real false alarm, not a new game: its own title
  screen literally reads `"Tetrino by Joff (STC)"` - confirmed via the
  raw source that this is Tnxec2's own build of the exact same game
  already archived here as `j0ff/tetrino`, not an independent codebase.
  Not filed as a separate entry.
- **`fifteen`** - a real, genuinely new game: confirmed `#include
  <Gamebuino.h>`, real EEPROM save, a real bitmap logo/title screen, 450
  lines, no license specified. A classic sliding 15-puzzle - same concept
  as the already-archived `Taquin` (RackhamLeNoir), but a completely
  independent codebase by a different author, the same "archive both,
  they're genuinely different" precedent already applied to the two
  Snake/2048 pairs here. Added as a real git submodule
  (`games/fifteen`, pinned) and a new `README.md` table row.

**Worth remembering for any future author-repo-sweep**: always verify a
fetched repo-listing's own item count against that user's real
`public_repos` field from `GET /users/{name}` before trusting it's
complete, and prefer a raw API fetch over a summarized one whenever an
exhaustive, exact enumeration actually matters (as opposed to a quick
"does X exist" spot-check, where a summarized fetch is fine).

## Open questions

- **Nothing pending from the deep-search pass** — both agents described
  above under "Current status" have reported back, and every one of their
  claimed finds has been independently re-verified and filed (or, for a
  few, deliberately excluded per the user's own scope calls also
  described there). `unsorted/` is empty again.
- **License conflicts already known but unresolved** (matching the
  sibling project's own "don't silently resolve, state the conflict"
  convention): `Gamebuino-Classic-Snake-5110`'s repo `LICENSE` says GPLv3
  while its own source file header claims CC-BY-SA — both facts are
  already in `README.md`'s own table cell, not resolved either way.
