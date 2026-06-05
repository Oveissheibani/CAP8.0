#!/bin/bash
# cap-daily.sh — unattended daily backup of the parton-tracking work.
#
# Stages every change, commits with a dated message, and pushes to the fork
# over HTTPS using the credential stored in the macOS keychain (so it never
# prompts).  Safe to run on a schedule: it does nothing when there is nothing
# to commit, and never touches anything but git.
#
# One-time setup (already done if your manual push worked):
#   git config --global credential.helper osxkeychain
#   # push once by hand so the token gets saved in the keychain
#
# Run by hand:      ./cap-daily.sh
# Schedule it:      see the launchd block at the bottom of this file.

set -uo pipefail

REPO="/Users/oveissheibani/cap8/CAP8.0-main"
BRANCH="feature/parton-tracking"
REMOTE_URL="https://github.com/Oveissheibani/CAP8.0.git"
LOG_DIR="$REPO/logs"

cd "$REPO" || { echo "cannot cd to $REPO"; exit 1; }
mkdir -p "$LOG_DIR"
STAMP="$(date +%Y%m%dT%H%M%S)"
LOG="$LOG_DIR/cap-daily-$STAMP.log"

# Everything below also tees to a per-run log so a scheduled run leaves a trace.
exec > >(tee -a "$LOG") 2>&1

echo "=== cap-daily $STAMP  (branch $BRANCH) ==="

# 1. Make sure we are on the right branch.
current="$(git rev-parse --abbrev-ref HEAD)"
if [ "$current" != "$BRANCH" ]; then
  echo "NOTE: on '$current', not '$BRANCH' — committing on the current branch."
  BRANCH="$current"
fi

# 2. Anything to do?
if [ -z "$(git status --porcelain)" ]; then
  echo "nothing changed — checking if local is ahead of the remote…"
  if git status -sb | grep -q "ahead"; then
    echo "local is ahead; pushing existing commit(s)."
  else
    echo "clean and in sync — nothing to do."
    exit 0
  fi
else
  # 3. Stage + commit everything (gitignore already excludes run output,
  #    presentations and the personal cap-* scripts).
  git add -A
  MSG="daily backup $STAMP"
  git commit -m "$MSG" && echo "committed: $MSG"
fi

# 4. Push to the fork.  Uses the keychain credential — silent on success.
if git push "$REMOTE_URL" "$BRANCH"; then
  echo "pushed to $REMOTE_URL ($BRANCH)"
else
  echo "PUSH FAILED — if it says the remote is ahead, run:"
  echo "    git pull --rebase $REMOTE_URL $BRANCH   then re-run this script"
  echo "  if it says auth failed, your token expired — make a new one at"
  echo "    https://github.com/settings/tokens  and push once by hand."
  exit 1
fi

echo "=== done ==="

# ----------------------------------------------------------------------------
# To run automatically every day at 18:00, create this launchd file once:
#
#   cat > ~/Library/LaunchAgents/com.cap.daily.plist <<'PLIST'
#   <?xml version="1.0" encoding="UTF-8"?>
#   <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
#     "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
#   <plist version="1.0"><dict>
#     <key>Label</key><string>com.cap.daily</string>
#     <key>ProgramArguments</key>
#       <array><string>/Users/oveissheibani/cap8/CAP8.0-main/cap-daily.sh</string></array>
#     <key>StartCalendarInterval</key><dict>
#       <key>Hour</key><integer>18</integer><key>Minute</key><integer>0</integer>
#     </dict>
#     <key>RunAtLoad</key><false/>
#   </dict></plist>
#   PLIST
#   launchctl load ~/Library/LaunchAgents/com.cap.daily.plist
#
# Stop it later with:  launchctl unload ~/Library/LaunchAgents/com.cap.daily.plist
# ----------------------------------------------------------------------------
