# HERWIG 7.3.0 + Full Dependency Stack on macOS Apple Silicon — Install Report

**Audience:** future AI agents and researchers who need to install, maintain,
or link software against this HERWIG installation. Written from a real
end-to-end install carried out on macOS 15 (Sequoia) on Apple Silicon (M-series)
in May 2026 against Homebrew's Clang 17 / GCC 15 / CMake 4.2.1.

This document has three jobs:

1. **History.** Record every error encountered and the technical reasoning
   behind each fix, so that the next agent is not surprised by the same
   modern-toolchain regressions on Apple Silicon.
2. **Filesystem map.** Document the absolute and relative path of every
   binary, library, header, and data file in the install, so the next agent
   knows exactly what got installed where.
3. **Linking guide.** Show how downstream code (a new physics analysis,
   another generator wrapping HERWIG, a Python binding, etc.) should
   discover and link against this installation.

The code, scripts, and patches that produced this install are committed in
`install/` next to this file. Read `README.md` for the layout and `HANDOVER.md`
for operational instructions; this file is the **why** and the **what**.

---

## 1. Why this is harder on macOS than on a Linux cluster

In the Wayne State grid (or any academic CERN/RHIC-adjacent Linux site), HERWIG
just works because the cluster admin has already absorbed the friction:

| Friction source | Linux cluster | macOS Apple Silicon |
|---|---|---|
| Compiler | GCC 11/12, libstdc++, decade of HEP testing | Apple Clang 17, libc++, very recent |
| Fortran | gfortran 9–12, lenient defaults | Homebrew gfortran 15, strict |
| C++17 enforcement | usually opt-in via `-std=c++17` | hard-default; removed symbols *gone* |
| Library install layout | `/usr` or `/cvmfs/...` | Homebrew `/opt/homebrew/Cellar/...` symlinked into `/opt/homebrew/{include,lib}` |
| `DYLD_LIBRARY_PATH` | n/a (`LD_LIBRARY_PATH` works) | stripped by SIP from system binaries; rpath baked in at link time |
| Pre-built physics libs | `module load HepMC FastJet ...` | none — every component built from source |
| `register` keyword | tolerated | removed in C++17 |
| `std::auto_ptr`, `std::random_shuffle`, `std::mem_fun`, `std::ptr_fun`, `std::bind1st/2nd`, `std::unary_function`, `std::binary_function` | tolerated as deprecated | **removed**, hard error |
| BSD vs GNU userland | GNU `make`, `libtool`, `install` | BSD `libtool` at `/usr/bin/libtool` (no `--version`!), GNU equivalents are `glibtool`, `ginstall`, `gmake` from Homebrew |

The bullet point about removed-in-C++17 standard library symbols is the single
biggest cause of HEP-software build pain on modern macOS. HERWIG, ThePEG, FastJet,
and Rivet were all designed in the early 2000s and carry idioms from C++03 and
C++98. Linux distributions still ship libstdc++ headers that retain
deprecated-but-not-removed wrappers; libc++ shipped with current Xcode does not.

**Lesson:** the install is not "broken on macOS" — it's "exposed to strictness
the same code has been getting away with for ~25 years."

---

## 2. The dependency stack

HERWIG 7 cannot run alone. It sits on top of a tower:

```
                 HERWIG 7.3.0
                      │
                  ThePEG 2.3.0
        ┌─────────────┼─────────────────────┐
        │             │                     │
     HepMC3 3.2.7   LHAPDF 6.5.4          Rivet 3.1.10
                                       ┌────┴────────┐
                                     YODA 1.9.10   FastJet 3.4.2
                                                     │
                                            FastJet-contrib 1.054
                              ┌──────────────────────┐
                              │                      │
                        Boost (system)         GSL (system)
                              │                      │
                          gfortran 15           Apple Clang 17
                          (Homebrew)              (Xcode)
```

### Version pinning rationale

| Component | Version | Why this version |
|---|---|---|
| HepMC3 | 3.2.7 | User requested HepMC3 (vs HepMC2). Latest stable at install time. |
| LHAPDF | 6.5.4 | Latest stable; clean Apple Silicon Clang build. |
| YODA | 1.9.10 | Pairs with Rivet 3.x. Rivet 4.x needs YODA 2.x — different ABI. |
| FastJet | 3.4.2 | Stable; only one C++17 source bug (D0RunIICone, easy patch). |
| FastJet-contrib | 1.054 | Required by some Rivet analyses and by HERWIG matchbox. |
| Rivet | 3.1.10 | Stable line; pairs with YODA 1.9.x. |
| ThePEG | 2.3.0 | Minimum version that supports HepMC3 (`--with-hepmcversion=3`) cleanly. |
| HERWIG | 7.3.0 | User-selected. Last release before HERWIG 7.4 (which has further C++17 cleanup). |

Newer versions of any of these may already include the patches we had to add
locally. Always check upstream changelogs first; only fall back to applying
the patches in `install/patches/` if you re-pin the same versions above.

---

## 3. Issues encountered, with full technical detail

The order below is the order they actually surfaced, because the next agent
should expect the same chronology if they re-run from scratch.

### 3.1 Boost detection — Homebrew keg-only state

**Symptom:**
```
[01-prereqs]   brew: boost already installed (boost 1.88.0)
[01-prereqs] ERROR Boost header missing at /opt/homebrew/include/boost/version.hpp
```

