# AGENTS.md

## Repo Shape
- Pebble watchface app in C.
- Main app entrypoint is `src/c/main.c`.
- `src/c/` is the app code; `src/pkjs/` is the phone-side JS path if used; `worker_src/c/` is only for a background worker if the directory exists.
- Bundled assets live under `resources/`.
- `wscript` drives the Pebble build; usually do not edit it unless changing build wiring.

## Commands
- Build: `pebble build`
- Install to emulator: `pebble install --emulator emery`
- Install to a paired phone: `pebble install --phone <ip>`

## Pebble-Specific Notes
- `package.json` is the source of truth for `targetPlatforms`; this repo targets both legacy Pebbles and modern hardware (`emery`, `gabbro`, `flint`).
- The app is configured as a watchface in `package.json` (`pebble.watchapp.watchface: true`).
- Custom fonts are declared in `package.json` and loaded from `resources/fonts/`.
- If you touch platform coverage, resource IDs, or watchface/watchapp mode, update `package.json` first and verify the build.
