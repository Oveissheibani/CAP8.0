#!/usr/bin/env bash
# Shared logging helpers for CAP setup / build scripts.
#
# Sourced by setup-cap, install scripts, and (indirectly via subprocess) by the
# GUI. Defines:
#   cap_log_init <component>   — open a fresh log file, redirect stdout/stderr
#   cap_log <level> <msg>      — write a structured line ([INFO]/[WARN]/[ERR])
#   cap_log_run <cmd...>       — execute a command, log its output and exit code
#   cap_log_section <title>    — visual separator inside the log
#   cap_log_close              — flush, print summary, exit code

set -uo pipefail

# These two are exported so subprocesses see the same log destination.
: "${CAP_LOG_DIR:=${CAP_ROOT_PATH:-$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)}/logs}"
: "${CAP_LOG_LEVEL:=INFO}"     # DEBUG, INFO, WARN, ERROR
mkdir -p "$CAP_LOG_DIR"

cap_log_init() {
    local component="${1:-setup}"
    local stamp; stamp="$(date -u +%Y%m%dT%H%M%SZ)"
    export CAP_LOG_FILE="$CAP_LOG_DIR/${component}-${stamp}.log"
    # Write a header.
    {
        printf '# CAP %s log\n' "$component"
        printf '# Started   : %s\n' "$(date -u +%FT%TZ)"
        printf '# Host      : %s\n' "$(hostname 2>/dev/null || echo unknown)"
        printf '# User      : %s\n' "${USER:-${LOGNAME:-unknown}}"
        printf '# CWD       : %s\n' "$(pwd)"
        printf '# Platform  : %s %s\n' "$(uname -s)" "$(uname -m)"
        printf '# Bash      : %s\n' "${BASH_VERSION:-?}"
        printf '# Args      : %s\n' "$*"
        printf '# Env (CAP_*):\n'
        env | grep -E '^(CAP_|ROOTSYS|PYTHIA|FASTJET|CONDA)' | sort | sed 's/^/#   /'
        printf '#====================================================================\n\n'
    } > "$CAP_LOG_FILE"

    # Maintain a "last" symlink for easy tailing.
    ln -snf "$CAP_LOG_FILE" "$CAP_LOG_DIR/last-${component}.log"

    export CAP_LOG_COMPONENT="$component"
}

cap_log() {
    local level="$1"; shift
    # macOS BSD date doesn't support %N for sub-second precision. Pick the
    # right format the first time we're called and cache it.
    if [ -z "${CAP_LOG_DATE_FMT:-}" ]; then
        case "$(uname -s 2>/dev/null)" in
            Darwin|FreeBSD|OpenBSD|NetBSD)
                export CAP_LOG_DATE_FMT='%FT%TZ' ;;          # second precision
            *)
                export CAP_LOG_DATE_FMT='%FT%T.%3NZ' ;;     # GNU date: ms precision
        esac
    fi
    local ts; ts="$(date -u +"$CAP_LOG_DATE_FMT" 2>/dev/null || date -u +%FT%TZ)"
    local line="[$ts] [$level] $*"
    printf '%s\n' "$line" >> "${CAP_LOG_FILE:-/dev/null}"
    # Echo to terminal too if INFO+ severity.
    case "$level" in
        DEBUG)  [ "$CAP_LOG_LEVEL" = "DEBUG" ] && printf '%s\n' "$line" >&2 ;;
        INFO)   printf '%s\n' "$*" >&2 ;;
        WARN)   printf '\033[33m%s\033[0m\n' "$*" >&2 ;;
        ERROR)  printf '\033[31m%s\033[0m\n' "$*" >&2 ;;
    esac
}

cap_log_section() {
    cap_log INFO "==== $* ===="
}

# Run a command, mirror its stdout+stderr to the log AND to the calling
# terminal, return its true exit code.
#
# Note: We deliberately disable `set -e` inside this function so a failed
# wrapped command does not abort the whole script before we record the
# failure. Callers should test the return code themselves.
#
# Usage: cap_log_run "label" cmd arg arg ...
cap_log_run() {
    local label="$1"; shift
    cap_log INFO ">>> $label"
    cap_log DEBUG "    cmd: $*"

    # Echo the command itself into the log file.
    printf '$ %s\n' "$*" >> "$CAP_LOG_FILE"

    # Run the command, streaming stdout+stderr both to the log file and to the
    # parent's stdout (so the GUI / terminal sees live progress).
    # PIPESTATUS[0] captures the wrapped command's actual exit code, not tee's.
    # `set +e` is required so the function reaches the error-reporting branch
    # even when the caller has `set -e` enabled.
    set +e
    "$@" 2>&1 | tee -a "$CAP_LOG_FILE"
    local ec=${PIPESTATUS[0]}
    set -e

    printf '\n' >> "$CAP_LOG_FILE"
    if [ "$ec" -eq 0 ]; then
        cap_log INFO "    ok $label (exit 0)"
    else
        cap_log ERROR "    FAIL $label (exit $ec)"
    fi
    return "$ec"
}

cap_log_close() {
    local ec="${1:-0}"
    {
        printf '\n#====================================================================\n'
        printf '# Finished  : %s\n' "$(date -u +%FT%TZ)"
        printf '# Exit code : %s\n' "$ec"
    } >> "${CAP_LOG_FILE:-/dev/null}"
    if [ "$ec" -ne 0 ]; then
        printf '\n\033[31mLog saved to %s\033[0m\n' "$CAP_LOG_FILE" >&2
        printf '   Inspect with: less "%s"\n' "$CAP_LOG_FILE" >&2
    else
        printf '\n\033[32mLog saved to %s\033[0m\n' "$CAP_LOG_FILE" >&2
    fi
}
