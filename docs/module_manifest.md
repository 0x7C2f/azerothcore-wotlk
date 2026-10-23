# AzerothCore Module Manifest Specification

Every AzerothCore module **should** include a `module.json` file in its root
directory. This file is read by the module loader at startup and emitted to
the server log so operators can audit loaded module versions.

## Schema

```json
{
  "$schema": "https://azerothcore.org/schemas/module-manifest/v1.json",
  "name": "mod-example",
  "version": "1.2.3",
  "description": "Short description of the module",
  "authors": ["Author Name <author@example.com>"],
  "ac_api_version": ">=1.0.0",
  "url": "https://github.com/azerothcore/mod-example"
}
```

## Fields

| Field | Required | Description |
|---|---|---|
| `$schema` | No | JSON Schema URI for editor validation |
| `name` | Yes | Module identifier, must match the directory name (e.g. `mod-example`) |
| `version` | Yes | Module version in [semver](https://semver.org/) format (`MAJOR.MINOR.PATCH`) |
| `description` | No | Short human-readable description of the module |
| `authors` | No | List of author strings, conventionally `"Name <email>"` |
| `ac_api_version` | Yes | Semver range of the AzerothCore Script API this module targets (see below) |
| `url` | No | URL to the module's source repository or documentation |

## `ac_api_version` and Hook Versioning

The `ac_api_version` field declares which version range of the AzerothCore
Script API (`AC_SCRIPT_API_VERSION` defined in `ScriptMgr.h`) this module is
compatible with.

The Script API version follows semantic versioning:

- **MAJOR** bump — breaking ABI change; all modules must recompile and may
  need source changes.
- **MINOR** bump — new hooks added; existing hooks unchanged; fully backward
  compatible.
- **PATCH** bump — documentation or comment-only changes.

Individual hook methods in the ScriptDefines headers are annotated with
`AC_HOOK_SINCE("X.Y.Z")` to record the API version in which they were
introduced. Deprecated hooks are annotated with
`AC_HOOK_DEPRECATED("X.Y.Z", "replacement message")` which also triggers a
compiler warning when the hook is overridden.

### Example version constraints

```json
"ac_api_version": ">=1.0.0"        // any 1.x release
"ac_api_version": ">=1.2.0 <2.0.0" // 1.2.x through 1.x, not 2.x
"ac_api_version": "1.0.0"           // exactly 1.0.0
```

## Example `module.json`

```json
{
  "$schema": "https://azerothcore.org/schemas/module-manifest/v1.json",
  "name": "mod-eluna",
  "version": "2.1.0",
  "description": "Eluna Lua Engine for AzerothCore",
  "authors": ["AzerothCore contributors"],
  "ac_api_version": ">=1.0.0",
  "url": "https://github.com/azerothcore/mod-eluna"
}
```