**Root cause:** `brew install` succeeded, but `brew link` had been blocked at
some prior point (typically by a previous Boost version's leftover symlinks).
The headers are in the Cellar (`/opt/homebrew/Cellar/boost/1.88.0/include/`)
but the canonical `/opt/homebrew/include/boost` symlink is missing. A bare
`-I/opt/homebrew/include` therefore can't see Boost.

**Fix:** ask Homebrew where `boost` actually lives via `brew --prefix boost`,
which always returns the correct directory regardless of link state. We
recorded the resolved prefix in `opt/.boost_prefix` and `opt/.gsl_prefix` and
exposed them as `HW_BOOST_PREFIX` / `HW_GSL_PREFIX`, then passed them
explicitly to `--with-boost=` and `--with-gsl=` in ThePEG and HERWIG configure.

**Generalizable rule:** on macOS, never assume `$HOMEBREW_PREFIX/include/<pkg>`
exists. Always use `brew --prefix <pkg>` to resolve a package's real location.

### 3.2 CMake 4.x dropped sub-3.5 compatibility

**Symptom:**
```
CMake Error at CMakeLists.txt:1 (cmake_minimum_required):
  Compatibility with CMake < 3.5 has been removed from CMake.
  Or, add -DCMAKE_POLICY_VERSION_MINIMUM=3.5 to try configuring anyway.
```

**Root cause:** HepMC3 3.2.7's `CMakeLists.txt` declares
`cmake_minimum_required(VERSION 3.0)`. CMake 4.x (released 2024) removed the
shim that pretended to be a CMake 3.0–3.4. Homebrew's CMake is the new line.

**Fix:** pass `-DCMAKE_POLICY_VERSION_MINIMUM=3.5` on the configure line. This
is the official escape hatch documented in CMake's own error message. We also
nuke any stale `CMakeCache.txt` from a failed prior configure before retrying,
because CMake's cache invalidation is conservative and a partial-cache state
can survive.

**Only HepMC3 in this stack uses CMake.** The rest are autoconf, so this fix
is a one-shot.

### 3.3 FastJet `_Et` typo — strict template lookup on Clang 17

**Symptom (in `D0RunIIConePlugin`):**
```
./ProtoJet.hpp:198:51: error: no member named '_Et' in 'ProtoJet<Item>'
  os<<"y phi Et = ("<<_y<<", "<<_phi<<", "<<this->_Et<<")"<<std::endl;
```

**Root cause:** `ProtoJet<Item>` has members `_y`, `_phi`, `_pT` — but **no
`_Et`**. The author wrote a debug-print line that referenced a non-existent
member. Older Clang's two-phase template name lookup was lenient enough to
accept it (the lookup deferred to instantiation, and the failing path was
never instantiated in the test suite). Apple Clang 17 with stricter lookup
catches it.

**Fix:** `this->_Et` → `_pT`. This is a debug print called only when
explicitly enabled, so behaviour is unchanged. Patch saved in
`install/patches/fastjet-protojet-Et-typo.patch`.

### 3.4 Rivet `.template setAnnotation` — explicit args required

**Symptom (in `Analysis.cc`):**
```
Analysis.cc:1148:34: error: a template argument list is expected after a
                     name prefixed by the template keyword
                     [-Wmissing-template-arg-list-after-template-kw]
      if (needsDP)  yao.template setAnnotation("WriterDoublePrecision", "1");
```

**Root cause:** when calling a template member function on a dependent type,
C++ allows two equivalent forms:
- `obj.template foo<T>(args)` — explicit args, `template` disambiguator required
- `obj.foo(args)` — args deduced, no `template` keyword needed

Apple Clang 17 elevates the long-standing warning to an error: if you write
`.template name`, you must follow it with `<...>`. Older Clang accepted
`.template name(args)` (no `<>`) and deduced.

**Fix:** drop the `template` keyword. The compiler deduces the parameter from
the runtime arguments. Equivalent semantics, portable across compilers.

### 3.5 Rivet install-data-local recursive doc build

**Symptom:**
```
Making install in doc
make[3]: *** No rule to make target `anaindex/analyses.dat', needed by `dat'.
make[2]: *** [install-data-local] Error 2
```

**Root cause:** Rivet's top-level `Makefile.am` runs `$(MAKE) -C doc dat json`
inside `install-data-local`. The `dat` and `json` targets in `doc/Makefile.am`
are **only generated when `ENABLE_PYEXT`** is true. We configured Rivet with
`--disable-pyext` (to avoid a Cython version war on macOS), so those targets
have no recipe and `make` fails.

**Fix:** prefix the failing recipe line with `-` (GNU make's "ignore exit
code" marker). The two `install_sh_DATA` lines that follow already use
`|| true`. Doc index isn't required at runtime.

### 3.6 The "build dir is stale relative to src" trap

**Symptom:** patches applied to `src/` were not visible during the next
`make` because the `build/` dir had been staged earlier with the unpatched
source, and `configure` regenerated `build/Makefile` from `build/Makefile.in`
(its own copy, not the freshly patched one in `src/`).

**Fix (structural, not a single patch):** added a `sync_src_to_build` helper
to `install/lib/patch.sh` that rsyncs source files from `src/` into an
existing `build/` dir, excluding compile artefacts (`*.o`, `*.lo`, `.libs/`,
`.deps/`, `config.status`, generated `Makefile`s). Wired this helper into
`05-fastjet.sh`, `07-rivet.sh`, `08-thepeg.sh`, `09-herwig.sh` to run after
`apply_patches`. Patches now propagate to in-progress builds without losing
compile state.

### 3.7 ThePEG `std::mem_fun`

**Symptom:**
```
include/ThePEG/Config/std.h:101:12: error: no member named 'mem_fun' in
                                    namespace 'std'; did you mean 'mem_fn'?
  101 | using std::mem_fun;
```

**Root cause:** `std::mem_fun` was deprecated in C++11 and **removed in C++17**.
ThePEG's `Config/std.h` aliases it into a project-wide using-set.

**Fix:** comment out the line. A defensive scan confirmed **no ThePEG source
actually references `mem_fun`** — only the using-declaration imports it. The
adjacent `using std::mem_fn` (the modern replacement) was already there.

### 3.8 thepeg-config quirks: no --version, exit-code lying

**Symptom:** `thepeg-config --prefix` printed the right prefix and exited 1.

**Root cause:** thepeg-config is a hand-written shell script that for each
flag does `tmp=$(echo "$*" | egrep -- '--\<flag\>'); test -n "$tmp" && echo
<value>`. The script has no `exit` at the end. Bash's exit code therefore
reflects the *last* `test -n` to run, which fails for any flag the user
didn't pass. Under `set -e` callers see a "failure".

**Fix to my code:** check stdout, not exit code. Capture, assert non-empty,
verify the printed `--libdir` is a real directory. The `thepeg-config`
behaviour itself is upstream and we don't patch it; we just don't trust its
exit code.

**Lesson:** `*-config` scripts in HEP land are not standardized. `lhapdf-config`,
`fastjet-config`, `rivet-config`, `yoda-config`, `HepMC3-config`,
`herwig-config` all support `--version`; `thepeg-config` does not.

### 3.9 HERWIG `std::random_shuffle`

**Symptom (4 sites in 2 files):**
```
HwRemDecayer.cc:1567:5: error: use of undeclared identifier 'random_shuffle'
ColourReconnector.cc:{237,289,434}: same
```

**Root cause:** `std::random_shuffle` was deprecated in C++14 and **removed
in C++17**. HERWIG uses the 3-argument form with `UseRandom::irnd` (a
`long(long)` RNG that returns an integer in `[0, N)`). The replacement
`std::shuffle` takes a *UniformRandomBitGenerator*, a different concept.

**Fix:** define a local Fisher-Yates template named `random_shuffle` in an
anonymous namespace at the top of each affected `.cc` file. The 4 call sites
are unchanged, the project's RNG seeding is preserved bit-for-bit, and we
avoid rewiring `UseRandom` to the C++11 random-engine concept.

### 3.10 HERWIG looptools — gfortran 15 strictness

**Symptom (in `D/D0funcC.F`):**
```
parameter (nz2 = -2147483648)
Error: Integer too big for its kind at (1).
        This check can be disabled with the option '-fno-range-check'
```

**Root cause:** gfortran parses `-2147483648` as `-(2147483648)`, and
`2147483648` overflows a 32-bit signed int by one. Older gfortran tolerated
it; gfortran 10+ rejects. HERWIG's looptools code is from the early 2000s.

**Fix:** add legacy-Fortran flags to `FFLAGS` and `FCFLAGS`:
```
-fno-range-check
-fallow-argument-mismatch
-fallow-invalid-boz
```
These are the canonical "modern gfortran on legacy Fortran" escape hatches.
Set in `install/lib/env.sh` and explicitly passed through to `configure`
(initial scripts had only forwarded `CXXFLAGS`/`LDFLAGS`/`CPPFLAGS` —
critical detail).

### 3.11 HERWIG configure forgetting to forward FFLAGS

**Symptom:** even after adding the legacy-Fortran flags to env.sh, the
Fortran build still failed.

**Root cause:** `09-herwig.sh`'s `./configure` invocation listed
`CC=$CC CXX=$CXX FC=$FC CXXFLAGS=... LDFLAGS=... CPPFLAGS=...` but
**no `FFLAGS` or `FCFLAGS`**. autoconf only picks up what you tell it; the
exported env-var path doesn't help here because configure runs in a subshell
and writes the resolved flag values into Makefiles.

**Fix:** explicit `FFLAGS='$FFLAGS' FCFLAGS='$FCFLAGS' F77='$FC'` on the
configure command line. Same fix to `08-thepeg.sh` for symmetry.

### 3.12 HERWIG default repository wants CT14nlo

**Symptom (in install-data-hook, after files are installed):**
```
Creating repository
Error: 'set /Herwig/Partons/HardNLOPDF:PDFName CT14nlo':
       PDF not installed. Try 'lhapdf install'.
```

**Root cause:** HERWIG's defaults reference NLO PDFs at repository-creation
time. We had only fetched `CT14lo` and `NNPDF31_nnlo_as_0118`.

**Fix:** added `CT14nlo`, `MMHT2014lo68cl`, `MMHT2014nlo68cl` to the PDF set
list in `03-lhapdf.sh`, and wrote a one-shot `install/fetch-missing-pdfs.sh`
helper to grab them on existing installs.

**Generalizable rule:** any PDF set referenced in
`opt/share/Herwig/defaults/*.in` must be on disk under
`opt/share/LHAPDF/<set>/` before the first `Herwig read`.

### 3.13 Smoke test LEP.in path

**Symptom:** the script aborted between `verify-bin` (which printed
`Herwig 7.3.0 / ThePEG 2.3.0` cleanly) and the smoke test, with no per-step
log because the abort happened in plain shell logic, not a `log_step`.

**Root cause:** my path detection used a `find ... | head -1 | read -r f`
pipeline and `set -e -o pipefail`. When `find` produces no matches, `read`
returns 1, pipefail propagates, script aborts. Also, the candidate paths
(`share/Herwig/defaults/LEP.in`) were wrong — the shipped LEP example lives
at the top of `share/Herwig/`, not under `defaults/`.

**Fix:** explicit candidate list, with `find -print -quit ... || true` as
the safe fallback. No more pipefail-induced suicide.

---

## 4. The repository — exact filesystem layout

All paths are absolute. The install root is:

```
/Users/oveissheibani/LocalHerwig/LocalHerwig/opt
```

Throughout this section we abbreviate it as `$HW_PREFIX`. Define it in your
shell:

```bash
export HW_PREFIX=/Users/oveissheibani/LocalHerwig/LocalHerwig/opt
```

### 4.1 Executables — `$HW_PREFIX/bin/`

```
$HW_PREFIX/bin/
├── HepMC3-config              # HepMC3 build/link helper
├── Herwig                     # main HERWIG driver (what users call)
├── herwig-config              # HERWIG build/link helper
├── herwig-makedistributions   # post-run analysis tools
├── herwig-mergegrids          #   "
├── herwig-combineruns         #   "
├── herwig-combinedistributions
├── ufo2herwig                 # UFO → Herwig model converter
├── mg2herwig                  # MadGraph → Herwig converter
├── slha2herwig                # SLHA spectrum → Herwig
├── gosam2herwig               # GoSam loop integration
├── runThePEG                  # ThePEG event runner (lower-level than Herwig)
├── setupThePEG                # ThePEG repository setup
├── thepeg-config              # ThePEG build/link helper (NOTE: no --version)
├── fastjet-config             # FastJet build/link helper
├── lhapdf                     # LHAPDF Python tool (PDF management)
├── lhapdf-config              # LHAPDF build/link helper
├── make-plots                 # Rivet plotting
├── rivet-build                # build a Rivet analysis plugin
├── rivet-buildplugin          # alternative plugin builder
├── rivet-config               # Rivet build/link helper
└── yoda-config                # YODA build/link helper
```

### 4.2 Shared libraries — `$HW_PREFIX/lib/`

Top-level `*.dylib` (linked against directly by C++ users):

| File | Provides | Soname | Notes |
|---|---|---|---|
| `libHepMC3.dylib` → `libHepMC3.3.dylib` | HepMC3 event record | `libHepMC3.3.dylib` | Versioned symlink chain |
| `libHepMC3search.dylib` → `libHepMC3search.5.dylib` | HepMC3 search helpers | `libHepMC3search.5.dylib` | |
| `libHepMC3-static.a` | HepMC3 static archive | n/a | Same code as dylib |
| `libLHAPDF.dylib` | LHAPDF C++ API | `libLHAPDF.6.dylib` | (libLHAPDF.la libtool descriptor present) |
| `libLHAPDF.a` | LHAPDF static archive | n/a | |
| `libfastjet.dylib` → `libfastjet.0.dylib` | FastJet core | `libfastjet.0.dylib` | |
| `libfastjet.a` | FastJet static archive | n/a | |
| `libsiscone*.dylib`, `libfastjetplugins.dylib`, `libfastjettools.dylib` | FastJet plugins/tools | | Loaded via FastJet |
| `libfastjetcontribfragile.dylib` | FastJet-contrib aggregate | | Single shared object for all contribs |
| `libCentauro.a`, `libConstituentSubtractor.a`, `libEnergyCorrelator.a`, `libFlavorCone.a`, `libGenericSubtractor.a`, `libJetCleanser.a`, `libJetFFMoments.a`, `libJetsWithoutJets.a`, `libLundPlane.a`, `libNsubjettiness.a`, `libQCDAwarePlugin.a`, `libRecursiveTools.a`, `libScJet.a`, `libSignalFreeBackgroundEstimator.a`, `libSoftKiller.a`, `libSubjetCounting.a`, `libValenciaPlugin.a`, `libVariableR.a`, `libClusteringVetoPlugin.a` | individual FJ-contrib algorithms | | Static; linked into `libfastjetcontribfragile.dylib` |
| `libYODA.dylib` | YODA histogram library | | |
| `libRivet.dylib` | Rivet core | | |

ThePEG and Herwig install their core libraries under **subdirectories**:

```
$HW_PREFIX/lib/ThePEG/    — 162 plugin .so files + the main ThePEG library
$HW_PREFIX/lib/Herwig/    — 193 plugin .so files including Herwig.so (the core)
$HW_PREFIX/lib/Rivet/     — 25 RivetXxxAnalyses.so plugin libraries
```

Notable ThePEG plugins (sample): `ACDCSampler.so`, `BreitWignerMass.so`,
`BudnevPDF.so`, `ColourPairDecayer.so`, `DalitzDecayer.so`, …

Notable Herwig plugins (sample): `Herwig.so` (core, 27.so versioned),
`Hw64Decay.so`, `HwADDModel.so`, `HwAnalysis.so`, …

`Rivet/Rivet*Analyses.so`: ALICE, ATLAS, BABAR, BELLE, BES, CDF, CESR, CMS,
D0, DORIS, Frascati, HERA, LEP, LHCb, LHCf, LHC (generic), MC, Misc,
Novosibirsk, Orsay, Petra, RHIC, SLAC, SPS, TOTEM, Tristan.

### 4.3 Headers — `$HW_PREFIX/include/`

Each top-level subdirectory mirrors an upstream's header tree:

```
$HW_PREFIX/include/
├── HepMC3/        — HepMC3 headers   (#include "HepMC3/GenEvent.h" etc.)
├── LHAPDF/        — LHAPDF headers   (#include "LHAPDF/LHAPDF.h")
├── fastjet/       — FastJet headers  (#include "fastjet/PseudoJet.hh")
├── siscone/       — SISCone helpers  (used by FastJet plugins)
├── YODA/          — YODA headers     (#include "YODA/Histo1D.h")
├── Rivet/         — Rivet headers    (#include "Rivet/Analysis.hh")
├── ThePEG/        — ThePEG headers   (#include "ThePEG/EventRecord/Event.h")
└── Herwig/        — HERWIG headers   (#include "Herwig/Shower/...")
```

### 4.4 Data — `$HW_PREFIX/share/`

```
$HW_PREFIX/share/
├── HepMC3/        — minimal data (mostly examples)
├── LHAPDF/
│   ├── lhapdf.conf            ← LHAPDF runtime config
│   ├── pdfsets.index          ← list of all known PDF sets
│   ├── CT14lo/                ← LO PDF set (ascii data)
│   ├── CT14nlo/               ← NLO PDF set
│   ├── MMHT2014lo68cl/
│   ├── MMHT2014nlo68cl/
│   └── NNPDF31_nnlo_as_0118/
├── fastjet/       — examples
├── YODA/          — reference data
├── Rivet/         — reference YODA files for ATLAS/CMS/etc analyses
├── ThePEG/        — defaults, snippets, repository data
├── Herwig/
│   ├── HerwigDefaults.rpo     ← BINARY default repository (created by install-data-hook)
│   ├── LEP.in, LHC.in, …      ← shipped input examples
│   ├── defaults/              ← *.in fragments loaded into the repo at build time
│   ├── snippets/              ← user-includable fragments (Rivet.in, EECollider.in, …)
│   ├── Matchbox/              ← NLO matching config templates
│   ├── Merging/               ← merging samples
│   └── Doc/                   ← reference manual files
└── doc/           — assorted docs
```

### 4.5 pkg-config metadata

Three components ship `.pc` files:

```
$HW_PREFIX/lib/pkgconfig/
├── lhapdf.pc
├── rivet.pc
└── yoda.pc
```

**HepMC3, FastJet, ThePEG, and HERWIG do not ship pkg-config files.** Use
their respective `*-config` shell helpers instead (see §5).

### 4.6 Environment-loader script

`$HW_PREFIX/herwig-env.sh` — generated by `install/01-prereqs.sh`. Source it
to set `PATH`, `PKG_CONFIG_PATH`, `DYLD_FALLBACK_LIBRARY_PATH`, and
`LHAPDF_DATA_PATH` for downstream use.

```bash
source /Users/oveissheibani/LocalHerwig/LocalHerwig/opt/herwig-env.sh
```

### 4.7 Source and build trees (relative to repository root)

These are **not** part of the install. They are kept for re-patching and
incremental rebuilds.

```
LocalHerwig/
├── opt/                          ← THE INSTALL (everything in §4.1–§4.6)
├── src/                          ← upstream source trees (patched in place)
│   ├── hepmc3-3.2.7/
│   ├── lhapdf-6.5.4/
│   ├── yoda-1.9.10/
│   ├── fastjet-3.4.2/
│   ├── fjcontrib-1.054/
│   ├── rivet-3.1.10/
│   ├── thepeg-2.3.0/
│   └── herwig-7.3.0/
├── build/                        ← out-of-source build dirs (mirror of src)
│   └── (same layout)
├── logs/                         ← per-step + master log
├── install/                      ← orchestration scripts (run these)
│   ├── 00-bootstrap.sh
│   ├── 01-prereqs.sh
│   ├── 02-hepmc3.sh
│   ├── 03-lhapdf.sh
│   ├── 04-yoda.sh
│   ├── 05-fastjet.sh
│   ├── 06-fjcontrib.sh
│   ├── 07-rivet.sh
│   ├── 08-thepeg.sh
│   ├── 09-herwig.sh
│   ├── fetch-missing-pdfs.sh
│   ├── diagnose.sh
│   ├── lib/
│   │   ├── env.sh
│   │   ├── log.sh
│   │   └── patch.sh
│   └── patches/                  ← all macOS source patches; see §3
└── smoke-test/                   ← LEP example output (post-install verification)
```

---

## 5. Linking against this HERWIG — guide for downstream software

This is the section the next AI agent will spend the most time in. Suppose
your job is: "I have a new C++ program (or another generator wrapper, or
a Python binding) that needs to call into this HERWIG installation. How do I
build it so it links cleanly?"

### 5.1 Authoritative path query — use the *-config tools

**Always prefer the install's own `*-config` helpers over hardcoded paths.**
They encode the right include and library directories, plus any deps, and
will continue to work if the install moves.

```bash
# Compile flags
HEPMC3_CXX=$($HW_PREFIX/bin/HepMC3-config --cflags)         # -I.../include
LHAPDF_CXX=$($HW_PREFIX/bin/lhapdf-config --cflags)
FASTJET_CXX=$($HW_PREFIX/bin/fastjet-config --cxxflags)
YODA_CXX=$($HW_PREFIX/bin/yoda-config --cppflags)
RIVET_CXX=$($HW_PREFIX/bin/rivet-config --cppflags)
THEPEG_CXX=$($HW_PREFIX/bin/thepeg-config --cppflags)
HERWIG_CXX=$($HW_PREFIX/bin/herwig-config --cppflags)

# Link flags
HEPMC3_LD=$($HW_PREFIX/bin/HepMC3-config --libs)            # -L.../lib -lHepMC3 ...
LHAPDF_LD=$($HW_PREFIX/bin/lhapdf-config --ldflags)
FASTJET_LD=$($HW_PREFIX/bin/fastjet-config --libs)
YODA_LD=$($HW_PREFIX/bin/yoda-config --ldflags)
RIVET_LD=$($HW_PREFIX/bin/rivet-config --ldflags)
THEPEG_LD=$($HW_PREFIX/bin/thepeg-config --ldflags)
HERWIG_LD=$($HW_PREFIX/bin/herwig-config --ldflags)

THEPEG_LIBS=$($HW_PREFIX/bin/thepeg-config --ldlibs)        # -lz -lreadline ...
HERWIG_LIBS=$($HW_PREFIX/bin/herwig-config --ldlibs)        # -lThePEG
```

**Caveat (covered in §3.8):** `thepeg-config` lies about its exit code. Always
capture stdout and check it's non-empty; never use `&&` after it.

### 5.2 The ground truth — what the *-config tools resolve to

For reproducibility, here is what each helper returns on this install
(verified live):

```
HepMC3-config --prefix     → $HW_PREFIX
fastjet-config --prefix    → $HW_PREFIX
fastjet-config --cxxflags  → -I$HW_PREFIX/include
fastjet-config --libs      → -L$HW_PREFIX/lib -lfastjettools -lfastjet -lm
                             -lfastjetplugins -lsiscone_spherical -lsiscone
                             -lfastjetcontribfragile -lfastjettools

lhapdf-config --prefix     → $HW_PREFIX
lhapdf-config --cflags     → -I$HW_PREFIX/include
lhapdf-config --ldflags    → -L$HW_PREFIX/lib -lLHAPDF

yoda-config --prefix       → $HW_PREFIX
yoda-config --cppflags     → -I$HW_PREFIX/include
yoda-config --libs         → -L$HW_PREFIX/lib -lYODA

rivet-config --prefix      → $HW_PREFIX
rivet-config --cppflags    → -I$HW_PREFIX/include -I.../HepMC3/include
                              -I.../YODA/include -I.../fastjet/include
rivet-config --libs        → -L$HW_PREFIX/lib -lRivet  (+ deps)

thepeg-config --prefix     → $HW_PREFIX
thepeg-config --libdir     → $HW_PREFIX/lib/ThePEG          ← NOTE: subdir
thepeg-config --includedir → $HW_PREFIX/include
thepeg-config --datadir    → $HW_PREFIX/share/ThePEG
thepeg-config --cppflags   → -I$HW_PREFIX/include
                              -I/opt/homebrew/opt/boost/include
                              -I/opt/homebrew/opt/gsl/include
thepeg-config --ldflags    → -L/opt/homebrew/lib
                              -L$HW_PREFIX/lib
                              -L/opt/homebrew/lib/gcc/15
                              -Wl,-rpath,$HW_PREFIX/lib
                              -Wl,-rpath,/opt/homebrew/lib
                              -Wl,-rpath,/opt/homebrew/lib/gcc/15
                              -L/opt/homebrew/lib/gcc/15 -lgfortran -lquadmath
thepeg-config --ldlibs     → -lz -lreadline

herwig-config --prefix     → $HW_PREFIX
herwig-config --libdir     → $HW_PREFIX/lib/Herwig            ← NOTE: subdir
herwig-config --datadir    → $HW_PREFIX/share/Herwig
herwig-config --cppflags   → -I$HW_PREFIX/include (×2)
                              -I/opt/homebrew/opt/boost/include
                              -I/opt/homebrew/opt/gsl/include
herwig-config --ldflags    → -L$HW_PREFIX/lib/ThePEG
herwig-config --ldlibs     → -lThePEG
```

**Critical observation #1:** `thepeg-config --libdir` returns
`$HW_PREFIX/lib/ThePEG`, **not** `$HW_PREFIX/lib`. ThePEG installs its plugins
into a subdirectory. If you hand-write `-L$HW_PREFIX/lib`, you will *not* find
ThePEG plugin libraries — you'll find HepMC3, FastJet, Rivet, YODA, LHAPDF
top-level libs.

**Critical observation #2:** the same is true for HERWIG —
`herwig-config --libdir` returns `$HW_PREFIX/lib/Herwig`. The core
`Herwig.so` and all its decay/shower/model plugins live there, not at
`$HW_PREFIX/lib`.

**Critical observation #3:** `thepeg-config --ldflags` already bakes in
**rpath** entries for `$HW_PREFIX/lib`, `/opt/homebrew/lib`, and
`/opt/homebrew/lib/gcc/15`. Use it as-is. Do not strip the rpath bits
"to clean up the link line" — they are why your binary will find shared libs
at runtime on macOS where `DYLD_LIBRARY_PATH` is unreliable.

### 5.3 Minimal C++ example linked against the full stack

Here is a self-contained `Makefile` for a downstream project that uses HepMC3,
FastJet, LHAPDF, ThePEG, and HERWIG. Every flag derives from `*-config`:

```make
HW_PREFIX := /Users/oveissheibani/LocalHerwig/LocalHerwig/opt

CXX      := /usr/bin/clang++
CXXSTD   := -std=c++17
OPT      := -O2 -g -fPIC

HEPMC3_CXX  := $(shell $(HW_PREFIX)/bin/HepMC3-config --cflags)
HEPMC3_LD   := $(shell $(HW_PREFIX)/bin/HepMC3-config --libs)
LHAPDF_CXX  := $(shell $(HW_PREFIX)/bin/lhapdf-config --cflags)
LHAPDF_LD   := $(shell $(HW_PREFIX)/bin/lhapdf-config --ldflags)
FASTJET_CXX := $(shell $(HW_PREFIX)/bin/fastjet-config --cxxflags)
FASTJET_LD  := $(shell $(HW_PREFIX)/bin/fastjet-config --libs)
THEPEG_CXX  := $(shell $(HW_PREFIX)/bin/thepeg-config --cppflags)
THEPEG_LD   := $(shell $(HW_PREFIX)/bin/thepeg-config --ldflags)
THEPEG_LIBS := $(shell $(HW_PREFIX)/bin/thepeg-config --ldlibs)
HERWIG_CXX  := $(shell $(HW_PREFIX)/bin/herwig-config --cppflags)
HERWIG_LD   := $(shell $(HW_PREFIX)/bin/herwig-config --ldflags)
HERWIG_LIBS := $(shell $(HW_PREFIX)/bin/herwig-config --ldlibs)

CXXFLAGS := $(CXXSTD) $(OPT) \
            $(HEPMC3_CXX) $(LHAPDF_CXX) $(FASTJET_CXX) $(THEPEG_CXX) $(HERWIG_CXX)

LDFLAGS  := $(THEPEG_LD) $(HERWIG_LD) $(LHAPDF_LD) \
            -L$(HW_PREFIX)/lib \
            -Wl,-rpath,$(HW_PREFIX)/lib \
            -Wl,-rpath,$(HW_PREFIX)/lib/ThePEG \
            -Wl,-rpath,$(HW_PREFIX)/lib/Herwig

LDLIBS   := $(HERWIG_LIBS) $(THEPEG_LIBS) \
            $(FASTJET_LD) $(HEPMC3_LD) -lYODA -lRivet

myprog: myprog.cc
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)
```

### 5.4 Runtime PDF data discovery

LHAPDF locates its data sets via the `LHAPDF_DATA_PATH` environment variable.
Set it before running anything that uses PDFs:

```bash
export LHAPDF_DATA_PATH=$HW_PREFIX/share/LHAPDF
```

`herwig-env.sh` sets this automatically. Without it, `LHAPDF::mkPDF("CT14lo")`
will throw `PDF not found in any path`.

Available sets on this install: `CT14lo`, `CT14nlo`, `MMHT2014lo68cl`,
`MMHT2014nlo68cl`, `NNPDF31_nnlo_as_0118`. Add more with:
```bash
$HW_PREFIX/bin/lhapdf install <SET_NAME>
```

### 5.5 Runtime plugin loading (HERWIG / ThePEG)

HERWIG loads plugin libraries dynamically by name. The plugin search path is
controlled by ThePEG's repository, not by `DYLD_LIBRARY_PATH`. The defaults
are baked into `$HW_PREFIX/share/Herwig/HerwigDefaults.rpo` to look in
`$HW_PREFIX/lib/Herwig/` and `$HW_PREFIX/lib/ThePEG/`.

If you move the install or want to use a non-default plugin directory, edit
`$HW_PREFIX/share/Herwig/defaults/HerwigDefaults.in` (the source of the .rpo)
or pass `-L<dir>` to the `Herwig` command. Do **not** copy plugins out of
their subdirectories — Herwig probes both directories independently.

To load HERWIG into your own program via dlopen-style injection:

```cpp
#include <dlfcn.h>
void* handle = dlopen(
    "/Users/oveissheibani/LocalHerwig/LocalHerwig/opt/lib/Herwig/Herwig.so",
    RTLD_NOW | RTLD_GLOBAL);
if (!handle) std::cerr << dlerror() << "\n";
```

`RTLD_GLOBAL` is important if you intend to subsequently load Herwig plugins
that depend on Herwig's global symbol table. With `RTLD_LOCAL` the plugins
won't see Herwig's classes and will fail to register.

### 5.6 What other code (yours or third-party) must do at startup

If your program is *not* `Herwig` itself but uses ThePEG/Herwig classes:

1. Construct or load a `ThePEG::PersistentIStream` from a `.run` file or
   programmatically initialize a `ThePEG::Repository`.
2. Ensure ThePEG can find its plugins: either set
   `THEPEG_REPO_DIR=$HW_PREFIX/share/ThePEG` (rare) or call
   `ThePEG::DynamicLoader::appendPath("$HW_PREFIX/lib/ThePEG")` and the
   same for `lib/Herwig`.
3. Set `LHAPDF_DATA_PATH=$HW_PREFIX/share/LHAPDF` for any PDF use.
4. If you want Rivet analyses inside your program, set
   `RIVET_ANALYSIS_PATH=$HW_PREFIX/lib/Rivet` and `RIVET_DATA_PATH=$HW_PREFIX/share/Rivet`.

The cleanest way to get all of this for free is:

```bash
source $HW_PREFIX/herwig-env.sh
```

before you launch anything. It's just `export` lines; safe to re-source.

### 5.7 Compiler ABI notes (read before mixing with system code)

Everything in this install was built with **Apple Clang 17 / libc++**. If you
link against this install, **you must also build your code with libc++**, not
libstdc++. On macOS this is the default for `clang++`; on Homebrew GCC
(`g++-15`) it is **not** — you would need `-stdlib=libc++` and matching
runtime flags, and even then class-template ABI mismatches can occur. Stick
with Apple Clang.

The Fortran runtime is gfortran 15's `libgfortran` and `libquadmath`, located
at `/opt/homebrew/lib/gcc/15`. ThePEG's `--ldflags` already adds this dir to
both `-L` and `-rpath`; if you bypass `thepeg-config` you must add it
manually.

C++ standard: **C++17**. HERWIG and ThePEG both require it. Building
downstream code with `-std=c++14` or earlier will fail to find the headers'
`if constexpr` and `std::optional` machinery.

---

## 6. Verification commands

Quick checks that the install is healthy:

```bash
# 1. Tools respond
$HW_PREFIX/bin/Herwig --version              # Herwig 7.3.0 / ThePEG 2.3.0
$HW_PREFIX/bin/herwig-config --prefix
$HW_PREFIX/bin/thepeg-config --prefix
$HW_PREFIX/bin/lhapdf-config --version
$HW_PREFIX/bin/fastjet-config --version
$HW_PREFIX/bin/HepMC3-config --version

# 2. Default repository is loadable
ls -l $HW_PREFIX/share/Herwig/HerwigDefaults.rpo

# 3. PDFs are present
$HW_PREFIX/bin/lhapdf list --installed

# 4. End-to-end smoke test
mkdir -p ~/herwig-smoke && cd ~/herwig-smoke
cp $HW_PREFIX/share/Herwig/LEP.in .
$HW_PREFIX/bin/Herwig read LEP.in            # produces LEP.run
$HW_PREFIX/bin/Herwig run LEP.run --numevents 100
ls -lh                                       # expect LEP.yoda or .hepmc output
```

If `Herwig run` produces a non-zero-byte output file, the install is fully
operational.

---

## 7. Update / re-run / debugging cheatsheet

```bash
cd /Users/oveissheibani/LocalHerwig/LocalHerwig

# Re-run a single component (after editing its script)
./install/00-bootstrap.sh --only 08-thepeg

# Resume from a specific step after fixing a problem
./install/00-bootstrap.sh --from 09-herwig

# Skip a step you don't want re-run
./install/00-bootstrap.sh --skip 03-lhapdf

# See what passed/failed across all components
./install/diagnose.sh
./install/diagnose.sh --tail            # also dump last 30 lines of each fail

# Live tail during a build
tail -f logs/master.log

# Find a specific compile error
grep -n "error:" logs/<component>-build-*.log | head
```

When a future macOS toolchain bump breaks a component, the workflow is:

1. Read the failing per-step log under `logs/`.
2. Edit the offending file under `src/<component>-<version>/`.
3. From that source dir, capture the diff:
   ```bash
   cd src/herwig-7.3.0
   git diff -- relative/path/to/file.cc > ../../install/patches/herwig-<reason>.patch
   ```
   (or `diff -u` if not a git checkout)
4. Add a `# Reason:` header to the patch with the upstream issue link.
5. Re-run the component:
   ```bash
   ./install/00-bootstrap.sh --from <component>
   ```
   `apply_patches` will detect the patch is already applied (via reverse
   dry-run) and skip cleanly. `sync_src_to_build` propagates the patch to
   the existing `build/` dir without losing compile state.

---

## 8. Caveats and known limitations

- **No external NLO loop providers.** GoSam, OpenLoops, MadGraph aMC@NLO
  are *not* installed. HERWIG's matchbox infrastructure works for the
  built-in matrix elements; loading external one-loop providers requires
  installing them separately (each has its own macOS build challenges).
- **No EvtGen.** B-physics decay simulations through EvtGen are unavailable.
  Add EvtGen and reconfigure HERWIG with `--with-evtgen=...` if needed.
- **Rivet built without Python extensions** (`--disable-pyext`). This was a
  deliberate choice to avoid the macOS Cython version dance. You can still
  run Rivet analyses from C++ and from `rivet` command-line; you cannot
  `import rivet` in Python. Same applies to YODA.
- **HERWIG built without HJets, OpenLoops integration, MadGraph integration.**
  Re-configure with the relevant `--with-...=` flags after installing those
  providers.
- **PDF coverage is intentionally minimal.** Five PDF sets: enough for
  defaults to load. Add more with `lhapdf install <SET>` as needed by your
  physics study.
- **No Doxygen documentation built.** `share/Herwig/Doc/` contains source
  for the reference manual but not generated HTML. Run `make doc` inside
  `build/herwig-7.3.0/Doc/` if you want the manual locally.

---

## 9. Versions of everything (May 2026 install)

```
macOS                15.x (Sequoia)
arch                 arm64 (Apple Silicon, M-series)
Xcode CLT            (Apple Clang 17, libc++)
Homebrew prefix      /opt/homebrew
gcc (Homebrew)       15.2.0 (provides gfortran-15)
cmake                4.2.1
autoconf             2.72
automake             1.18.1
libtool (GNU)        2.5.4 (as glibtool)
gsl                  2.8
boost                1.88.0
python (Homebrew)    3.12.13
pkg-config (pkgconf) 2.5.1
git                  2.54.0
wget                 1.25.0
```

Component versions: HepMC3 3.2.7, LHAPDF 6.5.4, YODA 1.9.10, FastJet 3.4.2,
FastJet-contrib 1.054, Rivet 3.1.10, ThePEG 2.3.0, HERWIG 7.3.0.

---

## Appendix A — patch index

Every patch under `install/patches/` includes a `# Reason:` header. Quick
table:

| Patch file | Component | What it fixes |
|---|---|---|
| `fastjet-protojet-Et-typo.patch` | FastJet 3.4.2 | `_Et` → `_pT` typo in `D0RunIICone/ProtoJet.hpp` |
| `rivet-template-kw-clang17.patch` | Rivet 3.1.10 | Drop `template` keyword in `Analysis::_setWriterPrecision` |
| `rivet-install-data-local-tolerant.patch` | Rivet 3.1.10 | `-` prefix on `make -C doc dat json` recipe |
| `thepeg-cpp17-remove-mem_fun.patch` | ThePEG 2.3.0 | Drop `using std::mem_fun` (removed in C++17) |
| `herwig-cpp17-random_shuffle.patch` | HERWIG 7.3.0 | Local Fisher-Yates `random_shuffle` template (removed in C++17) |

To re-apply on a fresh source extract: the install scripts call
`apply_patches <component> <src_dir>` which iterates this directory and
applies anything matching `<component>-*.patch`, idempotently, with
already-applied detection via `patch -R --dry-run`.

---

## Appendix B — comparison cheat sheet for someone moving from a Linux cluster

| Task | Linux cluster | Apple Silicon (this install) |
|---|---|---|
| Find HERWIG | `module load herwig` | `source $HW_PREFIX/herwig-env.sh` |
| Find a header | `-I/cvmfs/.../include` | `-I$HW_PREFIX/include` (use `*-config --cppflags`) |
| Find a library | `-L/cvmfs/.../lib` | `-L$HW_PREFIX/lib` for top-level; `-L$HW_PREFIX/lib/{ThePEG,Herwig}` for plugins |
| Set runtime path | `LD_LIBRARY_PATH=...` | rpath baked at link time; `DYLD_FALLBACK_LIBRARY_PATH` as fallback |
| Compile flags | `-std=c++17 -fPIC` | `-std=c++17 -fPIC` (same) |
| Fortran runtime | `-lgfortran` (in default paths) | `-L/opt/homebrew/lib/gcc/15 -lgfortran -lquadmath` (must be explicit) |
| `*-config` exit code | reliable | **`thepeg-config` exits 1 even when it printed the right answer** — check stdout, not `$?` |
| `register` keyword | tolerated by GCC | **error** under Clang 17 |
| `auto_ptr`/`mem_fun`/`random_shuffle` | tolerated as deprecated | **removed**, hard error |
| C++ stdlib | libstdc++ | libc++ — do not mix |

---

*End of report.*
